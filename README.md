# Simple-MIPS-and-assembler-logisim
简易MIPS+汇编器(logisim+c/c++)
------------------------------------
V1.0
简易版MIPS的logisim实现包含以下指令：
1. R型：add,sub,and,or,xor,slt,sll,srl,sra,jr
2. I型：addi,subi,andi,ori,xori,slti,slli,srli,srai,lw,sw,beq
3. J型：j

汇编器实现汇编指令：
1. R型：add,sub,and,or,xor,slt,seq,sgt,sll,srl,sra,jr
2. I型：addi,subi,andi,ori,xori,slti,seqi,sgti,slli,srli,srai,lw,sw,beq
3. J型：j,jmp

目前为止，已经初步验证了简易MIPS和汇编器的正确性
(实验过程中难免会有疏漏，请各位读者留意，希望读者在测试后发现bug能及时反馈)

目录结构：
------------------------------------
1. 汇编器：./MIPS简易汇编器/MIPS简易汇编器.exe
2. 简易版MIPS：./简易MIPS.circ
3. 2进制指令转16进制指令：./MIPS简易汇编器/(32位)2进制转16进制.exe

鸣谢
------------------------------------
1. https://blog.csdn.net/wbwwf8685/article/details/53762908  --  MIPS寄存器.jpg
2. https://blog.csdn.net/flyingqr/article/details/7073088  --  MIPS寄存器_cn.png
3. https://github.com/FlyMachinee/MIPS-tutorial

注意事项
------------------------------------
1. 读者需自行下载 Logisim 软件
2. 本MIPS实现完全按照：https://github.com/FlyMachinee/MIPS-tutorial的实现重现，还实现了其对应的汇编器

软件开发
------------------------------------
1. 汇编器：Visual Studio 2022
2. MIPS ：logisim2.7.1


