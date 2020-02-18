/*******************************************************

程序功能：步进电机驱动

开发环境：Keil3

硬件环境：CEPARK多功能开发学习板/实验箱（2013版），STC12C5A60S2，11.092M晶振
																							   
接线说明：当步进电机使用JP30接口时，单片机P00~P03口与底板JP23相连，分别为：P00-IN1,P01-IN2,P02-IN3，P03-IN4
          当步进电机使用JP34接口时，单片机P00~P03口与底板JP32相连，分别为：P00-IN1,P01-IN2,P02-IN3，P03-IN4

跳线说明：无

实验现象：步进电机正反转各两周
          
									 
论坛支持：http://bbs.cepark.com

淘宝店铺：http://cepark.taobao.com

作者：cepark 

时间：2013-01-18

*******************************************************/
#include <reg52.h>



sbit A1=P0^0; //定义步进电机连接端口
sbit B1=P0^1;
sbit C1=P0^2;
sbit D1=P0^3;


#define Coil_A1 {A1=1;B1=0;C1=0;D1=0;}//A相通电，其他相断电
#define Coil_B1 {A1=0;B1=1;C1=0;D1=0;}//B相通电，其他相断电
#define Coil_C1 {A1=0;B1=0;C1=1;D1=0;}//C相通电，其他相断电
#define Coil_D1 {A1=0;B1=0;C1=0;D1=1;}//D相通电，其他相断电
#define Coil_AB1 {A1=1;B1=1;C1=0;D1=0;}//AB相通电，其他相断电
#define Coil_BC1 {A1=0;B1=1;C1=1;D1=0;}//BC相通电，其他相断电
#define Coil_CD1 {A1=0;B1=0;C1=1;D1=1;}//CD相通电，其他相断电
#define Coil_DA1 {A1=1;B1=0;C1=0;D1=1;}//D相通电，其他相断电
#define Coil_OFF {A1=0;B1=0;C1=0;D1=0;}//全部断电

unsigned char Speed;
/*------------------------------------------------
 uS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
 长度如下 T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
 while(--t);
}
/*------------------------------------------------
 mS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //大致延时1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}
/*------------------------------------------------
                    主函数
------------------------------------------------*/
main()
{
 unsigned int i=512;//正向旋转2周
 Speed=60;
 Coil_OFF
 while(i--)  //正向
  {         
     Coil_A1                //遇到Coil_A1  用{A1=1;B1=0;C1=0;D1=0;}代替
     DelayMs(Speed);         //改变这个参数可以调整电机转速 ,
                             //数字越小，转速越大,力矩越小
     Coil_B1
     DelayMs(Speed);
     Coil_C1
     DelayMs(Speed);
     Coil_D1
     DelayMs(Speed);
  }
  Coil_OFF
  i=512;
  while(i--)//反向旋转2周
  {
     Coil_D1                //遇到Coil_A1  用{A1=1;B1=0;C1=0;D1=0;}代替
     DelayMs(Speed);         //改变这个参数可以调整电机转速 ,
	                         //数字越小，转速越大,力矩越小
     Coil_C1
     DelayMs(Speed);
     Coil_B1
     DelayMs(Speed);
     Coil_A1
     DelayMs(Speed);
  }
}
