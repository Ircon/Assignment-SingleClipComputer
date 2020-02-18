	.module graphic.c
	.area data(ram, con, rel)
_l_mask_array::
	.blkb 2
	.area idata
	.byte 1,2
	.area data(ram, con, rel)
	.blkb 2
	.area idata
	.byte 4,8
	.area data(ram, con, rel)
	.blkb 2
	.area idata
	.byte 16,32
	.area data(ram, con, rel)
	.blkb 2
	.area idata
	.byte 64,128
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\graphic.c
	.dbsym e l_mask_array _l_mask_array A[8:8]c
_l_display_array::
	.blkb 2
	.area idata
	.byte 0,0
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\graphic.c
	.blkb 1
	.area idata
	.byte 0
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\graphic.c
	.blkb 125
	.area idata
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\graphic.c
	.blkb 896
	.area idata
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.word 0,0,0,0,0
	.word 0,0,0,0,0
	.word 0,0,0,0,0
	.word 0,0,0,0,0
	.word 0,0,0,0,0
	.byte 0,0,0,0,0,0
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\graphic.c
	.dbsym e l_display_array _l_display_array A[1024:8:128]c
	.area text(rom, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\graphic.c
	.dbfunc e lcd_fill _lcd_fill fV
;           page -> R20
;            col -> R10
;        pattern -> R22
	.even
_lcd_fill::
	xcall push_gset3
	mov R22,R16
	.dbline -1
	.dbline 14
; //*****************************************************************
; //本头文件实现了用于液晶显示的缓冲区和基本的图形函数
; //包括了画点，画线，画圆，画矩形等函数，每种函数都有三种方式画
; //即 LCD_MODE_CLEAR， LCD_MODE_SET， LCD_MODE_XOR（清除，置位，异或）
; //             ---北京交通大学电气学院 杨罡 2008.8
; //*****************************************************************
; #include  "graphic.h"
; 
;  unsigned char l_mask_array[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
;  unsigned char l_display_array[LCD_Y_BYTES][LCD_X_BYTES]={0,0,0};
;  /* TODO: avoid or PROGMEM */
;  /* fill buffer and LCD with pattern */
; void lcd_fill( unsigned char pattern)
;  { unsigned char page, col;
	.dbline 16
; 
;    for (page=0; page<LCD_Y_BYTES; page++) 
	clr R20
	xjmp L5
L2:
	.dbline 17
;     { for (col=0; col<LCD_X_BYTES; col++) 
	.dbline 17
	clr R10
	xjmp L9
L6:
	.dbline 18
	ldi R24,128
	mul R24,R20
	movw R2,R0
	ldi R24,<_l_display_array
	ldi R25,>_l_display_array
	add R2,R24
	adc R3,R25
	mov R30,R10
	clr R31
	add R30,R2
	adc R31,R3
	std z+0,R22
L7:
	.dbline 17
	inc R10
L9:
	.dbline 17
	mov R24,R10
	cpi R24,128
	brlo L6
	.dbline 19
L3:
	.dbline 16
	inc R20
L5:
	.dbline 16
	cpi R20,8
	brlo L2
	.dbline 20
;          l_display_array[page][col]=pattern;
;     }
;    lcd_update_all();
	xcall _lcd_update_all
	.dbline -2
L1:
	xcall pop_gset3
	.dbline 0 ; func end
	ret
	.dbsym r page 20 c
	.dbsym r col 10 c
	.dbsym r pattern 22 c
	.dbend
	.dbfunc e lcd_erase _lcd_erase fV
	.even
_lcd_erase::
	.dbline -1
	.dbline 25
;  }
; 
; 
; void lcd_erase(void)
;  { lcd_fill(0x00);
	.dbline 25
	clr R16
	xcall _lcd_fill
	.dbline 26
;    lcd_update_all();
	xcall _lcd_update_all
	.dbline -2
L10:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e lcd_update _lcd_update fV
;             yt -> R10
;             yb -> R20
;              y -> R22
;         colptr -> R10,R11
;              x -> R12
;         bottom -> R20
;            top -> R22
	.even
_lcd_update::
	xcall push_gset4
	mov R20,R18
	mov R22,R16
	.dbline -1
	.dbline 41
;  }
; 
; /*
; **
; ** Updates area of the display. Writes data from "framebuffer" 
; ** RAM to the lcd display controller RAM.
; ** 
; ** Arguments Used:
; **    top     top line of area to update.
; **    bottom  bottom line of area to update.
; **    from MJK-Code
; **
; */
; void lcd_update( unsigned char top,  unsigned char bottom)
;  { unsigned char x;
	.dbline 48
;    unsigned char y;
;    unsigned char yt;
;    unsigned char yb;
;    unsigned char *colptr;
;   
;    /* setup bytes of range */
;    yb = bottom >> 3;
	lsr R20
	lsr R20
	lsr R20
	.dbline 49
;    yt = top >> 3;		
	mov R10,R22
	lsr R10
	lsr R10
	lsr R10
	.dbline 51
; 
;    for(y = yt; y <= yb; y++)
	mov R22,R10
	xjmp L15
L12:
	.dbline 52
;     {lcd_out_ctl(LCD_SET_PAGE+y,3);	    /* set page */
	.dbline 52
	ldi R18,3
	mov R16,R22
	subi R16,72    ; addi 184
	xcall _lcd_out_ctl
	.dbline 54
;                                        //     lcd_out_ctl(LCD_SET_COL+LCD_STARTCOL_REVERSE,3);
;      lcd_out_ctl(LCD_SET_COL+0,3);
	ldi R18,3
	ldi R16,64
	xcall _lcd_out_ctl
	.dbline 56
; 	 
;      colptr = &l_display_array[y][0];
	ldi R24,128
	mul R24,R22
	movw R10,R0
	ldi R24,<_l_display_array
	ldi R25,>_l_display_array
	add R10,R24
	adc R11,R25
	.dbline 58
; 
;       for (x=0; x < LCD_X_BYTES; x++)
	clr R12
	xjmp L19
L16:
	.dbline 59
;        { if ( x <LCD_X_BYTES/2 ) 
	.dbline 59
	mov R24,R12
	cpi R24,64
	brsh L20
	.dbline 60
;             lcd_out_dat_l(*colptr++);
	movw R30,R10
	ld R16,Z+
	movw R10,R30
	xcall _lcd_out_dat_l
	xjmp L21
L20:
	.dbline 62
;          else 
;             lcd_out_dat_r(*colptr++);
	movw R30,R10
	ld R16,Z+
	movw R10,R30
	xcall _lcd_out_dat_r
L21:
	.dbline 63
L17:
	.dbline 58
	inc R12
L19:
	.dbline 58
	mov R24,R12
	cpi R24,128
	brlo L16
	.dbline 65
L13:
	.dbline 51
	inc R22
L15:
	.dbline 51
	cp R20,R22
	brsh L12
	.dbline -2
L11:
	xcall pop_gset4
	.dbline 0 ; func end
	ret
	.dbsym r yt 10 c
	.dbsym r yb 20 c
	.dbsym r y 22 c
	.dbsym r colptr 10 pc
	.dbsym r x 12 c
	.dbsym r bottom 20 c
	.dbsym r top 22 c
	.dbend
	.dbfunc e lcd_update_all _lcd_update_all fV
	.even
_lcd_update_all::
	.dbline -1
	.dbline 71
;        }
;  
;     }
;  }
; 
; 
; 
; void lcd_update_all(void)
;  { lcd_update(SCRN_TOP,SCRN_BOTTOM);
	.dbline 71
	ldi R18,63
	clr R16
	xcall _lcd_update
	.dbline -2
L22:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e lcd_dot _lcd_dot fV
;        pBuffer -> R20,R21
;        bitmask -> R22
;          yByte -> R20
;         bitnum -> R22
;           mode -> y+6
;              y -> R18
;              x -> R16
	.even
_lcd_dot::
	xcall push_gset3
	.dbline -1
	.dbline 78
;  }
; 
; 
; 
; /* sets/clears/switchs(XOR) dot at (x,y) */
; void lcd_dot( unsigned char x,  unsigned char y,  unsigned char mode) 
;  { unsigned char bitnum, bitmask, yByte;
	.dbline 81
	ldi R24,127
	cp R24,R16
	brlo L26
	ldi R24,63
	cp R24,R18
	brsh L24
L26:
	.dbline 81
;    unsigned char *pBuffer; /* pointer used for optimisation */
; 
;    if ( ( x > SCRN_RIGHT ) || ( y > SCRN_BOTTOM ) ) return;
	xjmp L23
L24:
	.dbline 83
; 
;    yByte   = y >> 3; 
	mov R20,R18
	lsr R20
	lsr R20
	lsr R20
	.dbline 84
;    bitnum  = y & 0x07;
	mov R22,R18
	andi R22,7
	.dbline 85
;    bitmask = l_mask_array[bitnum]; // bitmask = ( 1 << (y & 0x07) );
	ldi R24,<_l_mask_array
	ldi R25,>_l_mask_array
	mov R30,R22
	clr R31
	add R30,R24
	adc R31,R25
	ldd R22,z+0
	.dbline 86
;    pBuffer = &(l_display_array[yByte][x]);
	ldi R24,128
	mul R24,R20
	movw R2,R0
	ldi R24,<_l_display_array
	ldi R25,>_l_display_array
	add R2,R24
	adc R3,R25
	mov R20,R16
	clr R21
	add R20,R2
	adc R21,R3
	.dbline 87
;    switch (mode) 
	ldd R10,y+6
	clr R11
	tst R10
	brne X0
	tst R11
	breq L31
X0:
	movw R24,R10
	cpi R24,1
	ldi R30,0
	cpc R25,R30
	breq L30
	cpi R24,2
	ldi R30,0
	cpc R25,R30
	breq L32
	xjmp L28
X1:
	.dbline 88
;     { case LCD_MODE_SET:
L30:
	.dbline 89
;          *pBuffer |= bitmask;
	movw R30,R20
	ldd R2,z+0
	or R2,R22
	std z+0,R2
	.dbline 90
;          break;
	xjmp L28
L31:
	.dbline 92
;       case LCD_MODE_CLEAR:
;          *pBuffer &= ~bitmask;
	mov R2,R22
	com R2
	movw R30,R20
	ldd R3,z+0
	and R3,R2
	std z+0,R3
	.dbline 93
;          break;
	xjmp L28
L32:
	.dbline 95
	movw R30,R20
	ldd R2,z+0
	eor R2,R22
	std z+0,R2
	.dbline 96
	.dbline 97
L28:
	.dbline -2
L23:
	xcall pop_gset3
	.dbline 0 ; func end
	ret
	.dbsym r pBuffer 20 pc
	.dbsym r bitmask 22 c
	.dbsym r yByte 20 c
	.dbsym r bitnum 22 c
	.dbsym l mode 6 c
	.dbsym r y 18 c
	.dbsym r x 16 c
	.dbend
	.dbfunc e lcd_line _lcd_line fV
;           yTmp -> y+6
;           xTmp -> y+5
;           yAlt -> R20
;              m -> R22,R23
;              y -> R10
;         length -> R12
;              i -> R14
;           mode -> y+23
;             y2 -> y+21
;             x2 -> y+19
;             y1 -> y+17
;             x1 -> R8
	.even
_lcd_line::
	st -y,r19
	st -y,r18
	xcall push_gset5
	mov R8,R16
	sbiw R28,7
	.dbline -1
	.dbline 105
;       case LCD_MODE_XOR:
;          *pBuffer ^= bitmask;
;          break;
;       default: break;
;     }         
;  }
; 
; 
; /* line- and circle-function from a KS0108-library by F. Thiele */
; 
; void lcd_line( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,  uint8_t mode ) 
;  { uint8_t length, xTmp, yTmp, i, y, yAlt;
	.dbline 108
;    int16_t m;
;    
;    if(x1 == x2) 
	ldd R0,y+19
	cp R8,R0
	brne L34
	.dbline 109
;     { // vertical line
	.dbline 111
;       // x1|y1 must be the upper point
;       if(y1 > y2) 
	ldd R0,y+21
	ldd R16,y+17
	cp R0,R16
	brsh L36
	.dbline 112
;        { xTmp = x1;
	.dbline 112
	std y+5,R8
	.dbline 113
;          yTmp = y1;
	ldd R0,y+17
	std y+6,R0
	.dbline 114
;          x1 = x2;
	ldd R8,y+19
	.dbline 115
;          y1 = y2;
	ldd R0,y+21
	std y+17,R0
	.dbline 116
;          x2 = xTmp;
	ldd R0,y+5
	std y+19,R0
	.dbline 117
;          y2 = yTmp;
	ldd R0,y+6
	std y+21,R0
	.dbline 118
;        }
L36:
	.dbline 119
;       length = y2-y1;
	ldd R12,y+21
	ldd R0,y+17
	sub R12,R0
	.dbline 120
;       for(i=0; i<=length; i++) 
	clr R14
	xjmp L41
L38:
	.dbline 121
	ldd R0,y+23
	std y+0,R0
	ldd R18,y+17
	add R18,R14
	mov R16,R8
	push R16
	xcall _lcd_dot
	pop R8
L39:
	.dbline 120
	inc R14
L41:
	.dbline 120
	cp R12,R14
	brsh L38
	.dbline 122
;          lcd_dot(x1, y1+i, mode);
;     } 
	xjmp L35
L34:
	.dbline 123
;    else if(y1 == y2) 
	ldd R0,y+17
	ldd R16,y+21
	cp R0,R16
	brne L42
	.dbline 124
;     { // horizontal line
	.dbline 126
;       // x1|y1 must be the left point
;       if(x1 > x2) 
	ldd R0,y+19
	cp R0,R8
	brsh L44
	.dbline 127
;        { xTmp = x1;
	.dbline 127
	std y+5,R8
	.dbline 128
;          yTmp = y1;
	ldd R0,y+17
	std y+6,R0
	.dbline 129
;          x1 = x2;
	ldd R8,y+19
	.dbline 130
;          y1 = y2;
	ldd R0,y+21
	std y+17,R0
	.dbline 131
;          x2 = xTmp;
	ldd R0,y+5
	std y+19,R0
	.dbline 132
;          y2 = yTmp;
	ldd R0,y+6
	std y+21,R0
	.dbline 133
;        }
L44:
	.dbline 135
; 
;       length = x2-x1;
	ldd R12,y+19
	sub R12,R8
	.dbline 136
;       for(i=0; i<=length; i++) 
	clr R14
	xjmp L49
L46:
	.dbline 137
	ldd R0,y+23
	std y+0,R0
	ldd R18,y+17
	mov R16,R8
	add R16,R14
	push R8
	xcall _lcd_dot
	pop R8
L47:
	.dbline 136
	inc R14
L49:
	.dbline 136
	cp R12,R14
	brsh L46
	.dbline 139
;          lcd_dot(x1+i, y1, mode);
;        
;     } 
	xjmp L43
L42:
	.dbline 141
;    else 
;     { // x1 must be smaller than x2
	.dbline 142
;       if(x1 > x2) 
	ldd R0,y+19
	cp R0,R8
	brsh L50
	.dbline 143
;        { xTmp = x1;
	.dbline 143
	std y+5,R8
	.dbline 144
;          yTmp = y1;
	ldd R0,y+17
	std y+6,R0
	.dbline 145
;          x1 = x2;
	ldd R8,y+19
	.dbline 146
;          y1 = y2;
	ldd R0,y+21
	std y+17,R0
	.dbline 147
;          x2 = xTmp;
	ldd R0,y+5
	std y+19,R0
	.dbline 148
;          y2 = yTmp;
	ldd R0,y+6
	std y+21,R0
	.dbline 149
;        }
L50:
	.dbline 151
; 		
;       if((y2-y1) >= (x2-x1) || (y1-y2) >= (x2-x1)) 
	mov R2,R8
	clr R3
	ldd R4,y+19
	clr R5
	sub R4,R2
	sbc R5,R3
	ldd R2,y+17
	clr R3
	ldd R6,y+21
	clr R7
	sub R6,R2
	sbc R7,R3
	cp R6,R4
	cpc R7,R5
	brge L54
	ldd R2,y+21
	clr R3
	ldd R6,y+17
	clr R7
	sub R6,R2
	sbc R7,R3
	cp R6,R4
	cpc R7,R5
	brge X2
	xjmp L52
X2:
L54:
	.dbline 152
;        { // angle larger or equal 45?
	.dbline 153
;          length = x2-x1;								// not really the length :)
	ldd R12,y+19
	sub R12,R8
	.dbline 154
;          m = ((y2-y1)*200)/length;
	ldd R2,y+21
	ldd R0,y+17
	sub R2,R0
	ldi R24,200
	mul R24,R2
	movw R16,R0
	mov R18,R12
	clr R19
	xcall div16s
	movw R22,R16
	.dbline 155
;          yAlt = y1;
	ldd R20,y+17
	.dbline 156
;          for(i=0; i<=length; i++) 
	clr R14
	xjmp L58
L55:
	.dbline 157
;           { y = ((m*i)/200)+y1;
	.dbline 157
	mov R18,R14
	clr R19
	movw R16,R22
	xcall empy16s
	ldi R18,200
	ldi R19,0
	xcall div16s
	mov R10,R16
	ldd R2,y+17
	clr R3
	add R10,R2
	adc R11,R3
	.dbline 158
;             if((m*i)%200 >= 100)
	mov R18,R14
	clr R19
	movw R16,R22
	xcall empy16s
	ldi R18,200
	ldi R19,0
	xcall mod16s
	cpi R16,100
	ldi R30,0
	cpc R17,R30
	brlt L59
	.dbline 159
;                y++;
	inc R10
	xjmp L60
L59:
	.dbline 160
;             else if((m*i)%200 <= -100)
	mov R18,R14
	clr R19
	movw R16,R22
	xcall empy16s
	ldi R18,200
	ldi R19,0
	xcall mod16s
	ldi R24,-100
	ldi R25,-1
	cp R24,R16
	cpc R25,R17
	brlt L61
	.dbline 161
;                y--;
	dec R10
L61:
L60:
	.dbline 163
; 				
;             lcd_line(x1+i, yAlt, x1+i, y, mode ); /* wuff wuff recurs. */
	ldd R0,y+23
	std y+4,R0
	std y+2,R10
	mov R2,R8
	add R2,R14
	std y+0,R2
	mov R18,R20
	mov R16,R2
	push R8
	xcall _lcd_line
	pop R8
	.dbline 164
;             if(length<=(y2-y1) && y1 < y2)
	ldd R2,y+21
	ldd R0,y+17
	sub R2,R0
	cp R2,R12
	brlo L63
	ldd R16,y+21
	cp R0,R16
	brsh L63
	.dbline 165
;                yAlt = y+1;
	mov R20,R10
	subi R20,255    ; addi 1
	xjmp L64
L63:
	.dbline 166
;             else if(length<=(y1-y2) && y1 > y2)
	ldd R2,y+17
	ldd R0,y+21
	sub R2,R0
	cp R2,R12
	brlo L65
	ldd R16,y+17
	cp R0,R16
	brsh L65
	.dbline 167
;                yAlt = y-1;
	mov R20,R10
	subi R20,1
	xjmp L66
L65:
	.dbline 169
;             else
;                yAlt = y;
	mov R20,R10
L66:
L64:
	.dbline 170
L56:
	.dbline 156
	inc R14
L58:
	.dbline 156
	cp R12,R14
	brlo X3
	xjmp L55
X3:
	.dbline 171
;           }
;        } 
	xjmp L53
L52:
	.dbline 173
;       else
;        { // angle smaller 45?
	.dbline 175
;          // y1 must be smaller than y2
;          if(y1 > y2)
	ldd R0,y+21
	ldd R16,y+17
	cp R0,R16
	brsh L67
	.dbline 176
;           { xTmp = x1;
	.dbline 176
	std y+5,R8
	.dbline 177
;             yTmp = y1;
	ldd R0,y+17
	std y+6,R0
	.dbline 178
;             x1 = x2;
	ldd R8,y+19
	.dbline 179
;             y1 = y2;
	ldd R0,y+21
	std y+17,R0
	.dbline 180
;             x2 = xTmp;
	ldd R0,y+5
	std y+19,R0
	.dbline 181
;             y2 = yTmp;
	ldd R0,y+6
	std y+21,R0
	.dbline 182
;           }
L67:
	.dbline 183
;          length = y2-y1;
	ldd R12,y+21
	ldd R0,y+17
	sub R12,R0
	.dbline 184
;          m = ((x2-x1)*200)/length;
	ldd R2,y+19
	sub R2,R8
	ldi R24,200
	mul R24,R2
	movw R16,R0
	mov R18,R12
	clr R19
	xcall div16s
	movw R22,R16
	.dbline 185
;          yAlt = x1;
	mov R20,R8
	.dbline 186
;          for(i=0; i<=length; i++)
	clr R14
	xjmp L72
L69:
	.dbline 187
;           { y = ((m*i)/200)+x1;
	.dbline 187
	mov R18,R14
	clr R19
	movw R16,R22
	xcall empy16s
	ldi R18,200
	ldi R19,0
	xcall div16s
	mov R10,R16
	mov R2,R8
	clr R3
	add R10,R2
	adc R11,R3
	.dbline 189
; 
;             if((m*i)%200 >= 100)
	mov R18,R14
	clr R19
	movw R16,R22
	xcall empy16s
	ldi R18,200
	ldi R19,0
	xcall mod16s
	cpi R16,100
	ldi R30,0
	cpc R17,R30
	brlt L73
	.dbline 190
;                y++;
	inc R10
	xjmp L74
L73:
	.dbline 191
;             else if((m*i)%200 <= -100)
	mov R18,R14
	clr R19
	movw R16,R22
	xcall empy16s
	ldi R18,200
	ldi R19,0
	xcall mod16s
	ldi R24,-100
	ldi R25,-1
	cp R24,R16
	cpc R25,R17
	brlt L75
	.dbline 192
;                y--;
	dec R10
L75:
L74:
	.dbline 194
; 
;             lcd_line(yAlt, y1+i, y, y1+i, mode); /* wuff */
	ldd R0,y+23
	std y+4,R0
	ldd R2,y+17
	add R2,R14
	std y+2,R2
	std y+0,R10
	mov R18,R2
	mov R16,R20
	push R8
	xcall _lcd_line
	pop R8
	.dbline 195
;             if(length<=(x2-x1) && x1 < x2)
	ldd R2,y+19
	sub R2,R8
	cp R2,R12
	brlo L77
	ldd R0,y+19
	cp R8,R0
	brsh L77
	.dbline 196
;                yAlt = y+1;
	mov R20,R10
	subi R20,255    ; addi 1
	xjmp L78
L77:
	.dbline 197
;             else if(length<=(x1-x2) && x1 > x2)
	mov R2,R8
	ldd R0,y+19
	sub R2,R0
	cp R2,R12
	brlo L79
	cp R0,R8
	brsh L79
	.dbline 198
;                yAlt = y-1;
	mov R20,R10
	subi R20,1
	xjmp L80
L79:
	.dbline 200
;             else
;                yAlt = y;
	mov R20,R10
L80:
L78:
	.dbline 201
L70:
	.dbline 186
	inc R14
L72:
	.dbline 186
	cp R12,R14
	brlo X4
	xjmp L69
X4:
	.dbline 202
;           }
;        }
L53:
	.dbline 203
;     }
L43:
L35:
	.dbline -2
L33:
	adiw R28,7
	xcall pop_gset5
	adiw R28,2
	.dbline 0 ; func end
	ret
	.dbsym l yTmp 6 c
	.dbsym l xTmp 5 c
	.dbsym r yAlt 20 c
	.dbsym r m 22 I
	.dbsym r y 10 c
	.dbsym r length 12 c
	.dbsym r i 14 c
	.dbsym l mode 23 c
	.dbsym l y2 21 c
	.dbsym l x2 19 c
	.dbsym l y1 17 c
	.dbsym r x1 8 c
	.dbend
	.dbfunc e lcd_circle _lcd_circle fV
;        tSwitch -> R20,R21
;              d -> R22
;              y -> R10,R11
;              x -> R12,R13
;           mode -> R14
;         radius -> y+15
;        yCenter -> y+13
;        xCenter -> y+11
	.even
_lcd_circle::
	xcall push_arg4
	xcall push_gset5
	sbiw R28,1
	ldd R14,y+17
	.dbline -1
	.dbline 209
;  }
; 
; 
; void lcd_circle( uint8_t xCenter, uint8_t yCenter, 
;                  uint8_t radius,  uint8_t mode) 
;  {  int tSwitch, y, x = 0;
	.dbline 209
	clr R12
	clr R13
	.dbline 212
;    uint8_t d;
;    
;    d = yCenter - xCenter;
	ldd R22,y+13
	ldd R0,y+11
	sub R22,R0
	.dbline 213
;    y = radius;
	ldd R10,y+15
	clr R11
	.dbline 214
;    tSwitch = 3 - 2 * radius;
	ldi R24,2
	mov R0,R10
	mul R24,R0
	ldi R20,3
	ldi R21,0
	sub R20,R0
	sbc R21,R1
	xjmp L83
L82:
	.dbline 217
; 
;    while (x <= y) 
;     { lcd_dot(xCenter + x, yCenter + y, mode);
	.dbline 217
	std y+0,R14
	ldd R18,y+13
	clr R19
	add R18,R10
	adc R19,R11
	ldd R16,y+11
	clr R17
	add R16,R12
	adc R17,R13
	xcall _lcd_dot
	.dbline 218
;       lcd_dot(xCenter + x, yCenter - y, mode);
	std y+0,R14
	ldd R18,y+13
	clr R19
	sub R18,R10
	sbc R19,R11
	ldd R16,y+11
	clr R17
	add R16,R12
	adc R17,R13
	xcall _lcd_dot
	.dbline 220
; 	
;       lcd_dot(xCenter - x, yCenter + y, mode);
	std y+0,R14
	ldd R18,y+13
	clr R19
	add R18,R10
	adc R19,R11
	ldd R16,y+11
	clr R17
	sub R16,R12
	sbc R17,R13
	xcall _lcd_dot
	.dbline 221
;       lcd_dot(xCenter - x, yCenter - y, mode);
	std y+0,R14
	ldd R18,y+13
	clr R19
	sub R18,R10
	sbc R19,R11
	ldd R16,y+11
	clr R17
	sub R16,R12
	sbc R17,R13
	xcall _lcd_dot
	.dbline 223
; 	
;       lcd_dot(yCenter + y - d, yCenter + x, mode);
	std y+0,R14
	ldd R18,y+13
	clr R19
	add R18,R12
	adc R19,R13
	ldd R16,y+13
	clr R17
	add R16,R10
	adc R17,R11
	mov R2,R22
	clr R3
	sub R16,R2
	sbc R17,R3
	xcall _lcd_dot
	.dbline 224
;       lcd_dot(yCenter + y - d, yCenter - x, mode);
	std y+0,R14
	ldd R18,y+13
	clr R19
	sub R18,R12
	sbc R19,R13
	ldd R16,y+13
	clr R17
	add R16,R10
	adc R17,R11
	mov R2,R22
	clr R3
	sub R16,R2
	sbc R17,R3
	xcall _lcd_dot
	.dbline 226
;    
;       lcd_dot(yCenter - y - d, yCenter + x, mode);
	std y+0,R14
	ldd R18,y+13
	clr R19
	add R18,R12
	adc R19,R13
	ldd R16,y+13
	clr R17
	sub R16,R10
	sbc R17,R11
	mov R2,R22
	clr R3
	sub R16,R2
	sbc R17,R3
	xcall _lcd_dot
	.dbline 227
;       lcd_dot(yCenter - y - d, yCenter - x, mode);
	std y+0,R14
	ldd R18,y+13
	clr R19
	sub R18,R12
	sbc R19,R13
	ldd R16,y+13
	clr R17
	sub R16,R10
	sbc R17,R11
	mov R2,R22
	clr R3
	sub R16,R2
	sbc R17,R3
	xcall _lcd_dot
	.dbline 229
; 
;       if (tSwitch<0) 
	cpi R20,0
	ldi R30,0
	cpc R21,R30
	brge L85
	.dbline 230
;          tSwitch += (4 * x + 6);
	ldi R16,4
	ldi R17,0
	movw R18,R12
	xcall empy16s
	movw R24,R16
	adiw R24,6
	add R20,R24
	adc R21,R25
	xjmp L86
L85:
	.dbline 232
;       else 
;        { tSwitch += (4 * (x - y) + 10);
	.dbline 232
	movw R18,R12
	sub R18,R10
	sbc R19,R11
	ldi R16,4
	ldi R17,0
	xcall empy16s
	movw R24,R16
	adiw R24,10
	add R20,R24
	adc R21,R25
	.dbline 233
;          y--;
	movw R24,R10
	sbiw R24,1
	movw R10,R24
	.dbline 234
;        }
L86:
	.dbline 235
	movw R24,R12
	adiw R24,1
	movw R12,R24
	.dbline 236
L83:
	.dbline 216
	cp R10,R12
	cpc R11,R13
	brlt X5
	xjmp L82
X5:
	.dbline -2
L81:
	adiw R28,1
	xcall pop_gset5
	adiw R28,4
	.dbline 0 ; func end
	ret
	.dbsym r tSwitch 20 I
	.dbsym r d 22 c
	.dbsym r y 10 I
	.dbsym r x 12 I
	.dbsym r mode 14 c
	.dbsym l radius 15 c
	.dbsym l yCenter 13 c
	.dbsym l xCenter 11 c
	.dbend
	.dbfunc e lcd_rect _lcd_rect fV
;           mode -> R14
;         height -> R12
;          width -> R22
;              y -> R20
;              x -> R10
	.even
_lcd_rect::
	xcall push_gset5
	mov R20,R18
	mov R10,R16
	sbiw R28,5
	ldd R22,y+15
	ldd R12,y+17
	ldd R14,y+19
	.dbline -1
	.dbline 241
;       x++;
;     }
;  }
; 
; 
; void lcd_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t mode) 
;  { width--;
	.dbline 241
	dec R22
	.dbline 242
;    height--;
	dec R12
	.dbline 243
;    lcd_line(x, y, x+width, y, mode);	// top
	std y+4,R14
	std y+2,R20
	mov R2,R10
	add R2,R22
	std y+0,R2
	mov R18,R20
	mov R16,R10
	xcall _lcd_line
	.dbline 244
;    lcd_line(x, y, x, y+height, mode);	// left
	std y+4,R14
	mov R2,R20
	add R2,R12
	std y+2,R2
	std y+0,R10
	mov R18,R20
	mov R16,R10
	xcall _lcd_line
	.dbline 245
;    lcd_line(x, y+height, x+width, y+height, mode);	// bottom
	std y+4,R14
	mov R2,R20
	add R2,R12
	std y+2,R2
	mov R4,R10
	add R4,R22
	std y+0,R4
	mov R18,R2
	mov R16,R10
	xcall _lcd_line
	.dbline 246
;    lcd_line(x+width, y, x+width, y+height, mode);		// right
	std y+4,R14
	mov R2,R20
	add R2,R12
	std y+2,R2
	mov R2,R10
	add R2,R22
	std y+0,R2
	mov R18,R20
	mov R16,R2
	xcall _lcd_line
	.dbline -2
L87:
	adiw R28,5
	xcall pop_gset5
	.dbline 0 ; func end
	ret
	.dbsym r mode 14 c
	.dbsym r height 12 c
	.dbsym r width 22 c
	.dbsym r y 20 c
	.dbsym r x 10 c
	.dbend
	.dbfunc e lcd_box _lcd_box fV
;              i -> R20
;           mode -> R22
;         height -> R10
;          width -> R12
;              y -> R14
;              x -> y+15
	.even
_lcd_box::
	xcall push_arg4
	xcall push_gset5
	mov R14,R18
	sbiw R28,5
	ldd R12,y+19
	ldd R10,y+21
	ldd R22,y+23
	.dbline -1
	.dbline 250
;  }
; 
; void lcd_box( uint8_t x, uint8_t y, uint8_t width,  uint8_t height,  uint8_t mode) 
;  { uint8_t i;
	.dbline 251
;    if (!width) return; 
	tst R12
	brne L89
	.dbline 251
	xjmp L88
L89:
	.dbline 253
; 
;    width--;
	dec R12
	.dbline 255
; 	
;    for (i=y;i<y+height;i++) 
	mov R20,R14
	xjmp L94
L91:
	.dbline 256
	std y+4,R22
	std y+2,R20
	ldd R2,y+15
	add R2,R12
	std y+0,R2
	mov R18,R20
	ldd R16,y+15
	xcall _lcd_line
L92:
	.dbline 255
	inc R20
L94:
	.dbline 255
	mov R2,R14
	add R2,R10
	cp R20,R2
	brlo L91
	.dbline -2
L88:
	adiw R28,5
	xcall pop_gset5
	adiw R28,4
	.dbline 0 ; func end
	ret
	.dbsym r i 20 c
	.dbsym r mode 22 c
	.dbsym r height 10 c
	.dbsym r width 12 c
	.dbsym r y 14 c
	.dbsym l x 15 c
	.dbend
