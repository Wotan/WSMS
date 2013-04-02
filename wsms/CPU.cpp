#include <iostream>

#include "CPU.hh"
#include "Disassembler.hh"

namespace WSMS {

Z80::Z80()
{
  AF = 0;
  BC = 0;
  DE = 0;
  HL = 0;
  _IX = _IY = 0;
  _SP = 0xdff0; // For testing purpose only, user have to set it
  _PC = 0;
  _I = 0;
  _R = 0;
  _sAF = _sBC = _sDE = _sHL = 0;
  _intMode = INT_MODE_0;
  _intEnabled = false;
}

Z80::~Z80()
{
}

int Z80::step()
{
  Disassembler disas;

  UBYTE opcode = READ_MEM(PC++);


  switch (opcode) {
    /* ADC */
  case 0x88 + 7: ADD8(A, A, F_C); return 4;
  case 0x88 + 0: ADD8(A, B, F_C); return 4;
  case 0x88 + 1: ADD8(A, C, F_C); return 4;
  case 0x88 + 2: ADD8(A, D, F_C); return 4;
  case 0x88 + 3: ADD8(A, E, F_C); return 4;
  case 0x88 + 4: ADD8(A, H, F_C); return 4;
  case 0x88 + 5: ADD8(A, L, F_C); return 4;
  case 0xCE: ADD8(A, READ_MEM(PC++), F_C); return 7;
  case 0x8E: ADD8(A, READ_MEM(HL), F_C); return 7;

    /* ADD */
  case 0x80 + 7: ADD8(A, A); return 4;
  case 0x80 + 0: ADD8(A, B); return 4;
  case 0x80 + 1: ADD8(A, C); return 4;
  case 0x80 + 2: ADD8(A, D); return 4;
  case 0x80 + 3: ADD8(A, E); return 4;
  case 0x80 + 4: ADD8(A, H); return 4;
  case 0x80 + 5: ADD8(A, L); return 4;
  case 0xC6: ADD8(A, READ_MEM(PC++)); return 7;
  case 0x86: ADD8(A, READ_MEM(HL)); return 7;
  case 0x09: ADD16(HL, BC); return 11;
  case 0x19: ADD16(HL, DE); return 11;
  case 0x29: ADD16(HL, HL); return 11;
  case 0x39: ADD16(HL, SP); return 11;

    /* AND */
  case 0xA0 + 7: AND(A, A); return 4;
  case 0xA0 + 0: AND(A, B); return 4;
  case 0xA0 + 1: AND(A, C); return 4;
  case 0xA0 + 2: AND(A, D); return 4;
  case 0xA0 + 3: AND(A, E); return 4;
  case 0xA0 + 4: AND(A, H); return 4;
  case 0xA0 + 5: AND(A, L); return 4;
  case 0xE6: AND(A, READ_MEM(PC++)); return 7;
  case 0xA6: AND(A, READ_MEM(HL)); return 7;

    /* CALL */
  case 0xCD: return CALL(true) ? 17 : 1;
  case 0xDC: return CALL(F_C) ? 17 : 1;
  case 0xD4: return CALL(!F_C) ? 17 : 1;
  case 0xFC: return CALL(F_S) ? 17 : 1;
  case 0xF4: return CALL(!F_S) ? 17 : 1;
  case 0xCC: return CALL(F_Z) ? 17 : 1;
  case 0xC4: return CALL(!F_Z) ? 17 : 1;
  case 0xEC: return CALL(F_PV) ? 17 : 1;
  case 0xE4: return CALL(!F_PV) ? 17 : 1;

    /* CCF */
  case 0x3F: F_C = ~F_C; F_N = 0; return 4;

    /* CP */
  case 0xB8 + 7: CP(A, A); return 4;
  case 0xB8 + 0: CP(A, B); return 4;
  case 0xB8 + 1: CP(A, C); return 4;
  case 0xB8 + 2: CP(A, D); return 4;
  case 0xB8 + 3: CP(A, E); return 4;
  case 0xB8 + 4: CP(A, H); return 4;
  case 0xB8 + 5: CP(A, L); return 4;
  case 0xFE: CP(A, READ_MEM(PC++)); return 7;
  case 0xBE: CP(A, READ_MEM(HL)); return 7;

    /* CPL */
  case 0x2F: A = ~A; F_N = F_H = 1; return 4;

    /* DDA */
  case 0x27: /* TODO */ return 4;

    /* DEC */
  case 0x3D: DEC8(A); return 4;
  case 0x05: DEC8(B); return 4;
  case 0x0D: DEC8(C); return 4;
  case 0x15: DEC8(D); return 4;
  case 0x1D: DEC8(E); return 4;
  case 0x25: DEC8(H); return 4;
  case 0x2D: DEC8(L); return 4;
  case 0x35: { UBYTE r = READ_MEM(HL); DEC8(r); WRITE_MEM(HL, r); } return 11;

  case 0x0B: DEC16(BC); return 6;
  case 0x1B: DEC16(DE); return 6;
  case 0x2B: DEC16(HL); return 6;
  case 0x3B: DEC16(SP); return 6;

    /* DI */
  case 0xF3: setIntEnable(false); return 4;

    /* DJNZ */
  case 0x10: {
    B--;
    if (B == 0) {
      PC += static_cast<BYTE>(READ_MEM(PC) + 1); return 13;
    } else {
      return 8;
    }
  }

    /* EI */
  case 0xFB: setIntEnable(true); return 4;

    /* EX */
  case 0xE3: { UWORD v = READ_MEM16(SP); EX(v, HL); WRITE_MEM16(SP, v); } return 19;
  case 0x08: EX(AF, _sAF); return 4;
  case 0xEB: EX(DE, HL); return 4;
  case 0xD9: EX(BC, _sBC); EX(DE, _sDE); EX(HL, _sHL); return 4;

    /* HALT */
  case 0x76: std::cout << "HALT" << std::endl; return 4;

    /* IN */
  case 0xDB: A = IN(READ_MEM(PC++)); return 11;

    /* INC */
  case 0x3C: INC8(A); return 4;
  case 0x04: INC8(B); return 4;
  case 0x0C: INC8(C); return 4;
  case 0x14: INC8(D); return 4;
  case 0x1C: INC8(E); return 4;
  case 0x24: INC8(H); return 4;
  case 0x2C: INC8(L); return 4;

  case 0x03: INC16(BC); return 6;
  case 0x13: INC16(DE); return 6;
  case 0x23: INC16(HL); return 6;
  case 0x33: INC16(SP); return 6;

  case 0x34: { UBYTE v = READ_MEM(HL); INC8(v); WRITE_MEM(HL, v); } return 11;

    /* JP */
  case 0xC3: PC = READ_MEM16(PC); return 10;
  case 0xE9: PC = READ_MEM16(HL); return 4;

  case 0xDA: if (F_C) return PC = READ_MEM16(PC), 10; else return PC += 2, 1;
  case 0xD2: if (!F_C) return PC = READ_MEM16(PC), 10; else return PC += 2, 1;
  case 0xFA: if (F_S) return PC = READ_MEM16(PC), 10; else return PC += 2, 1;
  case 0xF2: if (!F_S) return PC = READ_MEM16(PC), 10; else return PC += 2, 1;
  case 0xCA: if (F_Z) return PC = READ_MEM16(PC), 10; else return PC += 2, 1;
  case 0xC2: if (!F_Z) return PC = READ_MEM16(PC), 10; else return PC += 2, 1;
  case 0xEA: if (F_PV) return PC = READ_MEM16(PC), 10; else return PC += 2, 1;
  case 0xE2: if (!F_PV) return PC = READ_MEM16(PC), 10; else return PC += 2, 1;

    /* JR */
#define JR() PC += static_cast<BYTE>(READ_MEM(PC) + 1); return 12;
  case 0x18: JR();
  case 0x38: if (F_C) { JR(); } else return PC++, 4;
  case 0x30: if (!F_C) { JR(); } else return PC++, 4;
  case 0x28: if (F_Z) { JR(); } else return PC++, 4;
  case 0x20: if (!F_Z) { JR(); } else return PC++, 4;

    /* LD */
  case 0x78 + 7: A = A; return 4;
  case 0x78 + 0: A = B; return 4;
  case 0x78 + 1: A = C; return 4;
  case 0x78 + 2: A = D; return 4;
  case 0x78 + 3: A = E; return 4;
  case 0x78 + 4: A = H; return 4;
  case 0x78 + 5: A = L; return 4;
  case 0x3E: A = READ_MEM(PC++); return 7;
  case 0x0A: A = READ_MEM(BC); return 7;
  case 0x1A: A = READ_MEM(DE); return 7;
  case 0x7E: A = READ_MEM(HL); return 7;
  case 0x3A: A = READ_MEM(READ_MEM16(PC)); PC += 2; return 13;

  case 0x40 + 7: B = A; return 4;
  case 0x40 + 0: B = B; return 4;
  case 0x40 + 1: B = C; return 4;
  case 0x40 + 2: B = D; return 4;
  case 0x40 + 3: B = E; return 4;
  case 0x40 + 4: B = H; return 4;
  case 0x40 + 5: B = L; return 4;
  case 0x06: B = READ_MEM(PC++); return 7;
  case 0x40 + 6: B = READ_MEM(HL); return 7;

  case 0x48 + 7: C = A; return 4;
  case 0x48 + 0: C = B; return 4;
  case 0x48 + 1: C = C; return 4;
  case 0x48 + 2: C = D; return 4;
  case 0x48 + 3: C = E; return 4;
  case 0x48 + 4: C = H; return 4;
  case 0x48 + 5: C = L; return 4;
  case 0x0E: C = READ_MEM(PC++); return 7;
  case 0x48 + 6: C = READ_MEM(HL); return 7;

  case 0x50 + 7: D = A; return 4;
  case 0x50 + 0: D = B; return 4;
  case 0x50 + 1: D = C; return 4;
  case 0x50 + 2: D = D; return 4;
  case 0x50 + 3: D = E; return 4;
  case 0x50 + 4: D = H; return 4;
  case 0x50 + 5: D = L; return 4;
  case 0x16: D = READ_MEM(PC++); return 7;
  case 0x50 + 6: D = READ_MEM(HL); return 7;

  case 0x58 + 7: E = A; return 4;
  case 0x58 + 0: E = B; return 4;
  case 0x58 + 1: E = C; return 4;
  case 0x58 + 2: E = D; return 4;
  case 0x58 + 3: E = E; return 4;
  case 0x58 + 4: E = H; return 4;
  case 0x58 + 5: E = L; return 4;
  case 0x1E: E = READ_MEM(PC++); return 7;
  case 0x58 + 6: E = READ_MEM(HL); return 7;

  case 0x60 + 7: H = A; return 4;
  case 0x60 + 0: H = B; return 4;
  case 0x60 + 1: H = C; return 4;
  case 0x60 + 2: H = D; return 4;
  case 0x60 + 3: H = E; return 4;
  case 0x60 + 4: H = H; return 4;
  case 0x60 + 5: H = L; return 4;
  case 0x26: H = READ_MEM(PC++); return 7;
  case 0x60 + 6: H = READ_MEM(HL); return 7;

  case 0x68 + 7: L = A; return 4;
  case 0x68 + 0: L = B; return 4;
  case 0x68 + 1: L = C; return 4;
  case 0x68 + 2: L = D; return 4;
  case 0x68 + 3: L = E; return 4;
  case 0x68 + 4: L = H; return 4;
  case 0x68 + 5: L = L; return 4;
  case 0x2E: L = READ_MEM(PC++); return 7;
  case 0x68 + 6: L = READ_MEM(HL); return 7;

  case 0x01: BC = READ_MEM16(PC); PC += 2; return 10;
  case 0x11: DE = READ_MEM16(PC); PC += 2; return 10;
  case 0x2A: HL = READ_MEM16(READ_MEM16(PC)); PC += 2; return 10;
  case 0x21: HL = READ_MEM16(PC); PC += 2; return 10;
  case 0xF9: SP = HL; return 10;
  case 0x31: SP = READ_MEM16(PC); PC += 2; return 10;

  case 0x70 + 7: WRITE_MEM(HL, A); return 7;
  case 0x70 + 0: WRITE_MEM(HL, B); return 7;
  case 0x70 + 1: WRITE_MEM(HL, C); return 7;
  case 0x70 + 2: WRITE_MEM(HL, D); return 7;
  case 0x70 + 3: WRITE_MEM(HL, E); return 7;
  case 0x70 + 4: WRITE_MEM(HL, H); return 7;
  case 0x70 + 5: WRITE_MEM(HL, L); return 7;
  case 0x36: WRITE_MEM(HL, READ_MEM(PC++)); return 10;

  case 0x02: WRITE_MEM(BC, A); return 7;
  case 0x12: WRITE_MEM(DE, A); return 7;
  case 0x32: WRITE_MEM(READ_MEM16(PC), A); PC += 2; return 13;
  case 0x22: WRITE_MEM16(READ_MEM16(PC), HL); PC += 2; return 16;

    /* NOP */
  case 0x00: return 4;

    /* OR */
  case 0xB0 + 7: OR(A, A); return 4;
  case 0xB0 + 0: OR(A, B); return 4;
  case 0xB0 + 1: OR(A, C); return 4;
  case 0xB0 + 2: OR(A, D); return 4;
  case 0xB0 + 3: OR(A, E); return 4;
  case 0xB0 + 4: OR(A, H); return 4;
  case 0xB0 + 5: OR(A, L); return 4;
  case 0xF6: OR(A, READ_MEM(PC++)); return 7;
  case 0xB6: OR(A, READ_MEM(HL)); return 7;

    /* OUT */
  case 0xD3: WRITE_IO(READ_MEM(PC++), A); return 11;

    /* POP */
  case 0xF1: AF = pop16(); return 10;
  case 0xC1: BC = pop16(); return 10;
  case 0xD1: DE = pop16(); return 10;
  case 0xE1: HL = pop16(); return 10;

    /* PUSH */
  case 0xF5: push16(AF); return 11;
  case 0xC5: push16(BC); return 11;
  case 0xD5: push16(DE); return 11;
  case 0xE5: push16(HL); return 11;

    /* RET */
  case 0xC9: PC = pop16(); return 10;
  case 0xD8: if (F_C) return PC = pop16(), 10; else return 5;
  case 0xD0: if (!F_C) return PC = pop16(), 10; else return 5;
  case 0xF8: if (F_S) return PC = pop16(), 10; else return 5;
  case 0xF0: if (!F_S) return PC = pop16(), 10; else return 5;
  case 0xC8: if (F_Z) return PC = pop16(), 10; else return 5;
  case 0xC0: if (!F_Z) return PC = pop16(), 10; else return 5;
  case 0xE8: if (F_PV) return PC = pop16(), 10; else return 5;
  case 0xE0: if (!F_PV) return PC = pop16(), 10; else return 5;

    /* RLA SZHPNC -> --0-0* */
  case 0x17: {
    UBYTE s = F_C; F_C = (A & (1 << 7)); A <<= 1; A |= s; F_N = F_H = 0;
  } return 4;

    /* RLCA SZHPNC -> --0-0* */
  case 0x07: F_C = (A & (1 << 7)); A <<= 1; A |= F_C; F_N = F_H = 0; return 4;

    /* RRA SZHPNC ->  --0-0* */
  case 0x1F: {
    UBYTE s = F_C; F_C = (A & 1); A >>= 1; A |= s << 7; F_N = F_H = 0;
  } return 4;

    /* RRCA SZHPNC -> --0-0* */
  case 0x0f: F_C = (A & 1); A >>= 1; A |= F_C << 7; F_N = F_H = 0; return 4;

    /* RST */
  case 0xC7: push16(PC); PC = 0x00; return 11;
  case 0xCF: push16(PC); PC = 0x08; return 11;
  case 0xD7: push16(PC); PC = 0x10; return 11;
  case 0xDF: push16(PC); PC = 0x18; return 11;
  case 0xE7: push16(PC); PC = 0x20; return 11;
  case 0xEF: push16(PC); PC = 0x28; return 11;
  case 0xF7: push16(PC); PC = 0x30; return 11;
  case 0xFF: push16(PC); PC = 0x38; return 11;

    /* SBC */
  case 0x98 + 7: SUB8(A, A, F_C); return 4;
  case 0x98 + 0: SUB8(A, B, F_C); return 4;
  case 0x98 + 1: SUB8(A, C, F_C); return 4;
  case 0x98 + 2: SUB8(A, D, F_C); return 4;
  case 0x98 + 3: SUB8(A, E, F_C); return 4;
  case 0x98 + 4: SUB8(A, H, F_C); return 4;
  case 0x98 + 5: SUB8(A, L, F_C); return 4;
  case 0xDE: SUB8(A, READ_MEM(PC++), F_C); return 7;
  case 0x9E: SUB8(A, READ_MEM(HL), F_C); return 7;

    /* SCF SZHPNC -> --0-01 */
  case 0x37: F_H = F_N = 0; F_C = 1; return 4;

    /* SUB */
  case 0x90 + 7: SUB8(A, A, 0); return 4;
  case 0x90 + 0: SUB8(A, B, 0); return 4;
  case 0x90 + 1: SUB8(A, C, 0); return 4;
  case 0x90 + 2: SUB8(A, D, 0); return 4;
  case 0x90 + 3: SUB8(A, E, 0); return 4;
  case 0x90 + 4: SUB8(A, H, 0); return 4;
  case 0x90 + 5: SUB8(A, L, 0); return 4;
  case 0xD6: SUB8(A, READ_MEM(PC++), 0); return 7;
  case 0x96: SUB8(A, READ_MEM(HL), 0); return 7;

    /* XOR */
  case 0xA8 + 7: XOR(A, A); return 4;
  case 0xA8 + 0: XOR(A, B); return 4;
  case 0xA8 + 1: XOR(A, C); return 4;
  case 0xA8 + 2: XOR(A, D); return 4;
  case 0xA8 + 3: XOR(A, E); return 4;
  case 0xA8 + 4: XOR(A, H); return 4;
  case 0xA8 + 5: XOR(A, L); return 4;
  case 0xEE: XOR(A, READ_MEM(PC++)); return 7;
  case 0xAE: XOR(A, READ_MEM(HL)); return 7;

    /* EXTENDED Opcodes */
  case 0xDD: return indexInstructions(IX, 0xDD);
  case 0xFD: return indexInstructions(IY, 0xFD);

    /***************/
    /*     ED      */
    /***************/
  case 0xED: { // ED
    UBYTE extOpcode = READ_MEM(PC++);
    switch(extOpcode) {
    case 0x46: setIntMode(INT_MODE_0); return 8;
    case 0x56: setIntMode(INT_MODE_1); return 8;
    case 0x5E: setIntMode(INT_MODE_2); return 8;


    case 0x4A: ADC16(HL, BC); return 15;
    case 0x5A: ADC16(HL, DE); return 15;
    case 0x6A: ADC16(HL, HL); return 15;
    case 0x7A: ADC16(HL, SP); return 15;

    case 0xA9: CPD(); return 16;
    case 0xB9: return CPDR();
    case 0xA1: CPI(); return 16;
    case 0xB1: return CPIR();

      /* IN / OUT */
    case 0x70: IN(C); return 12;
    case 0x78: A = IN(C); return 12;
    case 0x40: B = IN(C); return 12;
    case 0x48: C = IN(C); return 12;
    case 0x50: D = IN(C); return 12;
    case 0x58: E = IN(C); return 12;
    case 0x60: H = IN(C); return 12;
    case 0x68: L = IN(C); return 12;

    case 0x71: OUT(0); return 12;
    case 0x79: OUT(A); return 12;
    case 0x41: OUT(B); return 12;
    case 0x49: OUT(C); return 12;
    case 0x51: OUT(D); return 12;
    case 0x59: OUT(E); return 12;
    case 0x61: OUT(H); return 12;
    case 0x69: OUT(L); return 12;

#define IND() WRITE_MEM(HL, IN(C)); HL--; B--; F_N = 1; F_Z = (B == 0);
    case 0xAA: IND(); return 16;
    case 0xBA: IND(); if (B == 0) { return 1; } PC -= 2; return 21;
#define INI() WRITE_MEM(HL, IN(C)); HL++; B--; F_N = 1; F_Z = (B == 0);
    case 0xA2: INI(); return 16;
    case 0xB2: INI(); if (B == 0) { return 1; } PC -= 2; return 21;

#define OUTD() OUT(READ_MEM(HL)); HL--; B--; F_N = 1; F_Z = (B == 0);
    case 0xAB: OUTD(); return 16;
    case 0xBB: OUTD(); if (B == 0) { return 1; } PC -= 2; return 21;
#define OUTI() OUT(READ_MEM(HL)); HL--; B--; F_N = 1; F_Z = (B == 0);
    case 0xA3: OUTI(); return 16;
    case 0xB3: OUTI(); if (B == 0) { return 1; } PC -= 2; return 21;

      /* LD */
    case 0x47: I = A; return 9;
    case 0x4F: R = A; return 9;
    case 0x57: A = I; return 9; // flags?
    case 0x5F: A = R; return 9; // flags?
    case 0x4B: BC = READ_MEM16(PC); PC += 2; return 20;
    case 0x5B: DE = READ_MEM16(PC); PC += 2; return 20;
    case 0x7B: SP = READ_MEM16(PC); PC += 2; return 20;
    case 0x43: WRITE_MEM16(READ_MEM16(PC), BC); PC += 2; return 20;
    case 0x53: WRITE_MEM16(READ_MEM16(PC), DE); PC += 2; return 20;
    case 0x73: WRITE_MEM16(READ_MEM16(PC), SP); PC += 2; return 20;

#define LDD() WRITE_MEM16(READ_MEM16(HL), DE); HL--; DE--; BC--; F_H = F_N = 0; F_PV = (BC == 0);
    case 0xA8: LDD(); return 16;
    case 0xB8: LDD(); if (BC == 0) { return 1; } PC -= 2; return 21;
#define LDI() WRITE_MEM16(READ_MEM16(HL), DE); HL++; DE++; BC--; F_H = F_N = 0; F_PV = (BC == 0);
    case 0xA0: LDI(); return 16;
    case 0xB0: LDI(); if (BC == 0) { return 1; } PC -= 2; return 21;

    case 0x44: NEG(A); return 8;

    case 0x42: SBC16(HL, BC); return 15;
    case 0x52: SBC16(HL, DE); return 15;
    case 0x62: SBC16(HL, HL); return 15;
    case 0x72: SBC16(HL, SP); return 15;

    default:
      std::cout << std::hex << "Unknown extended opcode ("
                << opcode << "): " << static_cast<int>(opcode) << std::endl;
      break;
    }
  } break; // END 0xED

    /***************/
    /*     CB      */
    /***************/
  case 0xCB: {
    UBYTE extOpcode = READ_MEM(PC++);
    switch(extOpcode) {

      /* RL */
    case 0x10 + 7: RL(A); return 8;
    case 0x10 + 0: RL(B); return 8;
    case 0x10 + 1: RL(C); return 8;
    case 0x10 + 2: RL(D); return 8;
    case 0x10 + 3: RL(E); return 8;
    case 0x10 + 4: RL(H); return 8;
    case 0x10 + 5: RL(L); return 8;
    case 0x16: { UBYTE t = READ_MEM(HL); RL(t); WRITE_MEM(HL, t); } return 15;

      /* RLC */
    case 0x00 + 7: RLC(A); return 8;
    case 0x00 + 0: RLC(B); return 8;
    case 0x00 + 1: RLC(C); return 8;
    case 0x00 + 2: RLC(D); return 8;
    case 0x00 + 3: RLC(E); return 8;
    case 0x00 + 4: RLC(H); return 8;
    case 0x00 + 5: RLC(L); return 8;
    case 0x06: { UBYTE t = READ_MEM(HL); RLC(t); WRITE_MEM(HL, t); } return 15;

      /* RR */
    case 0x18 + 7: RR(A); return 8;
    case 0x18 + 0: RR(B); return 8;
    case 0x18 + 1: RR(C); return 8;
    case 0x18 + 2: RR(D); return 8;
    case 0x18 + 3: RR(E); return 8;
    case 0x18 + 4: RR(H); return 8;
    case 0x18 + 5: RR(L); return 8;
    case 0x1E: { UBYTE t = READ_MEM(HL); RR(t); WRITE_MEM(HL, t); } return 15;

      /* RRC */
    case 0x08 + 7: RRC(A); return 8;
    case 0x08 + 0: RRC(B); return 8;
    case 0x08 + 1: RRC(C); return 8;
    case 0x08 + 2: RRC(D); return 8;
    case 0x08 + 3: RRC(E); return 8;
    case 0x08 + 4: RRC(H); return 8;
    case 0x08 + 5: RRC(L); return 8;
    case 0x0E: { UBYTE t = READ_MEM(HL); RRC(t); WRITE_MEM(HL, t); } return 15;

      /* SLA */
    case 0x20 + 7: SLA(A); return 8;
    case 0x20 + 0: SLA(B); return 8;
    case 0x20 + 1: SLA(C); return 8;
    case 0x20 + 2: SLA(D); return 8;
    case 0x20 + 3: SLA(E); return 8;
    case 0x20 + 4: SLA(H); return 8;
    case 0x20 + 5: SLA(L); return 8;
    case 0x26: { UBYTE t = READ_MEM(HL); SLA(t); WRITE_MEM(HL, t); } return 15;

      /* SRA */
    case 0x28 + 7: SRA(A); return 8;
    case 0x28 + 0: SRA(B); return 8;
    case 0x28 + 1: SRA(C); return 8;
    case 0x28 + 2: SRA(D); return 8;
    case 0x28 + 3: SRA(E); return 8;
    case 0x28 + 4: SRA(H); return 8;
    case 0x28 + 5: SRA(L); return 8;
    case 0x2E: { UBYTE t = READ_MEM(HL); SRA(t); WRITE_MEM(HL, t); } return 15;

      /* SLL */
    case 0x30 + 7: SLL(A); return 8;
    case 0x30 + 0: SLL(B); return 8;
    case 0x30 + 1: SLL(C); return 8;
    case 0x30 + 2: SLL(D); return 8;
    case 0x30 + 3: SLL(E); return 8;
    case 0x30 + 4: SLL(H); return 8;
    case 0x30 + 5: SLL(L); return 8;
    case 0x36: { UBYTE t = READ_MEM(HL); SLL(t); WRITE_MEM(HL, t); } return 15;

      /* SRL */
    case 0x38 + 7: SRL(A); return 8;
    case 0x38 + 0: SRL(B); return 8;
    case 0x38 + 1: SRL(C); return 8;
    case 0x38 + 2: SRL(D); return 8;
    case 0x38 + 3: SRL(E); return 8;
    case 0x38 + 4: SRL(H); return 8;
    case 0x38 + 5: SRL(L); return 8;
    case 0x3E: { UBYTE t = READ_MEM(HL); SRL(t); WRITE_MEM(HL, t); } return 15;

      /* SET */
#define MSET1(b, rb, v) case 0xC0 + 8 * b + rb: SET(v, b); return 8;
#define MSET(rb, v) MSET1(0, rb, v); MSET1(1, rb, v); MSET1(2, rb, v); MSET1(3, rb, v); \
      MSET1(4, rb, v); MSET1(5, rb, v); MSET1(6, rb, v); MSET1(7, rb, v);
      MSET(7, A); MSET(0, B); MSET(1, C); MSET(2, D);
      MSET(3, E); MSET(4, H); MSET(5, L);
#define MSETHL(b) case 0xC6 + 8 * b: { UBYTE t = READ_MEM(HL); SET(t, b); WRITE_MEM(HL, t); } return 15;
      MSETHL(0); MSETHL(1); MSETHL(2); MSETHL(3);
      MSETHL(4); MSETHL(5); MSETHL(6); MSETHL(7);

      /* RES */
#define MRES1(b, rb, v) case 0x80 + 8 * b + rb: RES(v, b); return 8;
#define MRES(rb, v) MRES1(0, rb, v); MRES1(1, rb, v); MRES1(2, rb, v); MRES1(3, rb, v); \
      MRES1(4, rb, v); MRES1(5, rb, v); MRES1(6, rb, v); MRES1(7, rb, v);
      MRES(7, A); MRES(0, B); MRES(1, C); MRES(2, D);
      MRES(3, E); MRES(4, H); MRES(5, L);
#define MRESHL(b) case 0x86 + 8 * b: { UBYTE t = READ_MEM(HL); RES(t, b); WRITE_MEM(HL, t); } return 15;
      MRESHL(0); MRESHL(1); MRESHL(2); MRESHL(3);
      MRESHL(4); MRESHL(5); MRESHL(6); MRESHL(7);

      /* BIT */
#define MBIT1(b, rb, v) case 0x40 + 8 * b + rb: BIT(v, b); return 8;
#define MBIT(rb, v) MBIT1(0, rb, v); MBIT1(1, rb, v); MBIT1(2, rb, v); MBIT1(3, rb, v); \
      MBIT1(4, rb, v); MBIT1(5, rb, v); MBIT1(6, rb, v); MBIT1(7, rb, v);
      MBIT(7, A); MBIT(0, B); MBIT(1, C); MBIT(2, D);
      MBIT(3, E); MBIT(4, H); MBIT(5, L);
#define MBITHL(b) case 0x46 + 8 * b: { UBYTE t = READ_MEM(HL); BIT(t, b); WRITE_MEM(HL, t); } return 12;
      MBITHL(0); MBITHL(1); MBITHL(2); MBITHL(3);
      MBITHL(4); MBITHL(5); MBITHL(6); MBITHL(7);

    default:
      std::cout << std::hex << "Unknown extended opcode ("
                << opcode << "): " << static_cast<int>(opcode) << std::endl;
      break;
    }
  } break; // END 0xCB


  default:
    std::cout << "Unknown opcode: " << std::hex
	      << static_cast<int>(opcode)
	      << std::endl;
    break;
  }

  return 0;
}

void Z80::push8(UBYTE value)
{
  WRITE_MEM(--SP, value);
}

void Z80::push16(UWORD value)
{
  SP -= 2;
  WRITE_MEM16(SP, value);
}

UBYTE Z80::pop8()
{
  return READ_MEM(SP++);
}

UWORD Z80::pop16()
{
  UWORD ret = READ_MEM16(SP);
  SP += 2;
  return ret;
}

/* SZHPNC -> ***V0* */
/* NB: use for both ADC 8-bit and ADD 8-bit */
void Z80::ADD8(UBYTE& a, UBYTE b, UBYTE carry)
{
  UBYTE r = a + b + carry;
  F_S = (r >> 7);
  F_Z = (r == 0);
  F_H = (r & 0xf) < (a & 0xf);
  F_PV = (a >> 7) == (b >> 7) && (a >> 7) != F_S;
  F_N = 0;
  F_C = (r < a);
  a = r;
}

/* SZHPNC -> --?-0* */
void Z80::ADD16(UWORD& a, UWORD b)
{
  UWORD r = a + b;
  F_H = (r & 0xf) < (a & 0xf);
  F_C = 0;
  a = r;
}

/* SZHPNC -> **?V0* */
void Z80::ADC16(UWORD& a, UWORD b)
{
  UWORD r = a + b + F_C;
  F_S = (r >> 15);
  F_Z = (r == 0);
  F_PV = (a >> 15) == (b >> 15) && (a >> 15) != F_S;
  F_N = 0;
  F_C = (r < a);
  a = r;
}

/* SZHPNC -> ***V1* */
void Z80::SUB8(UBYTE& a, UBYTE b, UBYTE carry)
{
  UBYTE r = a - b - carry;
  F_S = (r >> 7);
  F_Z = (r == 0);
  F_H = (r & 0xf) > (a & 0xf);
  F_PV = (a >> 7) == (b >> 7) && (a >> 7) != F_S; // to check
  F_N = 1;
  F_C = (r > a);
  a = r;
}

/* SZHPNC -> **?V1* */
void Z80::SBC16(UWORD& a, UWORD b)
{
  UWORD r = a - b - F_C;
  F_S = (r >> 15);
  F_Z = (r == 0);
  F_PV = (a >> 15) == (b >> 15) && (a >> 15) != F_S;
  F_N = 1;
  F_C = (r > a);
  a = r;
}

/* SZHPNC -> ***P00 */
void Z80::AND(UBYTE& a, UBYTE b)
{
  UBYTE r = a & b;
  F_S = (a >> 7);
  F_Z = (a == 0);
  F_H = 0; // always false?
  F_PV = (r % 2 || r == 1);
  F_N = F_C = 0;
  a = r;
}

/* SZHPNC -> ***P00 */
void Z80::OR(UBYTE& a, UBYTE b)
{
  UBYTE r = a | b;
  F_S = (a >> 7);
  F_Z = (a == 0);
  F_H = 0; // always false?
  F_PV = (r % 2 || r == 1);
  F_N = F_C = 0;
  a = r;
}

/* SZHPNC -> ***P00 */
void Z80::XOR(UBYTE& a, UBYTE b)
{
  UBYTE r = a ^ b;
  F_S = (a >> 7);
  F_Z = (a == 0);
  F_H = 0; // always false?
  F_PV = (r % 2 || r == 1);
  F_N = F_C = 0;
  a = r;
}

/* SZHPNC -> **0P0* */
void Z80::RL(UBYTE& a)
{
  UBYTE s = F_C;
  F_C = (a & (1 << 7));
  a <<= 1;
  a |= s;
  F_S = (a << 7);
  F_Z = (a == 0);
  F_H = 0;
  F_PV = (a % 2 || a == 1);
  F_N = 0;
}

/* SZHPNC -> **0P0* */
void Z80::RLC(UBYTE& a)
{
  F_C = (a & (1 << 7));
  a <<= 1;
  a |= F_C;
  F_S = (a << 7);
  F_Z = (a == 0);
  F_H = 0;
  F_PV = (a % 2 || a == 1);
  F_N = 0;
}

/* SZHPNC -> **0P0* */
void Z80::SLA(UBYTE& a)
{
  F_C = (a & (1 << 7));
  a = static_cast<BYTE>(a) << 1; // see SRA
  F_S = (a << 7);
  F_Z = (a == 0);
  F_H = 0;
  F_PV = (a % 2 || a == 1);
  F_N = 0;
}

/* SZHPNC -> **0P0* */
void Z80::SLL(UBYTE& a)
{
  F_C = (a & (1 << 7));
  a <<= 1;
  F_S = (a << 7);
  F_Z = (a == 0);
  F_H = 0;
  F_PV = (a % 2 || a == 1);
  F_N = 0;
}

/* SZHPNC -> **0P0* */
void Z80::RR(UBYTE& a)
{
  UBYTE s = F_C;
  F_C = (a & 1);
  a >>= 1;
  a |= s << 7;
  F_S = (a << 7);
  F_Z = (a == 0);
  F_H = 0;
  F_PV = (a % 2 || a == 1);
  F_N = 0;
}

/* SZHPNC -> **0P0* */
void Z80::RRC(UBYTE& a)
{
  F_C = (A & 1);
  A >>= 1;
  A |= F_C;
  F_S = (a << 7);
  F_Z = (a == 0);
  F_H = 0;
  F_PV = (a % 2 || a == 1);
  F_N = 0;
}

/* SZHPNC -> **0P0* */
/* Arithmetic shift have to preserve MSB */
void Z80::SRA(UBYTE& a)
{
  F_C = (a & 1);
  a = static_cast<BYTE>(a) >> 1; // cast to signed to preserver MSB
  F_S = (a << 7);
  F_Z = (a == 0);
  F_H = 0;
  F_PV = (a % 2 || a == 1);
  F_N = 0;
}

/* SZHPNC -> **0P0* */
void Z80::SRL(UBYTE& a)
{
  F_C = (a & 1);
  a >>= 1;
  F_S = (a << 7);
  F_Z = (a == 0);
  F_H = 0;
  F_PV = (a % 2 || a == 1);
  F_N = 0;
}

/* SZHPNC -> ?*1?0- */
void Z80::BIT(UBYTE value, UBYTE bit)
{
  F_Z = value & (1 << bit);
  F_H = 1;
  F_N = 0;
}

void Z80::RES(UBYTE& v, UBYTE bit)
{
  v &= ~(1 << bit);
}

void Z80::SET(UBYTE& v, UBYTE bit)
{
  v |= (1 << bit);
}

// flags not affected
bool Z80::CALL(bool cond)
{
  UWORD value = READ_MEM16(PC);
  PC += 2;
  if (!cond)
    return false;
  push16(PC);
  PC = value;
  return true;
}

// SZHPNC -> ***V1*
void Z80::CP(UBYTE a, UBYTE b)
{
  UBYTE r = a - b;
  F_S = r >> 7;
  F_Z = (r == 0);
  F_H = (r & 0xf) > (a & 0xf);
  F_PV = (a >> 7) == (b >> 7) && (a >> 7) != F_S; // to see
  F_N = 1;
  F_C = r > a;
}

void Z80::CPD()
{
  UBYTE a = A;
  UBYTE b = READ_MEM(HL);
  UBYTE r = a - b;
  HL--; BC--;
  F_S = (r >> 7); F_Z = (r == 0);
  F_H = (r & 0xf) > (a & 0xf);
  F_PV = (a >> 7) == (b >> 7) && (a >> 7) != F_S; // to see
  F_N = 1;
}

BYTE Z80::CPDR()
{
  CPD();
  if (F_Z == 0 || BC == 0)
    return 1;
  PC -= 2;
  return 21;
}

void Z80::CPI()
{
  UBYTE a = A;
  UBYTE b = READ_MEM(HL);
  UBYTE r = a - b;
  HL++; BC--;
  F_S = (r >> 7); F_Z = (r == 0);
  F_H = (r & 0xf) > (a & 0xf);
  F_PV = (a >> 7) == (b >> 7) && (a >> 7) != F_S; // to see
  F_N = 1;
}

BYTE Z80::CPIR()
{
  CPI();
  if (F_Z == 0 || BC == 0)
    return 1;
  PC -= 2;
  return 21;
}

// SZHPNC -> ***V1-
void Z80::DEC8(UBYTE& a)
{
  UBYTE r = a - 1;
  F_S = r >> 7;
  F_Z = (r == 0);
  F_H = (r & 0xf) > (a & 0xf);
  F_PV = (r == 127);
  F_N = 1;
  a = r;
}

void Z80::DEC16(UWORD& a)
{
  --a;
}

// SZHPNC -> ***V0-
void Z80::INC8(UBYTE& a)
{
  UBYTE r = a + 1;
  F_S = r >> 7;
  F_Z = (r == 0);
  F_H = (r & 0xf) < (a & 0xf);
  F_PV = (r == 128);
  F_N = 0;
  a = r;
}

void Z80::INC16(UWORD& a)
{
  ++a;
}

void Z80::EX(UWORD& a, UWORD& b)
{
  a ^= b; b ^= a; a ^= b;
}

UBYTE Z80::IN(UBYTE a)
{
  return READ_IO(a);
}

void Z80::OUT(UBYTE a)
{
  WRITE_IO(C, a);
}

void Z80::NEG(UBYTE& a)
{
  UBYTE r = 0 - a;
  F_S = (r >> 7);
  F_Z = (r == 0);
  F_H = (a == 0); // to check
  F_PV = (a == 0x80);
  F_N = 1;
  F_C = (a == 0);
  a = r;
}

} // !WSMS
