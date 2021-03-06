//*****************************************************************
//定义了显示位图的基本数据结构和加载位图的函数
//                             ---北京交通大学电气学院 杨罡 2008.8
//*****************************************************************
#ifndef BITMAP_H
#define BITMAP_H
#include  "graphic.h"
#include "m32macro.h"
typedef struct IMG_DEF 
 { const unsigned char width_in_pixels;      /* Image width */
   const unsigned char height_in_pixels;     /* Image height*/
   const unsigned char *char_table;          /* Image table start address in memory  */
 } IMG_DEF;


extern const struct IMG_DEF icon1_bmp;
extern const struct IMG_DEF frame_bmp ;

void lcd_bitmap( uint8_t left,  uint8_t top, const struct IMG_DEF *img_ptr, uint8_t mode);

#endif