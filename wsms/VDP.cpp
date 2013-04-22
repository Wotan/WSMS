#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "VDP.hh"

namespace WSMS {

VDP::VDP(int screenSizeY, eMode mode)
  : _maxScanline(mode == NTSC ? kMaxScanlineNTSC : kMaxScanlinePAL),
    _ctrlPortS2(false), _writeType(VRAM_WRITE),
    _screenSizeX(256), _screenSizeY(screenSizeY)
{
  _screen = new uint32_t[_screenSizeX * _screenSizeY];

  // works for supported resolution (I simply guess the formula)
  if (mode == NTSC)
    _VBlankScanline = (kMaxScanlineNTSC + _screenSizeY - 16) / 2;
  else
    _VBlankScanline = (kMaxScanlinePAL + _screenSizeY - 19) / 2;

  for (int i = 0; i < 0x4000; ++i)
    _VRAM[i] = 0;

  for (int i = 0; i < 192 * 256; ++i)
    _screen[i] = 0x00FFFFFF;
}

VDP::~VDP()
{
}

// BF
void VDP::writeControlPort(UBYTE v)
{
  if (!_ctrlPortS2) {
    _addressReg = v; // have to preserve MSB?
    _ctrlPortS2 = true;
  } else {
    _ctrlPortS2 = false;
    _addressReg |= ((static_cast<UWORD>(v) & 0x3F) << 8); // for all code?
    std::cout << std::hex << "addr change" << (int)_addressReg << std::endl;

    UBYTE code = v >> 6;
    switch (code) {
    case 0:
      _writeType = VRAM_WRITE;
      _readBuffer = _VRAM[_addressReg & 0x3FFF];
      break;
    case 1:
      _writeType = VRAM_WRITE;
      break;
    case 2:
      _writeType = VRAM_WRITE;
      registerWrite(v & 0xF, _addressReg & 0xFF);
      break;
    case 3:
      _writeType = CRAM_WRITE;
      break;
    }
  }
}

UBYTE VDP::readDataPort()
{
  UBYTE ret = _readBuffer;
  _readBuffer = _VRAM[_addressReg & 0x3FFF];
  _addressReg++;
  return ret;
}

// BEl
void VDP::writeDataPort(UBYTE v)
{
  if (_writeType == VRAM_WRITE) {
    _VRAM[_addressReg & 0x3FFF] = v;
  } else {
    std::cout << "CRAM_WRITE: " << (int)v << std::endl;

    _CRAM[_addressReg & 0x1F] = v;
  }
  if (_addressReg == 0x3FFF) {
    _addressReg = 0;
  } else {
    _addressReg++;
  }
}

// --BBGGRR
namespace {
uint32_t toColor32(UBYTE v)
{
  uint32_t color = 0;
  color |= (((v >> 0) & 0x3) * (0xFF / 3)) << (8 * 3); // R
  color |= (((v >> 2) & 0x3) * (0xFF / 3)) << (8 * 2); // G
  color |= (((v >> 4) & 0x3) * (0xFF / 3)) << (8 * 1); // B
  return color;
}
} // !unamed


void VDP::drawScanline(int line)
{
  uint32_t* scanline = _screen + (_screenSizeX * line);

  drawScanlineBG(scanline, line);
  drawScanlineSprites(scanline, line);
}

void VDP::drawScanlineBG(uint32_t* scanline, int line)
{
  int HScrollPixel = 0;
  int HScrollCol = 0;
  int VScrollPixel = 0;
  int VScrollRow = 0;

  for (int pixel = 0; pixel < _screenSizeX; ++pixel) {

    // Get background tile
    int addr = 0x3800 + ((line / 8) * 32 + (pixel / 8));
    UWORD tile = reinterpret_cast<UWORD*>(_VRAM)[addr];

    int i = 0;

    // Treat all visible pixels for current tile
    for (; i < 8 && ((i + pixel) < _screenSizeX) ; ++i) {
      // Get tile's pattern
      UBYTE* pattern = _VRAM + (tile & 0x1FF) * 32;

      // Adjust to point to correct line
      pattern += (line % 8) * 4;

      // To get color index from tile pattern (a little bit tricky)
      UBYTE colorIndex = 0;
      for (int j = 0; j < 4; ++j)
        colorIndex |= ((pattern[j] >> (7 - i)) & 1) << j;

      // Get color that coresponding to index in CRAM
      UBYTE color = _CRAM[0x10 + colorIndex];

      // Finaly convert and write color to scanline
      scanline[pixel + i] = toColor32(color);
    }
    pixel += i;
  }
}

void VDP::drawScanlineSprites(uint32_t* scanline, int line)
{
  int SAT = 0x3F00; // Sprite Attribute Table
  for (int sprite = 0; sprite < 64; sprite++) {
    UBYTE x = _VRAM[SAT + 0x80 + sprite * 2];
    UBYTE y = _VRAM[SAT + sprite];

    if (y == 0xD0)
      break;

    if (!(line <= y + 7  && line >= y))
      continue;

    UBYTE* pattern = _VRAM + _VRAM[SAT + 0x80 + sprite * 2 + 1] * 32;
    pattern += (line % 8) * 4;

    for (int i = 0; i < 8 && (x + i < 256); ++i)  {

      // To get color index from tile pattern (a little bit tricky)
      UBYTE colorIndex = 0;
      for (int j = 0; j < 4; ++j)
        colorIndex |= ((pattern[j] >> (7 - i)) & 1) << j;

      // Get color that coresponding to index in CRAM
      UBYTE color = _CRAM[0x10 + colorIndex];

      // Finaly convert and write color to scanline
      scanline[x + i] = toColor32(color);
    }
  }
}

bool VDP::update(uint32_t cycles)
{
  bool HSync = false;
  _HCounter += cycles;
  if (_HCounter >= kScanlineSize) {
    if (_curScanline < _screenSizeY) { // Active display
      drawScanline(_curScanline);
    } else if (_curScanline == (_screenSizeX + 1)) {
      HSync = true;
      //std::cout << "HEYYYYYYY" << std::endl;
      // Frame IRQ
    } else if (_curScanline == _VBlankScanline) { // VBlank
      _VCounter = 0xFF - (_maxScanline - _VBlankScanline);
    }
    _HCounter -= kScanlineSize;
    _curScanline = (_curScanline == _maxScanline) ? 0 : _curScanline + 1;
    _VCounter++;
  }
  return HSync;
}

void VDP::registerWrite(UBYTE a, UBYTE v)
{
}

UBYTE VDP::getStatusFlag()
{
  return 0;
}

} // !WSMS
