#ifndef _VDP_H_
# define _VDP_H_

# include "WSMS.hh"

namespace WSMS {

class VDP {
  enum eMode {
    NTSC,
    PAL
  };

  static const int kCRAMSize = 0x20;
  static const int kVRAMSize = 0x4000;
  static const int kScanlineSize = 342;
  static const int kMaxScanlineNTSC = 262;
  static const int kMaxScanlinePAL = 313;

public:
  VDP(int screenSizeY = 192, eMode mode = VDP::NTSC);
  virtual ~VDP();

  enum  eWriteType{
    VRAM_WRITE,
    CRAM_WRITE,
  };

  void writeControlPort(UBYTE v);

  UBYTE readDataPort();
  void writeDataPort(UBYTE v);

  void registerWrite(UBYTE reg, UBYTE v);

  // when screen have to be refresh .i.e on HSync
  bool update(uint32_t cycles);

  UBYTE getHCounter() const { return _HCounter >> 1; }
  UBYTE getVCounter() const { return _VCounter; }

  UBYTE getStatusFlag();

  uint32_t const* getScreen() const { return _screen; }

  void drawScanline(int line);
  void drawScanlineBG(uint32_t* scanline, int line);
  void drawScanlineSprites(uint32_t* scanline, int line);

private:
  UBYTE _VRAM[kVRAMSize];
  UBYTE _CRAM[kCRAMSize];

  int _HCounter; // HCounter is 9bits
  UBYTE _VCounter; // UBYTE to let the variable overflow
  int _curScanline;
  int const _maxScanline; // 262 on NTSC / 313 on PAL
  int _VBlankScanline; // Scanline where occur VBlank

  bool _ctrlPortS2; // True if control port is in sequence 2
  UWORD _addressReg;
  eWriteType _writeType;
  UBYTE _readBuffer;

  int const _screenSizeX;
  int const _screenSizeY;
  uint32_t* _screen;
};

} // !WSMS

#endif /* !_VDP_H_ */
