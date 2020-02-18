//**************************PCF8591_STC12C5A驱动库**************************
//功能：实现对PCF8591的读写
//原理：IIC协议
//注意：与PCF8591相连时必须外接上拉电阻
//stc12c5a  1T  11.0592MHz
//2013年1月9日8:21:03
//**************************************************************************

#ifndef __PCF8591_MCS51_H__
#define __PCF8591_MCS51_H__

#include <intrins.h>

#define pcf8591_read  0x91              //PCF8591读控制字
#define pcf8591_write 0x90              //PCF8591写控制字

sbit  scl=p3^7;  //i2c  SCL 时钟 PCF8591 10脚
sbit  sda=p3^6;  //i2c  SDA 数据 PCF8591 9脚


//*************** 函数定义 ******************
void i2c_start(void);
void i2c_stop(void);
uchar i2c_write(uchar c);
uchar i2c_read(uchar ack);

void pcf8591_ad_write(uchar c);
uchar pcf8591_ad_read(void);
void pcf8591_da_write(uchar dat);

void delay5us(void);            //精确延时函数


//**********************************i2c操作函数****************************
void i2c_start(void)  //起始条件
{
  sda=1;
  delay5us();
  scl=1;
  delay5us();
  sda=0;
  delay5us();
  scl=0;
  delay5us();
}

void i2c_stop(void)   //停止条件
{
  sda=0;
  delay5us();
  scl=1;
  delay5us();
  sda=1;
  delay5us();
}

//向总线写一字节,并返回有无应答
uchar i2c_write(uchar c)
{
  uchar i;
  uchar ack=1;
  for(i=0;i<8;i++)
  {
    if(c&0x80)
    {
      sda=1;
    }
    else
    {
      sda=0;
    }
    delay5us();    //very important//2012年3月26日9:56:29
    scl=1;
    delay5us();
    scl=0;
    c<<=1;
    delay5us();
  }
  sda=1;
  delay5us();
  scl=1;
  delay5us();
  if(sda==1)
  {
    ack=0;     //失败,无应答
  }
  scl=0;
  delay5us();
  return ack;
}

//读一字节 ack=1时应答,ack=0时不应答
uchar i2c_read(uchar ack)
{
  uchar ttt=0;
  uchar i;

  sda=1;
  for(i=0;i<8;i++)
  {
    delay5us();
    scl=0;
    delay5us();
    scl=1;
    delay5us();
    ttt<<=1;
    if(sda==1)
    {
      ttt++;
    }
  }
  delay5us();  //very important//2012年3月26日9:56:29
  scl=0;
  delay5us();
  if(ack)      //非应答
  {
    sda=0;
  }
  else         //应答
  {
    sda=1;
  }
  delay5us();
  scl=1;
  delay5us();
  scl=0;
  delay5us();
  return(ttt);
}

//11059200Hz  stc 1T时钟  5uS延时函数
void delay5us(void)   //误差 -0.026765046296us
{
    unsigned char a,b;
    for(b=3;b>0;b--)
        for(a=2;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}

//**********************************PCF8591操作函数****************************
void pcf8591_ad_write(uchar c)
{
  i2c_start();
  i2c_write(pcf8591_write); //发送器件地址+写指令
  i2c_write(c);
  i2c_stop();
}

uchar pcf8591_ad_read(void)
{
  uchar dat;
  i2c_start();
  i2c_write(pcf8591_read); //发送器件地址+读指令
  dat=i2c_read(0);
  i2c_stop();
  return(dat);
}

void pcf8591_da_write(uchar dat)
{
  i2c_start();
  i2c_write(pcf8591_write); //发送器件地址+写指令
  i2c_write(0x40);          //发送控制字节
  i2c_write(dat);           //发送DA数值
  i2c_stop();
}

#endif
