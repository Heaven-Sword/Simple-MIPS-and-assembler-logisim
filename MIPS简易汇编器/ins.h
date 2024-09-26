#pragma once
/*
* MIPS简单汇编器 *
* 包含以下指令
* R型：add,sub,and,or,xor,slt,seq,sgt,sll,srl,sra,jr
* I型：addi,subi,andi,ori,xori,slti,seqi,sgti,slli,srli,srai,lw,sw,beq
* J型：j,jmp
*/

// R型指令 OP rs rt rd shamt func
#define add  "000000XXXXXxxxxxXXXXXxxxxx000001"
#define sub  "000000XXXXXxxxxxXXXXXxxxxx000010"
#define and  "000000XXXXXxxxxxXXXXXxxxxx000011"
#define or   "000000XXXXXxxxxxXXXXXxxxxx000100"
#define xor  "000000XXXXXxxxxxXXXXXxxxxx000101"
#define slt  "000000XXXXXxxxxxXXXXXxxxxx001001"
#define seq  "000000XXXXXxxxxxXXXXXxxxxx001010"
#define sgt  "000000XXXXXxxxxxXXXXXxxxxx001011"
#define sll  "000000XXXXXxxxxxXXXXXxxxxx000110"
#define srl  "000000XXXXXxxxxxXXXXXxxxxx000111"
#define sra  "000000XXXXXxxxxxXXXXXxxxxx001000"
#define jr   "000000XXXXXxxxxxXXXXXxxxxx010000"


// I型指令 OP rs rt imm
#define addi "000001xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define subi "000010xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define andi "000011xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define ori  "000100xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define xori "000101xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define slti "001001xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define seqi "001010xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define sgti "001011xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define slli "000110xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define srli "000111xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define srai "001000xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define lw   "010000xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define sw   "010001xxxxxXXXXXxxxxxxxxxxxxxxxx"
#define beq  "100000xxxxxXXXXXxxxxxxxxxxxxxxxx"

// J型指令 OP addr
#define j    "100001xxxxxxxxxxxxxxxxxxxxxxxxxx"
#define jmp  "100001xxxxxxxxxxxxxxxxxxxxxxxxxx"
