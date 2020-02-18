#ifndef _HEAD_KS0108_
#define _HEAD_KS0108_
#include <lang.h>

// 外置接口：引脚配置
#define DATA (P0)
sbit RW = P2 ^ 2;
sbit EN = P2 ^ 4;
sbit RS = P2 ^ 3;
sbit CS1 = P2 ^ 0;
sbit CS2 = P2 ^ 1;

// =============内部功能===============


/*
	<- ROW     ->
	============= ^
	============= |
	============= PAGE
	============= |
	============= ^
*/
#define OPEN_DISY (0x3f)
#define CLOSE_DISY (0x3e)
#define PAGE (0xb8) // < 8
#define ROW (0x40) // < 64


void select_side(uchar buf){
	if(buf & 1)
		CS1 = 0;
	else
		CS1 = 1;
	
	if(buf & 2)
		CS2 = 0;
	else
		CS2 = 1;
}

void wait_busy() {
	RS = 0;
	RW = 1;
	do{
		DATA = 0x00;
		EN = 1;
		EN = 0;
	}while(DATA & 0x80);
}
void write_ord(uchar ord){
	wait_busy();
	
	RS = 0;
	RW = 0;
	DATA = ord;
	
	EN = 1;
	EN = 0;
}

void write_data(uchar dat){
	wait_busy();
	
	RS = 1;
	RW = 0;
	
	DATA = dat;
	
	EN = 1;
	EN = 0;
}

void led_init(){
	write_ord(OPEN_DISY);
	write_ord(PAGE);
	write_ord(ROW);
}

void cls_disy(uchar side){
	uchar i,j;
	select_side(side);
	for(i = 0 ; i < 8 ; ++i){
		write_ord(PAGE | i);
		write_ord(ROW);
		for(j = 0 ; j < 64 ; ++j)
			write_data(0xff);
	}
}
void cls_disy_dely(uchar side,uchar tim){
	uchar i,j;
	select_side(side);
	for(i = 0 ; i < 8 ; ++i){
		write_ord(PAGE | i);
		write_ord(ROW);
		for(j = 0 ; j < 64 ; ++j)
			write_data(0xff);
		dely_ms(tim);
	}
}
/*
x 横向
y 纵向
*/
void set_loc(uchar x,uchar y){
	if(x >= 64 || y >= 9) return;
	
	write_ord(PAGE | y);
	write_ord(ROW | x);
}

void show_font_8(uchar side,uchar x,uchar y,uchar* arr){
	uchar i = 0;
	select_side(side);
	set_loc(x,y);
	for(;i<8;++i) write_data(*(arr+i));
	set_loc(x,y+1);
	for(;i<16;++i) write_data(*(arr+i));
}

void show_font_8f(uchar side,uchar x,uchar y,uchar* arr,uchar isf){
	uchar i = 0;
	select_side(side);
	set_loc(x,y);
	for(;i<8;++i) 
		if(isf) write_data(~*(arr+i));
		else write_data(*(arr+i));
	set_loc(x,y+1);
	for(;i<16;++i) 
		if(isf) write_data(~*(arr+i));
		else write_data(*(arr+i));
}

void show_font_16(uchar side,uchar x,uchar y,uchar* arr){
	uchar i = 0;
	select_side(side);
	set_loc(x,y);
	for(;i<16;++i) write_data(*(arr+i));
	set_loc(x,y+1);
	for(;i<32;++i) write_data(*(arr+i));
}
#endif