/*******************************************************


�����ܣ�DS18B20�¶ȴ���������

����������Keil3

Ӳ��������CEPARK�๦�ܿ���ѧϰ��/ʵ���䣨2013�棩��STC12C5A60S2��11.092M����
																							   
����˵������Ƭ��P10~P14����װ�JP51�������ֱ�Ϊ��P10-DIN,P11-SCK,P12-DC��P13-RES��P14-CS.
          ��Ƭ��P00��װ�JP35������

����˵����J70

ʵ�����������ǰ��λ��ʾ��ǰ�¶�ֵ
          
									 
��̳֧�֣�http://bbs.cepark.com

�Ա����̣�http://cepark.taobao.com

���ߣ�cepark 

ʱ�䣺2013-01-06


*******************************************************/

#include "STC12C5A60S2.h"
#include "intrins.h"

sbit DQ = P0^0;	//�¶ȴ���������

unsigned char Array_Point[]={0,1,1,2,3,3,4,4,5,6,6,7,8,8,9,9};//С�����


void Delay_us(unsigned int uiUs);	//us��ʱ����,12MHZ������Ч
void Delay_ms(unsigned int uiMs);	//ms��ʱ����,12MHZ������Ч

void DisplayOneCharOnAddr(unsigned char Data,unsigned char Addr); //��ָ����λ������ʾһ������
void DS18B20_Init(void);   //DS18B20��ʼ��
unsigned char DS18B20_ReadOneChar(void);   //��ȡһ������
void DS18B20_WriteOneChar(unsigned char dat);	  //д��һ������
unsigned int DS18B20_ReadTemperature(void);		  //��ȡ�¶�

sbit SEG_DS = P2^0;		//74HC595оƬ����������
sbit SEG_SHCP = P2^1;	//74HC595оƬ�Ŀ������ţ���������������
sbit SEG_STCP = P2^2;	//74HC595оƬ�Ŀ������ţ������ظ�������

unsigned char code Seg_Data[]={	 //��������ܵı��룬�������ݶ�����CODE��
							0xC0,/*0*/
					        0xF9,/*1*/
					        0xA4,/*2*/
					        0xB0,/*3*/
					        0x99,/*4*/
					        0x92,/*5*/
					        0x82,/*6*/
					        0xF8,/*7*/
					        0x80,/*8*/
					        0x90,/*9*/
					        0x88,/*A*/
					        0x83,/*b*/
					        0xC6,/*C*/
					        0xA1,/*d*/
					        0x86,/*E*/
					        0x8E,/*F*/
							};
unsigned char code Seg_Addr[]={
							0x01,
							0x02,
							0x04,
							0x08,
							0x10,
							0x20,
							0x40,
							0x80,
							0xFF,//ALL ON
							0x00 //OFF
							};
void SEG_Send595OneByte(unsigned char ucData);	 //��74HC595д��һ��8λ������

/********************************************************************
* ��������: main
* ��������: main�������������
* �������: ��
* ���ز���: ��
**********************************************************************/
void main(void)
{
	while(1)
	{	
		DisplayOneCharOnAddr((DS18B20_ReadTemperature() >> 4)/100,1);
		DisplayOneCharOnAddr((DS18B20_ReadTemperature() >> 4)/10,2);
		DisplayOneCharOnAddr((DS18B20_ReadTemperature() >> 4)%10,3);
		DisplayOneCharOnAddr(Array_Point[DS18B20_ReadTemperature() & 0x000F],4); 
	}
}

void SEG_Send595OneByte(unsigned char ucData)
{
	unsigned char i;
	
	for(i = 0;i < 8;i++)	 //8λ��������д�룬��д���λ
	{
		SEG_DS = (ucData & 0x80);	//�ȶ����λ   x&0x80;
		SEG_SHCP = 0;
		SEG_SHCP = 1;
		SEG_SHCP = 0;	   //SHCP���ŵ���������������
		ucData <<=1;	   //��������
	}
		 
}

void DS18B20_Init(void)
{
	unsigned char x=0;
	DQ = 1;    //DQ��λ
	Delay_us(10);
	//Delay(8);  //������ʱ,10us
	DQ = 0;    //��Ƭ����DQ����
	Delay_us(500);
	//Delay(80); //��ȷ��ʱ ���� 480us ,498us
	DQ = 1;    //��������
	Delay_us(154);
	//Delay(14);	//154us
	x=DQ;      //������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
	Delay_us(212);
	//Delay(20); //212us
}



unsigned char DS18B20_ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; // �������ź�
		dat>>=1;
		DQ = 1; // �������ź�
		if(DQ)
		dat|=0x80;
		Delay_us(56);
		//Delay(4); //56us
	}
	return(dat);
}



void DS18B20_WriteOneChar(unsigned char dat)
{
	unsigned char i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_us(66);
		//Delay(5); //66us
		DQ = 1;
		dat>>=1;
	}
}



unsigned int DS18B20_ReadTemperature(void)
{
	unsigned char a=0;
	unsigned char b=0;
	unsigned int t=0;
	DS18B20_Init();
	DS18B20_WriteOneChar(0xCC); // ����������кŵĲ���
	DS18B20_WriteOneChar(0x44); // �����¶�ת��
	DS18B20_Init();
	DS18B20_WriteOneChar(0xCC); //����������кŵĲ���
	DS18B20_WriteOneChar(0xBE); //��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
	a=DS18B20_ReadOneChar();
	b=DS18B20_ReadOneChar();
	t=b;
	t<<=8;
	t=t|a;
	//t= t/2; //�Ŵ�10���������������---����û��
	return(t);
}


void DisplayOneCharOnAddr(unsigned char Data,unsigned char Addr)
{
		if(Addr ==3)
		{
			SEG_Send595OneByte(Seg_Addr[Addr-1]);
			SEG_Send595OneByte(Seg_Data[Data]& 0x7F);
			
		}
		else
		{
		SEG_Send595OneByte(Seg_Addr[Addr-1]);  //��ʾ����һ���������
		SEG_Send595OneByte( Seg_Data[Data]);  //��ʾ������
		}
		SEG_STCP = 0;
		SEG_STCP = 1;		 //STCP���ŵ������ظ�������
		SEG_STCP = 0;
	
}


void Delay_us(unsigned int uiUs)	//us��ʱ����
{
	for(;uiUs > 0;uiUs--)
	{
		_nop_();
	}	
}



void Delay_ms(unsigned int uiMs)	//ms��ʱ����
{
	unsigned int i;
	for(;uiMs > 0;uiMs--)
	{
		for(i = 1498;i > 0;i--);
	}
}


