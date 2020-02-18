/************STC12C5A60S2��Ƭ�� PCF8591 AD DAת��ʵ��**************

�����ܣ�PCF8591ģ�����

����������Keil3

Ӳ��������CEPARK�๦�ܿ���ѧϰ��/ʵ���䣨2013�棩��STC12C5A60S2��11.0592M����
																							   
����˵������Ƭ��P36�ӵװ�JP68.����Ϊ��SCL����P37 ��SDA����P36

����˵����J3��J66

ʵ�����󣺴���λ��������򿪴��ڣ����ںŴ��豸�������в鿴����������Ϊ115200. 
		  �� AD ת������DA ת����
		
			ADת������ʱ
		��λ��ѡ��һ��ת��ͨ�������Ӷ�Ӧ��J63�ϵ����ߣ����ڿɵ����裬��λ����ʾת�������Ӧ��ֵ�� 
			DAת������ʱ 
		���ڻ��������ı��������������ģ���ϵ�LED ָʾ��������Ӧ�ı䡣


									 
��̳֧�֣�http://bbs.cepark.com

�Ա����̣�http://cepark.taobao.com

���ߣ�cepark 

ʱ�䣺2013-01-01��Ԫ�����֣���


******************************************************************/

#include "STC12C5A.h"
#include "PCF8591_STC12C5A.h"

uint ii;
ulong ad_result=0;
uchar pcf8591_adchannel=0;

uchar da_val=0;

//*************** �������� ******************
void delay1ms(unsigned int j);

void senddata_uart0(uchar dc);
void sendstring_uart0(uchar *dd);
void senduint_uart0(uint dd,uchar* dec_hex);


//*************** ������ ********************
void main(void)
{
  //////////////����δ���Ƕ�뵽������////////////////
  if((PCON&0x10)==0)  //���POFλ=0
  {
    PCON=PCON|0x10;   //��POFλ��1
    IAP_CONTR|=0x60;  //��λ,��ISP���������
  }
  else
  {
    PCON=PCON&0xef;   //��POFλ����
  }
  ////////////////////////////////////////////////////

  SCON=0x50;         //uart0��ʽ1,������� 
  AUXR=BRTR|S1BRS|BRTx12; //���������ʷ�����ʱ��Ϊ1T,uart0ʹ�ò��������������ʷ�����
  BRT=0xfd;          //���ö��������ʷ�����������  115200bps
  ES=1;              //uart0�ж�����
  EA=1;              //�ж�������

  delay1ms(12000);
  sendstring_uart0("***          STC12C5A60S2         ***\r\n");
  sendstring_uart0("***     PCF8591 AD DAת��ʵ��     ***\r\n");
  sendstring_uart0("***      2013��1��9��8:29:16      ***\r\n");

  for(ii=0;ii<256;ii++)
  {
    pcf8591_da_write(ii);
    delay1ms(60);
  }

  delay1ms(8000);
  for(ii=254;ii!=0;ii--)
  {
    pcf8591_da_write(ii);
    delay1ms(60);
  }

  while(1)
  {
    pcf8591_ad_write(0x40+pcf8591_adchannel);
    ad_result=pcf8591_ad_read();

    ad_result=(ad_result*5000)/256;
    sendstring_uart0("pcf8591_adchannel");
    senduint_uart0(pcf8591_adchannel,"1d");
    sendstring_uart0("=");

    senduint_uart0(ad_result/1000,"1d");
    sendstring_uart0(".");
    senduint_uart0(ad_result%1000,"3d");
    sendstring_uart0("V  ");

    pcf8591_da_write(da_val);
    sendstring_uart0("pcf8591_da=0x");
    senduint_uart0(da_val,"2h");
    sendstring_uart0("\r\n");

    delay1ms(3800);
  }
}


//uart interrupt
void uart0(void) interrupt 4
{
  uchar tc[2];

  uint m=0;
  uint count=0;

  EA=0;
  if(RI)
  {
    while(count<10000)
    {
      if(RI)
      {
        RI=0;
        if(m<2)
        {
          tc[m]=SBUF;
        }
        m++;
        count=0;
      }
      else                //ѭ���ȴ�10000��,û�н��յ������˳�whileѭ��
      {
        count++;
      }
    }
  }

  if(1==m)
  {
    if(0xaa==tc[0])
    {
      IAP_CONTR=0x20;       //��λ,���û�����������
    }
    else
    {
      senddata_uart0(tc[0]+1);    //�����յ�������+1,�ٷ��ͻ�ȥ
    }
  }
  else if(2==m)
  {
    if(0x40==tc[0])
    {
      pcf8591_adchannel=tc[1]%4;
    }
    else if(0x50==tc[0])
    {
      da_val=tc[1];
    }
  }

  EA=1;
}

//***************************��������**************************
void senddata_uart0(uchar dc)
{
  SBUF=dc;
  while (!TI);     //��TI=0ʱ������δ������ѭ���ȴ�
  TI=0;            //��TI=1ʱ�����ͽ������������TI����
}

//************************�����ַ�������***********************
void sendstring_uart0(uchar *dd)
{
  while ((*dd)!=0)
  {
    senddata_uart0(*dd);
    dd++;
  }
}

//******************��ascii�뷽ʽ����uint������*****************
void senduint_uart0(uint dd,uchar* dec_hex)
{
  uchar code asciitab[16]={
                            '0','1','2','3','4','5','6','7',
                            '8','9','A','B','C','D','E','F',
                          };
  if (dec_hex=="1d")
  {
    if (dd<10)
    {
      senddata_uart0((dd%10)+'0');
    }
    else if (dd<100)
    {
      senddata_uart0((dd%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
    else if (dd<1000)
    {
      senddata_uart0((dd%1000/100)+'0');
      senddata_uart0((dd%1000%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
    else if (dd<10000)
    {
      senddata_uart0((dd/1000)+'0');
      senddata_uart0((dd%1000/100)+'0');
      senddata_uart0((dd%1000%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
    else
    {
      senddata_uart0((dd/10000)+'0');
      senddata_uart0((dd%10000/1000)+'0');
      senddata_uart0((dd%10000%1000/100)+'0');
      senddata_uart0((dd%10000%1000%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
  }
  else if (dec_hex=="2d")
  {
    if (dd<100)
    {
      senddata_uart0((dd%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
    else if (dd<1000)
    {
      senddata_uart0((dd%1000/100)+'0');
      senddata_uart0((dd%1000%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
    else if (dd<10000)
    {
      senddata_uart0((dd/1000)+'0');
      senddata_uart0((dd%1000/100)+'0');
      senddata_uart0((dd%1000%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
    else
    {
      senddata_uart0((dd/10000)+'0');
      senddata_uart0((dd%10000/1000)+'0');
      senddata_uart0((dd%10000%1000/100)+'0');
      senddata_uart0((dd%10000%1000%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
  }
  else if (dec_hex=="3d")
  {
    if (dd<1000)
    {
      senddata_uart0((dd%1000/100)+'0');
      senddata_uart0((dd%1000%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
    else if (dd<10000)
    {
      senddata_uart0((dd/1000)+'0');
      senddata_uart0((dd%1000/100)+'0');
      senddata_uart0((dd%1000%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
    else
    {
      senddata_uart0((dd/10000)+'0');
      senddata_uart0((dd%10000/1000)+'0');
      senddata_uart0((dd%10000%1000/100)+'0');
      senddata_uart0((dd%10000%1000%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
  }
  else if (dec_hex=="4d")
  {
    if (dd<10000)
    {
      senddata_uart0((dd/1000)+'0');
      senddata_uart0((dd%1000/100)+'0');
      senddata_uart0((dd%1000%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
    else
    {
      senddata_uart0((dd/10000)+'0');
      senddata_uart0((dd%10000/1000)+'0');
      senddata_uart0((dd%10000%1000/100)+'0');
      senddata_uart0((dd%10000%1000%100/10)+'0');
      senddata_uart0((dd%10)+'0');
    }
  }
  else if (dec_hex=="5d")
  {
    senddata_uart0((dd/10000)+'0');
    senddata_uart0((dd%10000/1000)+'0');
    senddata_uart0((dd%10000%1000/100)+'0');
    senddata_uart0((dd%10000%1000%100/10)+'0');
    senddata_uart0((dd%10)+'0');
  }

  else if (dec_hex=="2h")
  {
    if (dd<256)
    {
      senddata_uart0(asciitab[dd>>4]);
      senddata_uart0(asciitab[dd&0x0f]);
    }
    else
    {
      senddata_uart0(asciitab[dd>>12]);
      senddata_uart0(asciitab[(dd>>8)&0x000f]);
      senddata_uart0(asciitab[(dd>>4)&0x000f]);
      senddata_uart0(asciitab[dd&0x000f]);
    }
  }
  else if (dec_hex=="4h")
  {
    senddata_uart0(asciitab[dd>>12]);
    senddata_uart0(asciitab[(dd>>8)&0x000f]);
    senddata_uart0(asciitab[(dd>>4)&0x000f]);
    senddata_uart0(asciitab[dd&0x000f]);
  }
}

//*************************��ʱ n*1ms********************
void delay1ms(unsigned int j)
{
  unsigned int i;
  for (;j>0;j--)
  {
    for (i=0;i<124;i++)
    {;}
  }
}
