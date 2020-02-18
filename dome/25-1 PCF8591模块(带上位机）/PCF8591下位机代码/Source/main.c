/************STC12C5A60S2单片机 PCF8591 AD DA转换实验**************

程序功能：PCF8591模块测试

开发环境：Keil3

硬件环境：CEPARK多功能开发学习板/实验箱（2013版），STC12C5A60S2，11.0592M晶振
																							   
接线说明：单片机P36接底板JP68.具体为：SCL——P37 ，SDA——P36

跳线说明：J3，J66

实验现象：打开上位机软件。打开串口，串口号从设备管理器中查看。波特率设为115200. 
		  打开 AD 转换，打开DA 转换。
		
			AD转换测试时
		上位机选择一个转换通道，跳接对应的J63上的跳线，调节可调电阻，上位机显示转换后的相应数值。 
			DA转换测试时 
		调节滑动条，改变输入的数字量。模块上的LED 指示灯亮度相应改变。


									 
论坛支持：http://bbs.cepark.com

淘宝店铺：http://cepark.taobao.com

作者：cepark 

时间：2013-01-01（元旦快乐！）


******************************************************************/

#include "STC12C5A.h"
#include "PCF8591_STC12C5A.h"

uint ii;
ulong ad_result=0;
uchar pcf8591_adchannel=0;

uchar da_val=0;

//*************** 函数定义 ******************
void delay1ms(unsigned int j);

void senddata_uart0(uchar dc);
void sendstring_uart0(uchar *dd);
void senduint_uart0(uint dd,uchar* dec_hex);


//*************** 主程序 ********************
void main(void)
{
  //////////////将这段代码嵌入到程序中////////////////
  if((PCON&0x10)==0)  //如果POF位=0
  {
    PCON=PCON|0x10;   //将POF位置1
    IAP_CONTR|=0x60;  //软复位,从ISP监控区启动
  }
  else
  {
    PCON=PCON&0xef;   //将POF位清零
  }
  ////////////////////////////////////////////////////

  SCON=0x50;         //uart0方式1,允许接收 
  AUXR=BRTR|S1BRS|BRTx12; //独立波特率发生器时钟为1T,uart0使用并启动独立波特率发生器
  BRT=0xfd;          //设置独立波特率发生器波特率  115200bps
  ES=1;              //uart0中断允许
  EA=1;              //中断总允许

  delay1ms(12000);
  sendstring_uart0("***          STC12C5A60S2         ***\r\n");
  sendstring_uart0("***     PCF8591 AD DA转换实验     ***\r\n");
  sendstring_uart0("***      2013年1月9日8:29:16      ***\r\n");

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
      else                //循环等待10000次,没有接收到数据退出while循环
      {
        count++;
      }
    }
  }

  if(1==m)
  {
    if(0xaa==tc[0])
    {
      IAP_CONTR=0x20;       //软复位,从用户程序区启动
    }
    else
    {
      senddata_uart0(tc[0]+1);    //将接收到的数据+1,再发送回去
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

//***************************发送数据**************************
void senddata_uart0(uchar dc)
{
  SBUF=dc;
  while (!TI);     //当TI=0时，发送未结束，循环等待
  TI=0;            //当TI=1时，发送结束，用软件将TI清零
}

//************************发送字符串数据***********************
void sendstring_uart0(uchar *dd)
{
  while ((*dd)!=0)
  {
    senddata_uart0(*dd);
    dd++;
  }
}

//******************以ascii码方式发送uint型数据*****************
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

//*************************延时 n*1ms********************
void delay1ms(unsigned int j)
{
  unsigned int i;
  for (;j>0;j--)
  {
    for (i=0;i<124;i++)
    {;}
  }
}
