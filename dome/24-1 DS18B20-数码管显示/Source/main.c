/*******************************************************


程序功能：DS18B20温度传感器测试

开发环境：Keil3

硬件环境：CEPARK多功能开发学习板/实验箱（2013版），STC12C5A60S2，11.092M晶振
																							   
接线说明：单片机P10~P14口与底板JP51相连，分别为：P10-DIN,P11-SCK,P12-DC，P13-RES，P14-CS.
          单片机P00与底板JP35相连。

跳线说明：J70

实验现象：数码管前四位显示当前温度值
          
									 
论坛支持：http://bbs.cepark.com

淘宝店铺：http://cepark.taobao.com

作者：cepark 

时间：2013-01-06


*******************************************************/

#include "STC12C5A60S2.h"
#include "intrins.h"

sbit DQ = P0^0;	//温度传感器引脚

unsigned char Array_Point[]={0,1,1,2,3,3,4,4,5,6,6,7,8,8,9,9};//小数查表


void Delay_us(unsigned int uiUs);	//us延时函数,12MHZ晶振有效
void Delay_ms(unsigned int uiMs);	//ms延时函数,12MHZ晶振有效

void DisplayOneCharOnAddr(unsigned char Data,unsigned char Addr); //在指定的位置上显示一个数字
void DS18B20_Init(void);   //DS18B20初始化
unsigned char DS18B20_ReadOneChar(void);   //读取一个数据
void DS18B20_WriteOneChar(unsigned char dat);	  //写入一个数据
unsigned int DS18B20_ReadTemperature(void);		  //读取温度

sbit SEG_DS = P2^0;		//74HC595芯片的数据引脚
sbit SEG_SHCP = P2^1;	//74HC595芯片的控制引脚，上升沿移入数据
sbit SEG_STCP = P2^2;	//74HC595芯片的控制引脚，上升沿更新数据

unsigned char code Seg_Data[]={	 //共阳数码管的编码，并将数据定义在CODE区
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
void SEG_Send595OneByte(unsigned char ucData);	 //向74HC595写入一个8位的数据

/********************************************************************
* 函数名称: main
* 功能描述: main函数，程序入口
* 输入参数: 无
* 返回参数: 无
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
	
	for(i = 0;i < 8;i++)	 //8位数据依次写入，先写最低位
	{
		SEG_DS = (ucData & 0x80);	//先读入高位   x&0x80;
		SEG_SHCP = 0;
		SEG_SHCP = 1;
		SEG_SHCP = 0;	   //SHCP引脚的上升沿移入数据
		ucData <<=1;	   //数据左移
	}
		 
}

void DS18B20_Init(void)
{
	unsigned char x=0;
	DQ = 1;    //DQ复位
	Delay_us(10);
	//Delay(8);  //稍做延时,10us
	DQ = 0;    //单片机将DQ拉低
	Delay_us(500);
	//Delay(80); //精确延时 大于 480us ,498us
	DQ = 1;    //拉高总线
	Delay_us(154);
	//Delay(14);	//154us
	x=DQ;      //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	Delay_us(212);
	//Delay(20); //212us
}



unsigned char DS18B20_ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; // 给脉冲信号
		dat>>=1;
		DQ = 1; // 给脉冲信号
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
	DS18B20_WriteOneChar(0xCC); // 跳过读序号列号的操作
	DS18B20_WriteOneChar(0x44); // 启动温度转换
	DS18B20_Init();
	DS18B20_WriteOneChar(0xCC); //跳过读序号列号的操作
	DS18B20_WriteOneChar(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
	a=DS18B20_ReadOneChar();
	b=DS18B20_ReadOneChar();
	t=b;
	t<<=8;
	t=t|a;
	//t= t/2; //放大10倍输出并四舍五入---此行没用
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
		SEG_Send595OneByte(Seg_Addr[Addr-1]);  //显示在哪一个数码管上
		SEG_Send595OneByte( Seg_Data[Data]);  //显示的数据
		}
		SEG_STCP = 0;
		SEG_STCP = 1;		 //STCP引脚的上升沿更新数据
		SEG_STCP = 0;
	
}


void Delay_us(unsigned int uiUs)	//us延时函数
{
	for(;uiUs > 0;uiUs--)
	{
		_nop_();
	}	
}



void Delay_ms(unsigned int uiMs)	//ms延时函数
{
	unsigned int i;
	for(;uiMs > 0;uiMs--)
	{
		for(i = 1498;i > 0;i--);
	}
}


