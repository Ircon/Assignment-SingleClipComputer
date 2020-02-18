	.module font.c
	.area text(rom, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\font.c
	.dbfunc e lcd_glyph _lcd_glyph fV
;   glyph_offset -> R6
;        bit_pos -> R8
;              x -> R10
;      char_mask -> y+2
;     glyph_scan -> y+0
;         y_bits -> R20
;           mask -> R14
; remaining_bits -> R22
;    byte_offset -> R12
;    store_width -> y+23
;      glyph_ptr -> y+21
;         height -> y+19
;          width -> y+17
;            top -> y+15
;           left -> y+13
	.even
_lcd_glyph::
	xcall push_arg4
	xcall push_gset5
	sbiw R28,3
	.dbline -1
	.dbline 10
; //*****************************************************************
; //本头文件实现了用于字符显示的宏以及显示函数和显示的数据
; //                   ---北京交通大学电气学院 杨罡 2008.8
; //*****************************************************************
; 
; #include "font.h"
; 
; void lcd_glyph(uint8_t left, uint8_t top, uint8_t width, uint8_t height,
;                uint8_t *glyph_ptr, uint8_t store_width)
;  { uint8_t bit_pos;
	.dbline 20
;    uint8_t byte_offset;
;    uint8_t y_bits;
;    uint8_t remaining_bits;
;    uint8_t mask;
;    uint8_t char_mask;
;    uint8_t x;
;    uint8_t *glyph_scan;
;    uint8_t glyph_offset;
; 
;    bit_pos = top & 0x07;		/* get the bit offset into a byte */
	ldd R24,y+15
	andi R24,7
	mov R8,R24
	.dbline 21
;    glyph_offset = 0;			/* start at left side of the glyph rasters */
	clr R6
	.dbline 22
;    char_mask = 0x80;			/* initial character glyph mask */
	ldi R24,128
	std y+2,R24
	.dbline 24
; 
;    for (x = left; x < (left + width); x++)
	ldd R10,y+13
	xjmp L6
L3:
	.dbline 25
;     { 
	.dbline 27
; 	   
; 	  byte_offset = top >> 3;        	/* get the byte offset into y direction */
	ldd R12,y+15
	lsr R12
	lsr R12
	lsr R12
	.dbline 28
;       y_bits = height;		/* get length in y direction to write */
	ldd R20,y+19
	.dbline 29
;       remaining_bits = 8 - bit_pos;	/* number of bits left in byte */
	ldi R22,8
	sub R22,R8
	.dbline 30
;       mask = l_mask_array[bit_pos];	/* get mask for this bit */
	ldi R24,<_l_mask_array
	ldi R25,>_l_mask_array
	mov R30,R8
	clr R31
	add R30,R24
	adc R31,R25
	ldd R14,z+0
	.dbline 31
;       glyph_scan = glyph_ptr + glyph_offset;	 /* point to base of the glyph */
	mov R2,R6
	clr R3
	ldd R0,y+21
	ldd R1,y+22
	add R2,R0
	adc R3,R1
	std y+1,R3
	std y+0,R2
	xjmp L8
L7:
	.dbline 35
;       /* boundary checking here to account for the possibility of  */
;       /* write past the bottom of the screen.                        */
; 	  while((y_bits) && (byte_offset < LCD_Y_BYTES)) /* while there are bits still to write */
;        { /* check if the character pixel is set or not */
	.dbline 37
;          //if(*glyph_scan & char_mask)
;          if(pgm_read_byte(glyph_scan) & char_mask)
	ldd R16,y+0
	ldd R17,y+1
	push R8
	push R6
	xcall _pgm_read_byte
	pop R6
	pop R8
	mov R2,R16
	ldd R0,y+2
	and R2,R0
	breq L10
	.dbline 38
;             l_display_array[byte_offset][x] |= mask;	/* set image pixel */
	ldi R24,128
	mul R24,R12
	movw R2,R0
	ldi R24,<_l_display_array
	ldi R25,>_l_display_array
	add R2,R24
	adc R3,R25
	mov R4,R10
	clr R5
	add R4,R2
	adc R5,R3
	movw R30,R4
	ldd R2,z+0
	or R2,R14
	std z+0,R2
	xjmp L11
L10:
	.dbline 40
;          else
;             l_display_array[byte_offset][x] &= ~mask;	/* clear the image pixel */    
	ldi R24,128
	mul R24,R12
	movw R2,R0
	ldi R24,<_l_display_array
	ldi R25,>_l_display_array
	add R2,R24
	adc R3,R25
	mov R4,R10
	clr R5
	add R4,R2
	adc R5,R3
	mov R2,R14
	com R2
	movw R30,R4
	ldd R3,z+0
	and R3,R2
	std z+0,R3
L11:
	.dbline 41
;          if(l_mask_array[0] & 0x80)
	lds R2,_l_mask_array
	sbrs R2,7
	rjmp L12
	.dbline 42
;             mask >>= 1;
	lsr R14
	xjmp L13
L12:
	.dbline 44
;          else
;             mask <<= 1;
	lsl R14
L13:
	.dbline 46
; 			
;          y_bits--;
	dec R20
	.dbline 47
;          remaining_bits--;
	dec R22
	.dbline 48
;          if(remaining_bits == 0)
	brne L14
	.dbline 49
;           { /* just crossed over a byte boundry, reset byte counts */
	.dbline 50
;             remaining_bits = 8;
	ldi R22,8
	.dbline 51
;             byte_offset++;
	inc R12
	.dbline 52
;             mask = l_mask_array[0];
	lds R14,_l_mask_array
	.dbline 53
;           }
L14:
	.dbline 56
	ldd R2,y+0
	ldd R3,y+1
	ldd R4,y+23
	clr R5
	add R4,R2
	adc R5,R3
	std y+1,R5
	std y+0,R4
	.dbline 58
L8:
	.dbline 34
	tst R20
	breq L16
	mov R24,R12
	cpi R24,8
	brsh X0
	xjmp L7
X0:
L16:
	.dbline 61
; 		 
;           /* bump the glyph scan to next raster */
;           glyph_scan += store_width;
; 		  
;        }
;      
;       /* shift over to next glyph bit */
;       char_mask >>= 1;
	ldd R2,y+2
	lsr R2
	std y+2,R2
	.dbline 62
;       if(char_mask == 0)				/* reset for next byte in raster */
	mov R0,R2
	tst R2
	brne L17
	.dbline 63
;        { char_mask = 0x80;
	.dbline 63
	ldi R24,128
	std y+2,R24
	.dbline 64
;          glyph_offset++;
	inc R6
	.dbline 65
;        }
L17:
	.dbline 67
L4:
	.dbline 24
	inc R10
L6:
	.dbline 24
	ldd R2,y+13
	ldd R0,y+17
	add R2,R0
	cp R10,R2
	brsh X1
	xjmp L3
X1:
	.dbline -2
L2:
	adiw R28,3
	xcall pop_gset5
	adiw R28,4
	.dbline 0 ; func end
	ret
	.dbsym r glyph_offset 6 c
	.dbsym r bit_pos 8 c
	.dbsym r x 10 c
	.dbsym l char_mask 2 c
	.dbsym l glyph_scan 0 pc
	.dbsym r y_bits 20 c
	.dbsym r mask 14 c
	.dbsym r remaining_bits 22 c
	.dbsym r byte_offset 12 c
	.dbsym l store_width 23 c
	.dbsym l glyph_ptr 21 pc
	.dbsym l height 19 c
	.dbsym l width 17 c
	.dbsym l top 15 c
	.dbsym l left 13 c
	.dbend
	.dbfunc e lcd_text _lcd_text fV
; width_table_ptr -> y+11
;      glyph_end -> R20
; glyph_table_ptr -> y+9
;  defaultheight -> y+17
;      inprogmem -> y+16
;     fixedwidth -> R12
;      glyph_ptr -> y+7
;      glyph_beg -> y+15
;    store_width -> y+14
;         height -> R22
;          width -> R10
;              x -> y+13
;          glyph -> R14
;            str -> y+32
;           font -> y+30
;            top -> y+28
;           left -> R20
	.even
_lcd_text::
	st -y,r19
	st -y,r18
	xcall push_gset5
	mov R20,R16
	sbiw R28,18
	.dbline -1
	.dbline 82
; 	 
;    }
;  }
; 
; 
; /*
;  Prints the given string at location x,y in the specified font.
;  Prints each character given via calls to lcd_glyph. The entry string
;  is null terminated. (adapted function from the MJK-code)
;  Arguments are:
; 	left       coordinate of left start of string.
; 	top        coordinate of top of string.
; 	font       font number to use for display (see fonts.h)
; 	str        text string to display (null-terminated)
; */ 
;  void lcd_text(uint8_t left, uint8_t top, uint8_t font,   char *str)
;  { uint8_t x = left;
	.dbline 82
	std y+13,R20
	.dbline 92
;    uint8_t glyph;
;    uint8_t width;
;    uint8_t height, defaultheight;
;    uint8_t store_width;
;    uint8_t *glyph_ptr;
;    uint8_t *width_table_ptr;
;    uint8_t *glyph_table_ptr;
;    uint8_t glyph_beg, glyph_end;
;    uint8_t fixedwidth;
;    uint8_t inprogmem=0;//mine
	clr R0
	std y+16,R0
	.dbline 94
;   
;    defaultheight = (fonts[font].glyph_height);//pgm_read_byte ( &(fonts[font].glyph_height) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+1
	ldi R25,>_fonts+1
	add R30,R24
	adc R31,R25
	lpm R30,Z
	std y+17,R30
	.dbline 95
;    store_width =   (fonts[font].store_width);//pgm_read_byte ( &(fonts[font].store_width) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts
	ldi R25,>_fonts
	add R30,R24
	adc R31,R25
	lpm R30,Z
	std y+14,R30
	.dbline 96
;    width_table_ptr = (uint8_t*)(fonts[font].width_table);//(uint8_t*) pgm_read_word( &(fonts[font].width_table) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+5
	ldi R25,>_fonts+5
	add R30,R24
	adc R31,R25
	lpm R0,Z+
	lpm R1,Z
	movw R30,R0
	std y+12,R31
	std y+11,R30
	.dbline 97
;    glyph_table_ptr = (uint8_t*)(fonts[font].glyph_table);//(uint8_t*)pgm_read_word( &(fonts[font].glyph_table) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+2
	ldi R25,>_fonts+2
	add R30,R24
	adc R31,R25
	lpm R0,Z+
	lpm R1,Z
	movw R30,R0
	std y+10,R31
	std y+9,R30
	.dbline 98
;    glyph_beg  = (fonts[font].glyph_beg);//pgm_read_byte( &(fonts[font].glyph_beg) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+7
	ldi R25,>_fonts+7
	add R30,R24
	adc R31,R25
	lpm R30,Z
	std y+15,R30
	.dbline 99
;    glyph_end  = (fonts[font].glyph_end);//pgm_read_byte( &(fonts[font].glyph_end) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+8
	ldi R25,>_fonts+8
	add R30,R24
	adc R31,R25
	lpm R20,Z
	.dbline 100
;    fixedwidth = (fonts[font].fixed_width);//pgm_read_byte( &(fonts[font].fixed_width) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+4
	ldi R25,>_fonts+4
	add R30,R24
	adc R31,R25
	lpm R12,Z
	.dbline 102
; 
;    if (inprogmem) 
	ldd R0,y+16
	tst R0
	breq L26
	.dbline 103
;       glyph = pgm_read_byte(str);
	ldd R16,y+32
	ldd R17,y+33
	xcall _pgm_read_byte
	mov R14,R16
	xjmp L29
L26:
	.dbline 106
;     
;    else 
;       glyph = (uint8_t)*str;
	ldd R30,y+32
	ldd R31,y+33
	ldd R14,z+0
	xjmp L29
L28:
	.dbline 109
; 	
;    while(glyph != 0x00) // while(*str != 0x00)
;     { /* check to make sure the symbol is a legal one */
	.dbline 111
;       /* if not then just replace it with the default character */
;       if((glyph < glyph_beg) || (glyph > glyph_end))
	ldd R0,y+15
	cp R14,R0
	brlo L33
	cp R20,R14
	brsh L31
L33:
	.dbline 112
;          glyph = (fonts[font].glyph_def);//pgm_read_byte( &(fonts[font].glyph_def) ) ;
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+9
	ldi R25,>_fonts+9
	add R30,R24
	adc R31,R25
	lpm R14,Z
L31:
	.dbline 115
; 
;       /* make zero based index into the font data arrays */
;       glyph -= glyph_beg;
	ldd R0,y+15
	sub R14,R0
	.dbline 116
;       if(fixedwidth == 0)
	tst R12
	brne L35
	.dbline 118
;          // width=fonts[font].width_table[glyph];	/* get the variable width instead */
;          width=pgm_read_byte(width_table_ptr+glyph);
	ldd R2,y+11
	ldd R3,y+12
	mov R16,R14
	clr R17
	add R16,R2
	adc R17,R3
	xcall _pgm_read_byte
	mov R10,R16
	xjmp L36
L35:
	.dbline 120
;       else 
;          width = fixedwidth;
	mov R10,R12
L36:
	.dbline 122
; 		
;       height = defaultheight;
	ldd R22,y+17
	.dbline 124
;       //glyph_ptr = fonts[font].glyph_table + ((unsigned int)glyph * (unsigned int)store_width * (unsigned int)height);
;       glyph_ptr = glyph_table_ptr + ((unsigned int)glyph * (unsigned int)store_width * (unsigned int)height) ;
	ldd R0,y+14
	mul R14,R0
	mul R0,R22
	movw R2,R0
	ldd R4,y+9
	ldd R5,y+10
	add R2,R4
	adc R3,R5
	std y+8,R3
	std y+7,R2
	.dbline 127
; 
;       /* range check / limit things here */
;       if(x > SCRN_RIGHT)
	ldi R24,127
	ldd R0,y+13
	cp R24,R0
	brsh L37
	.dbline 128
;          x = SCRN_RIGHT;
	std y+13,R24
L37:
	.dbline 130
;        
;       if((x + width) > SCRN_RIGHT+1)
	ldd R2,y+13
	add R2,R10
	ldi R24,128
	cp R24,R2
	brsh L39
	.dbline 131
;          width = SCRN_RIGHT - x + 1;
	ldi R24,127
	ldd R0,y+13
	sub R24,R0
	subi R24,255    ; addi 1
	mov R10,R24
L39:
	.dbline 133
;        
;       if(top > SCRN_BOTTOM)
	ldi R24,63
	ldd R0,y+28
	cp R24,R0
	brsh L41
	.dbline 134
;          top = SCRN_BOTTOM;
	std y+28,R24
L41:
	.dbline 136
;        
;       if((top + height) > SCRN_BOTTOM+1)
	ldd R2,y+28
	add R2,R22
	ldi R24,64
	cp R24,R2
	brsh L43
	.dbline 137
;          height = SCRN_BOTTOM - top + 1;
	ldi R22,63
	ldd R0,y+28
	sub R22,R0
	subi R22,255    ; addi 1
L43:
	.dbline 139
;        
;       lcd_glyph(x,top,width,height,glyph_ptr,store_width);  /* plug symbol into buffer */
	ldd R0,y+14
	std y+6,R0
	ldd R2,y+7
	ldd R3,y+8
	std y+5,R3
	std y+4,R2
	std y+2,R22
	std y+0,R10
	ldd R18,y+28
	ldd R16,y+13
	xcall _lcd_glyph
	.dbline 141
; 
;       x += width;		/* move right for next character */
	ldd R0,y+13
	add R0,R10
	std y+13,R0
	.dbline 142
;       str++;			/* point to next character in string */
	ldd R24,y+32
	ldd R25,y+33
	adiw R24,1
	std y+33,R25
	std y+32,R24
	.dbline 143
;       if (inprogmem) 
	ldd R0,y+16
	tst R0
	breq L45
	.dbline 144
;          glyph = pgm_read_byte(str);
	movw R16,R24
	xcall _pgm_read_byte
	mov R14,R16
	xjmp L46
L45:
	.dbline 146
;       else 
;          glyph = (uint8_t)*str;
	ldd R30,y+32
	ldd R31,y+33
	ldd R14,z+0
L46:
	.dbline 148
L29:
	.dbline 108
	tst R14
	breq X2
	xjmp L28
X2:
	.dbline -2
L19:
	adiw R28,18
	xcall pop_gset5
	adiw R28,2
	.dbline 0 ; func end
	ret
	.dbsym l width_table_ptr 11 pc
	.dbsym r glyph_end 20 c
	.dbsym l glyph_table_ptr 9 pc
	.dbsym l defaultheight 17 c
	.dbsym l inprogmem 16 c
	.dbsym r fixedwidth 12 c
	.dbsym l glyph_ptr 7 pc
	.dbsym l glyph_beg 15 c
	.dbsym l store_width 14 c
	.dbsym r height 22 c
	.dbsym r width 10 c
	.dbsym l x 13 c
	.dbsym r glyph 14 c
	.dbsym l str 32 pc
	.dbsym l font 30 c
	.dbsym l top 28 c
	.dbsym r left 20 c
	.dbend
	.dbfunc e lcd_text_p _lcd_text_p fV
; width_table_ptr -> y+11
;      glyph_end -> R20
; glyph_table_ptr -> y+9
;  defaultheight -> y+17
;      inprogmem -> y+16
;     fixedwidth -> R12
;      glyph_ptr -> y+7
;      glyph_beg -> y+15
;    store_width -> y+14
;         height -> R22
;          width -> R10
;              x -> y+13
;          glyph -> R14
;            str -> y+32
;           font -> y+30
;            top -> y+28
;           left -> R20
	.even
_lcd_text_p::
	st -y,r19
	st -y,r18
	xcall push_gset5
	mov R20,R16
	sbiw R28,18
	.dbline -1
	.dbline 151
;        
;     }
;  }
; void lcd_text_p(uint8_t left, uint8_t top, uint8_t font,  const char *str)
;  { uint8_t x = left;
	.dbline 151
	std y+13,R20
	.dbline 161
;    uint8_t glyph;
;    uint8_t width;
;    uint8_t height, defaultheight;
;    uint8_t store_width;
;    uint8_t *glyph_ptr;
;    uint8_t *width_table_ptr;
;    uint8_t *glyph_table_ptr;
;    uint8_t glyph_beg, glyph_end;
;    uint8_t fixedwidth;
;    uint8_t inprogmem=1;//mine
	clr R0
	inc R0
	std y+16,R0
	.dbline 163
;    
;    defaultheight = (fonts[font].glyph_height);//pgm_read_byte ( &(fonts[font].glyph_height) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+1
	ldi R25,>_fonts+1
	add R30,R24
	adc R31,R25
	lpm R30,Z
	std y+17,R30
	.dbline 164
;    store_width =   (fonts[font].store_width);//pgm_read_byte ( &(fonts[font].store_width) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts
	ldi R25,>_fonts
	add R30,R24
	adc R31,R25
	lpm R30,Z
	std y+14,R30
	.dbline 165
;    width_table_ptr = (uint8_t*)(fonts[font].width_table);//(uint8_t*) pgm_read_word( &(fonts[font].width_table) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+5
	ldi R25,>_fonts+5
	add R30,R24
	adc R31,R25
	lpm R0,Z+
	lpm R1,Z
	movw R30,R0
	std y+12,R31
	std y+11,R30
	.dbline 166
;    glyph_table_ptr = (uint8_t*)(fonts[font].glyph_table);//(uint8_t*)pgm_read_word( &(fonts[font].glyph_table) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+2
	ldi R25,>_fonts+2
	add R30,R24
	adc R31,R25
	lpm R0,Z+
	lpm R1,Z
	movw R30,R0
	std y+10,R31
	std y+9,R30
	.dbline 167
;    glyph_beg  = (fonts[font].glyph_beg);//pgm_read_byte( &(fonts[font].glyph_beg) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+7
	ldi R25,>_fonts+7
	add R30,R24
	adc R31,R25
	lpm R30,Z
	std y+15,R30
	.dbline 168
;    glyph_end  = (fonts[font].glyph_end);//pgm_read_byte( &(fonts[font].glyph_end) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+8
	ldi R25,>_fonts+8
	add R30,R24
	adc R31,R25
	lpm R20,Z
	.dbline 169
;    fixedwidth = (fonts[font].fixed_width);//pgm_read_byte( &(fonts[font].fixed_width) );
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+4
	ldi R25,>_fonts+4
	add R30,R24
	adc R31,R25
	lpm R12,Z
	.dbline 171
; 
;    if (inprogmem) 
	ldd R0,y+16
	tst R0
	breq L54
	.dbline 172
;       glyph = pgm_read_byte(str);
	ldd R16,y+32
	ldd R17,y+33
	xcall _pgm_read_byte
	mov R14,R16
	xjmp L57
L54:
	.dbline 175
;     
;    else 
;       glyph = (uint8_t)*str;
	ldd R30,y+32
	ldd R31,y+33
	lpm R14,Z
	xjmp L57
L56:
	.dbline 178
; 	
;    while(glyph != 0x00) // while(*str != 0x00)
;     { /* check to make sure the symbol is a legal one */
	.dbline 180
;       /* if not then just replace it with the default character */
;       if((glyph < glyph_beg) || (glyph > glyph_end))
	ldd R0,y+15
	cp R14,R0
	brlo L61
	cp R20,R14
	brsh L59
L61:
	.dbline 181
;          glyph = (fonts[font].glyph_def);//pgm_read_byte( &(fonts[font].glyph_def) ) ;
	ldi R24,10
	ldd R0,y+30
	mul R24,R0
	movw R30,R0
	ldi R24,<_fonts+9
	ldi R25,>_fonts+9
	add R30,R24
	adc R31,R25
	lpm R14,Z
L59:
	.dbline 184
; 
;       /* make zero based index into the font data arrays */
;       glyph -= glyph_beg;
	ldd R0,y+15
	sub R14,R0
	.dbline 185
;       if(fixedwidth == 0)
	tst R12
	brne L63
	.dbline 187
;          // width=fonts[font].width_table[glyph];	/* get the variable width instead */
;          width=pgm_read_byte(width_table_ptr+glyph);
	ldd R2,y+11
	ldd R3,y+12
	mov R16,R14
	clr R17
	add R16,R2
	adc R17,R3
	xcall _pgm_read_byte
	mov R10,R16
	xjmp L64
L63:
	.dbline 189
;       else 
;          width = fixedwidth;
	mov R10,R12
L64:
	.dbline 191
; 		
;       height = defaultheight;
	ldd R22,y+17
	.dbline 193
;       //glyph_ptr = fonts[font].glyph_table + ((unsigned int)glyph * (unsigned int)store_width * (unsigned int)height);
;       glyph_ptr = glyph_table_ptr + ((unsigned int)glyph * (unsigned int)store_width * (unsigned int)height) ;
	ldd R0,y+14
	mul R14,R0
	mul R0,R22
	movw R2,R0
	ldd R4,y+9
	ldd R5,y+10
	add R2,R4
	adc R3,R5
	std y+8,R3
	std y+7,R2
	.dbline 196
; 
;       /* range check / limit things here */
;       if(x > SCRN_RIGHT)
	ldi R24,127
	ldd R0,y+13
	cp R24,R0
	brsh L65
	.dbline 197
;          x = SCRN_RIGHT;
	std y+13,R24
L65:
	.dbline 199
;        
;       if((x + width) > SCRN_RIGHT+1)
	ldd R2,y+13
	add R2,R10
	ldi R24,128
	cp R24,R2
	brsh L67
	.dbline 200
;          width = SCRN_RIGHT - x + 1;
	ldi R24,127
	ldd R0,y+13
	sub R24,R0
	subi R24,255    ; addi 1
	mov R10,R24
L67:
	.dbline 202
;        
;       if(top > SCRN_BOTTOM)
	ldi R24,63
	ldd R0,y+28
	cp R24,R0
	brsh L69
	.dbline 203
;          top = SCRN_BOTTOM;
	std y+28,R24
L69:
	.dbline 205
;        
;       if((top + height) > SCRN_BOTTOM+1)
	ldd R2,y+28
	add R2,R22
	ldi R24,64
	cp R24,R2
	brsh L71
	.dbline 206
;          height = SCRN_BOTTOM - top + 1;
	ldi R22,63
	ldd R0,y+28
	sub R22,R0
	subi R22,255    ; addi 1
L71:
	.dbline 208
;        
;       lcd_glyph(x,top,width,height,glyph_ptr,store_width);  /* plug symbol into buffer */
	ldd R0,y+14
	std y+6,R0
	ldd R2,y+7
	ldd R3,y+8
	std y+5,R3
	std y+4,R2
	std y+2,R22
	std y+0,R10
	ldd R18,y+28
	ldd R16,y+13
	xcall _lcd_glyph
	.dbline 210
; 
;       x += width;		/* move right for next character */
	ldd R0,y+13
	add R0,R10
	std y+13,R0
	.dbline 211
;       str++;			/* point to next character in string */
	ldd R24,y+32
	ldd R25,y+33
	adiw R24,1
	std y+33,R25
	std y+32,R24
	.dbline 212
;       if (inprogmem) 
	ldd R0,y+16
	tst R0
	breq L73
	.dbline 213
;          glyph = pgm_read_byte(str);
	movw R16,R24
	xcall _pgm_read_byte
	mov R14,R16
	xjmp L74
L73:
	.dbline 215
;       else 
;          glyph = (uint8_t)*str;
	ldd R30,y+32
	ldd R31,y+33
	lpm R14,Z
L74:
	.dbline 217
L57:
	.dbline 177
	tst R14
	breq X3
	xjmp L56
X3:
	.dbline -2
L47:
	adiw R28,18
	xcall pop_gset5
	adiw R28,2
	.dbline 0 ; func end
	ret
	.dbsym l width_table_ptr 11 pc
	.dbsym r glyph_end 20 c
	.dbsym l glyph_table_ptr 9 pc
	.dbsym l defaultheight 17 c
	.dbsym l inprogmem 16 c
	.dbsym r fixedwidth 12 c
	.dbsym l glyph_ptr 7 pc
	.dbsym l glyph_beg 15 c
	.dbsym l store_width 14 c
	.dbsym r height 22 c
	.dbsym r width 10 c
	.dbsym l x 13 c
	.dbsym r glyph 14 c
	.dbsym l str 32 pkc
	.dbsym l font 30 c
	.dbsym l top 28 c
	.dbsym r left 20 c
	.dbend
	.area lit(rom, con, rel)
_fonts::
	.byte 2
	.byte 8
	.word _six_dot_glyph_table
	.byte 0
	.word _six_dot_width_table
	.byte 32
	.byte 126
	.byte 46
	.byte 1
	.byte 12
	.word _nine_dot_glyph_table
	.byte 8
	.word 0
	.byte 32
	.byte 126
	.byte 46
	.dbstruct 0 10 FONT_DEF
	.dbfield 0 store_width kc
	.dbfield 1 glyph_height kc
	.dbfield 2 glyph_table pkc
	.dbfield 4 fixed_width kc
	.dbfield 5 width_table pkc
	.dbfield 7 glyph_beg kc
	.dbfield 8 glyph_end kc
	.dbfield 9 glyph_def kc
	.dbend
	.dbsym e fonts _fonts A[20:2]kS[FONT_DEF]
_six_dot_glyph_table::
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 0,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 160,0
	.byte 160,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 20,0
	.byte 126,0
	.byte 40,0
	.byte 252,0
	.byte 'P,0
	.byte 0,0
	.byte 0,0
	.byte 32,0
	.byte 'p,0
	.byte 128,0
	.byte 96,0
	.byte 16,0
	.byte 224,0
	.byte 32,0
	.byte 0,0
	.byte 'd,0
	.byte 168,0
	.byte 208,0
	.byte 44,0
	.byte 'T,0
	.byte 152,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 144,0
	.byte 64,0
	.byte 152,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 128,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 32,0
	.byte 64,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 64,0
	.byte 32,0
	.byte 128,0
	.byte 64,0
	.byte 32,0
	.byte 32,0
	.byte 32,0
	.byte 32,0
	.byte 64,0
	.byte 128,0
	.byte 'P,0
	.byte 32,0
	.byte 248,0
	.byte 32,0
	.byte 'P,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 32,0
	.byte 32,0
	.byte 248,0
	.byte 32,0
	.byte 32,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 64,0
	.byte 64,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 248,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 32,0
	.byte 32,0
	.byte 64,0
	.byte 64,0
	.byte 128,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 64,0
	.byte 192,0
	.byte 64,0
	.byte 64,0
	.byte 64,0
	.byte 64,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 144,0
	.byte 16,0
	.byte 32,0
	.byte 64,0
	.byte 240,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 144,0
	.byte 32,0
	.byte 16,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 32,0
	.byte 96,0
	.byte 160,0
	.byte 240,0
	.byte 32,0
	.byte 32,0
	.byte 0,0
	.byte 0,0
	.byte 240,0
	.byte 128,0
	.byte 224,0
	.byte 16,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 32,0
	.byte 64,0
	.byte 224,0
	.byte 144,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 240,0
	.byte 16,0
	.byte 32,0
	.byte 64,0
	.byte 64,0
	.byte 64,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 144,0
	.byte 96,0
	.byte 144,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 144,0
	.byte 144,0
	.byte 'p,0
	.byte 32,0
	.byte 64,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 64,0
	.byte 0,0
	.byte 0,0
	.byte 64,0
	.byte 64,0
	.byte 128,0
	.byte 0,0
	.byte 32,0
	.byte 64,0
	.byte 128,0
	.byte 64,0
	.byte 32,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 240,0
	.byte 0,0
	.byte 0,0
	.byte 240,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 128,0
	.byte 64,0
	.byte 32,0
	.byte 64,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 192,0
	.byte 32,0
	.byte 64,0
	.byte 64,0
	.byte 0,0
	.byte 64,0
	.byte 0,0
	.byte 0,0
	.byte 60,0
	.byte 'B,0
	.byte 154,0
	.byte 170,0
	.byte 180,0
	.byte 64,0
	.byte 56,0
	.byte 0,0
	.byte 32,0
	.byte 32,0
	.byte 'P,0
	.byte 'p,0
	.byte 136,0
	.byte 136,0
	.byte 0,0
	.byte 0,0
	.byte 224,0
	.byte 144,0
	.byte 224,0
	.byte 144,0
	.byte 144,0
	.byte 224,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 144,0
	.byte 128,0
	.byte 128,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 224,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 224,0
	.byte 0,0
	.byte 0,0
	.byte 224,0
	.byte 128,0
	.byte 224,0
	.byte 128,0
	.byte 128,0
	.byte 224,0
	.byte 0,0
	.byte 0,0
	.byte 224,0
	.byte 128,0
	.byte 224,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 144,0
	.byte 128,0
	.byte 176,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 144,0
	.byte 144,0
	.byte 240,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 0,0
	.byte 0,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 16,0
	.byte 16,0
	.byte 16,0
	.byte 16,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 144,0
	.byte 160,0
	.byte 192,0
	.byte 192,0
	.byte 160,0
	.byte 144,0
	.byte 0,0
	.byte 0,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 224,0
	.byte 0,0
	.byte 0,0
	.byte 136,0
	.byte 216,0
	.byte 168,0
	.byte 136,0
	.byte 136,0
	.byte 136,0
	.byte 0,0
	.byte 0,0
	.byte 144,0
	.byte 208,0
	.byte 208,0
	.byte 176,0
	.byte 176,0
	.byte 144,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 224,0
	.byte 144,0
	.byte 144,0
	.byte 224,0
	.byte 128,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 96,0
	.byte 16,0
	.byte 0,0
	.byte 224,0
	.byte 144,0
	.byte 144,0
	.byte 224,0
	.byte 160,0
	.byte 144,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 144,0
	.byte 64,0
	.byte 32,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 248,0
	.byte 32,0
	.byte 32,0
	.byte 32,0
	.byte 32,0
	.byte 32,0
	.byte 0,0
	.byte 0,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 136,0
	.byte 136,0
	.byte 'P,0
	.byte 'P,0
	.byte 32,0
	.byte 32,0
	.byte 0,0
	.byte 0,0
	.byte 136,128
	.byte 136,128
	.byte 'U,0
	.byte 'U,0
	.byte 34,0
	.byte 34,0
	.byte 0,0
	.byte 0,0
	.byte 136,0
	.byte 'P,0
	.byte 32,0
	.byte 32,0
	.byte 'P,0
	.byte 136,0
	.byte 0,0
	.byte 0,0
	.byte 136,0
	.byte 'P,0
	.byte 32,0
	.byte 32,0
	.byte 32,0
	.byte 32,0
	.byte 0,0
	.byte 0,0
	.byte 240,0
	.byte 16,0
	.byte 32,0
	.byte 64,0
	.byte 128,0
	.byte 240,0
	.byte 0,0
	.byte 0,0
	.byte 192,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 192,0
	.byte 0,0
	.byte 128,0
	.byte 128,0
	.byte 64,0
	.byte 64,0
	.byte 32,0
	.byte 32,0
	.byte 16,0
	.byte 16,0
	.byte 192,0
	.byte 64,0
	.byte 64,0
	.byte 64,0
	.byte 64,0
	.byte 64,0
	.byte 192,0
	.byte 0,0
	.byte 64,0
	.byte 160,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 252,0
	.byte 0,0
	.byte 128,0
	.byte 64,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 'p,0
	.byte 144,0
	.byte 144,0
	.byte 'p,0
	.byte 0,0
	.byte 0,0
	.byte 128,0
	.byte 128,0
	.byte 224,0
	.byte 144,0
	.byte 144,0
	.byte 224,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 128,0
	.byte 128,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 16,0
	.byte 16,0
	.byte 'p,0
	.byte 144,0
	.byte 144,0
	.byte 'p,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 176,0
	.byte 128,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 128,0
	.byte 192,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 'p,0
	.byte 144,0
	.byte 144,0
	.byte 'p,0
	.byte 16,0
	.byte 96,0
	.byte 128,0
	.byte 128,0
	.byte 224,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 0,0
	.byte 0,0
	.byte 128,0
	.byte 0,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 64,0
	.byte 0,0
	.byte 64,0
	.byte 64,0
	.byte 64,0
	.byte 64,0
	.byte 64,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 144,0
	.byte 224,0
	.byte 160,0
	.byte 144,0
	.byte 0,0
	.byte 0,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 240,0
	.byte 168,0
	.byte 168,0
	.byte 168,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 224,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 144,0
	.byte 144,0
	.byte 96,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 224,0
	.byte 144,0
	.byte 144,0
	.byte 224,0
	.byte 128,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 'p,0
	.byte 144,0
	.byte 144,0
	.byte 'p,0
	.byte 16,0
	.byte 16,0
	.byte 0,0
	.byte 0,0
	.byte 160,0
	.byte 192,0
	.byte 128,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 96,0
	.byte 192,0
	.byte 32,0
	.byte 192,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 128,0
	.byte 192,0
	.byte 128,0
	.byte 128,0
	.byte 64,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 144,0
	.byte 144,0
	.byte 144,0
	.byte 'p,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 160,0
	.byte 160,0
	.byte 64,0
	.byte 64,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 168,0
	.byte 168,0
	.byte 'P,0
	.byte 'P,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 160,0
	.byte 64,0
	.byte 64,0
	.byte 160,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 160,0
	.byte 160,0
	.byte 64,0
	.byte 64,0
	.byte 128,0
	.byte 128,0
	.byte 0,0
	.byte 0,0
	.byte 240,0
	.byte 32,0
	.byte 64,0
	.byte 240,0
	.byte 0,0
	.byte 0,0
	.byte 32,0
	.byte 64,0
	.byte 64,0
	.byte 128,0
	.byte 128,0
	.byte 64,0
	.byte 64,0
	.byte 32,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 128,0
	.byte 64,0
	.byte 64,0
	.byte 32,0
	.byte 32,0
	.byte 64,0
	.byte 64,0
	.byte 128,0
	.byte 'd,0
	.byte 152,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.dbsym e six_dot_glyph_table _six_dot_glyph_table A[1520:1520]kc
_six_dot_width_table::
	.byte 2,2
	.byte 4,8
	.byte 5,7
	.byte 6,2
	.byte 4,4
	.byte 6,6
	.byte 3,6
	.byte 2,4
	.byte 5,3
	.byte 5,5
	.byte 5,5
	.byte 5,5
	.byte 5,5
	.byte 2,3
	.byte 4,5
	.byte 4,4
	.byte 8,6
	.byte 5,5
	.byte 5,4
	.byte 4,5
	.byte 5,2
	.byte 5,5
	.byte 4,6
	.byte 5,5
	.byte 5,5
	.byte 5,5
	.byte 6,5
	.byte 6,10
	.byte 6,6
	.byte 5,3
	.byte 5,3
	.byte 4,7
	.byte 3,5
	.byte 5,4
	.byte 5,5
	.byte 4,5
	.byte 5,2
	.byte 3,5
	.byte 2,6
	.byte 5,5
	.byte 5,5
	.byte 4,4
	.byte 3,5
	.byte 4,6
	.byte 4,4
	.byte 5,4
	.byte 2,4
	.byte 7
	.dbsym e six_dot_width_table _six_dot_width_table A[95:95]kc
_nine_dot_glyph_table::
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,24
	.byte 60,60
	.byte 60,24
	.byte 24,0
	.byte 24,24
	.byte 0,0
	.byte 0,'f
	.byte 'f,'f
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,54
	.byte 54,127
	.byte 54,54
	.byte 54,127
	.byte 54,54
	.byte 0,0
	.byte 8,62
	.byte 'k,'h
	.byte 'h,62
	.byte 11,11
	.byte 'k,62
	.byte 8,8
	.byte 'p,216
	.byte 218,'v
	.byte 12,24
	.byte 48,'n
	.byte 91,27
	.byte 14,0
	.byte 0,56
	.byte 'l,'l
	.byte 56,'l
	.byte 199,198
	.byte 'n,59
	.byte 0,0
	.byte 0,24
	.byte 24,24
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,12
	.byte 24,24
	.byte 48,48
	.byte 48,48
	.byte 48,24
	.byte 24,12
	.byte 0,48
	.byte 24,24
	.byte 12,12
	.byte 12,12
	.byte 12,24
	.byte 24,48
	.byte 0,0
	.byte 0,54
	.byte 28,127
	.byte 28,54
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,24
	.byte 24,126
	.byte 24,24
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 24,24
	.byte 48,0
	.byte 0,0
	.byte 0,0
	.byte 0,126
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 24,24
	.byte 0,0
	.byte 0,6
	.byte 6,12
	.byte 12,24
	.byte 24,48
	.byte 48,96
	.byte 96,0
	.byte 0,28
	.byte 54,'c
	.byte 'k,'k
	.byte 'k,'c
	.byte 54,28
	.byte 0,0
	.byte 0,12
	.byte 28,60
	.byte 12,12
	.byte 12,12
	.byte 12,63
	.byte 0,0
	.byte 0,60
	.byte 'f,6
	.byte 6,12
	.byte 24,48
	.byte 96,126
	.byte 0,0
	.byte 0,60
	.byte 'f,'f
	.byte 6,28
	.byte 6,'f
	.byte 'f,60
	.byte 0,0
	.byte 0,6
	.byte 14,30
	.byte 54,'f
	.byte 'f,127
	.byte 6,6
	.byte 0,0
	.byte 0,126
	.byte 96,96
	.byte 96,124
	.byte 6,6
	.byte 'f,60
	.byte 0,0
	.byte 0,28
	.byte 48,96
	.byte 96,124
	.byte 'f,'f
	.byte 'f,60
	.byte 0,0
	.byte 0,126
	.byte 6,12
	.byte 12,24
	.byte 24,24
	.byte 24,24
	.byte 0,0
	.byte 0,60
	.byte 'f,'f
	.byte 'f,60
	.byte 'f,'f
	.byte 'f,60
	.byte 0,0
	.byte 0,60
	.byte 'f,'f
	.byte 'f,62
	.byte 6,6
	.byte 12,56
	.byte 0,0
	.byte 0,0
	.byte 0,24
	.byte 24,0
	.byte 0,0
	.byte 24,24
	.byte 0,0
	.byte 0,0
	.byte 0,24
	.byte 24,0
	.byte 0,0
	.byte 24,24
	.byte 48,0
	.byte 0,6
	.byte 12,24
	.byte 48,96
	.byte 48,24
	.byte 12,6
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 126,0
	.byte 126,0
	.byte 0,0
	.byte 0,0
	.byte 0,96
	.byte 48,24
	.byte 12,6
	.byte 12,24
	.byte 48,96
	.byte 0,0
	.byte 0,60
	.byte 'f,'f
	.byte 12,24
	.byte 24,0
	.byte 24,24
	.byte 0,0
	.byte 0,126
	.byte 195,195
	.byte 207,219
	.byte 219,207
	.byte 192,127
	.byte 0,0
	.byte 0,24
	.byte 60,'f
	.byte 'f,'f
	.byte 126,'f
	.byte 'f,'f
	.byte 0,0
	.byte 0,124
	.byte 'f,'f
	.byte 'f,124
	.byte 'f,'f
	.byte 'f,124
	.byte 0,0
	.byte 0,60
	.byte 'f,96
	.byte 96,96
	.byte 96,96
	.byte 'f,60
	.byte 0,0
	.byte 0,'x
	.byte 'l,'f
	.byte 'f,'f
	.byte 'f,'f
	.byte 'l,'x
	.byte 0,0
	.byte 0,126
	.byte 96,96
	.byte 96,124
	.byte 96,96
	.byte 96,126
	.byte 0,0
	.byte 0,126
	.byte 96,96
	.byte 96,124
	.byte 96,96
	.byte 96,96
	.byte 0,0
	.byte 0,60
	.byte 'f,96
	.byte 96,96
	.byte 'n,'f
	.byte 'f,62
	.byte 0,0
	.byte 0,'f
	.byte 'f,'f
	.byte 'f,126
	.byte 'f,'f
	.byte 'f,'f
	.byte 0,0
	.byte 0,60
	.byte 24,24
	.byte 24,24
	.byte 24,24
	.byte 24,60
	.byte 0,0
	.byte 0,6
	.byte 6,6
	.byte 6,6
	.byte 6,'f
	.byte 'f,60
	.byte 0,0
	.byte 0,'f
	.byte 'f,'l
	.byte 'l,'x
	.byte 'l,'l
	.byte 'f,'f
	.byte 0,0
	.byte 0,96
	.byte 96,96
	.byte 96,96
	.byte 96,96
	.byte 96,126
	.byte 0,0
	.byte 0,'c
	.byte 'w,127
	.byte 'k,'k
	.byte 'c,'c
	.byte 'c,'c
	.byte 0,0
	.byte 0,'c
	.byte 'c,'s
	.byte 123,'o
	.byte 'g,'c
	.byte 'c,'c
	.byte 0,0
	.byte 0,60
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,60
	.byte 0,0
	.byte 0,124
	.byte 'f,'f
	.byte 'f,124
	.byte 96,96
	.byte 96,96
	.byte 0,0
	.byte 0,60
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,60
	.byte 12,6
	.byte 0,124
	.byte 'f,'f
	.byte 'f,124
	.byte 'l,'f
	.byte 'f,'f
	.byte 0,0
	.byte 0,60
	.byte 'f,96
	.byte 48,24
	.byte 12,6
	.byte 'f,60
	.byte 0,0
	.byte 0,126
	.byte 24,24
	.byte 24,24
	.byte 24,24
	.byte 24,24
	.byte 0,0
	.byte 0,'f
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,60
	.byte 0,0
	.byte 0,'f
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,'f
	.byte 60,24
	.byte 0,0
	.byte 0,'c
	.byte 'c,'c
	.byte 'c,'k
	.byte 'k,'k
	.byte 62,54
	.byte 0,0
	.byte 0,'f
	.byte 'f,'f
	.byte 60,24
	.byte 60,'f
	.byte 'f,'f
	.byte 0,0
	.byte 0,'f
	.byte 'f,'f
	.byte 'f,60
	.byte 24,24
	.byte 24,24
	.byte 0,0
	.byte 0,126
	.byte 6,6
	.byte 12,24
	.byte 48,96
	.byte 96,126
	.byte 0,0
	.byte 60,48
	.byte 48,48
	.byte 48,48
	.byte 48,48
	.byte 48,48
	.byte 48,60
	.byte 0,96
	.byte 96,48
	.byte 48,24
	.byte 24,12
	.byte 12,6
	.byte 6,0
	.byte 60,12
	.byte 12,12
	.byte 12,12
	.byte 12,12
	.byte 12,12
	.byte 12,60
	.byte 24,60
	.byte 'f,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,255
	.byte 48,48
	.byte 24,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,60
	.byte 6,6
	.byte 62,'f
	.byte 'f,62
	.byte 0,0
	.byte 0,96
	.byte 96,124
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,124
	.byte 0,0
	.byte 0,0
	.byte 0,60
	.byte 'f,96
	.byte 96,96
	.byte 'f,60
	.byte 0,0
	.byte 0,6
	.byte 6,62
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,62
	.byte 0,0
	.byte 0,0
	.byte 0,60
	.byte 'f,'f
	.byte 126,96
	.byte 96,60
	.byte 0,0
	.byte 0,28
	.byte 48,48
	.byte 48,'x
	.byte 48,48
	.byte 48,48
	.byte 0,0
	.byte 0,0
	.byte 0,62
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,62
	.byte 6,124
	.byte 0,96
	.byte 96,124
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,'f
	.byte 0,0
	.byte 24,24
	.byte 0,'x
	.byte 24,24
	.byte 24,24
	.byte 24,126
	.byte 0,0
	.byte 12,12
	.byte 0,60
	.byte 12,12
	.byte 12,12
	.byte 12,12
	.byte 12,'x
	.byte 0,96
	.byte 96,'f
	.byte 'f,'l
	.byte 'x,'l
	.byte 'f,'f
	.byte 0,0
	.byte 0,'x
	.byte 24,24
	.byte 24,24
	.byte 24,24
	.byte 24,126
	.byte 0,0
	.byte 0,0
	.byte 0,126
	.byte 'k,'k
	.byte 'k,'k
	.byte 'k,'c
	.byte 0,0
	.byte 0,0
	.byte 0,124
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,'f
	.byte 0,0
	.byte 0,0
	.byte 0,60
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,60
	.byte 0,0
	.byte 0,0
	.byte 0,124
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,124
	.byte 96,96
	.byte 0,0
	.byte 0,62
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,62
	.byte 6,6
	.byte 0,0
	.byte 0,124
	.byte 'f,96
	.byte 96,96
	.byte 96,96
	.byte 0,0
	.byte 0,0
	.byte 0,62
	.byte 96,96
	.byte 60,6
	.byte 6,124
	.byte 0,0
	.byte 0,48
	.byte 48,'x
	.byte 48,48
	.byte 48,48
	.byte 48,28
	.byte 0,0
	.byte 0,0
	.byte 0,'f
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,62
	.byte 0,0
	.byte 0,0
	.byte 0,'f
	.byte 'f,'f
	.byte 'f,'f
	.byte 60,24
	.byte 0,0
	.byte 0,0
	.byte 0,'c
	.byte 'c,'c
	.byte 'k,'k
	.byte 62,54
	.byte 0,0
	.byte 0,0
	.byte 0,'f
	.byte 'f,60
	.byte 24,60
	.byte 'f,'f
	.byte 0,0
	.byte 0,0
	.byte 0,'f
	.byte 'f,'f
	.byte 'f,'f
	.byte 'f,62
	.byte 6,124
	.byte 0,0
	.byte 0,126
	.byte 6,12
	.byte 24,48
	.byte 96,126
	.byte 0,0
	.byte 12,24
	.byte 24,24
	.byte 24,48
	.byte 24,24
	.byte 24,24
	.byte 12,0
	.byte 24,24
	.byte 24,24
	.byte 24,0
	.byte 24,24
	.byte 24,24
	.byte 24,0
	.byte 48,24
	.byte 24,24
	.byte 24,12
	.byte 24,24
	.byte 24,24
	.byte 48,0
	.byte 0,'v
	.byte 220,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.byte 0,0
	.dbsym e nine_dot_glyph_table _nine_dot_glyph_table A[1140:1140]kc
