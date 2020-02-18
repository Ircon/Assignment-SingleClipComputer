;栈从 50H开始
;30H 40H 随便用
;------RAM表-------
CNT 				DATA 30H
SECOND 				DATA 31H
MINUTE 				DATA 32H
HOUR 				DATA 33H
TIMES				DATA 34H
;------引脚表--------
STCP 				EQU p2.0 ;输出存储器锁存时钟线
SHCP 				EQU p2.1 ;数据输入时钟线
DS_P 				EQU p2.2 ;数据

;-----常量表--------
B_DIR				EQU PSW.7
B_REG				EQU 0F0H
COS_SEC 			EQU 20H
COS_TMOD_FUN_WORD 	EQU 00000100B
COS_TH0 			EQU 03H
COS_TL0 			EQU 15H
COS_ZERO			EQU 0H

;#### interrupt ####
ORG 0000H
	LJMP INIT
ORG 000BH
	LCALL T0_int
org 001BH
	LCALL T1_INT
;#### Code ####
ORG 0030H
INIT:
	;move stack pointer
	MOV SP,#50H
	
	;初始化RAM
	MOV CNT,#COS_ZERO
	MOV SECOND,#COS_ZERO
	MOV MINUTE,#COS_ZERO
	MOV HOUR,#COS_ZERO
	MOV TIMES,#COS_ZERO
	
MAIN:
	CLR DS_P
	MOV R1,#64
CLS_DIG:
	SETB SHCP
	NOP
	NOP
	CLR SHCP
	DJNZ R1,CLS_DIG
	SETB STCP
	NOP
	NOP
	CLR STCP
	
	MOV DPTR,#NUM_DIG
	MOV A,#01H
	MOVC A,@A+DPTR
	MOV R1,#8
	MOV A,#010H
LOP1:
	RRC A
	MOV DS_P,C
;	CLR DS_P
	SETB SHCP
	NOP
	NOP
	CLR SHCP
	DJNZ R1,LOP1
	
	SETB STCP
	NOP
	NOP
	CLR STCP
	
;	MOV TMOD,#COS_TMOD_FUN_WORD
;	MOV TH0,#COS_TH0
;	MOV TL0,#COS_TL0
;	SETB TR0
;	
;	MOV TH1,#COS_ZERO
;	MOV TL1,#COS_ZERO
;	SETB TR1
	
;	LCALL NUM_SHOW
	
	LJMP $
	
	
	
;================
;
;  function
;
;================

;T1 INT
T1_INT:
	MOV A,TIMES
	INC A
	MOV A,TIMES
	LCALL NUM_SHOW
	RETI
; B HI A LO
SPILT_NUM:
	DA A
	MOV B,A
	ANL A,#0FH
	ANL B,#0F0H
	
	XCH A,B
	RR A
	RR A
	RR A
	RR A
	XCH A,B
	
	RET
;^^^^^^^^^^
;数字显示
NUM_SHOW:
	PUSH ACC
	PUSH B
	
	MOV A,TIMES
	LCALL SPILT_NUM
	
	LCALL SWI_TO_DIG
	LCALL SEND_BYTE_DIG
	
	MOV A,SECOND
	LCALL SPILT_NUM
	
	LCALL SWI_TO_DIG
	LCALL SEND_BYTE_DIG
	
	MOV A,B
	LCALL SWI_TO_DIG
	LCALL SEND_BYTE_DIG
	
	MOV A,MINUTE
	LCALL SPILT_NUM
	
	LCALL SWI_TO_DIG
	LCALL SEND_BYTE_DIG
	
	MOV A,B
	LCALL SWI_TO_DIG
	LCALL SEND_BYTE_DIG
	
	MOV A,HOUR
	LCALL SPILT_NUM
	
	LCALL SWI_TO_DIG
	LCALL SEND_BYTE_DIG
	
	MOV A,B
	LCALL SWI_TO_DIG
	LCALL SEND_BYTE_DIG
	
	LCALL PUT_NUM
	
	POP B
	POP ACC
	RET
;^^^^^^^^^^
;数字inc 1处理
NUM_INC_1:
	PUSH ACC
	PUSH PSW
	
	CLR C
	INC SECOND
	MOV A,#60
	SUBB A,SECOND
	JNB B_DIR,NUM_INC_1_END
	
	INC MINUTE
	MOV SECOND,00H
	
	CLR C
	MOV ACC,60
	SUBB A,MINUTE
	JNB B_DIR,NUM_INC_1_END
	
	INC HOUR
	MOV MINUTE,00H
	
	LCALL NUM_SHOW
NUM_INC_1_END:
	POP PSW
	POP ACC
	RET
	
;^^^^^^^^^^
;计数器中断
T0_INT:
	push ACC
	mov A,CNT
	cjne A,#COS_SEC,TO_NOK
	jc T0_OK
TO_NOK:
	;计数处理
	SETB TR0
	LCALL NUM_INC_1
	MOV A,0H
T0_OK:
	INC A
	
	MOV CNT,A
	POP ACC
	RETI

;^^^^ ^^^^ ^^^^ ^^^^
; in -- A
; out -- A
SWI_TO_DIG:
	;protest DPTR
	push DPH
	push DPL
	
	MOV DPTR,#NUM_DIG
	MOVC A,@A+DPTR
	
	pop DPL
	pop DPH
	RET

;^^^^ ^^^^ ^^^^ ^^^^
PUT_NUM:
	clr STCP
	;setb STCP
	nop
	nop
	setb STCP
	;clr STCP
	ret

;^^^^ ^^^^ ^^^^ ^^^^
; in -- A
SEND_BYTE_DIG:
	;portest reg
	push 01H
	push ACC
	
	; times of loop
	mov R1,#8H
	
	
send_lop1:
	rlc A
	mov DS_P,C
	
	;send one bit
	clr SHCP
	nop
	nop
	setb SHCP
	
	djnz R1,send_lop1
	
	pop ACC
	pop 01H
	ret

;================
;
;  Data
;
;================
NUM_DIG:
	DB 0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H,88H,83H,0C6H,0A1H,86H,8EH
end
	