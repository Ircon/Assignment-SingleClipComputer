#ifndef _HEAD_LANG_
#define _HEAD_LANG_

#include <reg51.h>
// === ���Ͷ��� ===
typedef unsigned char uchar;
typedef unsigned int uint;


// === ���ܺ��� ===
/*
��ʱ����
*/
void dely_US(uint cnt){
	while(cnt>0) --cnt;
}

#endif