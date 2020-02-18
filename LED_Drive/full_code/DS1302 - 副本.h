#ifndef _HEAD_DS1302_
#define _HEAD_DS1302_
/*
sbit CLK = P2 ^ 5;
sbit IO = P2 ^ 6;
sbit RSET = P2 ^ 7;*/
// ===================
#include <lang.h>

sbit T_IO=P3^5;								//实时时钟数据线引脚
sbit T_CLK=P3^6;								//实时时钟时钟线引脚
sbit T_RST=P3^7;								//实时时钟复位线引脚
sbit ACC0=ACC^0;							//加法器最低位
sbit ACC7=ACC^7;							//加法器最高位

void RTI_1302_1Byte(uchar d)					//往DS1302写入1Byte数据(内部函数)
{
	uchar i;									//循环变量
	ACC=d;									//数据送累加器A
	for(i=8;i>0;i--)							//循环8次，
	{
		T_IO=ACC0;							//将ACC0位值（先低位，后高位）送数据端口
		T_CLK=1;							//拉高时钟线电平写入数值
		T_CLK=0;							//拉低时钟线电平停止写入
		ACC=ACC>>1;						//将A中数据右移 1 位
	}
}

uchar RTO_1302_1Byte()						//从DS1302读取1Byte数据(内部函数)
{											
	uchar i;									//变量定义
	for(i=8;i>0;i--)							//循环8次
	{
		ACC=ACC>>1;						//将ACC中数据右移 1 位
		ACC7=T_IO;							//读取一位值（先低位，后高位）送到ACC7位
		T_CLK=1;							//停止读取
		T_CLK=0;							//释放总线
	}
	return(ACC);								//返回ACC值
}

void Write_1302(uchar ucAddr,uchar ucDa)			//往DS1302写入数据，先地址后数据
{
	T_RST=0;								//复位引脚为低电平，所有数据传送终止
	T_CLK=0;								//清时钟总线
	T_RST=1;								//复位引脚为高电平，逻辑控制有效
	RTI_1302_1Byte(ucAddr);					//写实时时钟地址，命令
	RTI_1302_1Byte(ucDa);						//写实时时钟写1Byte数据
	T_CLK=1;								//时钟上升沿，发送数据有效
	T_RST=0;								//逻辑操作完毕，清 RST
}

uchar Read_1302(uchar ucAddr)					//读取DS1302中指定地址数据
{
	uchar ucData;								//定义数据变量
	T_RST=0;								//复位引脚为低电平，所有数据传送终止
	T_CLK=0;								//拉低时钟电平，清时钟总线
	T_RST=1;								//复位引脚为高电平，逻辑控制有效
	RTI_1302_1Byte(ucAddr);					//写入指定地址，命令
	ucData=RTO_1302_1Byte();					//读取1Byte数据
	T_CLK=1;								//时钟上升沿，传送数据有效
	T_RST=0;								//逻辑操作完毕，清 RST
	return(ucData);							//返回数据值
}
/*
//初始化设置仅限第一次使用DS1302,之后即可正常运行,故屏蔽此程序段
void Set_1302(uchar *pClock)					//设置DS1302初始时间
{
	uchar i;									//循环变量定义
	uchar ucAddr=0x80;						//地址变量定义（初值0x80）
	Write_1302(0x8e,0x00);						//“写控制”命令,WP=0,允许写操作
	//说明：命令字0X8E 为写控制，命令字0x8F 为读控制，
	//当写入0x00，最高位为WP=0，允许读写；	当写入0X80，最高位为WP=1，禁止读写。	
	for(i=7;i>0;i--)							//循环7次，依次写入
	{
		Write_1302(ucAddr,*pClock);			//秒，分，时，日，月，星期，年
		pClock++;							//每写一个字符，数据地址加 1
		ucAddr+=2;							//DS1302内部寄存器地址加 2
	}
	Write_1302(0x8e,0x80);						//控制命令,WP=1,写保护
}
*/
uchar BCD_Decimal(uchar bcd)					//BCD码转十进制函数，输入BCD，
{
	uchar Decimal;							//变量定义
	Decimal=bcd>>4;							//BCD码右移 4 位后赋予变量
	Decimal=Decimal*10+(bcd&=0x0F);			//转换成十进制
	return Decimal;							//返回十制值
}

void Get_1302(uchar ucCurtime[])					//读取DS1302实时数据(BCD码)
{
	uchar i;
	uchar ucAddr=0x81;						//开始地址: 秒寄存器
	for(i=0;i<7;i++)							//依次读取7个值
	{
		ucCurtime[i]=							//顺序: 秒分时日月星期年
			BCD_Decimal(Read_1302(ucAddr));	//转换成十进制	
		ucAddr+=2;							//每读一个值后,地址加2
	}
}

#endif