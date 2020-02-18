//*****************************************************************
//本文件实现了显示位图的基本数据结构和加载位图的函数
//                                    ---北京交通大学电气学院 杨罡
//*****************************************************************

#include "bitmap.h"


/* Draws a bitmap into the Framebuffer.
   Bitmaps are converted from Windows BMP-Format to 
   C-Arrays with the fontgen-tool (see files bmp.h/bmp.c) */
void lcd_bitmap( uint8_t left,  uint8_t top, const struct IMG_DEF *img_ptr, uint8_t mode)
 { uint8_t width, heigth, h, w, pattern, mask;
   uint8_t* ptable;

   width  = pgm_read_byte( &(img_ptr->width_in_pixels) );
   heigth = pgm_read_byte( &(img_ptr->height_in_pixels) );
   ptable  = (uint8_t*) pgm_read_word( (uint16_t*)(&(img_ptr->char_table)) ); 
   //ptable  = (uint8_t*) pgm_read_word( &(img_ptr->char_table) ); 

   for ( h=0; h<heigth; h++ ) 
    { mask = 0x80;
      pattern = pgm_read_byte( ptable );
      ptable++;
      for ( w=0; w<width; w++ ) 
       { if ( pattern & mask ) 
            lcd_dot(w+left, h+top, mode);

         mask >>= 1;
         if ( mask == 0 ) 
          { mask = 0x80;
            pattern = pgm_read_byte( ptable );
            ptable++;
          }
       }
    }
 }

///////////////////////////////////////////////////
const unsigned char icon1_char_table[]  = 
 { 
  0  //add your bimap data here
 };

const struct IMG_DEF icon1_bmp  = {1,  1, icon1_char_table};//set the size of you bitmap
//                                 |   |
//                             width  height

//-----------------------------

const unsigned char frame_char_table[]  = 
 { 
 0    //add your bimap data here
 };


const struct IMG_DEF frame_bmp  = { 1,   1, frame_char_table};
//                                  |    |
       //                        width   height
	                         //set the size of you bitmap