#include <reg51.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

#define LED P2
uchar CONT = 0;
const char MSG_SEG[] = "wodemaye";
uchar MSG_SEG_POS = 0;
void send_msg(uchar msg){
	SBUF = msg;
}
void dely(uint tim){
	while(tim--);
}
void init(){
	EA = 1;
	
	// T1 初始化
	TMOD = 0x20;
	TH1 = 0xfd;
	TL1 = 0xfd;
	
	ET1 = 1;
	
	//串行通信
	SCON = 0x40;
	
	ES = 1;
	TR1 = 1;
}

void main(){
	init();
	send_msg(MSG_SEG[MSG_SEG_POS++]);
	//LED = 0;
	while(1);
}

void RS_INT() interrupt 4{
	TI = 0;
	P2 = ~CONT;
	++CONT;
	if(MSG_SEG[MSG_SEG_POS] == '\0'){
		MSG_SEG_POS = 0;	
	}
	send_msg(MSG_SEG[MSG_SEG_POS++]);
	dely(50000);
	
}




//可行的代码
#include <reg51.h>

void main (void)
{
      TMOD = 0x20;		//使用Timer1, 8位自动重新加载模式
      TH1  = 0xFD;		//9600波特率，已知fosc=11.0592MHZ
      TL1  = 0xFD;

      SCON = 0x40;		//SM0、SM1=01，模式1，8位数据，1位终止位

      TR1  = 1;			

      while(1)
      {
           SBUF = 'a';		//将字符‘a’不断的放入SBUF；
           while (TI == 0);	//等待发送结束(TI=1)；
			TI = 0;		//清零TI
      }
}

// 可行的代码
#include<reg52.h>
#define uchar unsigned char
unsigned char num;
void init();
void main()
{
	init();
	SBUF = num;
   	while(1)
	{
		;	
	}
}	
void init()
{
	TMOD=0x20;
	TH1=0xfd;
	TL1=0xfd;
	
	TR1=1;
	REN=0;
	
	SM0=0;
	SM1=1;
	ES=1;
	TI = 0;
	EA=1;
	num='a';	
}	
void ser() interrupt 4
{
	TI=0;
	ES = 1;
    SBUF=num;
}

//可用的版本
#include <reg51.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

#define LEDS P2
sbit LED = P1 ^ 0;
uchar CONT = 0;
const char code MSG_SEG[] = "11wodemaye";
uchar MSG_SEG_POS = 0;
void send_msg(uchar msg){
	SBUF = msg;
}
void dely(uint tim){
	while(tim--);
}
void init(){

	EA = 1;
	
	// T1 初始化
	TMOD = 0x20;
	TH1 = 0xfd;
	TL1 = 0xfd;
	TR1 = 1;
	//ET1 = 1;
	
	//串行通信
	SCON = 0x40;
	
	ES = 1;
	
	//TI = 0;

	
}

void main(){
	init();
	send_msg(MSG_SEG[MSG_SEG_POS++]);
	
	while(1);
}

void RS_INT() interrupt 4{
	LED = 0;
	LEDS = ~CONT;
	++CONT;
	if(MSG_SEG[MSG_SEG_POS] == '\0'){
		MSG_SEG_POS = 0;	
	}
	TI = 0;
	send_msg(MSG_SEG[MSG_SEG_POS++]);
	
}

///
#include <reg51.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

#define LEDS P2
sbit LED = P1 ^ 0;
uchar CONT = 0;
void init(){
	TMOD = 0x20;
	TH1 = 0xfd;
	TL1 = 0xfd;
	SCON = 0x50;
	
	EA = 1;
	TR1 = 1;
	ES = 1;	
}

void main(){
	init();
	while(1);
}

void ES_INT() interrupt 4{
	if(TI == 1){
		TI = 0;
	}
	if(RI == 1){
		RI = 0;
		ACC = SBUF;
		SBUF = ACC;
	}
	++CONT;
	LEDS = ~CONT;
}