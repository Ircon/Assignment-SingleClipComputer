#include <reg51.h>

typedef unsigned int uint;
typedef unsigned char uchar;

#define DATA (P0)
sbit CS1 = P2 ^ 0;
sbit CS2 = P2 ^ 1;
sbit RW = P2 ^ 2;
sbit RS = P2 ^ 3;
sbit EN = P2 ^ 4;

sbit LED1 = P2 ^ 5;
sbit LED2 = P2 ^ 6;
sbit LED3 = P2 ^ 7;


// ³£Á¿
void select_side (uchar buf){
	if(buf & 1){
		CS1 = 0;
	}else{
		CS1 = 1;
	}
	
	if(buf & (uchar)2){
		CS2 = 0;
	}else{
		CS2 = 1;
	}
}
void wait_busy(){ // ?
	uchar buf;
	RS = 0;
	RW = 1;
	LED2 = 0;
	//return;
	do{
		DATA = 0x00; // ?
		EN = 1;
		EN = 0;
		
		buf = DATA;
		P1 = DATA;
	}while(buf & 0x80);
	LED1 = 0;
}
void writ_com(uchar com){
	wait_busy();
	
	RS = 0;
	RW = 0;
	DATA = com;
	EN = 1;
	EN = 0;
}
void writ_data(uchar info){
	wait_busy();
	RS = 1;
	RW = 0;
	DATA = info;
	EN = 1;
	EN = 0;
}
void clr_view(){

	uchar YE = 0xb8; // < 8
	uchar LIE = 0x40; // < 64
	uchar play = 0x00;
	uchar i,j,z;
	select_side(3);
	/*
	for(i = 0 ; i < 8 ; ++i){
		writ_com(YE | i);
		for(j = 0 ; j < 64 ; ++j){

			
			writ_com(LIE | j);
			writ_data(play);
			//for(z = 0 ; z < 16 ; ++z)
				//writ_data(play);
			
			
		}
	}*/
	writ_com(YE | 0);
	writ_com(LIE | 32);
	writ_data(play);
	//for(z = 0 ; z < 64 ; ++z)
		
}

void lcd_init(){
	
	writ_com(0x3f);
	writ_com(0xc0);
}
void main(void){
	lcd_init();
	clr_view();
	while(1){}
}