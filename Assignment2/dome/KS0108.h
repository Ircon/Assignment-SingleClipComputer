//LCD-KS0108.h
//#include"Hzimo.h"	
#include<reg52.h>							 	//52????????
#include "math.h"
#define uint unsigned int							//??????????
#define uchar unsigned char						//??????????
#define PI 3.1415926							//???

//LCD????
#define	DATA P0								//????P0?
sbit lcd_RS=P2^3;  							//??/????(RS)
sbit lcd_RW=P2^2;  							//?/???
sbit lcd_EN=P2^4; 							//????

sbit lcd_CS1=P2^0;							//?lcd_CS1=0??????
sbit lcd_CS2=P2^1;  							//?lcd_CS2=0??????
//sbit lcd_RST=P2^5;							//???

//????:


//????,??LCD????,?????
void delay_us(uint t)							//us?????
{
	for( ;t>0;t--);								
}

//????,LCD???
void check_busy()		
{
	uchar dat;									//????(?????)
	lcd_RS=0; 								//??????? 
	lcd_RW=1; 								//?????? 
	do										//?????
	{
		DATA=0x00;							//?????0
		lcd_EN=1;							//????,??????
		delay_us(2);							//??,??????
		dat=DATA;							//?????????
		lcd_EN=0;							//????
		dat=0x80 & dat; 						//???7??0?????(??busy??)
	}
	while(!(dat==0x00));						//?busy??0,???;?0???
}

//???cmdcode?LCD??
void write_com(uchar cmdcode)
{
	check_busy();								//??LCD???
	lcd_RS=0;								//???????
	lcd_RW=0;								//??????
	P0=cmdcode;								//?????
	delay_us(2);								//??,???????
	lcd_EN=1;								//????
	delay_us(2);								//?????????
	lcd_EN=0;								//????
}

//????LCD??
void write_data(uchar lcddata)
{
	check_busy();								//??LCD???
	lcd_RS=1;								//???????
	lcd_RW=0;								//??????
	P0=lcddata;								//?????
	delay_us(2);								//??,?????
	lcd_EN=1;								//????
	delay_us(2);								//??,??????
	lcd_EN=0;								//????
}

//?LCD???
uchar read_data()
{
	uchar lcdat;								//??????
	check_busy();								//??LCD???
	lcd_RS=1;								//???????
	lcd_RW=1;								//??????
	lcd_EN=0;								//???
	lcd_EN=1;								//????
	delay_us(2);								//??,?????
	lcdat=DATA;								//?????
	delay_us(2);								//??,?????
	lcd_EN=0;								//??Y??
	return lcdat;								//?????
}

//?????,
void select_scr(uchar ss)
{
	switch(ss)								//????????
	{
		case 0: lcd_CS1=0;lcd_CS2=0;break;		//??????
		case 1: lcd_CS1=0;lcd_CS2=1;break;		//??????
		case 2: lcd_CS1=1;lcd_CS2=0;break;		//??????
		case 3: lcd_CS1=1;lcd_CS2=1;break;		//??????
	}
}

//??LCD????,
void Clr_Scr(uchar ss)							//??????
{
	uchar j,k;									//???,???
	select_scr(ss);								//????
 	for(k=0;k<8;k++)							//????0-7,?8?
 	{
		write_com(0xb8+k);					//?????
  		for(j=0;j<64;j++)						//????0-63,??????1
		{
      		write_com(0x40+j);				//?????
			write_data(0x00);					//??????,
		}
	}
}
void init_lcd(uchar ss)							//LCD?????
{
	check_busy();								//??LCD???
	select_scr(ss);								//????
	Clr_Scr(ss);								//LCD?????
	write_com(0x3e);			  				//???
	write_com(0xb8+0);				  		//??:?????
	write_com(0xc0+0);			  			//??:?????
	write_com(0x40+0);			  			//??:?????
	write_com(0x3f);				  			//???
}

void hz_Disp16(uchar ss,uchar page,uchar column,uchar hzk[][32],uchar n)	
{
	uchar k,p,c;								//??:???,?,?
	select_scr(ss);								//????
	for(k=0;k<n;k++)							//?????????
	{
		for(p=0;p<2;p++)						//????? 2 ?,???2?
		{
			write_com(0xb8+page+p);		   	//???????(page,page+1)
			write_com(0x40+column+16*k);		//???????(???,0~16~32~48)
			for(c=0;c<16;c++)					//??16?(?,????16???)
			write_data(hzk[k][16*p+c]);			//????16??????????
		}
	}
}
//void sz_Disp1(uchar ss,uchar page,uchar column,uchar szk[][16],uchar n)
void sz_Disp1(uchar ss,uchar page,uchar column,uchar szk[][16],uchar n)
{											//??: ?,?????????
	uchar i=0,j=0,k=0;				   			//??: ?,?,???
	select_scr(ss);								//????
	for(k=0;k<n;k++)
	{
		for(i=0;i<2;i++)					   	//????? 2 ?,???2?
		{
			write_com(0xb8+page+i);		   	//???????(pag,pag+1)
			write_com(0x40+column+8*k);		//???????(???)
			for(j=0;j<8;j++) 				   	//??8?
			write_data(szk[0][8*i+j]);			//??8?????
		}
	}
}