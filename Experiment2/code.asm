; P1.0-P1.3Ϊ����,P1.4-P1.7Ϊ����
;-------------�������--------------
DS_595 EQU P2.2
SHCP_595 EQU P2.1
STCP_595 EQU P2.0
DIS_DAT  EQU 31H
DIS_ADDR EQU 32H
KEY_NUM  EQU 30H
;-------------�����ַ--------------
ORG 0000H
	LJMP  MAIN
	
ORG 0030H
;-------------������---------------
MAIN:
	LCALL KEY_IN
	MOV  DIS_DAT,KEY_NUM
	MOV  DIS_ADDR,#00H
	LCALL SEND_LED
	CALL  DELAY
	LJMP  MAIN
;--------------�̶�λ������ʾ-----------
;32H: λѡ��ַ
;31H: ��ѡ����
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
;--------------595���---------------
OUT_595:
	LCALL WR_595 ;д595
	CLR STCP_595  ;�洢�Ĵ�������
	NOP
	NOP
	SETB  STCP_595
	NOP
	NOP
	CLR STCP_595
	RET
;--------------д595-------------------
WR_595:
	MOV  R5,#8  ;����ܵ�8λ
LOOP1:
	RLC A
	MOV  DS_595,C  ;�����ݽ�595
	SETB  SHCP_595  ;��λ�Ĵ���ʱ�ӣ�����DATASHEET�������ݽ�ȥҪһ��SCKʱ�����ڡ�
	NOP
	NOP
	CLR SHCP_595
	DJNZ  R5,LOOP1  ;8λ�����Ƿ�����
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
;------------����ʾ------------------
DIS_CLR:
	MOV  R3,#00H
LOOP_DIS: 
	INC R3
	MOV  DIS_ADDR,R3
	MOV  DIS_DAT,#0FFH
	LCALL SEND_LED
	CJNE  R3,#08H,LOOP_DIS
	RET
;------------����ɨ��----------------
KEY_IN: 
	MOV P1,#0F0H ;������Ϊ0������Ϊ1
	MOV A,P1
	ANL A,#0F0H
	MOV B,A
	MOV P1,#0FH ;������Ϊ1������Ϊ0
	MOV A,P1
	ANL A,#0FH
	ORL A,B ;����λ�����λ�������
	CJNE A,#0FFH,KEY_IN1 ;0FFHΪĩ����
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
	MOV A,R3 ;�ҵ���ȡ˳����
	MOV KEY_NUM,A
	RET
KEY_IN3: 
	CJNE A,#0FFH,KEY_IN2 ;ĩ�꣬������
	RET ;0FFHΪ������
;-----------------------------------
SEG_DAT: 
	DB 0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H,88H,83H,0C6H,0A1H,86H,8EH
SEG_ADDR: 
	DB 01H,02H,04H,08H,10H,20H,40H,80H,0FFH,00H
KEYTABLE:
	DB 0EEH,0EDH,0EBH,0E7H,0DEH ;0,1,2,3,4, ˳����
	DB 0DDH,0DBH,0D7H,0BEH,0BDH ;5,6,7,8,9,

	DB 0BBH,0B7H,07EH,07DH,07BH ;A,B,C,D,E,
	DB 077H,0FFH ;F 0FFΪ������

END
