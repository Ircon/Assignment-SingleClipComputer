#ifndef _HEAD_LCD12864_
#define _HEAD_LCD12864_

#include <LANG.h>
// ST7920
// === 引脚定义 ===
sbit LCD_RS = P3 ^ 4;
sbit LCD_RW = P3 ^ 3;
sbit LCD_EN = P3 ^ 2;
sbit LCD_CS1 = P3 ^ 1;
sbit LCD_CS2 = P3 ^ 0;
#define DATA_PORT (P1)

//sbit LED1 = P3 ^ 5;
//sbit LED2 = P3 ^ 6;

//常量
#define BE_COMMAND (0)
#define BE_DATA (1)

// === 功能函数 ===
/*
是否处于忙状态
*/
uchar is_busy(){
	uchar buf;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_EN = 1;
	buf = DATA_PORT;
	return !(!(buf & 0x80));
}

/*
忙等待。
*/
uchar wait_busy(){
	uchar buf,i = 0xFF;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_EN = 1;
	while((i > 0) && ((buf = DATA_PORT) & 0x80))
		--i;
	return !(!(buf & 0x80));
}

/*
写入的数据，数据还是命令
dat IO数据
isDat
	1:写入数据
	0:写入命令
*/
void LCD_IOData(uchar dat, bit isDat){
	wait_busy();
	LCD_RW = 0;
	LCD_RS = isDat;
	
	dely_US(10);
	DATA_PORT = dat;
	LCD_EN  = 1;
	dely_US(10);
	LCD_EN = 0;
	dely_US(10);
}

/*
初始化
*/
void LCD_Init(){
	dely_US(400);//延时
	LCD_CS2 = 1;	//8位并口工作模式
	dely_US(150);//延时
	
	LCD_IOData(0x36,0); //基本指令集
	dely_US(150);
	LCD_IOData(0x36,0);  //显示设置
	dely_US(150);
	LCD_IOData(0x3E,0);	//光标设置
	dely_US(150);
	LCD_IOData(0x0c,0);	//游标设置
	dely_US(150);
	LCD_IOData(0x01,0);  //清屏
	dely_US(150);
}
void LCD_Init2(){
	LCD_CS2 = 1;
	LCD_IOData(0x30,BE_COMMAND);
	LCD_IOData(0x01,BE_COMMAND);
	LCD_IOData(0x06,BE_COMMAND);
	LCD_IOData(0x0c,BE_COMMAND);
}
/*
文字显示
*/
void font_disp(uchar code *fon){
	uchar i,j;
	LCD_IOData(0x30,BE_COMMAND);
	LCD_IOData (0x80,BE_COMMAND);
	
	for(j = 0 ; j < 4 ; ++j){
		for(i = 0; i < 16 ; ++i){
			LCD_IOData(BE_DATA,fon[j*16 + i]);
		}
	}
}



void display_font8(uchar x,uchar y,uchar *fon){
	uchar i;
	for(i = 0 ; i < 16 ; ++i){
		LCD_IOData(0x80 | (i + x),BE_COMMAND);
		LCD_IOData(0x80 | y,BE_COMMAND);
		LCD_IOData(fon[i],BE_DATA);
	}
}
void display_font16(uchar x,uchar y,uchar *fon){
	uchar i,j=0;
	for(i = 0 ; i < 16 ; ++i){
		LCD_IOData(0x80 | (i + x),BE_COMMAND);
		LCD_IOData(0x80 | y,BE_COMMAND);
		LCD_IOData(*fon++,BE_DATA);
		LCD_IOData(*fon++,BE_DATA);
	}
}
void display_font16_l(uchar x,uchar y,uchar *fon){
	uchar i,j=0;
	for(i = 0 ; i < 16 ; ++i){
		LCD_IOData(0x80 | (i + x),BE_COMMAND);
		LCD_IOData(0x88 | y,BE_COMMAND);
		LCD_IOData(*fon++,BE_DATA);
		LCD_IOData(*fon++,BE_DATA);
	}
}
void cls_display(){
	uchar i,j;
	for(i = 0 ; i < 32 ; ++i){
		LCD_IOData(0x80 | i,BE_COMMAND);
		LCD_IOData(0x80,BE_COMMAND);
		for(j = 0 ; j < 16 ; ++j){
			LCD_IOData(0x00,BE_DATA);
		}
	}
	for(i = 0 ; i < 32 ; ++i){
		LCD_IOData(0x80 | i,BE_COMMAND);
		LCD_IOData(0x88,BE_COMMAND);
		for(j = 0 ; j < 16 ; ++j){
			LCD_IOData(0x00,BE_DATA);
		}
	}
}
void clr_ram(){
	LCD_IOData(0x30,BE_COMMAND);
	LCD_IOData(0x01,BE_COMMAND);
}
/*
void test_disply3(){
	//LCD_Init();
	cls_display();
	display_font16(0,0,font123);
}*/
#endif