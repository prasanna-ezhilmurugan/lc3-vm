#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MEMORY 1 << 16
uint16_t memory[MAX_MEMORY];

enum {
  R_R0 = 0,
  R_R1,
  R_R2,
  R_R3,
  R_R4,
  R_R5,
  R_R6,
  R_R7,
  R_PC,    // Program Couter
  R_COND,  // Conditional Flag
  R_COUNT, // Number of Register
};

uint16_t reg[R_COUNT];

enum {
  OP_BR = 0, /* branch */
  OP_ADD,    /* add  */
  OP_LD,     /* load */
  OP_ST,     /* store */
  OP_JSR,    /* jump register */
  OP_AND,    /* bitwise and */
  OP_LDR,    /* load register */
  OP_STR,    /* store register */
  OP_RTI,    /* unused */
  OP_NOT,    /* bitwise not */
  OP_LDI,    /* load indirect */
  OP_STI,    /* store indirect */
  OP_JMP,    /* jump */
  OP_RES,    /* reserved (unused) */
  OP_LEA,    /* load effective address */
  OP_TRAP,   /* execute trap */
};

enum {
  FL_POS = 1 << 0,
  FL_ZRO = 1 << 1,
  FL_NEG = 1 << 2,
};

uint16_t memoryRead(uint16_t);
int read_image(char *arg);
uint16_t sign_extend(uint16_t x, int bitcount);

int main(int argc, char *argv[]) {

  // handle arguments

  // for (int i = 0; i < argc; i++) {
  //   printf("%d: %s\n", i, argv[i]);
  // }

  if (argc < 2) {
    printf("lc3 [image-file] ... \n");
    exit(2);
  }

  for (int i = 1; i < argc; i++) {
    if (!read_image(argv[i])) {
      printf("Failed to load image: %s\n", argv[i]);
      exit(1);
    }
  }

  enum {
    PC_START = 0x3000,
  };

  reg[R_PC] = PC_START;
  bool isRunning = true;

  while (isRunning) {

    // fetch instruction
    uint16_t instruction = memoryRead(reg[R_PC]++);
    uint16_t opcode = instruction >> 12;

    switch (opcode) {
    case OP_ADD:
      // add
      break;
    case OP_AND:
      // and
      break;
    case OP_BR:
      // break
      break;
    case OP_JMP:
      // Jump
      break;
    case OP_JSR:
      // jump register
      break;
    case OP_LD:
      // load
      break;
    case OP_LDI:
      // load indirect
      break;
    case OP_LDR:
      // load register
      break;
    case OP_LEA:
      // load effective address
      break;
    case OP_NOT:
      // not
      break;
    case OP_RES:
      // reserved
      break;
    case OP_RTI:
      // unused
      break;
    case OP_ST:
      // store
      break;
    case OP_STI:
      // store indirect
      break;
    case OP_STR:
      // store register
      break;
    case OP_TRAP:
      // trap
      break;

    default:
      // bad opcode (no match)
      break;
    }
  }

  return 0;
}

uint16_t sign_extend(uint16_t x, int bit_count) {
  if ((x >> (bit_count - 1)) & 1) {
    x |= (0xFFFF << bit_count);
  }
  return x;
}

void update_flags(uint16_t r) {
  if (reg[r] == 0) {
    reg[R_COND] = FL_ZRO;
  } else if (reg[r] >> 15) {
    reg[R_COND] = FL_NEG;
  } else {
    reg[R_COND] = FL_POS;
  }
}

void ADD(uint16_t instruction) {
  // destination register
  uint16_t r0 = (instruction >> 9) & 0x7;
  // first operand sr1
  uint16_t r1 = (instruction >> 6) & 0x7;
  // flag whether immediate addressing
  uint16_t imm_flag = (instruction >> 5) & 0x1;

  if (imm_flag) {
    uint16_t imm5 = sign_extend(instruction & 0x1F, 5);
    reg[r0] = reg[r1] + imm5;
  } else {
    // second operand sr2
    uint16_t r2 = (instruction) & 0x7;
    reg[r0] = reg[r1] + reg[r2];
  }
  update_flags(r0);
}

void NOT(uint16_t instruction) {
  // destination register
  uint16_t r0 = (instruction >> 9) & 0x7;
  // src register
  uint16_t r1 = (instruction >> 6) & 0x7;

  reg[r0] = ~reg[r1];
  update_flags(r0);
}
