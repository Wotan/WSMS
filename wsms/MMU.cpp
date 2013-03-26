
#include <fstream>

#include "MMU.hh"

namespace WSMS {

MMU::MMU()
  : _cartridgeRom(0), _mapper(0)
{
}

MMU::~MMU()
{
  delete[] _cartridgeRom;
  delete _mapper;
}

void MMU::loadCartridge(std::string const& fileName)
{
  std::cout << "=== Loading Cartridge ===" << std::endl;
  if (_cartridgeRom)
    delete _cartridgeRom;
  _cartridgeRom = new UBYTE[maxCartridgeSize];
  std::ifstream file(fileName.c_str());
  if (!file.is_open()) {
    throw MMUException("%s: could not open file", fileName.c_str());
  }
  file.read(reinterpret_cast<char*>(_cartridgeRom), maxCartridgeSize);
  _cartridgeSize = file.gcount();
  if (_cartridgeSize < minCartridgeSize) {
    throw MMUException("%s: invalid cartridge: too small (%i)",
		       fileName.c_str(), _cartridgeSize);
  }
  _mapper = new SegaMapper(_cartridgeRom);
  _cartridgeInfos.fileName = fileName;
  readHeader();
}

void MMU::displayCartridgeInfo() const
{
  std::cout << "File : "  << _cartridgeInfos.fileName << std::endl;
  std::cout << std::hex << "Product code : 0x"  << _cartridgeInfos.productCode << std::endl;
  std::cout << "Region : ";
  switch (_cartridgeInfos.region) {
  case 0x3: std::cout << "SMS Japan"; break;
  case 0x4: std::cout << "SMS Export"; break;
  case 0x5: std::cout << "GG Japan"; break;
  case 0x6: std::cout << "GG Export"; break;
  case 0x7: std::cout << "GG International"; break;
  default: std::cout << "Unknown"; break;
  }
  std::cout << std::endl;

  std::cout << "Size : ";
  switch (_cartridgeInfos.sizeType) {
  case 0xa: std::cout << "8KB"; break;
  case 0xb: std::cout << "16KB"; break;
  case 0xc: std::cout << "32KB"; break;
  case 0xd: std::cout << "48KB"; break;
  case 0xe: std::cout << "64KB"; break;
  case 0xf: std::cout << "128KB"; break;
  case 0x0: std::cout << "256KB"; break;
  case 0x1: std::cout << "512KB"; break;
  case 0x2: std::cout << "1MB"; break;
  default: std::cout << "Unknown"; break;
  }
  std::cout << std::endl;
  std::cout << std::hex << "Checksum : 0x"  << _cartridgeInfos.checksum << std::endl;
}

void MMU::readHeader()
{
  int headerOffset[] = { 0x7ff0, 0x1ff0, 0x3ff0 };
  for (std::size_t i = 0; i < sizeof(headerOffset) / sizeof(*headerOffset); ++i) {
    UBYTE* header = _cartridgeRom + headerOffset[i];
    char* magic = reinterpret_cast<char*>(header);
    if (std::string(magic).compare(0, 8, "TMR SEGA") == 0) {
      _cartridgeInfos.version = (header[0xE] & 0x0F);
      _cartridgeInfos.sizeType = (header[0xF] & 0x0F);
      _cartridgeInfos.region = (header[0xF] >> 4);
      _cartridgeInfos.productCode = (header[0xc] | header[0xd] << 8) |
	((header[0xe] >> 4) << 16);
      _cartridgeInfos.checksum = (header[0xb] | header[0xa] << 8);
      displayCartridgeInfo();
      return;
    }
  }
  std::cout << "Cartridge seems to be not valid" << std::endl;
}

UBYTE MMU::readIOPorts(UWORD)
{
  return 0;
}

void MMU::writeIOPorts(UWORD, UBYTE)
{
}

UBYTE SegaMapper::read(UWORD addr)
{
  if (addr <= 0x03ff) { // unpaged
    return _cartridgeRom[addr];
  } else if (addr <= 0x3fff) { // slot 0
    return _cartridgeRom[(_ram[0x1ffd] & 0x3F) * 0x4000 + addr];
  } else if (addr <= 0x7fff) { // slot 1
    return _cartridgeRom[(_ram[0x1ffe] & 0x3F) * 0x4000 + addr - 0x4000];
  } else if (addr <= 0xbfff) { // slot 2 or ram
    return _cartridgeRom[(_ram[0x1fff] & 0x3F) * 0x4000 + addr - 0x8000];
  } else if (addr <= 0xdfff) { // ram
    return _ram[addr - 0xc000];
  } else if (addr <= 0xffff) { // miror ram
    return _ram[addr - 0xe000];
  } else {
    throw MMU::MMUException("MMU: invalid read address 0x%X", addr);
  }
}

void SegaMapper::write(UWORD addr, UBYTE value)
{
  if (addr <= 0x03ff) { // unpaged
    _cartridgeRom[addr] = value;
  } else if (addr <= 0x3fff) { // slot 0
    _cartridgeRom[(_ram[0x1ffd] & 0x3F) * 0x4000 + addr] = value;
  } else if (addr <= 0x7fff) { // slot 1
    _cartridgeRom[(_ram[0x1ffe] & 0x3F) * 0x4000 + addr] = value;
  } else if (addr <= 0xbfff) { // slot 2 or ram
    _cartridgeRom[(_ram[0x1fff] & 0x3F) * 0x4000 + addr] = value;
  } else if (addr <= 0xdfff) { // ram
    _ram[addr - 0xc000] = value;
  } else if (addr <= 0xffff) { // miror ram
    _ram[addr - 0xe000] = value;
  } else {
    throw MMU::MMUException("MMU: invalid write address 0x%X", addr);
  }
}

}
