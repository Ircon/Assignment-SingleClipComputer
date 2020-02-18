STCP EQU p2.0 ;输出存储器锁存时钟线
SHCP EQU p2.1 ;数据输入时钟线
DS_P EQU p2.2 ;数据

;#### interrupt ####
org 0000H
	ljmp init



;#### Code ####
org 0030H
init:
	;move stack pointer
	mov SP,#30H
	
main:
	
	ljmp $
	
	
	
;================
;
;  function
;
;================

;^^^^ ^^^^ ^^^^ ^^^^
; in -- A
; out -- A
switch_to_dig:
	;protest DPTR
	push DPH
	push DPL
	
	MOV DPTR,#NUM_DIG
	MOVC A,@A+DPTR
	
	pop DPL
	pop DPH
	ret

;^^^^ ^^^^ ^^^^ ^^^^
show_num:
	clr STCP
	nop
	nop
	setb STCP
	ret

;^^^^ ^^^^ ^^^^ ^^^^
; in -- A
send_8_bit_to_dig:
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
	DB 0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H,88H,83H,0c6H,0a1H,86H,8eH
end
	