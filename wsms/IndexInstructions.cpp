#include "CPU.hh"

namespace WSMS {

/*
** Opcode 0xDD/0xFD
** IX/IY related instructions
*/
UBYTE Z80::indexInstructions(UWORD& I, UBYTE origOpcode)
{
  UBYTE opcode = READ_MEM(PC++);
  switch (opcode) {
  case 0x8E: ADD8(A, READ_MEM(I + READ_MEM(PC++)), F_C); return 19;
  case 0x86: ADD8(A, READ_MEM(I + READ_MEM(PC++)), 0); return 19;
  case 0x09: ADD16(I, BC); return 15;
  case 0x19: ADD16(I, DE); return 15;
  case 0x29: ADD16(I, I); return 15;
  case 0x39: ADD16(I, SP); return 15;
  case 0xA6: AND(A, READ_MEM(I + READ_MEM(PC++))); return 19;
  case 0xBE: CP(A, READ_MEM(I + READ_MEM(PC++))); return 19;
  case 0x96: SUB8(A, READ_MEM(I + READ_MEM(PC++)), 0); return 19;
  case 0xAE: XOR(A, READ_MEM(I + READ_MEM(PC++))); return 19;
  case 0x35: {
    UBYTE v = I + READ_MEM(PC++); UBYTE s = READ_MEM(v); DEC8(s); WRITE_MEM(v, s);
  } return 23;
  case 0x2B: DEC16(I); return 10;
  case 0xE3: { UWORD s = READ_MEM16(SP); EX(s, I); WRITE_MEM16(SP, s); } return 23;
  case 0x23: INC16(I); return 10;
  case 0x34: {
    UBYTE v = I + READ_MEM(PC++); UBYTE s = READ_MEM(v); INC8(s); WRITE_MEM(v, s);
  } return 23;

    /* JP */
  case 0xE9: PC = READ_MEM16(I); return 8;

    /* LD */
  case 0x7E: A = READ_MEM(I + READ_MEM(PC++)); return 19;
  case 0x46: B = READ_MEM(I + READ_MEM(PC++)); return 19;
  case 0x4E: C = READ_MEM(I + READ_MEM(PC++)); return 19;
  case 0x56: D = READ_MEM(I + READ_MEM(PC++)); return 19;
  case 0x5E: E = READ_MEM(I + READ_MEM(PC++)); return 19;
  case 0x66: H = READ_MEM(I + READ_MEM(PC++)); return 19;
  case 0x6E: L = READ_MEM(I + READ_MEM(PC++)); return 19;

  case 0xF9: SP = I; return 19;
  case 0x2A: I = READ_MEM16(READ_MEM16(PC)); PC += 2; return 20;
  case 0x21: I = READ_MEM16(PC); PC += 2; return 14;
  case 0x22: WRITE_MEM16(READ_MEM16(PC), I); PC += 2; return 20;

  case 0x70 + 7: WRITE_MEM(I + READ_MEM(PC++), A); return 19;
  case 0x70 + 0: WRITE_MEM(I + READ_MEM(PC++), B); return 19;
  case 0x70 + 1: WRITE_MEM(I + READ_MEM(PC++), C); return 19;
  case 0x70 + 2: WRITE_MEM(I + READ_MEM(PC++), D); return 19;
  case 0x70 + 3: WRITE_MEM(I + READ_MEM(PC++), E); return 19;
  case 0x70 + 4: WRITE_MEM(I + READ_MEM(PC++), H); return 19;
  case 0x70 + 5: WRITE_MEM(I + READ_MEM(PC++), L); return 19;
  case 0x36: WRITE_MEM(I + READ_MEM(PC), READ_MEM(PC + 1)); PC += 2; return 19;

  case 0xB6: OR(A, READ_MEM(I + READ_MEM(PC++))); return 19;

  case 0xE1: I = pop16(); return 14;
  case 0xE5: push16(I); return 15;

  case 0x9E: SUB8(A, READ_MEM(I + READ_MEM(PC++)), F_C); return 19;

  case 0xCB: { // DD CB
    UBYTE arg = READ_MEM(PC++);
    UBYTE extOpcode = READ_MEM(PC++);
    switch (extOpcode) {
#define RES_I(b) case 0x86 + 8 * b: { UBYTE s = READ_MEM(I + arg);    \
        RES(s, b); WRITE_MEM(I + arg, s); } return 23;
#define SET_I(b) case 0xC6 + 8 * b: { UBYTE s = READ_MEM(I + arg);    \
        SET(s, b); WRITE_MEM(I + arg, s); } return 23;
#define BIT_I(b) case 0x46 + 8 * b: { UBYTE s = READ_MEM(I + arg);    \
        BIT(s, b); WRITE_MEM(I + arg, s); } return 23;

      /* BIT b,(I+N) */
      BIT_I(0); BIT_I(1); BIT_I(2); BIT_I(3);
      BIT_I(4); BIT_I(5); BIT_I(6); BIT_I(7);

      /* RES b,(I+N) */
      RES_I(0); RES_I(1); RES_I(2); RES_I(3);
      RES_I(4); RES_I(5); RES_I(6); RES_I(7);

      /* SET b,(I+N) */
      SET_I(0); SET_I(1); SET_I(2); SET_I(3);
      SET_I(4); SET_I(5); SET_I(6); SET_I(7);

    case 0x16: { /* RL (I+N) */
      UBYTE s = READ_MEM(I + arg); RL(s); WRITE_MEM(I + arg, s);
    } return 23;
    case 0x06: { /* RLC (I+N) */
      UBYTE s = READ_MEM(I + arg); RLC(s); WRITE_MEM(I + arg, s);
    } return 23;
    case 0x1E: { /* RR (I+N) */
      UBYTE s = READ_MEM(I + arg); RR(s); WRITE_MEM(I + arg, s);
    } return 23;
    case 0x0E: { /* RRC (I+N) */
      UBYTE s = READ_MEM(I + arg); RRC(s); WRITE_MEM(I + arg, s);
    } return 23;
    case 0x26: { /* SLA (I+N) */
      UBYTE s = READ_MEM(I + arg); SLA(s); WRITE_MEM(I + arg, s);
    } return 23;
    case 0x2E: { /* SRA (I+N) */
      UBYTE s = READ_MEM(I + arg); SRA(s); WRITE_MEM(I + arg, s);
    } return 23;
    case 0x36: { /* SLL (I+N) */
      UBYTE s = READ_MEM(I + arg); SLL(s); WRITE_MEM(I + arg, s);
    } return 23;
    case 0x3E: { /* SRL (I+N) */
      UBYTE s = READ_MEM(I + arg); SRL(s); WRITE_MEM(I + arg, s);
    } return 23;

    default:
      std::cout << std::hex << "Unknown extended opcode ("
                << origOpcode << ":" << opcode << "): " << static_cast<int>(extOpcode) << std::endl;
      return 0;
    }
  } break;

  default:
    std::cout << std::hex << "Unknown extended opcode ("
              << origOpcode << "): " << static_cast<int>(opcode) << std::endl;
    break;
  }
  return 0;
}

} // !WSMS
