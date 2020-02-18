#include <reg51.h>
#include "math.h"
#define test

#define DATA_PORT (P0)
#define OUT_PORT (P1)

#define OPEN_SHOW_WORD (0x3F)
#define CLOSE_SHOW_WORD (0x3E)

typedef unsigned int uint;
typedef unsigned char uchar;

sbit CS1 = P2 ^ 0;
sbit CS2 = P2 ^ 1;
sbit RW = P2 ^ 2;
sbit RS = P2 ^ 3;
sbit E = P2 ^ 4;

sbit LED1 = P2 ^ 5;
sbit LED2 = P2 ^ 6;
sbit LED3 = P2 ^ 7;
//程序延时，用于LCD读写数据,以及初始化
void delay_us(uint t)							//us级延时程序
{
	for( ;t>0;t--);								
}

uchar code num[][16]=		//????:??12,?x?=8x16,????,????, ??
{
	0xFF,0x1F,0xEF,0xF7,0xF7,0xEF,0x1F,0xFF,0xFF,0xF0,0xEF,0xDF,0xDF,0xEF,0xF0,0xFF,//0,0
	0xFF,0xEF,0xEF,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xDF,0xDF,0xC0,0xDF,0xDF,0xFF,0xFF,//1,1
	0xFF,0x8F,0xF7,0xF7,0xF7,0x77,0x8F,0xFF,0xFF,0xCF,0xD7,0xDB,0xDD,0xDE,0xCF,0xFF,//2,2
	0xFF,0xCF,0xF7,0x77,0x77,0xB7,0xCF,0xFF,0xFF,0xE7,0xDF,0xDF,0xDF,0xEE,0xF1,0xFF,//3,3
	0xFF,0xFF,0x3F,0xDF,0xEF,0x07,0xFF,0xFF,0xFF,0xF8,0xFB,0xDB,0xDB,0xC0,0xDB,0xFF,//4,4
	0xFF,0x07,0xF7,0x77,0x77,0xF7,0xF7,0xFF,0xFF,0xE6,0xDE,0xDF,0xDF,0xEE,0xF1,0xFF,//5,5
	0xFF,0x1F,0xEF,0x77,0x77,0xE7,0xFF,0xFF,0xFF,0xF0,0xEE,0xDF,0xDF,0xEE,0xF1,0xFF,//6,6
	0xFF,0xC7,0xF7,0xF7,0x37,0xC7,0xF7,0xFF,0xFF,0xFF,0xFF,0xC0,0xFF,0xFF,0xFF,0xFF,//7,7
	0xFF,0x8F,0x77,0xF7,0xF7,0x77,0x8F,0xFF,0xFF,0xE3,0xDD,0xDE,0xDE,0xDD,0xE3,0xFF,//8,8
	0xFF,0x1F,0xEF,0xF7,0xF7,0xEF,0x1F,0xFF,0xFF,0xFF,0xCE,0xDD,0xDD,0xEE,0xF0,0xFF,//9,9
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//?10
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,//-,11
	0xFF,0xFF,0xFF,0x3F,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xCF,0xCF,0xFF,0xFF,0xFF,//:,12
	/*--  鏂囧瓧:  1  --*/
	/*--  瀹嬩綋12;  姝ゅ瓧浣撲笅瀵瑰簲鐨勭偣闃典负锛氬x楂?8x16   --*/
	0x00,0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,//13
	/*--  鏂囧瓧:  2  --*/
	/*--  瀹嬩綋12;  姝ゅ瓧浣撲笅瀵瑰簲鐨勭偣闃典负锛氬x楂?8x16   --*/
	0x00,0x70,0x08,0x08,0x08,0x08,0xF0,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//14

	/*--  鏂囧瓧:  3  --*/
	/*--  瀹嬩綋12;  姝ゅ瓧浣撲笅瀵瑰簲鐨勭偣闃典负锛氬x楂?8x16   --*/
	0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x20,0x21,0x21,0x22,0x1C,0x00,//15

	/*--  鏂囧瓧:  4  --*/
	/*--  瀹嬩綋12;  姝ゅ瓧浣撲笅瀵瑰簲鐨勭偣闃典负锛氬x楂?8x16   --*/
	0x00,0x00,0x80,0x40,0x30,0xF8,0x00,0x00,0x00,0x06,0x05,0x24,0x24,0x3F,0x24,0x24,//16

	/*--  鏂囧瓧:  5  --*/
	/*--  瀹嬩綋12;  姝ゅ瓧浣撲笅瀵瑰簲鐨勭偣闃典负锛氬x楂?8x16   --*/
	0x00,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x20,0x20,0x20,0x11,0x0E,0x00,//17
};

//状态检查，LCD是否忙
void check_busy()		
{
	uchar dat;									//状态信息（判断是否忙）
	RS=0; 								//选择指令寄存器 
	RW=1; 								//选择读取方式 
	do										//按以下操作
	{
		DATA_PORT=0x00;							//数据总线置0
		E=1;							//使能读出,状态值送总线
		delay_us(2);							//延时,等待数据稳定
		dat=DATA_PORT;							//读取总线并赋予变量
		E=0;							//停止读取
		dat=0x80 & dat; 						//仅当第7位为0时才可操作(判别busy信号)
	}
	while(!(dat==0x00));						//若busy不为0,则循环;为0则结束
}

//写命令cmdcode到LCD程序
void write_com(uchar cmdcode)
{
	check_busy();								//检测LCD是否忙
	RS=0;								//选择指令寄存器
	RW=0;								//选择写入方式
	P0=cmdcode;								//指令送总线
	delay_us(2);								//延时,待指令数据稳定
	E=1;								//使能写入
	delay_us(2);								//延时以确保写入完整
	E=0;								//停止写入
}

//写数据到LCD程序
void write_DATA_PORT(uchar lcdDATA_PORT)
{
	check_busy();								//检测LCD是否忙
	RS=1;								//选择数据寄存器
	RW=0;								//选择写入方式
	P0=lcdDATA_PORT;								//数据送总线
	delay_us(2);								//延时,待数据稳定
	E=1;								//使能写入
	delay_us(2);								//延时,确保写入完整
	E=0;								//停止写入
}

//从LCD读数据
uchar read_DATA_PORT()
{
	uchar lcdat;								//定义读数变量
	check_busy();								//检测LCD是否忙
	RS=1;								//选择数据寄存器
	RW=1;								//选择读取方式
	E=0;								//准备读
	E=1;								//使能输出
	delay_us(2);								//延时,待数据稳定
	lcdat=DATA_PORT;								//读取端口值
	delay_us(2);								//延时,确保读完整
	E=0;								//停止Y读取
	return lcdat;								//返回数据值
}

//左右屏选择,
void select_scr(uchar ss)
{
	switch(ss)								//依据选屏参数判断
	{
		case 0: CS1=0;CS2=0;break;		//选中全屏操作
		case 1: CS1=0;CS2=1;break;		//选中左屏操作
		case 2: CS1=1;CS2=0;break;		//选中右屏操作
		case 3: CS1=1;CS2=1;break;		//左右屏无操作
	}
}

//清除LCD内存程序,
void Clr_Scr(uchar ss)							//清理屏幕显示
{
	uchar j,k;									//定义页，列变量
	select_scr(ss);								//选择屏幕
 	for(k=0;k<8;k++)							//控制页数0-7，共8页
 	{
		write_com(0xb8+k);					//写入页地址
  		for(j=0;j<64;j++)						//控制列数0-63，列地址自动加1
		{
      		write_com(0x40+j);				//写入列地址
			write_DATA_PORT(0x00);					//写入空白内容，
		}
	}
}

//初始化LCD（不同的应用，初始化方法不同）
void init_lcd(uchar ss)							//LCD初始化程序
{
	check_busy();								//检测LCD是否忙
	select_scr(ss);								//选择屏幕
	Clr_Scr(ss);								//LCD清屏幕显示
	write_com(0x3e);			  				//关显示
	write_com(0xb8+0);				  		//设置:起始页地址
	write_com(0xc0+0);			  			//设置:起始行地址
	write_com(0x40+0);			  			//设置:起始列地址
	write_com(0x3f);				  			//开显示
}

//显示操作：

//指定地址按行显示编码数组hzk[][32]中连续的1~4个汉字(16*16)										
//参数:	ss=0~2; page=0~7; col=0~63; hzk——hzk[][32]; n=1~4
void hz_Disp16(uchar ss,uchar page,uchar column,uchar hzk[][32],uchar n)	
{
	uchar k,p,c;								//变量:汉字数,页,列
	select_scr(ss);								//选择屏幕
	for(k=0;k<n;k++)							//按汉字个数循环写入
	{
		for(p=0;p<2;p++)						//每个字符占 2 页,所以写2次
		{
			write_com(0xb8+page+p);		   	//写入显示页地址(page,page+1)
			write_com(0x40+column+16*k);		//写入显示列地址(起始列,0~16~32~48)
			for(c=0;c<16;c++)					//连续16次(上,下半字各16个编码)
			write_DATA_PORT(hzk[k][16*p+c]);			//每页写入16个字节的汉字编码数据
		}
	}
}

//重复显示二维数组szk[][16]中相同字符1~8次
void sz_Disp1(uchar ss,uchar page,uchar column,uchar szk[][16],uchar n)
{											//参数: 屏,页、列、码、字符数
	uchar i=0,j=0,k=0;				   			//变量: 页,列,字符数
	select_scr(ss);								//选择屏幕
	for(k=0;k<n;k++)
	{
		for(i=0;i<2;i++)					   	//每个字符占 2 页,所以写2次
		{
			write_com(0xb8+page+i);		   	//写入显示页地址(pag,pag+1)
			write_com(0x40+column+8*k);		//写入显示列地址(起始列)
			for(j=0;j<8;j++) 				   	//连续8次
			write_DATA_PORT(szk[0][8*i+j]);			//写入8个字节数据
		}
	}
}

//半屏图像显示函数（带参数：屏,页,列,数据）
void picture_disp12(uchar ss,uchar page,uchar column,uchar table[8][64])
{
	uchar i,j;									//页,列变量
	select_scr(ss);								//选择屏
  	if(column>=64)							//如果列地址大于等于64
	column=column-64; 						//列首地址仍从0x40开始显示
    for(i=0;i<8;i++)							//分8页写入
    {  
		write_com(0xb8+page+i);				//写页地址
        write_com(0x40+column);				//写列地址
      	for(j=0;j<64;j++)						//依次写入0~63列
        {
           write_DATA_PORT(table[i][j]);				//写图像数据
        }
    }
}

//绘图操作：

//反白功能打点函数,任意指定坐标画点	color=1，黑；color=0，白
void LCD_DrawDot(uchar x,uchar y,uchar color)//x水平坐标，y垂直坐标，color显示颜色 
{	
	char dat[8],Byte[8];  						//定义列字节数组
	uchar j;  								//循环变量
 	for(j=0;j<8;j++) dat[j]=0x00;     			//数组元素赋初值 0x00

	if(x<64)									//列坐标小于64
	select_scr(1);								//选择左半屏  
	else  									//列坐标大于等于64
	{select_scr(2);x-=64;}						//选择右半屏,列坐标调整为0~63
	for(j=1;j<7;j++)  							//每屏8页(0~7),8个字节
	{
		write_com(0xb8+j);    				//写页地址
		write_com(0x40+x);    				//写列地址
		read_DATA_PORT();							//空读一次
		Byte[j]=read_DATA_PORT();					//读出一字节数据
	 
		if(color)								//如果要显示点
		{
			dat[y>>3]|=0x01<<(y&0x07);		//y值所在页对应字节位置 1
			Byte[j]|=dat[j];					//修正原读数("或"运算)
		}					
		else 								//如果要擦除点
		{
			dat[y>>3]&=~(0x01<<(y&0x07));		//y值所在页对应字节位置 0
			Byte[j]&=dat[j];					//修正原读数("与"运算)
		}
		write_com(0x40+x);    				//重写列地址(因读数而改变)
		write_DATA_PORT(Byte[j]); 					//写入修正后的字节数据
	}
}


void dynamic_display()							//动态显示（随时更新的数值）
{
	sz_Disp1(2,0, 0,num[1],1);			//右屏0页第 0列显示'年十位'
	//sz_Disp1(2,0, 8,num[2],1);			//右屏0页第 8列显示'年个位'
	//sz_Disp1(2,0,24,num[3],1);			//右屏0页第24列显示'月十位'
	//sz_Disp1(2,0,32,num[4],1);			//右屏0页第32列显示'月个位'
	//sz_Disp1(2,0,48,num[5],1);			//右屏0页第48列显示'日十位'
	//sz_Disp1(2,0,56,num[6],1);			//右屏0页第56列显示'日个位'
	/*				
	sz_Disp1(2,4, 0,num[7],1);			//右屏4页第 0列显示'时十位'
	sz_Disp1(2,4, 8,num[8],1);			//右屏4页第 8列显示'时个位'
	sz_Disp1(2,4,24,num[9],1);			//右屏4页第24列显示'分十位'
	sz_Disp1(2,4,32,num[1],1);			//右屏4页第32列显示'分个位'
	sz_Disp1(2,4,48,num[2],1);			//右屏4页第48列显示'秒十位'
	sz_Disp1(2,4,56,num[3],1);			//右屏4页第56列显示'秒个位'
	*/
}
void fixed_display() 							//固定显示(按显示屏幕、页,列,字码,字数定位)
{
	sz_Disp1(1,0,16,num[11],1);					//左屏0页第16列显示'-'
	sz_Disp1(1,0,40,num[11],1);					//左屏0页第40列显示'-'
	//hz_Disp16(1,2,0,HZ[8],2);					//左屏2页第0列显示"星期"
	sz_Disp1(1,2,32,num[10],2);					//左屏2页第32列填充黑块"■" 2个
	sz_Disp1(1,4,16,num[12],1);					//左屏4页第16列显示':'
	sz_Disp1(1,4,40,num[12],1);					//左屏4页第40列显示':'
	//hz_Disp16(1,6,0,HZ[10],2);					//左屏6页第0列显示"气温"
	//hz_Disp16(1,6,48,HZ[0],1);					//左屏6页第48列显示"℃"
	
	//hz_Disp16(2,0,0,HZ[18],4);					//左屏第一行时间提醒
	//hz_Disp16(2,0,0,HZ[20],2);					//提醒1
	sz_Disp1(2,0,34,num[13],1);
	//hz_Disp16(2,2,0,HZ[20],2);					//提醒2
	sz_Disp1(2,2,34,num[14],1);
	//hz_Disp16(2,4,0,HZ[20],2);					//提醒3
	sz_Disp1(2,4,34,num[15],1);
}
void main(void){
	init_lcd(0);
	wait_busy();
		//dynamic_display();
		
	while(1){		
		fixed_display();
	}
	return;
}