#ifndef _HEAD_LANG_
#define _HEAD_LANG_
#include <reg51.h>
typedef unsigned char uchar;
typedef unsigned int uint;


void dely_ms(uint tim){
	uint i;
	while(tim--)
		for(i =0 ; i < 121 ; ++i);
}


#endif