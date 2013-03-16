#ifndef _MEMORY_H_
# define _MEMORY_H_

# include "navi/Exception.hh"

# include <iostream>
# include <cassert>

# include "WSMS.hh"

namespace WSMS {

class MemoryMapper {
public:
  virtual ~MemoryMapper() {}
  static std::size_t const maxRamSize = 0x2000;

public:
  virtual inline UBYTE read(UWORD addr) = 0;
  virtual inline void write(UWORD addr, UBYTE value) = 0;
};

class SegaMapper : public MemoryMapper {
public:
  explicit SegaMapper(UBYTE* cartridgeRom)
    : _ram(), _cartridgeRom(cartridgeRom) {
    _ram[0x1ffd] = 0;
    _ram[0x1ffe] = 1;
    _ram[0x1fff] = 2;
    for (std::size_t i = 0; i < MemoryMapper::maxRamSize; ++i)
      _ram[i] = 0;
  }
  virtual ~SegaMapper() {}

  virtual inline UBYTE read(UWORD addr);
  virtual inline void write(UWORD addr, UBYTE value);

protected:
  UBYTE _ram[MemoryMapper::maxRamSize];
  UBYTE* _cartridgeRom;
};

class MMU {
public:
  DEFINE_EXCEPTION(MMUException);
  static std::size_t const minCartridgeSize = 0x8000;
  static std::size_t const maxCartridgeSize = 0x100000;

public:
  MMU();
  ~MMU();
  void loadCartridge(std::string const& fileName);
  UBYTE readIOPorts(UWORD);
  void writeIOPorts(UWORD, UBYTE);

  inline UBYTE readMem(UWORD addr) {
    assert(_mapper);
    return _mapper->read(addr);
  }
  inline void writeMem(UWORD addr, UBYTE value) {
    assert(_mapper);
    _mapper->write(addr, value);
  }

private:
  void displayCartridgeInfo() const;
  void readHeader();

private:

  struct {
    std::string fileName;
    int version;
    int sizeType;
    int region;
    int productCode;
    int checksum;
  } _cartridgeInfos;
  std::size_t _cartridgeSize;
  UBYTE* _cartridgeRom;
  MemoryMapper* _mapper;
};


}


#endif /* !_MEMORY_H_ */