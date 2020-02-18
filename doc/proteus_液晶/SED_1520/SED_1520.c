#include<avr/io.h>
#include<avr/interrupt.h>
#include<avr/pgmspace.h>
#include <util/delay.h>
#define uchar unsigned char
#define uint unsigned int
unsigned char workstate=0;
unsigned char workmode=0xB1;
typedef struct _bit_struct
{
 unsigned char bit0:1;
 unsigned char bit1:1;
 unsigned char bit2:1;
 unsigned char bit3:1;
 unsigned char bit4:1;
 unsigned char bit5:1;
 unsigned char bit6:1;
 unsigned char bit7:1;
 }bit_filed;
#define _PORTB 0x25
#define E1 (*(volatile bit_filed*)(_PORTB)).bit5
#define E2 (*(volatile bit_filed*)(_PORTB)).bit4
#define RW (*(volatile bit_filed*)(_PORTB)).bit3
#define A0 (*(volatile bit_filed*)(_PORTB)).bit2
#define DATA PORTD
#define PIN PIND
#define DDR DDRD
const unsigned char bmp001[] =
{/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x08,0x31,0x86,0x60,0x00,0xFE,0x02,0xF2,0x02,0xFE,0x00,0xF8,0x00,0x00,0xFF,0x00,
0x04,0xFC,0x03,0x00,0x80,0x47,0x30,0x0F,0x10,0x67,0x00,0x07,0x40,0x80,0x7F,0x00,

/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x40,0x42,0xDC,0x08,0x00,0x90,0x90,0x90,0x90,0x90,0xFF,0x10,0x12,0x1C,0x10,0x00,
0x00,0x00,0x7F,0x20,0x10,0x20,0x20,0x1F,0x10,0x10,0x01,0x06,0x18,0x20,0x78,0x00,
};

//��ҳ
//��������
void OUTMD(unsigned char i)
{
RW=0;
A0=1; //д����
E1=1;
DATA=i;
E1=0;
}
//��ҳ
//��������
void OUTMI(unsigned char i)
{
RW=0;
A0=0; //дָ��
E1=1;
DATA=i;
E1=0;
}
//��ҳ
//��������
void OUTSD(unsigned char i)
{
RW=0;
A0=1; //д����
E2=1;
DATA=i;
E2=0;
}
//��ҳ
//��������
void OUTSI(unsigned char i)
{
RW=0;
A0=0; //дָ��
E2=1;
DATA=i;
E2=0;
}
//��ʼ��
void lcdini(void)
{
RW=0;
OUTMI(0XE2);
OUTSI(0XE2);//��λ
OUTMI(0XAE);
OUTSI(0XAE);//POWER SAVE
OUTMI(0XA4);
OUTSI(0XA4);//��̬����
OUTMI(0XA9);
OUTSI(0XA9);//1/32ռ�ձ�
OUTMI(0XA1);
OUTSI(0XA1);//ʱ�������
OUTMI(0XEE);
OUTSI(0XEE);//дģʽ

OUTMI(0X00);
OUTMI(0XC0);
OUTSI(0X00);
OUTSI(0XC0);

OUTMI(0XAF);
OUTSI(0XAF);

}
/*
����˵����ͬʱ����������ʾҳΪ0��3ҳ�����ں�����˽�У��û���ֱ�ӵ��ã�
------------------------------------------------------------------------
*/
void SetPage(uchar page0,uchar page1)
{
OUTMI(0xB8|page1);OUTSI(0xB8|page0);
}

/*
--------------------------------------------------------------------------------
����˵����ͬʱ���������е�ַΪ0��121�����ں�����˽�У��û���ֱ�ӵ��ã�
--------------------------------------------------------------------------------
*/
void SetAddress(uchar address0,uchar address1)
{
OUTMI(address0&0x7f);OUTSI(address1&0x7f);
}

/*
--------------------------------------------------------------------------------

���÷�ʽ��void PutChar0(uchar ch)
����˵��������ҳ��ǰ��ַ��һ���ֽ�8���㡣���ں�����˽�У��û���ֱ�ӵ��ã�
--------------------------------------------------------------------------------
-*/
void PutCharR(uchar ch)
{
OUTSD(ch);
}

/*
--------------------------------------------------------------------------------

���÷�ʽ��void PutChar1(uchar ch)
����˵��������ҳ��ǰ��ַ��һ���ֽ�8���㡣���ں�����˽�У��û���ֱ�ӵ��ã�
--------------------------------------------------------------------------------
-*/
void PutCharL(uchar ch)
{
OUTMD(ch);
}




/*
--------------------------------------------------------------------------------

���÷�ʽ��void clrscr(void)
����˵��������
--------------------------------------------------------------------------------
-*/
void clrscr(void)
{
uchar i;
uchar page;
for (page=0;page<4;page++)
{
SetPage(page,page);
SetAddress(0,0);
for (i=0;i<61;i++){PutCharR(0x00);PutCharL(0x00);}
}
}

//---------------------------------------------------------------
//---------------------------------------------------------------
//����zimo2 ��ʾ����,�Ժ��ֵ���ʾ��ʽ
//void Draw_word(uchar d_where,uint x_add,bit layer,uchar width)
//d_where��ʾ������еڼ������֣�x_add������λ��,layer��ʾ�Ĳ�, width��ʾ�Ŀ�ȡ�
//---------------------------------------------------------------


void Draw_word(unsigned int d_where,uint x_add,uchar layer,uchar width)
{
uchar x,address; //address��ʾ�Դ�������ַ
uchar window=0; //window��ʾ������ҳ
//putcharR //�ұ�
//putcharL //���
d_where=d_where*32;

for (x=width;x>1;x--)
{
	if (x_add>60) {
		window=1;
		address=x_add%61;
	}
	else 
		address=x_add;
	if(layer==0) //��ʾһ�а˸���
	{
		SetPage(0,0);
		SetAddress(address,address);
		if(window==1)
			PutCharR(bmp001[d_where]);//�ұ�
		else 
			PutCharL(bmp001[d_where]);//���
		SetPage(1,1);
		SetAddress(address,address);
		if(window==1)
			PutCharR(bmp001[d_where+width]);
		else 
			PutCharL(bmp001[d_where+width]);

}
else
{ //��ʾ�ڶ��а˸�����
	SetPage(2,2);
	SetAddress(address,address);
	if(window==1)
		PutCharR(bmp001[d_where]);//�ұ�
	else 
		PutCharL(bmp001[d_where]);//���
	SetPage(3,3);
	SetAddress(address,address);
	if(window==1)
		PutCharR(pgm_read_byte(bmp001[d_where+width]));
	else 
		PutCharL(pgm_read_byte(bmp001[d_where+width]));

	}
	x_add++;
	d_where++;
}
}

//-------------------------------------------------------
int main(void)
{
unsigned char i=0,j;
DDRB=0xff;
DDRD=0xff;
lcdini(); //reset
clrscr(); 
Draw_word(0,0,0,16);
Draw_word(1,16,0,16);
while(1);


}

   

   

   
