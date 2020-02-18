//**************************PCF8591_STC12C5A������**************************
//���ܣ�ʵ�ֶ�PCF8591�Ķ�д
//ԭ��IICЭ��
//ע�⣺��PCF8591����ʱ���������������
//stc12c5a  1T  11.0592MHz
//2013��1��9��8:21:03
//**************************************************************************

#ifndef __PCF8591_MCS51_H__
#define __PCF8591_MCS51_H__

#include <intrins.h>

#define pcf8591_read  0x91              //PCF8591��������
#define pcf8591_write 0x90              //PCF8591д������

sbit  scl=p3^7;  //i2c  SCL ʱ�� PCF8591 10��
sbit  sda=p3^6;  //i2c  SDA ���� PCF8591 9��


//*************** �������� ******************
void i2c_start(void);
void i2c_stop(void);
uchar i2c_write(uchar c);
uchar i2c_read(uchar ack);

void pcf8591_ad_write(uchar c);
uchar pcf8591_ad_read(void);
void pcf8591_da_write(uchar dat);

void delay5us(void);            //��ȷ��ʱ����


//**********************************i2c��������****************************
void i2c_start(void)  //��ʼ����
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

void i2c_stop(void)   //ֹͣ����
{
  sda=0;
  delay5us();
  scl=1;
  delay5us();
  sda=1;
  delay5us();
}

//������дһ�ֽ�,����������Ӧ��
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
    delay5us();    //very important//2012��3��26��9:56:29
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
    ack=0;     //ʧ��,��Ӧ��
  }
  scl=0;
  delay5us();
  return ack;
}

//��һ�ֽ� ack=1ʱӦ��,ack=0ʱ��Ӧ��
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
  delay5us();  //very important//2012��3��26��9:56:29
  scl=0;
  delay5us();
  if(ack)      //��Ӧ��
  {
    sda=0;
  }
  else         //Ӧ��
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

//11059200Hz  stc 1Tʱ��  5uS��ʱ����
void delay5us(void)   //��� -0.026765046296us
{
    unsigned char a,b;
    for(b=3;b>0;b--)
        for(a=2;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}

//**********************************PCF8591��������****************************
void pcf8591_ad_write(uchar c)
{
  i2c_start();
  i2c_write(pcf8591_write); //����������ַ+дָ��
  i2c_write(c);
  i2c_stop();
}

uchar pcf8591_ad_read(void)
{
  uchar dat;
  i2c_start();
  i2c_write(pcf8591_read); //����������ַ+��ָ��
  dat=i2c_read(0);
  i2c_stop();
  return(dat);
}

void pcf8591_da_write(uchar dat)
{
  i2c_start();
  i2c_write(pcf8591_write); //����������ַ+дָ��
  i2c_write(0x40);          //���Ϳ����ֽ�
  i2c_write(dat);           //����DA��ֵ
  i2c_stop();
}

#endif
