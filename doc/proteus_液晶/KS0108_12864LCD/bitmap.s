	.module bitmap.c
	.area text(rom, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\bitmap.c
	.dbfunc e lcd_bitmap _lcd_bitmap fV
	.dbstruct 0 4 IMG_DEF
	.dbfield 0 width_in_pixels kc
	.dbfield 1 height_in_pixels kc
	.dbfield 2 char_table pkc
	.dbend
;         heigth -> y+2
;              h -> R22
;          width -> y+1
;         ptable -> R12,R13
;        pattern -> R20
;              w -> R14
;           mask -> R10
;           mode -> y+19
;        img_ptr -> y+17
;            top -> y+15
;           left -> y+13
	.even
_lcd_bitmap::
	xcall push_arg4
	xcall push_gset5
	sbiw R28,3
	.dbline -1
	.dbline 13
; //*****************************************************************
; //本文件实现了显示位图的基本数据结构和加载位图的函数
; //                                    ---北京交通大学电气学院 杨罡
; //*****************************************************************
; 
; #include "bitmap.h"
; 
; 
; /* Draws a bitmap into the Framebuffer.
;    Bitmaps are converted from Windows BMP-Format to 
;    C-Arrays with the fontgen-tool (see files bmp.h/bmp.c) */
; void lcd_bitmap( uint8_t left,  uint8_t top, const struct IMG_DEF *img_ptr, uint8_t mode)
;  { uint8_t width, heigth, h, w, pattern, mask;
	.dbline 16
;    uint8_t* ptable;
; 
;    width  = pgm_read_byte( &(img_ptr->width_in_pixels) );
	ldd R16,y+17
	ldd R17,y+18
	xcall _pgm_read_byte
	std y+1,R16
	.dbline 17
;    heigth = pgm_read_byte( &(img_ptr->height_in_pixels) );
	ldd R16,y+17
	ldd R17,y+18
	subi R16,255  ; offset = 1
	sbci R17,255
	xcall _pgm_read_byte
	std y+2,R16
	.dbline 18
;    ptable  = (uint8_t*) pgm_read_word( (uint16_t*)(&(img_ptr->char_table)) ); 
	ldd R16,y+17
	ldd R17,y+18
	subi R16,254  ; offset = 2
	sbci R17,255
	xcall _pgm_read_word
	movw R12,R16
	.dbline 21
;    //ptable  = (uint8_t*) pgm_read_word( &(img_ptr->char_table) ); 
; 
;    for ( h=0; h<heigth; h++ ) 
	clr R22
	xjmp L5
L2:
	.dbline 22
;     { mask = 0x80;
	.dbline 22
	ldi R24,128
	mov R10,R24
	.dbline 23
;       pattern = pgm_read_byte( ptable );
	movw R16,R12
	xcall _pgm_read_byte
	mov R20,R16
	.dbline 24
;       ptable++;
	movw R24,R12
	adiw R24,1
	movw R12,R24
	.dbline 25
;       for ( w=0; w<width; w++ ) 
	clr R14
	xjmp L9
L6:
	.dbline 26
;        { if ( pattern & mask ) 
	.dbline 26
	mov R2,R20
	and R2,R10
	breq L10
	.dbline 27
;             lcd_dot(w+left, h+top, mode);
	ldd R0,y+19
	std y+0,R0
	mov R18,R22
	ldd R0,y+15
	add R18,R0
	mov R16,R14
	ldd R0,y+13
	add R16,R0
	xcall _lcd_dot
L10:
	.dbline 29
; 
;          mask >>= 1;
	lsr R10
	.dbline 30
;          if ( mask == 0 ) 
	brne L12
	.dbline 31
;           { mask = 0x80;
	.dbline 31
	ldi R24,128
	mov R10,R24
	.dbline 32
;             pattern = pgm_read_byte( ptable );
	movw R16,R12
	xcall _pgm_read_byte
	mov R20,R16
	.dbline 33
;             ptable++;
	movw R24,R12
	adiw R24,1
	movw R12,R24
	.dbline 34
;           }
L12:
	.dbline 35
L7:
	.dbline 25
	inc R14
L9:
	.dbline 25
	ldd R0,y+1
	cp R14,R0
	brlo L6
	.dbline 36
L3:
	.dbline 21
	inc R22
L5:
	.dbline 21
	ldd R0,y+2
	cp R22,R0
	brlo L2
	.dbline -2
L1:
	adiw R28,3
	xcall pop_gset5
	adiw R28,4
	.dbline 0 ; func end
	ret
	.dbsym l heigth 2 c
	.dbsym r h 22 c
	.dbsym l width 1 c
	.dbsym r ptable 12 pc
	.dbsym r pattern 20 c
	.dbsym r w 14 c
	.dbsym r mask 10 c
	.dbsym l mode 19 c
	.dbsym l img_ptr 17 pkS[IMG_DEF]
	.dbsym l top 15 c
	.dbsym l left 13 c
	.dbend
	.area lit(rom, con, rel)
_icon1_char_table::
	.byte 0
	.dbsym e icon1_char_table _icon1_char_table A[1:1]kc
_icon1_bmp::
	.byte 1
	.byte 1
	.word _icon1_char_table
	.dbsym e icon1_bmp _icon1_bmp kS[IMG_DEF]
_frame_char_table::
	.byte 0
	.dbsym e frame_char_table _frame_char_table A[1:1]kc
_frame_bmp::
	.byte 1
	.byte 1
	.word _frame_char_table
	.dbsym e frame_bmp _frame_bmp kS[IMG_DEF]
