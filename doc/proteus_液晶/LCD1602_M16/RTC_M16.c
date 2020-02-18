// Project:LCD-162
// CPU:Atmega48
// F_CPU: Inner clock 8M
// Build:WinAvr2006.04.21(GCC)
// Design: 楼孟杰　marginlou@sohu.com
// 2007.02.4

#include <avr/io.h> 
#include <avr/interrupt.h>
#include "avr/interrupt.h"
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdlib.h>
#include <String.h>
//连接
// 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
// |  |  |  |  |  |  |__|__|__|__|__|__|__|  |  |
//GND |  VO RS |  E              |           +  -
//   VCC      R/W              PORTB
#define ON         1
#define OFF        0
#define DATA       PORTB
#define DATA_PORT  PORTB
#define DATA_DDR   DDRB
#define DATA_PIN   PINB
#define CMD_PORT   PORTD
#define CMD_DDR    DDRD
#define RS         0    //Pd0
#define RW         1   //Pd1
#define EN         2   //Pd2
#define TRUE         1
#define FALSE        0

#define SET_RS  CMD_PORT|=_BV(RS)
#define CLR_RS  CMD_PORT&=~_BV(RS)
#define SET_RW  CMD_PORT|=_BV(RW)
#define CLR_RW  CMD_PORT&=~_BV(RW)
#define SET_EN  CMD_PORT|=_BV(EN)
#define CLR_EN  CMD_PORT&=~_BV(EN)

volatile signed int Second=0;
volatile signed int Minute=0;
volatile signed int Hour=0;
volatile unsigned int Cursorx=16;
volatile unsigned int Cursory=1;

volatile unsigned char Timechange=FALSE;
volatile unsigned char Displaychange=0;
void  LCD_Init(void);
void  LCD_Char(unsigned char Xpos,unsigned char Ypos,unsigned char Lchar);
void  LCD_String(unsigned char Xpos,unsigned char Ypos,unsigned char *Lstring);
void  LCD_MoveTo(unsigned char Xpos,unsigned char Ypos);
void  LCD_CurShow(unsigned char Onoff);
void  LCD_CharBlink(unsigned char Onoff);
void  LCD_Command(unsigned char command);
void  LCD_Wait(void);
char  LCD_GetAddress(void);
void  LCD_Data(unsigned char Lchar);
void  LCD_Clear(void);
void  LCD_HZ(void);
void  LCD_HZ2(void);
//定义在EEPROM 中							 
 unsigned char LOU[48] EEMEM={
				             0x03,0x03,0x03,0x03,0x0f,0x03,0x03,0x03,
                             0x01,0x0d,0x07,0x0f,0x13,0x03,0x05,0x09,
                             0x00,0x0c,0x08,0x1e,0x10,0x10,0x0c,0x06,
                             0x07,0x0f,0x0b,0x13,0x03,0x03,0x03,0x00,
                             0x03,0x1f,0x06,0x04,0x01,0x01,0x06,0x00,
                             0x00,0x1f,0x18,0x10,0x00,0x1c,0x06,0x00
                            };
							 
//函数作用：读地址和BUSY标志
char LCD_GetAddress(void)
{
 char address;
 DATA_DDR=0;
  SET_RW;
  _delay_us(5);
 SET_EN;
 _delay_us(5);
 address=DATA_PIN;
 _delay_us(5); 
  CLR_EN;
 CLR_RW;
  _delay_us(50);
 return address;
}

//函数作用:如果忙，则等待
void LCD_Wait(void)
{
 while((LCD_GetAddress())&0x80);
}

//函数作用：送命令字
void LCD_Command(unsigned char command)
{
 LCD_Wait();
 DATA_DDR=0xFF;
 DATA_PORT=command;
 CLR_RW;
  CLR_RS;//command
  _delay_us(5);
 SET_EN;
 _delay_us(5);
 CLR_EN;
 _delay_us(5);
 SET_RS;
 SET_RW;
 DATA_DDR=0x00;
  _delay_us(50);
}

//函数作用:在位置Xpos,Ypos 处显示一个字符
void LCD_Char(unsigned char Xpos,unsigned char Ypos,unsigned char Lchar)
{
LCD_MoveTo(Xpos,Ypos);
LCD_Data(Lchar);
}

//函数作用：送数据
void LCD_Data(unsigned char Lchar)
{
 LCD_Wait();
 DATA_DDR=0xFF;
 DATA_PORT=Lchar;
 _delay_us(5);
 CLR_RW;
 SET_RS; //data
 _delay_us(5);
 SET_EN;

 _delay_us(5);
 CLR_EN;
 CLR_RS;
 SET_RW;
 DATA_DDR=0x00;
  _delay_us(50);
}

//函数作用：清屏
void  LCD_Clear(void)
{
 LCD_Command(0x01);
}

void  LCD_HZ2(void)
{
unsigned char i=0,StartX=0;
_delay_ms(10);
StartX=1;
   for(i=0;i<64;i++)
      {
       LCD_Command(0x40+i-1);
       LCD_Data(0);
      }
      LCD_Clear();
	  _delay_ms(100);
      for(i=0;i<48;i++)
      {//也是CGRAM的地址AC不能到0,进行了这样的处理
	    if(i==0)
		  LCD_Command(0x40);
		else
	      LCD_Command(0x40+i-1);
		
        LCD_Data(eeprom_read_byte(LOU+i));
      }
	  _delay_ms(100);
      LCD_Char(StartX+0,0,0x00);
      LCD_Char(StartX+1,0,0x01);
      LCD_Char(StartX+2,0,0x02);
      LCD_Char(StartX+0,1,0x03);
      LCD_Char(StartX+1,1,0x04);
      LCD_Char(StartX+2,1,0x05);
}

//函数作用：在Xpos,Ypos处显示字符串，
//          此函数实际使用时有问题，
//          主要是光标不能回复到(0,0)
void LCD_String(unsigned char Xpos,unsigned char Ypos,unsigned char *Lstring)
{
 LCD_MoveTo(Xpos,Ypos);
 while(*Lstring) LCD_Data(*Lstring++);
}

//函数作用:把光标移动到 Xpos,Ypos
void LCD_MoveTo(unsigned char Xpos,unsigned char Ypos)
{
  unsigned char address=0;
  if(Xpos<=0) Xpos=0;
  if(Ypos<=0) Ypos=0;
  if(Xpos>=15) Xpos=15;
  if(Ypos>=1)  Ypos=1;
  address=0x80+Xpos+0x40*Ypos;
  LCD_Command(address);
  //好像有问题，MoveTo(0,0),实际在第一行第二个字符
  //一直不能定位到第一个字符处。？？？
}

//函数作用：光标的打开，关闭
void LCD_CurShow(unsigned char Onoff)
{
 if(Onoff==0)
   LCD_Command(0x0C);
 else
   LCD_Command(0x0E);
}

//函数作用：光标的闪烁 的 开和关
void LCD_CharBlink(unsigned char Onoff)
{
 if(Onoff==0)
   LCD_Command(0x0C);
 else
   LCD_Command(0x0D);
}

//函数作用:LCD初始化
void LCD_Init(void)
{
 
 DATA_PORT=0;
 DATA_DDR=0xFF;
 CMD_PORT=0;
 CMD_DDR=0xFF;
 //_delay_ms(100);
 LCD_Command(0x38);
 _delay_ms(5);
 LCD_Command(0x38);
 _delay_us(100);
 LCD_Command(0x38);
 _delay_us(100);
 LCD_Command(0x38);//8bit mode, 2X16, 5X7 FONTS
 LCD_Command(0x0C); //
 LCD_Command(0x01); //clear screen
 LCD_Command(0x06); //cursor shift
}
void time2_RTCinit(void)
{
	TCCR2=0X07;
	ASSR=0X08;
	TCNT2=0Xdf;
	while(ASSR&0X05);
	TIMSK=0X40;
	asm("sei");
}
void key_deal(void)
{
	if(PINA&0X0f){
			_delay_ms(100);
		if(PINA&0X0f){			
			if(PINA&0X01){
				Cursorx-=3;
				if(	Cursorx<6)
					Cursorx=13;
				Timechange=TRUE;				
				
			}
			else if((PINA&0X02)&&(Timechange==TRUE)){
				Displaychange=2;
				if(Cursorx==13){
					Second++;
					if(Second==60)
						Second=0;
				}
				else if(Cursorx==10){		
					Minute++;
					if(Minute==60)
						Minute=0;
				}
				else{
					Hour++;
					if(Hour==25)
						Hour=0;
				}
					
			}
			else if((PINA&0X04)&&(Timechange==TRUE)){
				Displaychange=2;
				if(Cursorx==13){
					Second--;
					if(Second<0)
						Second=59;
				}
				else if(Cursorx==10){		
					Minute--;
					if(Minute<0)
						Minute=59;
				}
				else{
					Hour--;
					if(Hour<0)
						Hour=24;
				}
			}
			else{
		//		LCD_CharBlink(OFF);
				Timechange=FALSE;
			}
		}
	}
}
int main(void)
{
	unsigned char i=0;
	
	unsigned char mtimer[10]="00:00:00  ";
	unsigned char  avr[]={"AVR"};
 	LCD_Init();
  	LCD_Clear();
  	LCD_CharBlink(OFF);
 	LCD_Clear();
  	LCD_HZ2();
 	_delay_ms(100);
 	LCD_String(7,0,avr);
 	i=0;
 	DDRC=0XFF;
	DDRA=0X00;
 	time2_RTCinit();
//	LCD_String(5,1,mtimer);
 	while(1)
 	{
		
		if(Second==60){
			Second=0;
			Minute++;
			
			if(Minute==60){
				Minute=0;
				Hour++;
				if(Hour==24){
					Hour=0;
				}
			}
			
		}
		mtimer[7]=Second%10+'0';
		mtimer[6]=Second/10+'0';
		mtimer[4]=Minute%10+'0';
		mtimer[3]=Minute/10+'0';
		mtimer[1]=Hour%10+'0';
		mtimer[0]=Hour/10+'0';	
		key_deal();
		if(Timechange==FALSE)	
  			LCD_String(5,1,mtimer);
			
		else{	
			if(Displaychange%2==0){
				LCD_String(5,1,mtimer);
			}
			else{
				LCD_Char(Cursorx-1,Cursory,' ');
				LCD_Char(Cursorx-2,Cursory,' ');	
			}			
		}
		
 	}
	
}
ISR(TIMER2_OVF_vect)
{
	TCNT2=0Xdf;
	while(ASSR&0X04);
	if(Timechange==FALSE)
		Second++;
	else
		Displaychange++;
	
}
