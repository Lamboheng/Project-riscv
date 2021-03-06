#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "types.h"
#include "utils.h"

void print_rtype(char *, Instruction);
void print_itype_except_load(char *, Instruction, int);
void print_load(char *, Instruction);
void print_store(char *, Instruction);
void print_branch(char *, Instruction);
void print_utype(char*, Instruction);
void print_jal(Instruction);
void print_ecall(Instruction);
void write_rtype(Instruction);
void write_itype_except_load(Instruction); 
void write_load(Instruction);
void write_store(Instruction);
void write_branch(Instruction);


void decode_instruction(uint32_t instruction_bits) {
    Instruction instruction = parse_instruction(instruction_bits); // Look in utils.c
    switch(instruction.opcode) {
        case 0x33:
            write_rtype(instruction);
            break;
        case 0x13:
            write_itype_except_load(instruction);
            break;
        case 0x3:
            write_load(instruction);
            break;
        case 0x67:
            print_itype_except_load("jalr", instruction, 8);
            break;
        case 0x23:
            write_store(instruction);
            break;
        case 0x63:
            write_branch(instruction);
            break;
        case 0x37:
            print_utype("lui", instruction);
            break;
        case 0x17:
            print_utype("auipc",instruction);
            break;
        case 0x6F:
            print_jal(instruction);
            break;
        case 0x73:
            print_ecall(instruction);
            break;
        default: // undefined opcode
            handle_invalid_instruction(instruction);
            break;
    }
}

void write_rtype(Instruction instruction) {
    /* HINT: Hmmm, it's seems that there's way more R-Type instructions that funct3 possibilities... */
    switch (instruction.rtype.funct3) {
        case 0x0:
            if(instruction.rtype.funct7 == 0x00){
                print_rtype("add", instruction);
            } else if (instruction.rtype.funct7 == 0x01){
                print_rtype("mul", instruction);
            } else if (instruction.rtype.funct7 == 0x20){
                print_rtype("sub", instruction);
            } else {
                handle_invalid_instruction(instruction);
            }
            break;
        case 0x1:
            if (instruction.rtype.funct7 == 0x00) {
                print_rtype("sll", instruction);
            } else if (instruction.rtype.funct7 == 0x01) {
                print_rtype("mulh", instruction);
            } else {
                handle_invalid_instruction(instruction);
            }
            break;
        case 0x2:
            if (instruction.rtype.funct7 == 0x00) {
                print_rtype("slt", instruction);
            } else {
                handle_invalid_instruction(instruction);
            }
            break;
        case 0x3:
            if (instruction.rtype.funct7 == 0x00){
                print_rtype("sltu", instruction);
            } else if (instruction.rtype.funct7 == 0x01) {
                print_rtype("mulhu", instruction);
            } else {
                handle_invalid_instruction(instruction);
            }
            break;
        case 0x4:
            if (instruction.rtype.funct7 == 0x00) {
                print_rtype("xor", instruction);
            } else if (instruction.rtype.funct7 == 0x01) {
                print_rtype("div", instruction);
            } else {
                handle_invalid_instruction(instruction);
            }
            break;
        case 0x5:
            if(instruction.rtype.funct7 == 0x00){
                print_rtype("srl", instruction);
            } else if (instruction.rtype.funct7 == 0x01){
                print_rtype("divu", instruction);
            } else if (instruction.rtype.funct7 == 0x20){
                print_rtype("sra", instruction);
            } else {
                handle_invalid_instruction(instruction);
            }
            break;
        case 0x6:
            if (instruction.rtype.funct7 == 0x00) {
                print_rtype("or", instruction);
            } else if (instruction.rtype.funct7 == 0x01) {
                print_rtype("rem", instruction);
            } else {
                handle_invalid_instruction(instruction);
            }
            break;
        case 0x7:
            if (instruction.rtype.funct7 == 0x00) {
                print_rtype("and", instruction);
            } else if (instruction.rtype.funct7 == 0x01) {
                print_rtype("remu", instruction);
            } else {
                handle_invalid_instruction(instruction);
            }
            break;
        default:
            handle_invalid_instruction(instruction);
            break;
    }
}

void write_itype_except_load(Instruction instruction) {
    int n = -1;
    switch (instruction.itype.funct3) {
        case 0x0:
            print_itype_except_load("addi", instruction, sign_extend_number(instruction.itype.imm, 12));
            break;
        case 0x1:
            print_itype_except_load("slli", instruction, sign_extend_number(instruction.itype.imm, 12));
            break;
        case 0x2:
            print_itype_except_load("slti", instruction, sign_extend_number(instruction.itype.imm, 12));
            break;
        case 0x3:
            print_itype_except_load("sltiu", instruction, sign_extend_number(instruction.itype.imm, 12));
            break;
        case 0x4:
            print_itype_except_load("xori", instruction, sign_extend_number(instruction.itype.imm, 12));
            break;
        case 0x5:
            n = (instruction.itype.imm & 0x01f);
            if ((instruction.itype.imm & 0xfe0) == 0x00) {
                print_itype_except_load("srli", instruction, n);
            } else {
                print_itype_except_load("srai", instruction, n);
            }
            break;
        case 0x6:
            print_itype_except_load("ori", instruction, sign_extend_number(instruction.itype.imm, 12));
            break;
        case 0x7:
            print_itype_except_load("andi", instruction, sign_extend_number(instruction.itype.imm, 12));
            break;
        default:
            handle_invalid_instruction(instruction);
            break;  
    }
}

void write_load(Instruction instruction) {
    switch (instruction.itype.funct3) {
        case 0x0:
            print_load("lb", instruction);
            break;
        case 0x1:
            print_load("lh", instruction);
            break;
        case 0x2:
            print_load("lw", instruction);
            break;
        case 0x4:
            print_load("lbu", instruction);
            break;
        case 0x5:
            print_load("lhu", instruction);
            break;
        default:
            handle_invalid_instruction(instruction);
            break;
    }
}

void write_store(Instruction instruction) {
    switch (instruction.stype.funct3) {
        case 0x0:
            print_store("sb", instruction);
            break;
        case 0x1:
            print_store("sh", instruction);
            break;
        case 0x2:
            print_store("sw", instruction);
            break;
        default:
            handle_invalid_instruction(instruction);
            break;
    }
}

void write_branch(Instruction instruction) {
    switch (instruction.sbtype.funct3) {
        case 0x0:
            print_branch("beq", instruction);
            break;
        case 0x1:
            print_branch("bne", instruction);
            break;
        case 0x4:
            print_branch("blt", instruction);
            break;
        case 0x5:
            print_branch("bge", instruction);
            break;
        case 0x6:
            print_branch("bltu", instruction);
            break;
        case 0x7:
            print_branch("bgeu", instruction);
            break;
        default:
            handle_invalid_instruction(instruction);
            break;
    }
}

/* utils.c and utils.h might be useful here... */

void print_utype(char* name, Instruction instruction) {
    printf(UTYPE_FORMAT, name, instruction.utype.rd, instruction.utype.imm);
}

void print_jal(Instruction instruction) {
    printf(JAL_FORMAT, instruction.ujtype.rd, get_jump_offset(instruction));
}

void print_ecall(Instruction instruction) {
    printf(ECALL_FORMAT);
}

void print_rtype(char *name, Instruction instruction) {
    printf(RTYPE_FORMAT, name, instruction.rtype.rd, instruction.rtype.rs1, instruction.rtype.rs2);
}

void print_itype_except_load(char *name, Instruction instruction, int imm) {
    printf(ITYPE_FORMAT, name, instruction.itype.rd, instruction.itype.rs1, (sWord)imm);
}

void print_load(char *name, Instruction instruction) {
    printf(MEM_FORMAT, name, instruction.itype.rd, (sWord)sign_extend_number(instruction.itype.imm, 12), instruction.itype.rs1);
}

void print_store(char *name, Instruction instruction) {
    printf(MEM_FORMAT, name, instruction.stype.rs2, get_store_offset(instruction), instruction.stype.rs1);
}

void print_branch(char *name, Instruction instruction) {
    printf(BRANCH_FORMAT, name, instruction.sbtype.rs1, instruction.sbtype.rs2, get_branch_offset(instruction));
}
