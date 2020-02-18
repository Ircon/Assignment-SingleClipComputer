; P1.0-P1.3为列线,P1.4-P1.7为行线
;-------------定义变量--------------
DS_595 EQU P2.2
SHCP_595 EQU P2.1
STCP_595 EQU P2.0
DIS_DAT  EQU 31H
DIS_ADDR EQU 32H
KEY_NUM  EQU 30H
;-------------定义地址--------------
ORG 0000H
	LJMP  MAIN
	
ORG 0030H
;-------------主函数---------------
MAIN:
	LCALL KEY_IN
	MOV  DIS_DAT,KEY_NUM
	MOV  DIS_ADDR,#00H
	LCALL SEND_LED
	CALL  DELAY
	LJMP  MAIN
;--------------固定位置送显示-----------
;32H: 位选地址
;31H: 段选数据
SEND_LED:
	MOV  A,DIS_ADDR
	MOV  DPTR,#SEG_ADDR
	MOVC A,@A+DPTR
	LCALL OUT_595
	MOV  A,DIS_DAT
	MOV  DPTR,#SEG_DAT
	MOVC A,@A+DPTR
	LCALL OUT_595
	RET
;--------------595输出---------------
OUT_595:
	LCALL WR_595 ;写595
	CLR STCP_595  ;存储寄存器输入
	NOP
	NOP
	SETB  STCP_595
	NOP
	NOP
	CLR STCP_595
	RET
;--------------写595-------------------
WR_595:
	MOV  R5,#8  ;数码管的8位
LOOP1:
	RLC A
	MOV  DS_595,C  ;送数据进595
	SETB  SHCP_595  ;移位寄存器时钟，根据DATASHEET，送数据进去要一个SCK时钟周期。
	NOP
	NOP
	CLR SHCP_595
	DJNZ  R5,LOOP1  ;8位数据是否送完
	RET
;--------------DELAY------------------
DELAY:
	MOV  R2,#02H
L0:
	MOV  R6,#00H
L1:
	MOV  R7,#00H
L2:
	DJNZ  R7,L2
	DJNZ  R6,L1
	DJNZ  R2,L0
	RET
;------------清显示------------------
DIS_CLR:
	MOV  R3,#00H
LOOP_DIS: 
	INC R3
	MOV  DIS_ADDR,R3
	MOV  DIS_DAT,#0FFH
	LCALL SEND_LED
	CJNE  R3,#08H,LOOP_DIS
	RET
;------------按键扫描----------------
KEY_IN: 
	MOV P1,#0F0H ;置列线为0，行线为1
	MOV A,P1
	ANL A,#0F0H
	MOV B,A
	MOV P1,#0FH ;置列线为1，行线为0
	MOV A,P1
	ANL A,#0FH
	ORL A,B ;高四位与低四位重新组合
	CJNE A,#0FFH,KEY_IN1 ;0FFH为末按键
	RET
KEY_IN1: 
	MOV B,A
	MOV DPTR,#KEYTABLE
	MOV R3,#0FFH ;
KEY_IN2: 
	INC R3
	MOV A,R3
	MOVC A,@A+DPTR
	CJNE A,B,KEY_IN3
	MOV A,R3 ;找到，取顺序码
	MOV KEY_NUM,A
	RET
KEY_IN3: 
	CJNE A,#0FFH,KEY_IN2 ;末完，继续查
	RET ;0FFH为结束码
;-----------------------------------
SEG_DAT: 
	DB 0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H,88H,83H,0C6H,0A1H,86H,8EH
SEG_ADDR: 
	DB 01H,02H,04H,08H,10H,20H,40H,80H,0FFH,00H
KEYTABLE:
	DB 0EEH,0EDH,0EBH,0E7H,0DEH ;0,1,2,3,4, 顺序码
	DB 0DDH,0DBH,0D7H,0BEH,0BDH ;5,6,7,8,9,

	DB 0BBH,0B7H,07EH,07DH,07BH ;A,B,C,D,E,
	DB 077H,0FFH ;F 0FF为结束码

END
