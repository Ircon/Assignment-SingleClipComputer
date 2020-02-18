	.module m32macro.c
	.area text(rom, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\m32macro.c
	.dbfunc e _delay_us __delay_us fV
;              i -> R16
	.even
__delay_us::
	.dbline -1
	.dbline 8
; //******************************************************
; //本程序实现了m32macro.h头文件中函数
; //                          ---北京交通大学电气学院 杨罡 
; //******************************************************
; #include "m32macro.h"
; 
; void _delay_us(unsigned char i)
; {
L2:
	.dbline 9
L3:
	.dbline 9
;   while(--i);
	mov R24,R16
	subi R24,1
	mov R16,R24
	tst R24
	brne L2
	.dbline -2
L1:
	.dbline 0 ; func end
	ret
	.dbsym r i 16 c
	.dbend
	.dbfunc e _delay_ms __delay_ms fV
;             ii -> R20,R21
;              i -> R16,R17
	.even
__delay_ms::
	xcall push_gset1
	.dbline -1
	.dbline 12
; }
; void _delay_ms(unsigned int i)
; {
	.dbline 14
;   unsigned int ii;
;   for(;i>0;--i)
	xjmp L9
L6:
	.dbline 15
	ldi R20,500
	ldi R21,1
	xjmp L13
L10:
	.dbline 15
L11:
	.dbline 15
	subi R20,1
	sbci R21,0
L13:
	.dbline 15
	cpi R20,0
	cpc R20,R21
	brne L10
X0:
L7:
	.dbline 14
	subi R16,1
	sbci R17,0
L9:
	.dbline 14
	cpi R16,0
	cpc R16,R17
	brne L6
X1:
	.dbline -2
L5:
	xcall pop_gset1
	.dbline 0 ; func end
	ret
	.dbsym r ii 20 i
	.dbsym r i 16 i
	.dbend
	.dbfunc e pgm_read_byte _pgm_read_byte fc
;              p -> R16,R17
	.even
_pgm_read_byte::
	.dbline -1
	.dbline 19
;     for(ii=500;ii>0;--ii);  
; }
; 
; unsigned char pgm_read_byte(const unsigned char *p)
; {
	.dbline 20
;  return *p;
	movw R30,R16
	lpm R16,Z
	.dbline -2
L14:
	.dbline 0 ; func end
	ret
	.dbsym r p 16 pkc
	.dbend
	.dbfunc e pgm_read_word _pgm_read_word fi
;              p -> R16,R17
	.even
_pgm_read_word::
	.dbline -1
	.dbline 23
; }
; unsigned int pgm_read_word( const unsigned int *p)
; {
	.dbline 24
;  return *p;
	movw R30,R16
	lpm R16,Z+
	lpm R17,Z
	.dbline -2
L15:
	.dbline 0 ; func end
	ret
	.dbsym r p 16 pki
	.dbend
