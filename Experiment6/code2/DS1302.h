#ifndef _HEAD_DS1302_
#define _HEAD_DS1302_

// ===================
#include <LANG.h>

 
   

/***********************
 1302���Ŷ���
***********************/
sbit  T_RST = P3^7;
sbit  T_DAT  = P3^6;
sbit  T_CLK = P3^5;
/********************************
 1302�ӳ�����
********************************/

uchar BCD_Decimal(uchar bcd)					//BCD��תʮ���ƺ���������BCD��
{
	uchar Decimal;							//��������
	Decimal=bcd>>4;							//BCD������ 4 λ�������
	Decimal=Decimal*10+(bcd&=0x0F);			//ת����ʮ����
	return Decimal;							//����ʮ��ֵ
}

uchar Dec_BCD(uchar dec){
	return (dec % 10) + ((dec / 10)<<4);
}

/*******************************
 DS1302д��1�ֽ�����
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
 ��DS1302����1�ֽ�����
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
 DS1302д��ָ�������
********************************/
void write_1302(uchar addr,uchar dat)
{
 T_RST=0; //RST=0        
    T_CLK=0; //CLK=0
    T_RST=1; //RST=1,�������ݴ���
    
    write_1302_byte(addr);
    write_1302_byte(dat);
  
    T_RST=0; //RST=0    
}
/********************************
 ��DS1302���Ӧ�Ĵ���������
*********************************/
uchar read_1302(uchar addr)
{
 uchar dat;
    
    T_RST=0; //RST=0        
    T_CLK=0; //CLK=0
    T_RST=1; //RST=1,�������ݴ���
    
    write_1302_byte(addr);
    dat=read_1302_byte();
    
    T_RST=0; //RST=0    
    
    return dat;
}
/******************************
 ��ȡ1302��ʱ��
ʱ�����ݷ����β����������Ϊ
�ꡢ���ڡ��¡��ա�ʱ���֡���
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
 ��ʼ��1302ʱ��
ʱ�����ݷ����β����������Ϊ
�ꡢ���ڡ��¡��ա�ʱ���֡���
******************************/
void write_time(uchar *p)
{
 uchar i;

 write_1302(0x8e,0x00); //����д����
 for(i=0x8c;i>=0x80;i--)
 {
  write_1302(i,Dec_BCD(*p++));
  i--;
 }
 write_1302(0x8e,0x80); //��ֹд����
}


#endif