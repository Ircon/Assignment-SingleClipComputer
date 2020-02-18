	.module main.c
	.area text(rom, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run/m32_init.c
	.dbfunc e port_init _port_init fV
	.even
_port_init::
	.dbline -1
	.dbline 9
; //******************************************************************************
; //                                 本程序介绍
; //                                          ---北京交通大学电气学院 杨罡 2008.8
; //
; //       编译环境ICC6.31，
; //       使用ATMEGA32单片机 & KS0108B/HD61202控制器的128*64液晶
; //
; //   1、 本程序实现了基本的GUI绘图函数
; //       void lcd_fill( uint8_t pattern);全屏幕填充函数
	.dbline 10
; //       void lcd_erase(void);           擦除全屏幕的函数
	clr R2
	out 0x1b,R2
	.dbline 11
; //       void lcd_update_all(void);      把缓冲区显示到液晶上的刷新函数
	ldi R24,255
	out 0x1a,R24
	.dbline 12
; //       void lcd_dot( uint8_t x,  uint8_t y,  uint8_t mode); 任意坐标画点的函数
	out 0x18,R2
	.dbline 13
; //       void lcd_line( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode );
	out 0x17,R24
	.dbline 14
; //       任意（x1,y1）（x2,y2）两点之间的画直线函数，这个函数化很短的斜线时会出问题，一般的长斜线无问题
	out 0x15,R2
	.dbline 15
; //       void lcd_rect(  uint8_t x, uint8_t y, uint8_t width, uint8_t height,  uint8_t mode);
	out 0x14,R24
	.dbline 16
; //       画以（x,y）为左上角，高height，长width 的矩形的函数
	out 0x12,R2
	.dbline 17
; //       void lcd_box( uint8_t x,  uint8_t y, uint8_t width,  uint8_t height,  uint8_t mode);
	out 0x11,R24
	.dbline -2
L1:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e init_devices _init_devices fV
	.even
_init_devices::
	.dbline -1
	.dbline 22
; //       画以（x,y）为左上角，高height，长width 的矩形并且用mode方式填充的函数
; //       void lcd_circle( uint8_t xCenter,  uint8_t yCenter, uint8_t radius,  uint8_t mode);
; //       画（xCenter,yCenter）为圆心，半径为radius的函数，圆心可以在屏幕外面，所以可以画弧:)
; //    2、本程序实现多种字体
; //         可以根据需求去font.h文件选择你需要的字体，提供了5点6点7点9点10点15点18点
	.dbline 24
; //       的英文字体供用户选择，中文字库太大没有实现，不过一般英文就公用了吧
; //       void lcd_text(uint8_t left, uint8_t top, uint8_t font,   char *str);
	cli
	.dbline 25
; //       显示在坐标为（left,top）处显示字体为 font 的即时字符串 str
	xcall _port_init
	.dbline 27
; //       void lcd_text_p(uint8_t left, uint8_t top, uint8_t font,  const char *str);	
; //       显示在坐标为（left,top）处显示字体为 font 的存放在flash中的字符串 str
	clr R2
	out 0x35,R2
	.dbline 28
; //       font 参见font.h中的定义，需要先选择后使用
	out 0x3b,R2
	.dbline 29
; //    3、移植性
	out 0x39,R2
	.dbline 30
; //         液晶很多，这个只是其中一种的程序，但是本程序是可以移植的，你只需根据具体
	sei
	.dbline -2
L2:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfile C:\ygksb0108\programme2_can_run\main.c
	.dbfunc e project_init _project_init fV
	.even
_project_init::
	.dbline -1
	.dbline 51
; //       具体的液晶控制器去重写 LCD_DRIVER.H 和LCD_DRIVER.C 就可以了 
; //    4、使用                                           
; //         由于使用显示缓存区的缘故所以RAM容量在1K一下的AVR器件将无法使用本程序
; //       建议使用ATMEGA32或者ATMEGA64等大容量的片子，或者你课以将缓存区去掉，直接使用
; //       液晶的显示缓存读写，那么就可以用于任何AVR了:)
; //       !!!!!!!!值得注意的是：当你下载到片子运行时，请把LCD_DRIVER.C文件中的这一行注释掉
; //               #define debug_protues     
; //       否则将产生显示混乱。！！！！！！！！！！！！！！！！！！！！！！！    
; //     5、关于demo
; //        demo程序放在了main函数中，一共三个画面，可以改动其中的函数来熟悉以上介绍
; //       函数。调入位图的函数没有在demo中运行，你可以试着调用，但是需要加位图数据。                             
; //******************************************************************************
; #include "LCD_DRIVER.h"
; #include "m32_init.c"
; #include  "connection.h"
; #include  "font.h"
; #include  "bitmap.h"
; #include  "graphic.h"
; #include  "m32macro.h"
; void project_init(void)
; { 
	.dbline 52
;   init_devices();
	xcall _init_devices
	.dbline 53
;   PORTB=0x00;
	clr R2
	out 0x18,R2
	.dbline 54
;   lcd_init();
	xcall _lcd_init
	.dbline -2
L4:
	.dbline 0 ; func end
	ret
	.dbend
	.dbfunc e main _main fV
	.even
_main::
	sbiw R28,5
	.dbline -1
	.dbline 58
; }
; 
; void main()
; {
	.dbline 59
;  project_init();
	xcall _project_init
	xjmp L7
L6:
	.dbline 60
	.dbline 64
	ldi R24,<L9
	ldi R25,>L9
	std y+3,R25
	std y+2,R24
	clr R2
	std y+0,R2
	ldi R18,13
	ldi R16,15
	xcall _lcd_text
	.dbline 65
	ldi R24,1
	std y+4,R24
	ldi R24,64
	std y+2,R24
	ldi R24,128
	std y+0,R24
	clr R18
	clr R16
	xcall _lcd_rect
	.dbline 66
	ldi R24,1
	std y+2,R24
	ldi R24,10
	std y+0,R24
	ldi R18,31
	ldi R16,48
	xcall _lcd_circle
	.dbline 67
	ldi R24,1
	std y+2,R24
	ldi R24,10
	std y+0,R24
	ldi R18,31
	ldi R16,63
	xcall _lcd_circle
	.dbline 68
	ldi R24,1
	std y+2,R24
	ldi R24,10
	std y+0,R24
	ldi R18,31
	ldi R16,78
	xcall _lcd_circle
	.dbline 69
	ldi R24,1
	std y+2,R24
	ldi R24,10
	std y+0,R24
	ldi R18,46
	ldi R16,58
	xcall _lcd_circle
	.dbline 70
	ldi R24,1
	std y+2,R24
	ldi R24,10
	std y+0,R24
	ldi R18,46
	ldi R16,70
	xcall _lcd_circle
	.dbline 71
	xcall _lcd_update_all
	.dbline 73
	ldi R16,3000
	ldi R17,11
	xcall __delay_ms
	.dbline 74
	xcall _lcd_erase
	.dbline 75
	xcall _lcd_update_all
	.dbline 80
	ldi R24,1
	std y+4,R24
	ldi R24,10
	std y+2,R24
	std y+0,R24
	ldi R18,10
	ldi R16,10
	xcall _lcd_rect
	.dbline 81
	ldi R24,1
	std y+4,R24
	ldi R24,10
	std y+2,R24
	std y+0,R24
	ldi R18,10
	ldi R16,30
	xcall _lcd_box
	.dbline 82
	ldi R24,<L10
	ldi R25,>L10
	std y+3,R25
	std y+2,R24
	clr R2
	std y+0,R2
	ldi R18,25
	ldi R16,10
	xcall _lcd_text
	.dbline 83
	ldi R24,<L11
	ldi R25,>L11
	std y+3,R25
	std y+2,R24
	ldi R24,1
	std y+0,R24
	ldi R18,40
	ldi R16,10
	xcall _lcd_text
	.dbline 84
	xcall _lcd_update_all
	.dbline 86
	ldi R16,3000
	ldi R17,11
	xcall __delay_ms
	.dbline 87
	xcall _lcd_erase
	.dbline 88
	xcall _lcd_update_all
	.dbline 94
	ldi R24,<L12
	ldi R25,>L12
	std y+3,R25
	std y+2,R24
	clr R2
	std y+0,R2
	ldi R18,2
	ldi R16,30
	xcall _lcd_text
	.dbline 95
	ldi R24,2
	std y+4,R24
	ldi R24,11
	std y+2,R24
	ldi R24,128
	std y+0,R24
	clr R18
	clr R16
	xcall _lcd_box
	.dbline 96
	ldi R24,1
	std y+4,R24
	ldi R24,64
	std y+2,R24
	ldi R24,128
	std y+0,R24
	clr R18
	clr R16
	xcall _lcd_rect
	.dbline 97
	ldi R24,<L13
	ldi R25,>L13
	std y+3,R25
	std y+2,R24
	clr R2
	std y+0,R2
	ldi R18,30
	ldi R16,20
	xcall _lcd_text
	.dbline 98
	ldi R24,<L14
	ldi R25,>L14
	std y+3,R25
	std y+2,R24
	clr R2
	std y+0,R2
	ldi R18,50
	ldi R16,80
	xcall _lcd_text
	.dbline 99
	ldi R24,<L15
	ldi R25,>L15
	std y+3,R25
	std y+2,R24
	clr R2
	std y+0,R2
	ldi R18,50
	ldi R16,100
	xcall _lcd_text
	.dbline 100
	ldi R24,2
	std y+4,R24
	ldi R24,10
	std y+2,R24
	ldi R24,15
	std y+0,R24
	ldi R18,48
	ldi R16,79
	xcall _lcd_box
	.dbline 101
	ldi R24,2
	std y+4,R24
	ldi R24,10
	std y+2,R24
	ldi R24,20
	std y+0,R24
	ldi R18,48
	ldi R16,99
	xcall _lcd_box
	.dbline 102
	xcall _lcd_update_all
	.dbline 104
	ldi R16,3000
	ldi R17,11
	xcall __delay_ms
	.dbline 105
	xcall _lcd_erase
	.dbline 106
	xcall _lcd_update_all
	.dbline 108
L7:
	.dbline 60
	xjmp L6
X0:
	.dbline -2
L5:
	adiw R28,5
	.dbline 0 ; func end
	ret
	.dbend
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\main.c
L15:
	.blkb 5
	.area idata
	.byte 'e,'x,'i,'t,0
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\main.c
L14:
	.blkb 3
	.area idata
	.byte 'o,'k,0
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\main.c
L13:
	.blkb 21
	.area idata
	.byte 'w,'e,'l,'c,'o,'m,'e,32,'t,'o,32,'m,'y,32,'w,'i
	.byte 'n,'d,'o,'w,0
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\main.c
L12:
	.blkb 18
	.area idata
	.byte 'y,'a,'n,'g,'g,'a,'n,'g,39,'s,32,'w,'i,'n,'d,'o
	.byte 'w,0
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\main.c
L11:
	.blkb 9
	.area idata
	.byte 'y,'a,'n,'g,'g,'a,'n,'g,0
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\main.c
L10:
	.blkb 18
	.area idata
	.byte 'y,'a,'n,'g,'g,'a,'n,'g,32,49,50,58,48,48,58,48
	.byte 48,0
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\main.c
L9:
	.blkb 27
	.area idata
	.byte 'w,'e,'l,'c,'o,'m,'e,32,'t,'o,32,'b,'e,'i,'j,'i
	.byte 'n,'g,32,'o,'l,'y,'m,'p,'i,'c,0
	.area data(ram, con, rel)
	.dbfile C:\ygksb0108\programme2_can_run\main.c
