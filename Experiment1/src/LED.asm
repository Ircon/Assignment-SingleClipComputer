ORG 0000H
	LJMP START 

ORG 0100H
START:
	MOV A,#0FFH 
	CLR C ;状态寄存器的psw的cy位清零
	MOV R4,#08H 
LOOP1:
	RRC A
	MOV P1,A 
	LCALL DELAY
	DJNZ R4,LOOP1 ;R4-1后，如果为0不转移
	MOV A,#0FFH
	CLR C
	MOV R4,#08H 
LOOP2:
	RLC A
	MOV P1,A
	LCALL DELAY
	DJNZ R4,LOOP2
	JMP START

DELAY:
	MOV R1,#20H
D0:
	MOV R2,#00H
D1:
	MOV R3,#00H ;?
	DJNZ R3,$
	DJNZ R2,D1
	DJNZ R1,D0
	RET

END