#pragma warning(disable:4996)
#include <stdio.h>
#include <cstring>

#include <iostream>
#include <string>
#include <map>

#include "reg.h"  //�Ĵ�������
#include "ins.h"  //ָ�����

#define MIPSBIT 32  //оƬλ��
#define MIPSBYTE 4  
#define ADDRESSLENGTH 16  //����洢���ĵ�ַ����

using namespace std;
//����reg.h��ins.h�ĺ궨�����������µ�ӳ���ϵ
map<string, const char*> reg_to_bit{};
map<string, const char*> ins_to_bit{};
map<string, char> ins_to_type{};

typedef int _ADDRESS;

//�ж϶�ȡ����(��ע��ǰ)�Ƿ�Ϊ����
bool emptyLine(const char* target) {
	while (*target != '\0') {
		if (*target > 32 && *target < 127) return false;
		target++;
	}
	return true;
}


//��ʼ��
void init();
//��ʾ
void help();


int main(void) {
	//��ʼ��
	init();
	//�����ʾ
	help();

	//�����ļ���Ŀ¼
	char root_path[128];
	//������ļ���Ŀ¼(�������ļ�ͬĿ¼)
	char write_root_path[128];
	//����ʱ��debug�ļ�(���ڲ鿴�Ƿ�����)
	char debug_root_path[128];
	printf("�������������ļ�·����");
	scanf("%[^\n]s", root_path);
	getchar();

	//��ͼ���ļ�
	FILE* assembly_flie = fopen(root_path, "r");
	if (assembly_flie == NULL) {
		perror("���ļ�ʧ�ܣ�");
	}

	//��������ļ�
	strcpy(write_root_path, root_path);
	strcat(write_root_path, "_result.txt");
	FILE* result_file = fopen(write_root_path, "w");
	//����debug�ļ�
	strcpy(debug_root_path, root_path);
	strcat(debug_root_path, "_debug.txt");
	FILE* debug_file = fopen(debug_root_path, "w");

	char instruction[128];  //�洢��ȡ��һ��ָ��
	int address = 0;  //��ַ

	map<string, _ADDRESS> label_to_address;  //��ǩ����ַ��ӳ��
	

	//��һ��ɨ�裬�����б�ǩ�ĵ�ַ����
	char label_head[64];
	while (!feof(assembly_flie)) {
		//���ж�ȡ����
		fgets(instruction, 128, assembly_flie);
		//�ж��Ƿ�Ϊ����
		if (emptyLine(instruction)) continue;
		//���Ҵ治����ע��
		if (strchr(instruction, ';') != NULL) continue;
		//����":"���������ô���Ǳ�ǩ
		char* targetChar = strchr(instruction, ':');
		if (targetChar != NULL) {
			//�����ض�
			*targetChar = '\0';
			//���ӳ��
			sscanf(instruction, "%s", label_head);
			label_to_address[string(label_head)] = address;
		}
		//��Ȼ���õ�ַ++
		else address++;
	}


	//�ڶ���ɨ�裬�����е�ָ��ӳ��Ϊ������
	char instruction_head[16];  //ָ��ͷ���� add,sub...
	char op[16], rs[16], rt[16], rd[16], shamt[16], func[16], imm[24], addr[32];
	char instruction_bit[33];  //2������Ϣ
	//���ÿ�ͷ
	fseek(assembly_flie, 0L, SEEK_SET);
	int line = 0;  //�������������Ϣ


	while (!feof(assembly_flie) ) {


		//���ж�ȡ����
		fgets(instruction, 128, assembly_flie);
		//�ж��Ƿ�Ϊ����
		if (emptyLine(instruction)) continue;

		//���Ҵ治����ע��
		if (strchr(instruction, ';') != NULL) continue;

		//д��debug
		fputs(instruction, debug_file);
		printf("%s", instruction);
		//����":"���������ô���Ǳ�ǩ����ǩ����ӳ��Ϊ������
		if (strchr(instruction, ':') != NULL) continue;

		//����ָ��
		sscanf(instruction, "%s", instruction_head);

		//�жϵ����յ�
		if (!strcmp(instruction_head, "END")) break;

		//�����ж�
		auto it = ins_to_type.find(instruction_head);
		if (it == ins_to_type.end()) { 
			printf("Error:ָ��ͷ����%s , ָ�%s , �У�%d", instruction_head, instruction, line);
			break;
		}

		//�õ�ָ��
		strcpy(instruction_bit, ins_to_bit[instruction_head]);
		//ӳ��ָ��
		if (it->second == 'R') {
			sscanf(instruction, "%s %[^,], %[^,], %s", instruction_head, rd, rs, rt);  //���磺sub $rd,$rs,$rt  => rd = rs - rt
			//��� �ο� add "000000XXXXXxxxxxXXXXXxxxxx000001"
			fprintf(result_file, "%.*s%05s%05s%05s%05s%.*s\n", 6, instruction_bit, reg_to_bit[rs], reg_to_bit[rt], reg_to_bit[rd], "0", 6, (instruction_bit + 26));
			

		}
		else if (it->second == 'r') {
			if (!strcmp(instruction_head, "jr")) {
				sscanf(instruction, "%s %s", instruction_head, rs);  //���� jr $rs
				//��� �ο� jr   "000000XXXXXxxxxxXXXXXxxxxx010000"
				fprintf(result_file, "%.*s%05s%05s%05s%05s%.*s\n", 6, instruction_bit, reg_to_bit[rs], "0", "0", "0", 6, (instruction_bit + 26));

			}
			else {
				sscanf(instruction, "%s %[^,], %[^,], %s", instruction_head, rd, rt, shamt);  //���磺sll $rt,$rt,101  => rt = rt << 5b'101
				//��� �ο� sra  "000000XXXXXxxxxxXXXXXxxxxx001000"
				fprintf(result_file, "%.*s%05s%05s%05s%05s%.*s\n", 6, instruction_bit, "0", reg_to_bit[rt], reg_to_bit[rd], shamt, 6, (instruction_bit + 26));
			}
		}
		else if (it->second == 'I') {
			sscanf(instruction, "%s %[^,], %[^,], %s", instruction_head, rt, rs, imm);  //���� addi $rt,$rs,$imm  => rt = rs + imm
			//��� �ο� slti "001001xxxxxXXXXXxxxxxxxxxxxxxxxx"
			fprintf(result_file, "%.*s%05s%05s%016s\n", 6, instruction_bit, reg_to_bit[rs], reg_to_bit[rt], imm);

		}
		else if (it->second == 'J') {
			sscanf(instruction, "%s %s", instruction_head, addr);  // ���� j 101011
			//��� �ο� j "100001xxxxxxxxxxxxxxxxxxxxxxxxxx"
			if (!strcmp(instruction_head, "jmp")) {
				auto it = label_to_address.find(string(addr));
				if (it == label_to_address.end()) {
					printf("Error:�����ڴ˱�ǩ��%s ,ָ�%s , �У�%d\n", addr, instruction, line);
					break;
				}
				_itoa(it->second, addr, 2);
			}
			fprintf(result_file, "%.*s%026s\n", 6, instruction_bit, addr);
			
		}
		line++;
	}

	//�ر��ļ�
	fclose(assembly_flie);
	fclose(result_file);
	fclose(debug_file);

	//�˳�ǰͣ��
	printf("\n������������س����رմ˴���. . .");
	getchar();

	return 0;
}

void help() {
	printf("* MIPS�򵥻���� *\n");
	printf("* �ɱ������»��ָ�� *\n");
	printf("R�ͣ�add,sub,and,or,xor,slt,seq,sgt,sll,srl,sra,jr\n");
	printf("I�ͣ�addi,subi,andi,ori,xori,slti,seqi,sgti,slli,srli,srai,lw,sw,beq\n");
	printf("J�ͣ�j,jmp\n");
	printf("*******************************��ָ������\n");
	printf("label:\n");
	printf("	add $s0, $s1, $s2         \n");
	printf("	sub $s0, $s1, $s2		  \n");
	printf("	and $s0, $s1, $s2		  \n");
	printf("	or $s0, $s1, $s2		  \n");
	printf("	xor $s0, $s1, $s2		  \n");
	printf("	slt $s0, $s1, $s2		  \n");
	printf("	seq $s0, $s1, $s2		  \n");
	printf("	sgt $s0, $s1, $s2		  \n");
	printf("	sll $s0, $s1, 1010		  \n");
	printf("	srl $s0, $s1, 110		  \n");
	printf("	sra $s0, $s1, 1001		  \n");
	printf("	jr $s0					  \n");
	printf("							  \n");
	printf("	addi $s0, $s1, 1010110	  \n");
	printf("	subi $s0, $s1, 1010110	  \n");
	printf("	andi $s0, $s1, 1010110	  \n");
	printf("	ori $s0, $s1, 1010110	  \n");
	printf("	xori $s0, $s1, 1010110	  \n");
	printf("	slti $s0, $s1, 1010110	  \n");
	printf("	seqi $s0, $s1, 1010110	  \n");
	printf("	sgti $s0, $s1, 1010110	  \n");
	printf("	slli $s0, $s1, 1010		  \n");
	printf("	srli $s0, $s1, 110		  \n");
	printf("	srai $s0, $s1, 1001		  \n");
	printf("							  \n");
	printf("	lw $s0, $s1, 1101		  \n");
	printf("	sw $s0, $s1, 1101		  \n");
	printf("	beq $s0, $s1, 1101		  \n");
	printf("							  \n");
	printf("	j 11010010				  \n");
	printf("	jmp label				  \n");
	printf("END\n");
	printf("*******************************��ܰ��ʾ\n");
	printf("1.�벻Ҫ�ڴ����к������ע��\n");
	printf("2.���ӳ�䰴˳��ִ�У�������Ҫ�����ļ���ʼ�ĵ�һ�м���jmp _start\n");
	printf("3.�ļ������������END�������������벻���Ĵ���Ŷ(�������һ��ָ�������)\n");
	printf("*******************************\n");
}


void init() {
	//����ӳ��
	reg_to_bit["$zero"] = $zero;
	reg_to_bit["$at"] = $at;
	reg_to_bit["$v0"] = $v0;
	reg_to_bit["$v1"] = $v1;
	reg_to_bit["$a0"] = $a0;
	reg_to_bit["$a1"] = $a1;
	reg_to_bit["$a2"] = $a2;
	reg_to_bit["$a3"] = $a3;
	reg_to_bit["$t2"] = $t2;
	reg_to_bit["$t0"] = $t0;
	reg_to_bit["$t1"] = $t1;
	reg_to_bit["$t3"] = $t3;
	reg_to_bit["$t4"] = $t4;
	reg_to_bit["$t5"] = $t5;
	reg_to_bit["$t6"] = $t6;
	reg_to_bit["$t7"] = $t7;
	reg_to_bit["$s0"] = $s0;
	reg_to_bit["$s1"] = $s1;
	reg_to_bit["$s2"] = $s2;
	reg_to_bit["$s3"] = $s3;
	reg_to_bit["$s4"] = $s4;
	reg_to_bit["$s5"] = $s5;
	reg_to_bit["$s6"] = $s6;
	reg_to_bit["$s7"] = $s7;
	reg_to_bit["$t8"] = $t8;
	reg_to_bit["$t9"] = $t9;
	reg_to_bit["$k0"] = $k0;
	reg_to_bit["$k1"] = $k1;
	reg_to_bit["$gp"] = $gp;
	reg_to_bit["$sp"] = $sp;
	reg_to_bit["$s8"] = $s8;
	reg_to_bit["$fp"] = $fp;
	reg_to_bit["$ra"] = $ra;
	///////////////////////
	ins_to_bit["add"] = add;
	ins_to_bit["sub"] = sub;
	ins_to_bit["and"] = and;
	ins_to_bit["or"] = or ;
	ins_to_bit["xor"] = xor;
	ins_to_bit["slt"] = slt;
	ins_to_bit["seq"] = seq;
	ins_to_bit["sgt"] = sgt;
	ins_to_bit["sll"] = sll;
	ins_to_bit["srl"] = srl;
	ins_to_bit["sra"] = sra;
	ins_to_bit["jr"] = jr;

	ins_to_bit["addi"] = addi;
	ins_to_bit["subi"] = subi;
	ins_to_bit["andi"] = andi;
	ins_to_bit["ori"] = ori;
	ins_to_bit["xori"] = xori;
	ins_to_bit["slti"] = slti;
	ins_to_bit["seqi"] = seqi;
	ins_to_bit["sgti"] = sgti;
	ins_to_bit["slli"] = slli;
	ins_to_bit["srli"] = srli;
	ins_to_bit["srai"] = srai;
	ins_to_bit["lw"] = lw;
	ins_to_bit["sw"] = sw;
	ins_to_bit["beq"] = beq;

	ins_to_bit["j"] = j;
	ins_to_bit["jmp"] = jmp;
	///////////////////////
	ins_to_type["add"] = 'R';
	ins_to_type["sub"] = 'R';
	ins_to_type["and"] = 'R';
	ins_to_type["or"] = 'R';
	ins_to_type["xor"] = 'R';
	ins_to_type["slt"] = 'R';
	ins_to_type["seq"] = 'R';
	ins_to_type["sgt"] = 'R';
	ins_to_type["sll"] = 'r';
	ins_to_type["srl"] = 'r';
	ins_to_type["sra"] = 'r';
	ins_to_type["jr"] = 'r';

	ins_to_type["addi"] = 'I';
	ins_to_type["subi"] = 'I';
	ins_to_type["andi"] = 'I';
	ins_to_type["ori"] = 'I';
	ins_to_type["xori"] = 'I';
	ins_to_type["slti"] = 'I';
	ins_to_type["seqi"] = 'I';
	ins_to_type["sgti"] = 'I';
	ins_to_type["slli"] = 'I';
	ins_to_type["srli"] = 'I';
	ins_to_type["srai"] = 'I';
	ins_to_type["lw"] = 'I';
	ins_to_type["sw"] = 'I';
	ins_to_type["beq"] = 'I';

	ins_to_type["j"] = 'J';
	ins_to_type["jmp"] = 'J';

}


/*
000000  10001 10010 10000 00000 000001
000000  10001 10010 10000 00000 000010
000000  10001 10010 10000 00000 000011
000000  10001 10010 10000 00000 000100
000000  10001 10010 10000 00000 000101
000000  10001 10010 10000 00000 001001
000000  10001 10010 10000 00000 001010
000000  10001 10010 10000 00000 001011

000000  00000 10001 10000 01010 000110
000000  00000 10001 10000 00110 000111
000000  00000 10001 10000 01001 001000

000000  10000 00000 00000 00000 010000


000001  10001 10000 0000000001010110
000010  10001 10000 0000000001010110
000011  10001 10000 0000000001010110
000100  10001 10000 0000000001010110
000101  10001 10000 0000000001010110
001001  10001 10000 0000000001010110
001010  10001 10000 0000000001010110
001011  10001 10000 0000000001010110
000110  10001 10000 0000000000001010
000111  10001 10000 0000000000000110
001000  10001 10000 0000000000001001

010000  10001 10000 0000000000001101
010001  10001 10000 0000000000001101
100000  10001 10000 0000000000001101


100001  00000000000000000011010010
100001  00000000000000000000000000
*/