#include <REGX52.H>
#define uc unsigned char
uc code kkk[]=
{
	0x40,0x41,0xce,0x4,0x0,0x2,0x82,0x42,0xf2,0xe,0x42,0x82,0x2,0x2,0x0,0x0,
	0x40,0x20,0x1f,0x20,0x42,0x41,0x40,0x40,0x5f,0x40,0x40,0x40,0x47,0x42,0x40,0x00
/*
0x10,0x12,0xD2,0xFE,0x91,0x11,0x80,0xBF,// 程
0xA1,0xA1,0xA1,0xA1,0xBF,0x80,0x00,0x00,
0x04,0x03,0x00,0xFF,0x00,0x41,0x44,0x44,
0x44,0x7F,0x44,0x44,0x44,0x44,0x40,0x00
*/
};
//控制引脚定义
sbit E=0xB4; //lcd使能
sbit RW=0XB3; //读写控制
sbit RS=0XB2;//数据、指令控制
sbit L=0XB1;//左半屏选择
sbit R=0XB0;//右半屏选择
sbit Busy=0XA7;
//函数声明
void initLCD(void); //LCD初始化
void chkBusy(void); //检测LCD忙状态
void wrcode(uc); //写指令字
void wrdata(uc); //写现实数据
void disword(uc,uc,uc*);//显示子程序
//变量声明
uc j;
void main()
{
	P2=0x00;
	P1=0x00;
	initLCD();
	while(1)
	{
		disword(0xb8,0x40,&kkk);
	}
}
void initLCD(void) //LCD初始化
{
	wrcode(0x3f); //开显示
	wrcode(0xc0); //显示起始行设置
	wrcode(0xb8); //页面地址设置 此处选择0页
	wrcode(0x40); //列地址选择 此处选择00H
}
void chkBusy(void) //检测LCD忙状态
{
	E=1;
	RS=0;
	RW=1;
	P2=0xff;
	while(~Busy);
}
void wrcode(uc codech) //写指令字
{
	chkBusy();
	E=1;
	RW=0;
	RS=0;
	P2=codech;
	E=0;
}
void wrdata(uc datch) //写现实数据
{
chkBusy();
E=1;
RW=0;
RS=1;
P2=datch;
E=0;
}
void disword(uc page,uc col,uc* temp) //显示子程序
{
L=1;
R=0;
if(col>=0x80)
{
R=1;
L=0;
col-=0x10;
}
wrcode(page);
wrcode(col);
for(j=0;j<16;j++)
{
wrdata(~temp[j]);
}
wrcode(page+1);
wrcode(col);
for(j=16;j<32;j++)
{
wrdata(~temp[j]);
}
}