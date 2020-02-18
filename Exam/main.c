#include <reg51.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

#define RES (P2)
sbit ADDA = P1 ^ 3;
sbit ADDB = P1 ^ 4;
sbit ADDC = P1 ^ 5;

sbit ALE = P1 ^ 1;

sbit START = P1 ^ 0;
sbit OE = P1 ^ 2;
sbit LED = P1 ^ 6;
sbit EO = P3 ^ 2;
uchar RES_DATA;

void init(){
	// ÷–∂œ
	EA = 1;
	IT0 = 1;
	EX0 = 1;
	
	//µÿ÷∑
	ADDA = 1;
	ADDB = 1;
	ADDC = 1;
	ALE = 1;
	
	START = 1;
	START = 0;
}

void main(){
	init();
	while(1)
		P0 = ~RES_DATA;
		
}

void INT0_INT() interrupt 0{
	LED = ~LED;
	OE = 1;
	RES_DATA = RES;
	OE = 0;
	EO = 1;
	START = 1;
	START = 0;
}