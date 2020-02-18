	.module LCD_DRIVER.c
	.area text(rom, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\LCD_DRIVER.c
	.dbfunc e lcd_out_dat_l _lcd_out_dat_l fV
;            dat -> R20
	.even
_lcd_out_dat_l::
	xcall push_gset1
	mov R20,R16
	.dbline -1
	.dbline 18
; //**********************************************************
; //本文件实现了用于驱动KS0108B/HD61202的基本常量和
; //函数，用于基本的读写操作（没有实现读的函数，
; //当然你可以自己实现）.
; //              ---北京交通大学电气学院 杨罡 2008.8
; //**********************************************************
; 
; #include  "LCD_DRIVER.H"
; 
; //***********************************************************
; //if you want it to run on the protues then define this below
; //***********************************************************
; #define debug_protues
; //***********************************************************
; 
; void lcd_out_dat_l( unsigned char dat)
; 
; {    
	.dbline 19
;      choose_left_screen();
	xcall _choose_left_screen
	.dbline 20
;      LCD_DATA_MODE();   //D_I = CMD_MODE;
	sbi 0x18,2
	.dbline 21
; 	 LCD_WRITE_MODE();  //R_W=WRITE_MODE;
	cbi 0x18,3
	.dbline 22
; 	 LCD_E_HIGH();      //E=1;
	sbi 0x18,4
	.dbline 23
; 	 DATA_PORT=dat;
	out 0x1b,R20
	.dbline 24
; 	 _delay_us(2);
	ldi R16,2
	xcall __delay_us
	.dbline 25
; 	 LCD_E_LOW();       //E=0;
	cbi 0x18,4
	.dbline 26
; 	  _delay_us(2);
	ldi R16,2
	xcall __delay_us
	.dbline -2
L1:
	xcall pop_gset1
	.dbline 0 ; func end
	ret
	.dbsym r dat 20 c
	.dbend
	.dbfunc e lcd_out_dat_r _lcd_out_dat_r fV
;            dat -> R20
	.even
_lcd_out_dat_r::
	xcall push_gset1
	mov R20,R16
	.dbline -1
	.dbline 30
; }
; 
; void lcd_out_dat_r(  unsigned char dat)
; {    
	.dbline 31
;      choose_right_screen() ;
	xcall _choose_right_screen
	.dbline 32
;      LCD_DATA_MODE();   //D_I = CMD_MODE;
	sbi 0x18,2
	.dbline 33
; 	 LCD_WRITE_MODE();  //R_W=WRITE_MODE;
	cbi 0x18,3
	.dbline 34
; 	 LCD_E_HIGH();      //E=1;
	sbi 0x18,4
	.dbline 35
; 	 DATA_PORT=dat;
	out 0x1b,R20
	.dbline 36
; 	  _delay_us(2);
	ldi R16,2
	xcall __delay_us
	.dbline 37
; 	 LCD_E_LOW();       //E=0;
	cbi 0x18,4
	.dbline 38
; 	  _delay_us(2);
	ldi R16,2
	xcall __delay_us
	.dbline -2
L2:
	xcall pop_gset1
	.dbline 0 ; func end
	ret
	.dbsym r dat 20 c
	.dbend
	.dbfunc e lcd_out_ctl_l _lcd_out_ctl_l fV
;            cmd -> R20
	.even
_lcd_out_ctl_l::
	xcall push_gset1
	mov R20,R16
	.dbline -1
	.dbline 41
; }
; void lcd_out_ctl_l( unsigned char cmd)
; {
	.dbline 42
;      choose_left_screen();
	xcall _choose_left_screen
	.dbline 43
;      LCD_CMD_MODE();    //D_I = CMD_MODE;
	cbi 0x18,2
	.dbline 44
; 	 LCD_WRITE_MODE();  //R_W=WRITE_MODE;
	cbi 0x18,3
	.dbline 45
; 	 LCD_E_HIGH();      //E=1;
	sbi 0x18,4
	.dbline 46
; 	 DATA_PORT=cmd;
	out 0x1b,R20
	.dbline 47
; 	  _delay_us(2);
	ldi R16,2
	xcall __delay_us
	.dbline 48
; 	 LCD_E_LOW();       //E=0;
	cbi 0x18,4
	.dbline 49
;       _delay_us(2);
	ldi R16,2
	xcall __delay_us
	.dbline -2
L3:
	xcall pop_gset1
	.dbline 0 ; func end
	ret
	.dbsym r cmd 20 c
	.dbend
	.dbfunc e lcd_out_ctl_r _lcd_out_ctl_r fV
;            cmd -> R20
	.even
_lcd_out_ctl_r::
	xcall push_gset1
	mov R20,R16
	.dbline -1
	.dbline 52
; }
; void lcd_out_ctl_r( unsigned char cmd)
; {    
	.dbline 53
;      choose_right_screen() ;
	xcall _choose_right_screen
	.dbline 54
;      LCD_CMD_MODE();    //D_I = CMD_MODE;
	cbi 0x18,2
	.dbline 55
; 	 LCD_WRITE_MODE();  //R_W=WRITE_MODE;
	cbi 0x18,3
	.dbline 56
; 	 LCD_E_HIGH();      //E=1;
	sbi 0x18,4
	.dbline 57
; 	 DATA_PORT=cmd;
	out 0x1b,R20
	.dbline 58
; 	  _delay_us(2);
	ldi R16,2
	xcall __delay_us
	.dbline 59
; 	 LCD_E_LOW();       //E=0;
	cbi 0x18,4
	.dbline 60
;       _delay_us(2);
	ldi R16,2
	xcall __delay_us
	.dbline -2
L4:
	xcall pop_gset1
	.dbline 0 ; func end
	ret
	.dbsym r cmd 20 c
	.dbend
	.dbfunc e lcd_out_ctl _lcd_out_ctl fV
;          nctrl -> R22
;            cmd -> R20
	.even
_lcd_out_ctl::
	xcall push_gset2
	mov R22,R18
	mov R20,R16
	.dbline -1
	.dbline 63
; }
; void lcd_out_ctl( unsigned char cmd,  unsigned char nctrl)
; {
	.dbline 64
;       if(nctrl==1)lcd_out_ctl_l(cmd);
	cpi R22,1
	brne L6
	.dbline 64
	mov R16,R20
	xcall _lcd_out_ctl_l
L6:
	.dbline 65
; 	  if(nctrl==2)lcd_out_ctl_r(cmd);
	cpi R22,2
	brne L8
	.dbline 65
	mov R16,R20
	xcall _lcd_out_ctl_r
L8:
	.dbline 66
	cpi R22,3
	brne L10
	.dbline 66
	.dbline 67
	mov R16,R20
	xcall _lcd_out_ctl_l
	.dbline 68
	mov R16,R20
	xcall _lcd_out_ctl_r
	.dbline 69
L10:
	.dbline -2
L5:
	xcall pop_gset2
	.dbline 0 ; func end
	ret
	.dbsym r nctrl 22 c
	.dbsym r cmd 20 c
	.dbend
	.dbfunc e lcd_out_dat _lcd_out_dat fV
;          nctrl -> R22
;            dat -> R20
	.even
_lcd_out_dat::
	xcall push_gset2
	mov R22,R18
	mov R20,R16
	.dbline -1
	.dbline 72
; 	  if(nctrl==3){
; 	                lcd_out_ctl_l(cmd);
; 					lcd_out_ctl_r(cmd);
; 				   }
; }
; void lcd_out_dat( unsigned char dat,  unsigned char nctrl)
; {
	.dbline 73
;      if(nctrl==1)lcd_out_dat_l(dat);
	cpi R22,1
	brne L13
	.dbline 73
	mov R16,R20
	xcall _lcd_out_dat_l
L13:
	.dbline 74
; 	  if(nctrl==2)lcd_out_dat_r(dat);
	cpi R22,2
	brne L15
	.dbline 74
	mov R16,R20
	xcall _lcd_out_dat_r
L15:
	.dbline 75
	cpi R22,3
	brne L17
	.dbline 75
	.dbline 76
	mov R16,R20
	xcall _lcd_out_dat_l
	.dbline 77
	mov R16,R20
	xcall _lcd_out_dat_r
	.dbline 78
L17:
	.dbline -2
L12:
	xcall pop_gset2
	.dbline 0 ; func end
	ret
	.dbsym r nctrl 22 c
	.dbsym r dat 20 c
	.dbend
	.dbfunc e choose_left_screen _choose_left_screen fV
	.even
_choose_left_screen::
	.dbline -1
	.dbline 81
; 	  if(nctrl==3){
; 	               lcd_out_dat_l(dat);
; 				   lcd_out_dat_r(dat);
; 				  }
; }
; void   choose_left_screen(void)   
; { 
	.dbline 86
;    #ifndef debug_protues
;   LCD_LEFT_SELECT();
;   LCD_RIGHT_STOP();
;   #else
;   LCD_LEFT_STOP();
	sbi 0x18,0
	.dbline 87
;   LCD_RIGHT_SELECT();
	cbi 0x18,1
	.dbline -2
L19:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e choose_right_screen _choose_right_screen fV
	.even
_choose_right_screen::
	.dbline -1
	.dbline 91
;   #endif
; }
; void   choose_right_screen(void)  
; {
	.dbline 96
;   #ifndef debug_protues
;   LCD_LEFT_STOP();
;   LCD_RIGHT_SELECT();
;   #else
;   LCD_LEFT_SELECT();
	cbi 0x18,0
	.dbline 97
;   LCD_RIGHT_STOP();
	sbi 0x18,1
	.dbline -2
L20:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e choose_full_screen _choose_full_screen fV
	.even
_choose_full_screen::
	.dbline -1
	.dbline 101
;   #endif
; }
; void   choose_full_screen(void)  
; {
	.dbline 102
;   LCD_LEFT_SELECT();
	cbi 0x18,0
	.dbline 103
;   LCD_RIGHT_SELECT();
	cbi 0x18,1
	.dbline -2
L21:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e choose_no_screen _choose_no_screen fV
	.even
_choose_no_screen::
	.dbline -1
	.dbline 106
; }
; void   choose_no_screen(void)     
; {
	.dbline 107
;    LCD_LEFT_STOP();
	sbi 0x18,0
	.dbline 108
;    LCD_RIGHT_STOP();
	sbi 0x18,1
	.dbline -2
L22:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e lcd_read_byte _lcd_read_byte fc
;              y -> R18
;              x -> R16
	.even
_lcd_read_byte::
	.dbline -1
	.dbline 112
; }
; 
; unsigned char lcd_read_byte( unsigned char x,unsigned char y )
; {
	.dbline 149
;     /*if(which & 1== 1)
; 	{
; 	  choose_left_screen();
; 	  LCD_CMD_MODE();//D_I=0;
; 	  LCD_READ_MODE();//R_W=READ_MODE;
; 	  do
; 	 {DATA_PORT=0xff;//??????????????????????????????
; 	  LCD_E_HIGH():E=1;
; 	  tmp=DATA_PORT;
; 	  LCD_E_LOW();//E=0;
; 	 }while( !(tmp&0x80) );
; 	 LCD_DATA_MODE();//D_I=DATA_MODE;
; 	 DATA_PORT=0x0ff;
; 	 LCD_E_HIGH();//E=1
; 	 tmp=DATA_PORT;
; 	 LCD_E_LOW()://E=0;
; 	 return tmp;
; 	}
; 	if(which &2== 2)
; 	{
; 	  choose_right_screen();
; 	  LCD_CMD_MODE();//D_I=0;
; 	  LCD_READ_MODE();//R_W=READ_MODE;
; 	  do
; 	 {DATA_PORT=0xff;//????????????????????????????????
; 	  LCD_E_HIGH();//E=1;
; 	  tmp=DATA_PORT;
; 	  LCD_E_LOW();//E=0;
; 	 }while( !(tmp&0x80) );
; 	 LCD_DATA_MODE();//D_I=DATA_MODE;
; 	 DATA_PORT=0x0ff;
; 	 LCD_E_HIGH();//E=1
; 	 tmp=DATA_PORT;
;      LCD_E_LOW();//E=0;
; 	 return tmp;
; 	}*/
; 	return 1;
	ldi R16,1
	.dbline -2
L23:
	.dbline 0 ; func end
	ret
	.dbsym r y 18 c
	.dbsym r x 16 c
	.dbend
	.dbfunc e lcd_is_busy _lcd_is_busy fc
;            tmp -> R16
	.even
_lcd_is_busy::
	.dbline -1
	.dbline 152
; }
; unsigned char lcd_is_busy( void )
; {  
	.dbline 154
;    unsigned char tmp;
;    LCD_CMD_MODE();//D_I=CMD_MODE;
	cbi 0x18,2
	.dbline 155
;    LCD_READ_MODE();//R_W=READ_MODE;
	sbi 0x18,3
	.dbline 156
;    DATA_PORT=0x0ff;//?????????????????????????
	ldi R24,255
	out 0x1b,R24
	.dbline 157
;    LCD_E_HIGH();//E=1;
	sbi 0x18,4
	.dbline 158
;    tmp=DATA_PORT;
	in R16,0x1b
	.dbline 159
;    LCD_E_LOW();//E=0;
	cbi 0x18,4
	.dbline 160
;    return tmp&0x80;
	andi R16,128
	.dbline -2
L24:
	.dbline 0 ; func end
	ret
	.dbsym r tmp 16 c
	.dbend
	.dbfunc e lcd_init _lcd_init fV
	.even
_lcd_init::
	.dbline -1
	.dbline 163
; }
; void lcd_init(void)
; {
	.dbline 164
;   _delay_us(3000);
	ldi R16,184
	xcall __delay_us
	.dbline 169
;   //choose_full_screen();
;   //write_cmd(DISPLAY_FIRST_ROW);//first row display 0x0c0
;   //write_cmd(DISPLAY_ON);//open display 0x03f
;   //choose_no_screen(); 
;   lcd_out_ctl(DISPLAY_FIRST_ROW,3);
	ldi R18,3
	ldi R16,192
	xcall _lcd_out_ctl
	.dbline 170
;   lcd_out_ctl(DISPLAY_ON,3);
	ldi R18,3
	ldi R16,63
	xcall _lcd_out_ctl
	.dbline -2
L25:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e write_rect _write_rect fV
;            dat -> R2,R3
;             y0 -> R1
;          page0 -> R0
	.even
_write_rect::
	.dbline -1
	.dbline 176
;  
;   
; }
; void write_rect(unsigned char page0,unsigned char y0,
;                 unsigned char *dat)
; {
	.dbline -2
L26:
	.dbline 0 ; func end
	ret
	.dbsym l dat 4 pc
	.dbsym l y0 2 c
	.dbsym l page0 0 c
	.dbend
	.dbfunc e read_rect _read_rect fV
;             y1 -> R3
;             x1 -> R2
;             y0 -> R1
;             x0 -> R0
	.even
_read_rect::
	.dbline -1
	.dbline 181
; 
; }
; void read_rect(unsigned char x0,unsigned char y0,
;                unsigned char x1,unsigned char y1)
; {
	.dbline -2
L27:
	.dbline 0 ; func end
	ret
	.dbsym l y1 6 c
	.dbsym l x1 4 c
	.dbsym l y0 2 c
	.dbsym l x0 0 c
	.dbend
	.dbfunc e write_to_xy _write_to_xy fV
;           page -> R22
;            dat -> y+4
;              y -> R22
;              x -> R20
	.even
_write_to_xy::
	xcall push_gset2
	mov R22,R18
	mov R20,R16
	.dbline -1
	.dbline 186
; 
; }			   
; 
; void write_to_xy(unsigned char x,unsigned char y,unsigned char dat)
; {
	.dbline 188
;   unsigned char page;
;   page=y>>3;
	lsr R22
	lsr R22
	lsr R22
	.dbline 189
;   if(x>63)
	ldi R24,63
	cp R24,R20
	brsh L29
	.dbline 190
;   { x=x-64;
	.dbline 190
	subi R20,64
	.dbline 192
;     //choose_right_screen();
;     lcd_out_ctl_r(LCD_SET_PAGE+page);
	mov R16,R22
	subi R16,72    ; addi 184
	xcall _lcd_out_ctl_r
	.dbline 193
; 	lcd_out_ctl_r(LCD_SET_COL+x);
	mov R16,R20
	subi R16,192    ; addi 64
	xcall _lcd_out_ctl_r
	.dbline 194
; 	lcd_out_ctl_r(dat);
	ldd R16,y+4
	xcall _lcd_out_ctl_r
	.dbline 195
;   }
	xjmp L30
L29:
	.dbline 197
	.dbline 198
	mov R16,R22
	subi R16,72    ; addi 184
	xcall _lcd_out_ctl_l
	.dbline 199
	mov R16,R20
	subi R16,192    ; addi 64
	xcall _lcd_out_ctl_l
	.dbline 200
	ldd R16,y+4
	xcall _lcd_out_ctl_l
	.dbline 201
L30:
	.dbline -2
L28:
	xcall pop_gset2
	.dbline 0 ; func end
	ret
	.dbsym r page 22 c
	.dbsym l dat 4 c
	.dbsym r y 22 c
	.dbsym r x 20 c
	.dbend
	.dbfunc e set_xy _set_xy fV
;           page -> R22
;              y -> R22
;              x -> R20
	.even
_set_xy::
	xcall push_gset2
	mov R22,R18
	mov R20,R16
	.dbline -1
	.dbline 205
;   else    
;   { //choose_left_screen();
;     lcd_out_ctl_l(LCD_SET_PAGE+page);
; 	lcd_out_ctl_l(LCD_SET_COL+x);
; 	lcd_out_ctl_l(dat);
;   }
; }
; 
; void set_xy(unsigned char x,unsigned char y)
; {
	.dbline 207
;  unsigned char page;
;   page=y>>3;
	lsr R22
	lsr R22
	lsr R22
	.dbline 208
;   if(x>63)
	ldi R24,63
	cp R24,R20
	brsh L32
	.dbline 209
;   { x=x-64;
	.dbline 209
	subi R20,64
	.dbline 211
;     //choose_right_screen();
;     lcd_out_ctl_r(LCD_SET_PAGE+page);//write_cmd(PAGE_CO+page);
	mov R16,R22
	subi R16,72    ; addi 184
	xcall _lcd_out_ctl_r
	.dbline 212
; 	lcd_out_ctl_r(LCD_SET_COL+x);//write_cmd(ROW_CO+x);
	mov R16,R20
	subi R16,192    ; addi 64
	xcall _lcd_out_ctl_r
	.dbline 213
;   }
	xjmp L33
L32:
	.dbline 215
	.dbline 216
	mov R16,R22
	subi R16,72    ; addi 184
	xcall _lcd_out_ctl_l
	.dbline 217
	mov R16,R20
	subi R16,192    ; addi 64
	xcall _lcd_out_ctl_l
	.dbline 218
L33:
	.dbline -2
L31:
	xcall pop_gset2
	.dbline 0 ; func end
	ret
	.dbsym r page 22 c
	.dbsym r y 22 c
	.dbsym r x 20 c
	.dbend
	.dbfunc e write_to_lcd _write_to_lcd fV
;            dat -> R20
	.even
_write_to_lcd::
	xcall push_gset1
	mov R20,R16
	.dbline -1
	.dbline 222
;   else    
;   { //choose_left_screen();
;     lcd_out_ctl_l(LCD_SET_PAGE+page);
; 	lcd_out_ctl_l(LCD_SET_COL+x);
;   }
; }
; 
; void write_to_lcd(unsigned char dat)
; {
	.dbline 223
;   lcd_out_dat_l(dat);
	mov R16,R20
	xcall _lcd_out_dat_l
	.dbline -2
L34:
	xcall pop_gset1
	.dbline 0 ; func end
	ret
	.dbsym r dat 20 c
	.dbend
