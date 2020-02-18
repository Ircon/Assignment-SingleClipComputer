#ifndef _HEAD_DS1302_
#define _HEAD_DS1302_

// ===================
#include <LANG.h>

 
   

/***********************
 1302引脚定义
***********************/
sbit  T_RST = P3^7;
sbit  T_DAT  = P3^6;
sbit  T_CLK = P3^5;
/********************************
 1302子程序区
********************************/

uchar BCD_Decimal(uchar bcd)					//BCD码转十进制函数，输入BCD，
{
	uchar Decimal;							//变量定义
	Decimal=bcd>>4;							//BCD码右移 4 位后赋予变量
	Decimal=Decimal*10+(bcd&=0x0F);			//转换成十进制
	return Decimal;							//返回十制值
}

uchar Dec_BCD(uchar dec){
	return (dec % 10) + ((dec / 10)<<4);
}

/*******************************
 DS1302写入1字节数据
*******************************/
void write_1302_byte(uchar dat)
{  
   uchar i;
    for(i=0;i<8;i++)
    {
        if(dat&0x01)T_DAT=1;
        else T_DAT=0;
        T_CLK=1;
  dat=dat>>1;   
        T_CLK=0;
    }//for
}
/*******************************
 从DS1302读出1字节数据
*******************************/
uchar read_1302_byte()
{        
 uchar i,dat;
 T_DAT=1;
    for(i=0;i<8;i++)
    {
      dat=dat>>1;
        if(T_DAT==1)dat=dat|0x80;
        T_CLK=1;
        T_CLK=0;    
    }//for            
    return dat;
}
/********************************
 DS1302写入指令或数据
********************************/
void write_1302(uchar addr,uchar dat)
{
 T_RST=0; //RST=0        
    T_CLK=0; //CLK=0
    T_RST=1; //RST=1,启动数据传输
    
    write_1302_byte(addr);
    write_1302_byte(dat);
  
    T_RST=0; //RST=0    
}
/********************************
 读DS1302里对应寄存器的数据
*********************************/
uchar read_1302(uchar addr)
{
 uchar dat;
    
    T_RST=0; //RST=0        
    T_CLK=0; //CLK=0
    T_RST=1; //RST=1,启动数据传输
    
    write_1302_byte(addr);
    dat=read_1302_byte();
    
    T_RST=0; //RST=0    
    
    return dat;
}
/******************************
 读取1302的时间
时间数据放在形参数组里，依次为
年、星期、月、日、时、分、秒
*******************************/
void read_time(uchar *p)
{
 uchar i;
 for(i=0x8d;i>=0x81;i--)
 {
  *p++=BCD_Decimal(read_1302(i));
  i--;
 }
}
/*****************************
 初始化1302时间
时间数据放在形参数组里，依次为
年、星期、月、日、时、分、秒
******************************/
void write_time(uchar *p)
{
 uchar i;

 write_1302(0x8e,0x00); //允许写操作
 for(i=0x8c;i>=0x80;i--)
 {
  write_1302(i,Dec_BCD(*p++));
  i--;
 }
 write_1302(0x8e,0x80); //禁止写操作
}


#endif