#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/* Sign extends the given field to a 32-bit integer where field is
 * interpreted an n-bit integer. Look in test_utils.c for examples. */ 
int sign_extend_number( unsigned int field, unsigned int n) {
    unsigned bitStatus = (field >> (n-1) ) & 1; 
    if(!bitStatus) return field;
    for(int i = n; i < 32; i++) {
        field = ((bitStatus << i)| (field & (~(1 << i))));
    }
    return field;
}

/* Unpacks the 32-bit machine code instruction given into the correct
 * type within the instruction struct. Look at types.h */ 
Instruction parse_instruction(uint32_t instruction_bits) {
    Instruction instruction;
    instruction.bits = instruction_bits;
    return instruction;
}

/* Return the number of bytes (from the current PC) to the branch label using the given
 * branch instruction */
int get_branch_offset(Instruction instruction) {
    unsigned offset = 0;
    int imm = instruction.sbtype.imm5;
    offset = (0x1e & imm)|offset;
    offset = (0x800 & (imm<< 11))|offset;
    imm = instruction.sbtype.imm7;
    offset = (0x7e0 & (imm << 5))|offset;
    offset = (0x1000 & (imm << 6))|offset;
    return sign_extend_number(offset, 13);
}

/* Returns the number of bytes (from the current PC) to the jump label using the given
 * jump instruction */
int get_jump_offset(Instruction instruction) {
    unsigned offset = 0;
    int imm = instruction.ujtype.imm;
    offset = (0x800 & (imm << 3))|offset;
    offset = (0x7fe & (imm >> 8))|offset;
    offset = (0xff000 & (imm << 12))|offset;
    offset = (0x100000 & (imm << 2))|offset;
    return sign_extend_number(offset, 21);
}

/* Returns the byte offset (from the address in rs2) for storing info using the given
 * store instruction */ 
int get_store_offset(Instruction instruction) {
    unsigned offset = 0;
    int imm = instruction.stype.imm5;
    offset = (0x1f & imm)|offset;
    imm = instruction.stype.imm7;
    offset = (0xfe0 & (imm << 5))|offset;
    return sign_extend_number(offset, 12);
}

void handle_invalid_instruction(Instruction instruction) {
    printf("Invalid Instruction: 0x%08x\n", instruction.bits); 
    exit(-1);
}

void handle_invalid_read(Address address) {
    printf("Bad Read. Address: 0x%08x\n", address);
    exit(-1);
}

void handle_invalid_write(Address address) {
    printf("Bad Write. Address: 0x%08x\n", address);
    exit(-1);
}

