#ifndef _12864LCD_HEAD_
#define _12864LCD_HEAD_

#include <LANG.h>

#define WORD_CLS (0x01)
#define WORD_CLS_DRE (0x02)
#define WORD_CLOS (0x0f)
// Òý½Å¶¨Òå
sbit CS2 = P2 ^ 0;
sbit CS1 = P2 ^ 1;
sbit EN = P2 ^ 2;
sbit RW = P2 ^ 3;
sbit RS = P2 ^ 4;
#define IO_PORT (P0)
void chose_side(uchar side) {
	if(side & 1){
		CS1 = 1;
	}else{
		CS1 = 0;
	}

	if(side & 2){
		CS2 = 1;
	}else{
		CS2 =  0;
	}
}

void send_ord(uchar ord){
	RW = 0;
	RS = 0;
	EN = 1;
	dely_US(10);
	IO_PORT = ord;
	dley_US(10);
	EN = 0;
	dely_US(10);
	EN = 1;
	dely_US(10);
	EN = 0;
}

void wait_busy(){
	
}
uchar is_busy(){
	uchar buf;
	RW = 1;
	RS = 0;
	EN = 1;
	dely_US(10);
	buf = IO_PORT;
	
}

void test_draw(){
	

}


#endif