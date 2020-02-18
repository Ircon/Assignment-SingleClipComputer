#ifndef _HEAD_LANG_
#define _HEAD_LANG_

#include <reg51.h>
// === 类型定义 ===
typedef unsigned char uchar;
typedef unsigned int uint;


// === 功能函数 ===
/*
延时函数
*/
void dely_US(uint cnt){
	while(cnt>0) --cnt;
}

#endif