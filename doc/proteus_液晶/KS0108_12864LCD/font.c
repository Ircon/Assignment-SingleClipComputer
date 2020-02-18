//*****************************************************************
//本头文件实现了用于字符显示的宏以及显示函数和显示的数据
//                   ---北京交通大学电气学院 杨罡 2008.8
//*****************************************************************

#include "font.h"

void lcd_glyph(uint8_t left, uint8_t top, uint8_t width, uint8_t height,
               uint8_t *glyph_ptr, uint8_t store_width)
 { uint8_t bit_pos;
   uint8_t byte_offset;
   uint8_t y_bits;
   uint8_t remaining_bits;
   uint8_t mask;
   uint8_t char_mask;
   uint8_t x;
   uint8_t *glyph_scan;
   uint8_t glyph_offset;

   bit_pos = top & 0x07;		/* get the bit offset into a byte */
   glyph_offset = 0;			/* start at left side of the glyph rasters */
   char_mask = 0x80;			/* initial character glyph mask */

   for (x = left; x < (left + width); x++)
    { 
	   
	  byte_offset = top >> 3;        	/* get the byte offset into y direction */
      y_bits = height;		/* get length in y direction to write */
      remaining_bits = 8 - bit_pos;	/* number of bits left in byte */
      mask = l_mask_array[bit_pos];	/* get mask for this bit */
      glyph_scan = glyph_ptr + glyph_offset;	 /* point to base of the glyph */
      /* boundary checking here to account for the possibility of  */
      /* write past the bottom of the screen.                        */
	  while((y_bits) && (byte_offset < LCD_Y_BYTES)) /* while there are bits still to write */
       { /* check if the character pixel is set or not */
         //if(*glyph_scan & char_mask)
         if(pgm_read_byte(glyph_scan) & char_mask)
            l_display_array[byte_offset][x] |= mask;	/* set image pixel */
         else
            l_display_array[byte_offset][x] &= ~mask;	/* clear the image pixel */    
         if(l_mask_array[0] & 0x80)
            mask >>= 1;
         else
            mask <<= 1;
			
         y_bits--;
         remaining_bits--;
         if(remaining_bits == 0)
          { /* just crossed over a byte boundry, reset byte counts */
            remaining_bits = 8;
            byte_offset++;
            mask = l_mask_array[0];
          }
		 
          /* bump the glyph scan to next raster */
          glyph_scan += store_width;
		  
       }
     
      /* shift over to next glyph bit */
      char_mask >>= 1;
      if(char_mask == 0)				/* reset for next byte in raster */
       { char_mask = 0x80;
         glyph_offset++;
       }
	 
   }
 }


/*
 Prints the given string at location x,y in the specified font.
 Prints each character given via calls to lcd_glyph. The entry string
 is null terminated. (adapted function from the MJK-code)
 Arguments are:
	left       coordinate of left start of string.
	top        coordinate of top of string.
	font       font number to use for display (see fonts.h)
	str        text string to display (null-terminated)
*/ 
 void lcd_text(uint8_t left, uint8_t top, uint8_t font,   char *str)
 { uint8_t x = left;
   uint8_t glyph;
   uint8_t width;
   uint8_t height, defaultheight;
   uint8_t store_width;
   uint8_t *glyph_ptr;
   uint8_t *width_table_ptr;
   uint8_t *glyph_table_ptr;
   uint8_t glyph_beg, glyph_end;
   uint8_t fixedwidth;
   uint8_t inprogmem=0;//mine
  
   defaultheight = (fonts[font].glyph_height);//pgm_read_byte ( &(fonts[font].glyph_height) );
   store_width =   (fonts[font].store_width);//pgm_read_byte ( &(fonts[font].store_width) );
   width_table_ptr = (uint8_t*)(fonts[font].width_table);//(uint8_t*) pgm_read_word( &(fonts[font].width_table) );
   glyph_table_ptr = (uint8_t*)(fonts[font].glyph_table);//(uint8_t*)pgm_read_word( &(fonts[font].glyph_table) );
   glyph_beg  = (fonts[font].glyph_beg);//pgm_read_byte( &(fonts[font].glyph_beg) );
   glyph_end  = (fonts[font].glyph_end);//pgm_read_byte( &(fonts[font].glyph_end) );
   fixedwidth = (fonts[font].fixed_width);//pgm_read_byte( &(fonts[font].fixed_width) );

   if (inprogmem) 
      glyph = pgm_read_byte(str);
    
   else 
      glyph = (uint8_t)*str;
	
   while(glyph != 0x00) // while(*str != 0x00)
    { /* check to make sure the symbol is a legal one */
      /* if not then just replace it with the default character */
      if((glyph < glyph_beg) || (glyph > glyph_end))
         glyph = (fonts[font].glyph_def);//pgm_read_byte( &(fonts[font].glyph_def) ) ;

      /* make zero based index into the font data arrays */
      glyph -= glyph_beg;
      if(fixedwidth == 0)
         // width=fonts[font].width_table[glyph];	/* get the variable width instead */
         width=pgm_read_byte(width_table_ptr+glyph);
      else 
         width = fixedwidth;
		
      height = defaultheight;
      //glyph_ptr = fonts[font].glyph_table + ((unsigned int)glyph * (unsigned int)store_width * (unsigned int)height);
      glyph_ptr = glyph_table_ptr + ((unsigned int)glyph * (unsigned int)store_width * (unsigned int)height) ;

      /* range check / limit things here */
      if(x > SCRN_RIGHT)
         x = SCRN_RIGHT;
       
      if((x + width) > SCRN_RIGHT+1)
         width = SCRN_RIGHT - x + 1;
       
      if(top > SCRN_BOTTOM)
         top = SCRN_BOTTOM;
       
      if((top + height) > SCRN_BOTTOM+1)
         height = SCRN_BOTTOM - top + 1;
       
      lcd_glyph(x,top,width,height,glyph_ptr,store_width);  /* plug symbol into buffer */

      x += width;		/* move right for next character */
      str++;			/* point to next character in string */
      if (inprogmem) 
         glyph = pgm_read_byte(str);
      else 
         glyph = (uint8_t)*str;
       
    }
 }
void lcd_text_p(uint8_t left, uint8_t top, uint8_t font,  const char *str)
 { uint8_t x = left;
   uint8_t glyph;
   uint8_t width;
   uint8_t height, defaultheight;
   uint8_t store_width;
   uint8_t *glyph_ptr;
   uint8_t *width_table_ptr;
   uint8_t *glyph_table_ptr;
   uint8_t glyph_beg, glyph_end;
   uint8_t fixedwidth;
   uint8_t inprogmem=1;//mine
   
   defaultheight = (fonts[font].glyph_height);//pgm_read_byte ( &(fonts[font].glyph_height) );
   store_width =   (fonts[font].store_width);//pgm_read_byte ( &(fonts[font].store_width) );
   width_table_ptr = (uint8_t*)(fonts[font].width_table);//(uint8_t*) pgm_read_word( &(fonts[font].width_table) );
   glyph_table_ptr = (uint8_t*)(fonts[font].glyph_table);//(uint8_t*)pgm_read_word( &(fonts[font].glyph_table) );
   glyph_beg  = (fonts[font].glyph_beg);//pgm_read_byte( &(fonts[font].glyph_beg) );
   glyph_end  = (fonts[font].glyph_end);//pgm_read_byte( &(fonts[font].glyph_end) );
   fixedwidth = (fonts[font].fixed_width);//pgm_read_byte( &(fonts[font].fixed_width) );

   if (inprogmem) 
      glyph = pgm_read_byte(str);
    
   else 
      glyph = (uint8_t)*str;
	
   while(glyph != 0x00) // while(*str != 0x00)
    { /* check to make sure the symbol is a legal one */
      /* if not then just replace it with the default character */
      if((glyph < glyph_beg) || (glyph > glyph_end))
         glyph = (fonts[font].glyph_def);//pgm_read_byte( &(fonts[font].glyph_def) ) ;

      /* make zero based index into the font data arrays */
      glyph -= glyph_beg;
      if(fixedwidth == 0)
         // width=fonts[font].width_table[glyph];	/* get the variable width instead */
         width=pgm_read_byte(width_table_ptr+glyph);
      else 
         width = fixedwidth;
		
      height = defaultheight;
      //glyph_ptr = fonts[font].glyph_table + ((unsigned int)glyph * (unsigned int)store_width * (unsigned int)height);
      glyph_ptr = glyph_table_ptr + ((unsigned int)glyph * (unsigned int)store_width * (unsigned int)height) ;

      /* range check / limit things here */
      if(x > SCRN_RIGHT)
         x = SCRN_RIGHT;
       
      if((x + width) > SCRN_RIGHT+1)
         width = SCRN_RIGHT - x + 1;
       
      if(top > SCRN_BOTTOM)
         top = SCRN_BOTTOM;
       
      if((top + height) > SCRN_BOTTOM+1)
         height = SCRN_BOTTOM - top + 1;
       
      lcd_glyph(x,top,width,height,glyph_ptr,store_width);  /* plug symbol into buffer */

      x += width;		/* move right for next character */
      str++;			/* point to next character in string */
      if (inprogmem) 
         glyph = pgm_read_byte(str);
      else 
         glyph = (uint8_t)*str;
       
    }
 }
 
 
const struct FONT_DEF fonts[FONT_COUNT]  = 
 { //
#ifdef EN_FIVE_DOT
   {1,  7, five_dot_glyph_table, 0, five_dot_width_table,' ','~','.'},
#endif

#ifdef EN_SIX_DOT
   {2,  8, six_dot_glyph_table, 0, six_dot_width_table,' ','~','.'},
#endif

#ifdef EN_SEVEN_DOT
   //{2,  8, seven_dot_glyph_table, 0, seven_dot_width_table,' ','~','.'},
	{2,  8, seven_dot_glyph_table, 0, seven_dot_width_table,' ',DEG_CHAR,'.'},
#endif

#ifdef EN_NINE_DOT
   {1, 12, nine_dot_glyph_table, 8, NULL,' ','~','.'},
#endif

#ifdef EN_TEN_DOT
   {2, 12, ten_dot_glyph_table, 9, NULL,' ','~','.'},
#endif

#ifdef EN_FIFTEEN_DOT
   {3, 18, fifteen_dot_glyph_table, 0, fifteen_dot_width_table,' ','~','.'},
#endif

#ifdef EN_EIGHTEEN_DOT
   // {3, 18, eighteen_dot_glyph_table, 0, eighteen_dot_width_table,' ','9','.'},
	{3, 18, eighteen_dot_glyph_table, 0, eighteen_dot_width_table,' ',':','.'},
#endif
 };
 
 //*****************************************************************************
 #ifdef EN_FIVE_DOT
const unsigned char five_dot_glyph_table[]  = 
 { /* ' ' charwidth: 2 */
   0x00,    /*  [  ]  */
   0x00,    /*  [  ]  */
   0x00,    /*  [  ]  */
   0x00,    /*  [  ]  */
   0x00,    /*  [  ]  */
   0x00,    /*  [  ]  */
   0x00,    /*  [  ]  */
	
   /* '!' charwidth: 2 */
   0x00,    /*  [  ]  */
   0x80,    /*  [* ]  */
   0x80,    /*  [* ]  */
   0x80,    /*  [* ]  */
   0x00,    /*  [  ]  */
   0x80,    /*  [* ]  */
   0x00,    /*  [  ]  */
	
   /* '"' charwidth: 4 */
   0x00,    /*  [    ]  */
   0xA0,    /*  [* * ]  */
   0xA0,    /*  [* * ]  */
   0x00,    /*  [    ]  */
   0x00,    /*  [    ]  */
   0x00,    /*  [    ]  */
   0x00,    /*  [    ]  */
	
   /* '#' charwidth: 8 */
   0x00,    /*  [        ]  */
   0x14,    /*  [   * *  ]  */
   0x7E,    /*  [ ****** ]  */
   0x28,    /*  [  * *   ]  */
   0xFC,    /*  [******  ]  */
   0x50,    /*  [ * *    ]  */
   0x00,    /*  [        ]  */
	
   /* '$' charwidth: 4 */
   0x40,    /*  [ *  ]  */
   0x60,    /*  [ ** ]  */
   0x80,    /*  [*   ]  */
   0x40,    /*  [ *  ]  */
   0x20,    /*  [  * ]  */
   0xC0,    /*  [**  ]  */
   0x40,    /*  [ *  ]  */
	
   /* '%' charwidth: 8 */
   0x00,    /*  [        ]  */
   0x64,    /*  [ **  *  ]  */
   0xA8,    /*  [* * *   ]  */
   0xD6,    /*  [** * ** ]  */
   0x2A,    /*  [  * * * ]  */
   0x4C,    /*  [ *  **  ]  */
   0x00,    /*  [        ]  */
	
   /* '&' charwidth: 6 */
   0x00,    /*  [      ]  */
   0x60,    /*  [ **   ]  */
   0x90,    /*  [*  *  ]  */
   0x40,    /*  [ *    ]  */
   0x98,    /*  [*  ** ]  */
   0x60,    /*  [ **   ]  */
   0x00,    /*  [      ]  */
	
   /* ''' charwidth: 2 */
   0x00,    /*  [  ]  */
   0x80,    /*  [* ]  */
   0x80,    /*  [* ]  */
   0x00,    /*  [  ]  */
   0x00,    /*  [  ]  */
   0x00,    /*  [  ]  */
   0x00,    /*  [  ]  */
	
   /* '(' charwidth: 3 */
   0x40,    /*  [ * ]  */
   0x80,    /*  [*  ]  */
   0x80,    /*  [*  ]  */
   0x80,    /*  [*  ]  */
   0x80,    /*  [*  ]  */
   0x80,    /*  [*  ]  */
   0x40,    /*  [ * ]  */
	
   /* ')' charwidth: 3 */
   0x80,    /*  [*  ]  */
   0x40,    /*  [ * ]  */
   0x40,    /*  [ * ]  */
   0x40,    /*  [ * ]  */
   0x40,    /*  [ * ]  */
   0x40,    /*  [ * ]  */
   0x80,    /*  [*  ]  */
	
   /* '*' charwidth: 6 */
   0x00,    /*  [      ]  */
   0x50,    /*  [ * *  ]  */
   0x20,    /*  [  *   ]  */
   0xF8,    /*  [***** ]  */
   0x20,    /*  [  *   ]  */
   0x50,    /*  [ * *  ]  */
   0x00,    /*  [      ]  */
	
   /* '+' charwidth: 6 */
   0x00,    /*  [      ]  */
   0x20,    /*  [  *   ]  */
   0x20,    /*  [  *   ]  */
   0xF8,    /*  [***** ]  */
   0x20,    /*  [  *   ]  */
   0x20,    /*  [  *   ]  */
   0x00,    /*  [      ]  */
	
   /* ',' charwidth: 3 */
   0x00,    /*  [   ]  */
   0x00,    /*  [   ]  */
   0x00,    /*  [   ]  */
   0x00,    /*  [   ]  */
   0x40,    /*  [ * ]  */
   0x40,    /*  [ * ]  */
   0x80,    /*  [*  ]  */
	
   /* '-' charwidth: 4 */
   0x00,    /*  [    ]  */
   0x00,    /*  [    ]  */
   0x00,    /*  [    ]  */
   0xE0,    /*  [*** ]  */
   0x00,    /*  [    ]  */
   0x00,    /*  [    ]  */
   0x00,    /*  [    ]  */
		
   /* '.' charwidth: 2 */
   0x00, 	/*  [  ]  */
   0x00, 	/*  [  ]  */
   0x00, 	/*  [  ]  */
   0x00, 	/*  [  ]  */
   0x00, 	/*  [  ]  */
   0x80, 	/*  [* ]  */
   0x00, 	/*  [  ]  */
   
   /* '/' charwidth: 4 */
   0x20, 	/*  [  * ]  */
   0x20, 	/*  [  * ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0x80, 	/*  [*   ]  */
   0x80, 	/*  [*   ]  */
   0x00, 	/*  [    ]  */
   
   /* '0' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x60, 	/*  [ **  ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x60, 	/*  [ **  ]  */
   0x00, 	/*  [     ]  */
   
   /* '1' charwidth: 3 */
   0x00, 	/*  [   ]  */
   0x40, 	/*  [ * ]  */
   0xC0, 	/*  [** ]  */
   0x40, 	/*  [ * ]  */
   0x40, 	/*  [ * ]  */
   0x40, 	/*  [ * ]  */
   0x00, 	/*  [   ]  */
   
   /* '2' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x60, 	/*  [ **  ]  */
   0x90, 	/*  [*  * ]  */
   0x20, 	/*  [  *  ]  */
   0x40, 	/*  [ *   ]  */
   0xF0, 	/*  [**** ]  */
   0x00, 	/*  [     ]  */
   
   /* '3' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x60, 	/*  [ **  ]  */
   0x90, 	/*  [*  * ]  */
   0x20, 	/*  [  *  ]  */
   0x90, 	/*  [*  * ]  */
   0x60, 	/*  [ **  ]  */
   0x00,    /*  [     ]  */
   
   /* '4' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x20, 	/*  [  *  ]  */
   0x60, 	/*  [ **  ]  */
   0xA0, 	/*  [* *  ]  */
   0xF0, 	/*  [**** ]  */
   0x20, 	/*  [  *  ]  */
   0x00, 	/*  [     ]  */
   
   /* '5' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0xE0, 	/*  [*** ]  */
   0x80, 	/*  [*   ]  */
   0xE0, 	/*  [*** ]  */
   0x20, 	/*  [  * ]  */
   0xC0, 	/*  [**  ]  */
   0x00, 	/*  [    ]  */
   
   /* '6' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x20, 	/*  [  *  ]  */
   0x40, 	/*  [ *   ]  */
   0xE0, 	/*  [***  ]  */
   0x90, 	/*  [*  * ]  */
   0x60, 	/*  [ **  ]  */
   0x00, 	/*  [     ]  */
   
   /* '7' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0xE0, 	/*  [*** ]  */
   0x20, 	/*  [  * ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0x00, 	/*  [    ]  */
   
   /* '8' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x60, 	/*  [ **  ]  */
   0x90, 	/*  [*  * ]  */
   0x60, 	/*  [ **  ]  */
   0x90, 	/*  [*  * ]  */
   0x60, 	/*  [ **  ]  */
   0x00, 	/*  [     ]  */
   
   /* '9' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x60, 	/*  [ **  ]  */
   0x90, 	/*  [*  * ]  */
   0x70, 	/*  [ *** ]  */
   0x20, 	/*  [  *  ]  */
   0x40, 	/*  [ *   ]  */
   0x00, 	/*  [     ]  */
   
   /* ':' charwidth: 2 */
   0x00, 	/*  [  ]  */
   0x00, 	/*  [  ]  */
   0x80, 	/*  [* ]  */
   0x00, 	/*  [  ]  */
   0x00, 	/*  [  ]  */
   0x80, 	/*  [* ]  */
   0x00, 	/*  [  ]  */
   
   /* ';' charwidth: 3 */
   0x00, 	/*  [   ]  */
   0x00, 	/*  [   ]  */
   0x40, 	/*  [ * ]  */
   0x00, 	/*  [   ]  */
   0x40, 	/*  [ * ]  */
   0x40, 	/*  [ * ]  */
   0x80, 	/*  [*  ]  */
   
   /* '<' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x20, 	/*  [  * ]  */
   0x40, 	/*  [ *  ]  */
   0x80, 	/*  [*   ]  */
   0x40, 	/*  [ *  ]  */
   0x20, 	/*  [  * ]  */
   0x00, 	/*  [    ]  */
   
   /* '=' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x00, 	/*  [     ]  */
   0xF0, 	/*  [**** ]  */
   0x00, 	/*  [     ]  */
   0xF0, 	/*  [**** ]  */
   0x00, 	/*  [     ]  */
   0x00, 	/*  [     ]  */
   
   /* '>' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x80, 	/*  [*   ]  */
   0x40, 	/*  [ *  ]  */
   0x20, 	/*  [  * ]  */
   0x40, 	/*  [ *  ]  */
   0x80, 	/*  [*   ]  */
   0x00, 	/*  [    ]  */
   
   /* '?' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0xC0, 	/*  [**  ]  */
   0x20, 	/*  [  * ]  */
   0x40, 	/*  [ *  ]  */
   0x00, 	/*  [    ]  */
   0x40, 	/*  [ *  ]  */
   0x00, 	/*  [    ]  */
   
   /* '@' charwidth: 8 */
   0x38, 	/*  [  ***   ]  */
   0x44, 	/*  [ *   *  ]  */
   0x9A, 	/*  [*  ** * ]  */
   0xAA, 	/*  [* * * * ]  */
   0xB4, 	/*  [* ** *  ]  */
   0x40, 	/*  [ *      ]  */
   0x30, 	/*  [  **    ]  */
   
   /* 'A' charwidth: 6 */
   0x00, 	/*  [      ]  */
   0x20, 	/*  [  *   ]  */
   0x20, 	/*  [  *   ]  */
   0x50, 	/*  [ * *  ]  */
   0x70, 	/*  [ ***  ]  */
   0x88, 	/*  [*   * ]  */
   0x00, 	/*  [      ]  */
   
   /* 'B' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0xE0, 	/*  [***  ]  */
   0x90, 	/*  [*  * ]  */
   0xE0, 	/*  [***  ]  */
   0x90, 	/*  [*  * ]  */
   0xE0, 	/*  [***  ]  */
   0x00, 	/*  [     ]  */
   
   /* 'C' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x60, 	/*  [ **  ]  */
   0x90, 	/*  [*  * ]  */
   0x80, 	/*  [*    ]  */
   0x90, 	/*  [*  * ]  */
   0x60, 	/*  [ **  ]  */
   0x00, 	/*  [     ]  */
   
   /* 'D' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0xE0, 	/*  [***  ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0xE0, 	/*  [***  ]  */
   0x00, 	/*  [     ]  */
   
   /* 'E' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0xE0, 	/*  [*** ]  */
   0x80, 	/*  [*   ]  */
   0xE0, 	/*  [*** ]  */
   0x80, 	/*  [*   ]  */
   0xE0, 	/*  [*** ]  */
   0x00, 	/*  [    ]  */
   
   /* 'F' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0xE0, 	/*  [*** ]  */
   0x80, 	/*  [*   ]  */
   0xE0, 	/*  [*** ]  */
   0x80, 	/*  [*   ]  */
   0x80, 	/*  [*   ]  */
   0x00, 	/*  [    ]  */
   
   /* 'G' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x70, 	/*  [ *** ]  */
   0x80, 	/*  [*    ]  */
   0xB0, 	/*  [* ** ]  */
   0x90, 	/*  [*  * ]  */
   0x70, 	/*  [ *** ]  */
   0x00, 	/*  [     ]  */
   
   /* 'H' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0xF0, 	/*  [**** ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x00, 	/*  [     ]  */
   
   /* 'I' charwidth: 2 */
   0x00, 	/*  [  ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x00, 	/*  [  ]  */
   
   /* 'J' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x10, 	/*  [   * ]  */
   0x10, 	/*  [   * ]  */
   0x10, 	/*  [   * ]  */
   0x90, 	/*  [*  * ]  */
   0x60, 	/*  [ **  ]  */
   0x00, 	/*  [     ]  */
   
   /* 'K' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x90, 	/*  [*  * ]  */
   0xA0, 	/*  [* *  ]  */
   0xC0, 	/*  [**   ]  */
   0xA0, 	/*  [* *  ]  */
   0x90, 	/*  [*  * ]  */
   0x00, 	/*  [     ]  */
   
   /* 'L' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x80, 	/*  [*   ]  */
   0x80, 	/*  [*   ]  */
   0x80, 	/*  [*   ]  */
   0x80, 	/*  [*   ]  */
   0xE0, 	/*  [*** ]  */
   0x00, 	/*  [    ]  */
   
   /* 'M' charwidth: 6 */
   0x00, 	/*  [      ]  */
   0x88, 	/*  [*   * ]  */
   0xD8, 	/*  [** ** ]  */
   0xA8, 	/*  [* * * ]  */
   0x88, 	/*  [*   * ]  */
   0x88, 	/*  [*   * ]  */
   0x00, 	/*  [      ]  */
   
   /* 'N' charwidth: 6 */
   0x00, 	/*  [      ]  */
   0x88, 	/*  [*   * ]  */
   0xC8, 	/*  [**  * ]  */
   0xA8, 	/*  [* * * ]  */
   0x98, 	/*  [*  ** ]  */
   0x88, 	/*  [*   * ]  */
   0x00, 	/*  [      ]  */
   
   /* 'O' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x60, 	/*  [ **  ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x60, 	/*  [ **  ]  */
   0x00, 	/*  [     ]  */
   
   /* 'P' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0xE0, 	/*  [***  ]  */
   0x90, 	/*  [*  * ]  */
   0xE0, 	/*  [***  ]  */
   0x80, 	/*  [*    ]  */
   0x80, 	/*  [*    ]  */
   0x00, 	/*  [     ]  */
   
   /* 'Q' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x60, 	/*  [ **  ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x60, 	/*  [ **  ]  */
   0x10, 	/*  [   * ]  */
   
   /* 'R' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0xE0, 	/*  [***  ]  */
   0x90, 	/*  [*  * ]  */
   0xE0, 	/*  [***  ]  */
   0xA0, 	/*  [* *  ]  */
   0x90, 	/*  [*  * ]  */
   0x00, 	/*  [     ]  */
   
   /* 'S' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x60, 	/*  [ ** ]  */
   0x80, 	/*  [*   ]  */
   0x40, 	/*  [ *  ]  */
   0x20, 	/*  [  * ]  */
   0xC0, 	/*  [**  ]  */
   0x00, 	/*  [    ]  */
   
   /* 'T' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0xE0, 	/*  [*** ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0x00, 	/*  [    ]  */
   
   /* 'U' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x60, 	/*  [ **  ]  */
   0x00, 	/*  [     ]  */
   
   /* 'V' charwidth: 6 */
   0x00, 	/*  [      ]  */
   0x88, 	/*  [*   * ]  */
   0x50, 	/*  [ * *  ]  */
   0x50, 	/*  [ * *  ]  */
   0x20, 	/*  [  *   ]  */
   0x20, 	/*  [  *   ]  */
   0x00, 	/*  [      ]  */
   
   /* 'W' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x82, 	/*  [*     * ]  */
   0x54, 	/*  [ * * *  ]  */
   0x54, 	/*  [ * * *  ]  */
   0x28, 	/*  [  * *   ]  */
   0x28, 	/*  [  * *   ]  */
   0x00, 	/*  [        ]  */
   
   /* 'X' charwidth: 6 */
   0x00, 	/*  [      ]  */
   0x88, 	/*  [*   * ]  */
   0x50, 	/*  [ * *  ]  */
   0x20, 	/*  [  *   ]  */
   0x50, 	/*  [ * *  ]  */
   0x88, 	/*  [*   * ]  */
   0x00, 	/*  [      ]  */
   
   /* 'Y' charwidth: 6 */
   0x00, 	/*  [      ]  */
   0x88, 	/*  [*   * ]  */
   0x50, 	/*  [ * *  ]  */
   0x20, 	/*  [  *   ]  */
   0x20, 	/*  [  *   ]  */
   0x20, 	/*  [  *   ]  */
   0x00, 	/*  [      ]  */
   
   /* 'Z' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0xE0, 	/*  [*** ]  */
   0x20, 	/*  [  * ]  */
   0x40, 	/*  [ *  ]  */
   0x80, 	/*  [*   ]  */
   0xE0, 	/*  [*** ]  */
   0x00, 	/*  [    ]  */
   
   /* '[' charwidth: 3 */
   0xC0, 	/*  [** ]  */
   0x80, 	/*  [*  ]  */
   0x80, 	/*  [*  ]  */
   0x80, 	/*  [*  ]  */
   0x80, 	/*  [*  ]  */
   0x80, 	/*  [*  ]  */
   0xC0, 	/*  [** ]  */
   
   /* '\' charwidth: 4 */
   0x80, 	/*  [*   ]  */
   0x80, 	/*  [*   ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0x20, 	/*  [  * ]  */
   0x20, 	/*  [  * ]  */
   0x00, 	/*  [    ]  */
   
   /* ']' charwidth: 3 */
   0xC0, 	/*  [** ]  */
   0x40, 	/*  [ * ]  */
   0x40, 	/*  [ * ]  */
   0x40, 	/*  [ * ]  */
   0x40, 	/*  [ * ]  */
   0x40, 	/*  [ * ]  */
   0xC0, 	/*  [** ]  */
   
   /* '^' charwidth: 6 */
   0x00, 	/*  [      ]  */
   0x20, 	/*  [  *   ]  */
   0x50, 	/*  [ * *  ]  */
   0x88, 	/*  [*   * ]  */
   0x00, 	/*  [      ]  */
   0x00, 	/*  [      ]  */
   0x00, 	/*  [      ]  */
   
   /* '_' charwidth: 6 */
   0x00, 	/*  [      ]  */
   0x00, 	/*  [      ]  */
   0x00, 	/*  [      ]  */
   0x00, 	/*  [      ]  */
   0x00, 	/*  [      ]  */
   0x00, 	/*  [      ]  */
   0xF8, 	/*  [***** ]  */
   
   /* '`' charwidth: 3 */
   0x00, 	/*  [   ]  */
   0x80, 	/*  [*  ]  */
   0x40, 	/*  [ * ]  */
   0x00, 	/*  [   ]  */
   0x00, 	/*  [   ]  */
   0x00, 	/*  [   ]  */
   0x00, 	/*  [   ]  */
   
   /* 'a' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x00, 	/*  [     ]  */
   0x70, 	/*  [ *** ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x70, 	/*  [ *** ]  */
   0x00, 	/*  [     ]  */
   
   /* 'b' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x80, 	/*  [*    ]  */
   0xE0, 	/*  [***  ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0xE0, 	/*  [***  ]  */
   0x00, 	/*  [     ]  */
   
   /* 'c' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x00, 	/*  [    ]  */
   0x60, 	/*  [ ** ]  */
   0x80, 	/*  [*   ]  */
   0x80, 	/*  [*   ]  */
   0x60, 	/*  [ ** ]  */
   0x00, 	/*  [    ]  */
   
   /* 'd' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x10, 	/*  [   * ]  */
   0x70, 	/*  [ *** ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x70, 	/*  [ *** ]  */
   0x00, 	/*  [     ]  */
   
   /* 'e' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x00, 	/*  [    ]  */
   0x60, 	/*  [ ** ]  */
   0xA0, 	/*  [* * ]  */
   0xC0, 	/*  [**  ]  */
   0x60, 	/*  [ ** ]  */
   0x00, 	/*  [    ]  */
   
   /* 'f' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x60, 	/*  [ ** ]  */
   0x80, 	/*  [*   ]  */
   0xC0, 	/*  [**  ]  */
   0x80, 	/*  [*   ]  */
   0x80, 	/*  [*   ]  */
   0x00, 	/*  [    ]  */
   
   /* 'g' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x70, 	/*  [ *** ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x70, 	/*  [ *** ]  */
   0x10, 	/*  [   * ]  */
   0x60, 	/*  [ **  ]  */
   
   /* 'h' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x80, 	/*  [*    ]  */
   0xE0, 	/*  [***  ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x00, 	/*  [     ]  */
   
   /* 'i' charwidth: 2 */
   0x80, 	/*  [* ]  */
   0x00, 	/*  [  ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x00, 	/*  [  ]  */
   
   /* 'j' charwidth: 3 */
   0x40, 	/*  [ * ]  */
   0x00, 	/*  [   ]  */
   0x40, 	/*  [ * ]  */
   0x40, 	/*  [ * ]  */
   0x40, 	/*  [ * ]  */
   0x40, 	/*  [ * ]  */
   0x80, 	/*  [*  ]  */
   
   /* 'k' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x80, 	/*  [*    ]  */
   0x90, 	/*  [*  * ]  */
   0xE0, 	/*  [***  ]  */
   0xA0, 	/*  [* *  ]  */
   0x90, 	/*  [*  * ]  */
   0x00, 	/*  [     ]  */
   
   /* 'l' charwidth: 2 */
   0x00, 	/*  [  ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x00, 	/*  [  ]  */
   
   /* 'm' charwidth: 6 */
   0x00, 	/*  [      ]  */
   0x00, 	/*  [      ]  */
   0xF0, 	/*  [****  ]  */
   0xA8, 	/*  [* * * ]  */
   0xA8, 	/*  [* * * ]  */
   0xA8, 	/*  [* * * ]  */
   0x00, 	/*  [      ]  */
   
   /* 'n' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x00, 	/*  [     ]  */
   0xE0, 	/*  [***  ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x00, 	/*  [     ]  */
   
   /* 'o' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x00, 	/*  [     ]  */
   0x60, 	/*  [ **  ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x60, 	/*  [ **  ]  */
   0x00, 	/*  [     ]  */
   
   /* 'p' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0xE0, 	/*  [***  ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0xE0, 	/*  [***  ]  */
   0x80, 	/*  [*    ]  */
   0x80, 	/*  [*    ]  */
   
   /* 'q' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x70, 	/*  [ *** ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x70, 	/*  [ *** ]  */
   0x10, 	/*  [   * ]  */
   0x10, 	/*  [   * ]  */
   
   /* 'r' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x00, 	/*  [    ]  */
   0xA0, 	/*  [* * ]  */
   0xC0, 	/*  [**  ]  */
   0x80, 	/*  [*   ]  */
   0x80, 	/*  [*   ]  */
   0x00, 	/*  [    ]  */
   
   /* 's' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x00, 	/*  [    ]  */
   0x60, 	/*  [ ** ]  */
   0xC0, 	/*  [**  ]  */
   0x20, 	/*  [  * ]  */
   0xC0, 	/*  [**  ]  */
   0x00, 	/*  [    ]  */
   
   /* 't' charwidth: 3 */
   0x00, 	/*  [   ]  */
   0x80, 	/*  [*  ]  */
   0xC0, 	/*  [** ]  */
   0x80, 	/*  [*  ]  */
   0x80, 	/*  [*  ]  */
   0x40, 	/*  [ * ]  */
   0x00, 	/*  [   ]  */
   
   /* 'u' charwidth: 5 */
   0x00, 	/*  [     ]  */
   0x00, 	/*  [     ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x90, 	/*  [*  * ]  */
   0x70, 	/*  [ *** ]  */
   0x00, 	/*  [     ]  */
   
   /* 'v' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x00, 	/*  [    ]  */
   0xA0, 	/*  [* * ]  */
   0xA0, 	/*  [* * ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0x00, 	/*  [    ]  */
   
   /* 'w' charwidth: 6 */
   0x00, 	/*  [      ]  */
   0x00, 	/*  [      ]  */
   0xA8, 	/*  [* * * ]  */
   0xA8, 	/*  [* * * ]  */
   0x50, 	/*  [ * *  ]  */
   0x50, 	/*  [ * *  ]  */
   0x00, 	/*  [      ]  */
   
   /* 'x' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x00, 	/*  [    ]  */
   0xA0, 	/*  [* * ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0xA0, 	/*  [* * ]  */
   0x00, 	/*  [    ]  */
   
   /* 'y' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x00, 	/*  [    ]  */
   0xA0, 	/*  [* * ]  */
   0xA0, 	/*  [* * ]  */
   0x40, 	/*  [ *  ]  */
   0x80, 	/*  [*   ]  */
   0x80, 	/*  [*   ]  */
   
   /* 'z' charwidth: 4 */
   0x00, 	/*  [    ]  */
   0x00, 	/*  [    ]  */
   0xE0, 	/*  [*** ]  */
   0x40, 	/*  [ *  ]  */
   0x80, 	/*  [*   ]  */
   0xE0, 	/*  [*** ]  */
   0x00, 	/*  [    ]  */
   
   /* '{' charwidth: 4 */
   0x20, 	/*  [  * ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0x80, 	/*  [*   ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0x20, 	/*  [  * ]  */
   
   /* '|' charwidth: 2 */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x80, 	/*  [* ]  */
   0x00, 	/*  [  ]  */
   
   /* '}' charwidth: 4 */
   0x80, 	/*  [*   ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0x20, 	/*  [  * ]  */
   0x40, 	/*  [ *  ]  */
   0x40, 	/*  [ *  ]  */
   0x80, 	/*  [*   ]  */
   
   /* '~' charwidth: 7 */
   0x00, 	/*  [       ]  */
   0x64, 	/*  [ **  * ]  */
   0x98, 	/*  [*  **  ]  */
   0x00, 	/*  [       ]  */
   0x00, 	/*  [       ]  */
   0x00, 	/*  [       ]  */
   0x00, 	/*  [       ]  */
 };
   
const unsigned char five_dot_width_table[]  = 
 {  2, /* ' ' */
    2, /* '!' */
    4, /* '"' */
    8, /* '#' */
    4, /* '$' */
    8, /* '%' */
    6, /* '&' */
    2, /* ''' */
    3, /* '(' */
    3, /* ')' */
    6, /* '*' */
    6, /* '+' */
    3, /* ',' */
    4, /* '-' */
    2, /* '.' */
    4, /* '/' */
    5, /* '0' */
    3, /* '1' */
    5, /* '2' */
    5, /* '3' */
    5, /* '4' */
    4, /* '5' */
    5, /* '6' */
    4, /* '7' */
    5, /* '8' */
    5, /* '9' */
    2, /* ':' */
    3, /* ';' */
    4, /* '<' */
    5, /* '=' */
    4, /* '>' */
    4, /* '?' */
    8, /* '@' */
    6, /* 'A' */
    5, /* 'B' */
    5, /* 'C' */
    5, /* 'D' */
    4, /* 'E' */
    4, /* 'F' */
    5, /* 'G' */
    5, /* 'H' */
    2, /* 'I' */
    5, /* 'J' */
    5, /* 'K' */
    4, /* 'L' */
    6, /* 'M' */
    6, /* 'N' */
    5, /* 'O' */
    5, /* 'P' */
    5, /* 'Q' */
    5, /* 'R' */
    4, /* 'S' */
    4, /* 'T' */
    5, /* 'U' */
    6, /* 'V' */
    8, /* 'W' */
    6, /* 'X' */
    6, /* 'Y' */
    4, /* 'Z' */
    3, /* '[' */
    4, /* '\' */
    3, /* ']' */
    6, /* '^' */
    6, /* '_' */
    3, /* '`' */
    5, /* 'a' */
    5, /* 'b' */
    4, /* 'c' */
    5, /* 'd' */
    4, /* 'e' */
    4, /* 'f' */
    5, /* 'g' */
    5, /* 'h' */
    2, /* 'i' */
    3, /* 'j' */
    5, /* 'k' */
    2, /* 'l' */
    6, /* 'm' */
    5, /* 'n' */
    5, /* 'o' */
    5, /* 'p' */
    5, /* 'q' */
    4, /* 'r' */
    4, /* 's' */
    3, /* 't' */
    5, /* 'u' */
    4, /* 'v' */
    6, /* 'w' */
    4, /* 'x' */
    4, /* 'y' */
    4, /* 'z' */
    4, /* '{' */
    2, /* '|' */
    4, /* '}' */
    7, /* '~' */
 };
#endif
   
#ifdef EN_SIX_DOT
const unsigned char six_dot_glyph_table[]  = 
 { /* ' ' charwidth: 2 */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   
   /* '!' charwidth: 2 */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   
   /* '"' charwidth: 4 */
   0xA0, 0x00, 	/*  [* * ]  */
   0xA0, 0x00, 	/*  [* * ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* '#' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x14, 0x00, 	/*  [   * *  ]  */
   0x7E, 0x00, 	/*  [ ****** ]  */
   0x28, 0x00, 	/*  [  * *   ]  */
   0xFC, 0x00, 	/*  [******  ]  */
   0x50, 0x00, 	/*  [ * *    ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* '$' charwidth: 5 */
   0x20, 0x00, 	/*  [  *  ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '%' charwidth: 7 */
   0x64, 0x00, 	/*  [ **  * ]  */
   0xA8, 0x00, 	/*  [* * *  ]  */
   0xD0, 0x00, 	/*  [** *   ]  */
   0x2C, 0x00, 	/*  [  * ** ]  */
   0x54, 0x00, 	/*  [ * * * ]  */
   0x98, 0x00, 	/*  [*  **  ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '&' charwidth: 6 */
   0x60, 0x00, 	/*  [ **   ]  */
   0x90, 0x00, 	/*  [*  *  ]  */
   0x40, 0x00, 	/*  [ *    ]  */
   0x98, 0x00, 	/*  [*  ** ]  */
   0x90, 0x00, 	/*  [*  *  ]  */
   0x60, 0x00, 	/*  [ **   ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* ''' charwidth: 2 */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   
   /* '(' charwidth: 4 */
   0x20, 0x00, 	/*  [  * ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x20, 0x00, 	/*  [  * ]  */
   
   /* ')' charwidth: 4 */
   0x80, 0x00, 	/*  [*   ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x80, 0x00, 	/*  [*   ]  */
   
   /* '*' charwidth: 6 */
   0x50, 0x00, 	/*  [ * *  ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0xF8, 0x00, 	/*  [***** ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x50, 0x00, 	/*  [ * *  ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* '+' charwidth: 6 */
   0x00, 0x00, 	/*  [      ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0xF8, 0x00, 	/*  [***** ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* ',' charwidth: 3 */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x80, 0x00, 	/*  [*  ]  */
   
   /* '-' charwidth: 6 */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   0xF8, 0x00, 	/*  [***** ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* '.' charwidth: 2 */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   
   /* '/' charwidth: 4 */
   0x20, 0x00, 	/*  [  * ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* '0' charwidth: 5 */
   0x60, 0x00, 	/*  [ **  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '1' charwidth: 3 */
   0x40, 0x00, 	/*  [ * ]  */
   0xC0, 0x00, 	/*  [** ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   
   /* '2' charwidth: 5 */
   0x60, 0x00, 	/*  [ **  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0xF0, 0x00, 	/*  [**** ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '3' charwidth: 5 */
   0x60, 0x00, 	/*  [ **  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '4' charwidth: 5 */
   0x20, 0x00, 	/*  [  *  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0xA0, 0x00, 	/*  [* *  ]  */
   0xF0, 0x00, 	/*  [**** ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '5' charwidth: 5 */
   0xF0, 0x00, 	/*  [**** ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '6' charwidth: 5 */
   0x20, 0x00, 	/*  [  *  ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '7' charwidth: 5 */
   0xF0, 0x00, 	/*  [**** ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '8' charwidth: 5 */
   0x60, 0x00, 	/*  [ **  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '9' charwidth: 5 */
   0x60, 0x00, 	/*  [ **  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* ':' charwidth: 2 */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   
   /* ';' charwidth: 3 */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x80, 0x00, 	/*  [*  ]  */
   
   /* '<' charwidth: 4 */
   0x00, 0x00, 	/*  [    ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* '=' charwidth: 5 */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0xF0, 0x00, 	/*  [**** ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0xF0, 0x00, 	/*  [**** ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '>' charwidth: 4 */
   0x00, 0x00, 	/*  [    ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* '?' charwidth: 4 */
   0xC0, 0x00, 	/*  [**  ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* '@' charwidth: 8 */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x42, 0x00, 	/*  [ *    * ]  */
   0x9A, 0x00, 	/*  [*  ** * ]  */
   0xAA, 0x00, 	/*  [* * * * ]  */
   0xB4, 0x00, 	/*  [* ** *  ]  */
   0x40, 0x00, 	/*  [ *      ]  */
   0x38, 0x00, 	/*  [  ***   ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'A' charwidth: 6 */
   0x20, 0x00, 	/*  [  *   ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x50, 0x00, 	/*  [ * *  ]  */
   0x70, 0x00, 	/*  [ ***  ]  */
   0x88, 0x00, 	/*  [*   * ]  */
   0x88, 0x00, 	/*  [*   * ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* 'B' charwidth: 5 */
   0xE0, 0x00, 	/*  [***  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'C' charwidth: 5 */
   0x60, 0x00, 	/*  [ **  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'D' charwidth: 5 */
   0xE0, 0x00, 	/*  [***  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'E' charwidth: 4 */
   0xE0, 0x00, 	/*  [*** ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0xE0, 0x00, 	/*  [*** ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0xE0, 0x00, 	/*  [*** ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* 'F' charwidth: 4 */
   0xE0, 0x00, 	/*  [*** ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0xE0, 0x00, 	/*  [*** ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* 'G' charwidth: 5 */
   0x60, 0x00, 	/*  [ **  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0xB0, 0x00, 	/*  [* ** ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'H' charwidth: 5 */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0xF0, 0x00, 	/*  [**** ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'I' charwidth: 2 */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   
   /* 'J' charwidth: 5 */
   0x10, 0x00, 	/*  [   * ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'K' charwidth: 5 */
   0x90, 0x00, 	/*  [*  * ]  */
   0xA0, 0x00, 	/*  [* *  ]  */
   0xC0, 0x00, 	/*  [**   ]  */
   0xC0, 0x00, 	/*  [**   ]  */
   0xA0, 0x00, 	/*  [* *  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'L' charwidth: 4 */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0xE0, 0x00, 	/*  [*** ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* 'M' charwidth: 6 */
   0x88, 0x00, 	/*  [*   * ]  */
   0xD8, 0x00, 	/*  [** ** ]  */
   0xA8, 0x00, 	/*  [* * * ]  */
   0x88, 0x00, 	/*  [*   * ]  */
   0x88, 0x00, 	/*  [*   * ]  */
   0x88, 0x00, 	/*  [*   * ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* 'N' charwidth: 5 */
   0x90, 0x00, 	/*  [*  * ]  */
   0xD0, 0x00, 	/*  [** * ]  */
   0xD0, 0x00, 	/*  [** * ]  */
   0xB0, 0x00, 	/*  [* ** ]  */
   0xB0, 0x00, 	/*  [* ** ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'O' charwidth: 5 */
   0x60, 0x00, 	/*  [ **  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'P' charwidth: 5 */
   0xE0, 0x00, 	/*  [***  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'Q' charwidth: 5 */
   0x60, 0x00, 	/*  [ **  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'R' charwidth: 5 */
   0xE0, 0x00, 	/*  [***  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0xA0, 0x00, 	/*  [* *  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'S' charwidth: 5 */
   0x60, 0x00, 	/*  [ **  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'T' charwidth: 6 */
   0xF8, 0x00, 	/*  [***** ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* 'U' charwidth: 5 */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'V' charwidth: 6 */
   0x88, 0x00, 	/*  [*   * ]  */
   0x88, 0x00, 	/*  [*   * ]  */
   0x50, 0x00, 	/*  [ * *  ]  */
   0x50, 0x00, 	/*  [ * *  ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* 'W' charwidth: 10 */
   0x88, 0x80, 	/*  [*   *   * ]  */
   0x88, 0x80, 	/*  [*   *   * ]  */
   0x55, 0x00, 	/*  [ * * * *  ]  */
   0x55, 0x00, 	/*  [ * * * *  ]  */
   0x22, 0x00, 	/*  [  *   *   ]  */
   0x22, 0x00, 	/*  [  *   *   ]  */
   0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 	/*  [          ]  */
   
   /* 'X' charwidth: 6 */
   0x88, 0x00, 	/*  [*   * ]  */
   0x50, 0x00, 	/*  [ * *  ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x50, 0x00, 	/*  [ * *  ]  */
   0x88, 0x00, 	/*  [*   * ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* 'Y' charwidth: 6 */
   0x88, 0x00, 	/*  [*   * ]  */
   0x50, 0x00, 	/*  [ * *  ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x20, 0x00, 	/*  [  *   ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* 'Z' charwidth: 5 */
   0xF0, 0x00, 	/*  [**** ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0xF0, 0x00, 	/*  [**** ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '[' charwidth: 3 */
   0xC0, 0x00, 	/*  [** ]  */
   0x80, 0x00, 	/*  [*  ]  */
   0x80, 0x00, 	/*  [*  ]  */
   0x80, 0x00, 	/*  [*  ]  */
   0x80, 0x00, 	/*  [*  ]  */
   0x80, 0x00, 	/*  [*  ]  */
   0xC0, 0x00, 	/*  [** ]  */
   0x00, 0x00, 	/*  [   ]  */
   
   /* '\' charwidth: 5 */
   0x80, 0x00, 	/*  [*    ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x10, 0x00, 	/*  [   * ]  */
   
   /* ']' charwidth: 3 */
   0xC0, 0x00, 	/*  [** ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0xC0, 0x00, 	/*  [** ]  */
   0x00, 0x00, 	/*  [   ]  */
   
   /* '^' charwidth: 4 */
   0x40, 0x00, 	/*  [ *  ]  */
   0xA0, 0x00, 	/*  [* * ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* '_' charwidth: 7 */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0xFC, 0x00, 	/*  [****** ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '`' charwidth: 3 */
   0x80, 0x00, 	/*  [*  ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   
   /* 'a' charwidth: 5 */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'b' charwidth: 5 */
   0x80, 0x00, 	/*  [*    ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'c' charwidth: 4 */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* 'd' charwidth: 5 */
   0x10, 0x00, 	/*  [   * ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'e' charwidth: 5 */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0xB0, 0x00, 	/*  [* ** ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'f' charwidth: 4 */
   0x60, 0x00, 	/*  [ ** ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0xC0, 0x00, 	/*  [**  ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* 'g' charwidth: 5 */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   
   /* 'h' charwidth: 5 */
   0x80, 0x00, 	/*  [*    ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'i' charwidth: 2 */
   0x80, 0x00, 	/*  [* ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   
   /* 'j' charwidth: 3 */
   0x40, 0x00, 	/*  [ * ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x80, 0x00, 	/*  [*  ]  */
   
   /* 'k' charwidth: 5 */
   0x80, 0x00, 	/*  [*    ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0xA0, 0x00, 	/*  [* *  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'l' charwidth: 2 */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x00, 0x00, 	/*  [  ]  */
   0x00, 0x00, 	/*  [  ]  */
   
   /* 'm' charwidth: 6 */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   0xF0, 0x00, 	/*  [****  ]  */
   0xA8, 0x00, 	/*  [* * * ]  */
   0xA8, 0x00, 	/*  [* * * ]  */
   0xA8, 0x00, 	/*  [* * * ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* 'n' charwidth: 5 */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'o' charwidth: 5 */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'p' charwidth: 5 */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0xE0, 0x00, 	/*  [***  ]  */
   0x80, 0x00, 	/*  [*    ]  */
   0x80, 0x00, 	/*  [*    ]  */
   
   /* 'q' charwidth: 5 */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x10, 0x00, 	/*  [   * ]  */
   
   /* 'r' charwidth: 4 */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0xA0, 0x00, 	/*  [* * ]  */
   0xC0, 0x00, 	/*  [**  ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* 's' charwidth: 4 */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0xC0, 0x00, 	/*  [**  ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0xC0, 0x00, 	/*  [**  ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* 't' charwidth: 3 */
   0x00, 0x00, 	/*  [   ]  */
   0x80, 0x00, 	/*  [*  ]  */
   0xC0, 0x00, 	/*  [** ]  */
   0x80, 0x00, 	/*  [*  ]  */
   0x80, 0x00, 	/*  [*  ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   
   /* 'u' charwidth: 5 */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x90, 0x00, 	/*  [*  * ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'v' charwidth: 4 */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0xA0, 0x00, 	/*  [* * ]  */
   0xA0, 0x00, 	/*  [* * ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* 'w' charwidth: 6 */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   0xA8, 0x00, 	/*  [* * * ]  */
   0xA8, 0x00, 	/*  [* * * ]  */
   0x50, 0x00, 	/*  [ * *  ]  */
   0x50, 0x00, 	/*  [ * *  ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* 'x' charwidth: 4 */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0xA0, 0x00, 	/*  [* * ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0xA0, 0x00, 	/*  [* * ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* 'y' charwidth: 4 */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0xA0, 0x00, 	/*  [* * ]  */
   0xA0, 0x00, 	/*  [* * ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   
   /* 'z' charwidth: 5 */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0xF0, 0x00, 	/*  [**** ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0xF0, 0x00, 	/*  [**** ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '{' charwidth: 4 */
   0x20, 0x00, 	/*  [  * ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x80, 0x00, 	/*  [*   ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x20, 0x00, 	/*  [  * ]  */
   
   /* '|' charwidth: 2 */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   0x80, 0x00, 	/*  [* ]  */
   
   /* '}' charwidth: 4 */
   0x80, 0x00, 	/*  [*   ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x80, 0x00, 	/*  [*   ]  */
   
   /* '~' charwidth: 7 */
   0x64, 0x00, 	/*  [ **  * ]  */
   0x98, 0x00, 	/*  [*  **  ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
 };
   
   
const unsigned char six_dot_width_table[]  = 
 {  2, /* ' ' */
    2, /* '!' */
    4, /* '"' */
    8, /* '#' */
    5, /* '$' */
    7, /* '%' */
    6, /* '&' */
    2, /* ''' */
    4, /* '(' */
    4, /* ')' */
    6, /* '*' */
    6, /* '+' */
    3, /* ',' */
    6, /* '-' */
    2, /* '.' */
    4, /* '/' */
    5, /* '0' */
    3, /* '1' */
    5, /* '2' */
    5, /* '3' */
    5, /* '4' */
    5, /* '5' */
    5, /* '6' */
    5, /* '7' */
    5, /* '8' */
    5, /* '9' */
    2, /* ':' */
    3, /* ';' */
    4, /* '<' */
    5, /* '=' */
    4, /* '>' */
    4, /* '?' */
    8, /* '@' */
    6, /* 'A' */
    5, /* 'B' */
    5, /* 'C' */
    5, /* 'D' */
    4, /* 'E' */
    4, /* 'F' */
    5, /* 'G' */
    5, /* 'H' */
    2, /* 'I' */
    5, /* 'J' */
    5, /* 'K' */
    4, /* 'L' */
    6, /* 'M' */
    5, /* 'N' */
    5, /* 'O' */
    5, /* 'P' */
    5, /* 'Q' */
    5, /* 'R' */
    5, /* 'S' */
    6, /* 'T' */
    5, /* 'U' */
    6, /* 'V' */
   10, /* 'W' */
    6, /* 'X' */
    6, /* 'Y' */
    5, /* 'Z' */
    3, /* '[' */
    5, /* '\' */
    3, /* ']' */
    4, /* '^' */
    7, /* '_' */
    3, /* '`' */
    5, /* 'a' */
    5, /* 'b' */
    4, /* 'c' */
    5, /* 'd' */
    5, /* 'e' */
    4, /* 'f' */
    5, /* 'g' */
    5, /* 'h' */
    2, /* 'i' */
    3, /* 'j' */
    5, /* 'k' */
    2, /* 'l' */
    6, /* 'm' */
    5, /* 'n' */
    5, /* 'o' */
    5, /* 'p' */
    5, /* 'q' */
    4, /* 'r' */
    4, /* 's' */
    3, /* 't' */
    5, /* 'u' */
    4, /* 'v' */
    6, /* 'w' */
    4, /* 'x' */
    4, /* 'y' */
    5, /* 'z' */
    4, /* '{' */
    2, /* '|' */
    4, /* '}' */
    7, /* '~' */
 };
#endif
   
#ifdef EN_SEVEN_DOT
const unsigned char seven_dot_glyph_table[]   = 
 { /* ' ' charwidth: 5 */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '!' charwidth: 4 */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* '"' charwidth: 6 */
   0x48, 0x00, 	/*  [ *  * ]  */
   0x48, 0x00, 	/*  [ *  * ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* '#' charwidth: 7 */
   0x28, 0x00, 	/*  [  * *  ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x28, 0x00, 	/*  [  * *  ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x28, 0x00, 	/*  [  * *  ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '$' charwidth: 7 */
   0x3C, 0x00, 	/*  [  **** ]  */
   0x68, 0x00, 	/*  [ ** *  ]  */
   0x68, 0x00, 	/*  [ ** *  ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x2C, 0x00, 	/*  [  * ** ]  */
   0x2C, 0x00, 	/*  [  * ** ]  */
   0x78, 0x00, 	/*  [ ****  ]  */
   0x28, 0x00, 	/*  [  * *  ]  */
   
   /* '%' charwidth: 8 */
   0x64, 0x00, 	/*  [ **  *  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x08, 0x00, 	/*  [    *   ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x10, 0x00, 	/*  [   *    ]  */
   0x36, 0x00, 	/*  [  ** ** ]  */
   0x26, 0x00, 	/*  [  *  ** ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* '&' charwidth: 9 */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x7D, 0x00, 	/*  [ ***** * ]  */
   0x67, 0x00, 	/*  [ **  *** ]  */
   0x67, 0x00, 	/*  [ **  *** ]  */
   0x3D, 0x00, 	/*  [  **** * ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* ''' charwidth: 3 */
   0x40, 0x00, 	/*  [ * ]  */
   0x40, 0x00, 	/*  [ * ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   0x00, 0x00, 	/*  [   ]  */
   
   /* '(' charwidth: 5 */
   0x10, 0x00, 	/*  [   * ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x10, 0x00, 	/*  [   * ]  */
   
   /* ')' charwidth: 5 */
   0x40, 0x00, 	/*  [ *   ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   
   /* '*' charwidth: 7 */
   0x10, 0x00, 	/*  [   *   ]  */
   0x54, 0x00, 	/*  [ * * * ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x54, 0x00, 	/*  [ * * * ]  */
   0x10, 0x00, 	/*  [   *   ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '+' charwidth: 7 */
   0x00, 0x00, 	/*  [       ]  */
   0x10, 0x00, 	/*  [   *   ]  */
   0x10, 0x00, 	/*  [   *   ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x10, 0x00, 	/*  [   *   ]  */
   0x10, 0x00, 	/*  [   *   ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* ',' charwidth: 4 */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   
   /* '-' charwidth: 7 */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '.' charwidth: 4 */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* '/' charwidth: 5 */
   0x10, 0x00, 	/*  [   * ]  */
   0x10, 0x00, 	/*  [   * ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x20, 0x00, 	/*  [  *  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '0' charwidth: 7 */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '1' charwidth: 5 */
   0x30, 0x00, 	/*  [  ** ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* '2' charwidth: 7 */
   0x78, 0x00, 	/*  [ ****  ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '3' charwidth: 7 */
   0x78, 0x00, 	/*  [ ****  ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x78, 0x00, 	/*  [ ****  ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '4' charwidth: 7 */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x1C, 0x00, 	/*  [   *** ]  */
   0x2C, 0x00, 	/*  [  * ** ]  */
   0x4C, 0x00, 	/*  [ *  ** ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '5' charwidth: 7 */
   0x78, 0x00, 	/*  [ ****  ]  */
   0x40, 0x00, 	/*  [ *     ]  */
   0x78, 0x00, 	/*  [ ****  ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x78, 0x00, 	/*  [ ****  ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '6' charwidth: 7 */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x78, 0x00, 	/*  [ ****  ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '7' charwidth: 7 */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x18, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 	/*  [   **  ]  */
   0x30, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 	/*  [  **   ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '8' charwidth: 7 */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '9' charwidth: 7 */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x3C, 0x00, 	/*  [  **** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* ':' charwidth: 4 */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* ';' charwidth: 4 */
   0x00, 0x00, 	/*  [    ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x20, 0x00, 	/*  [  * ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   
   /* '<' charwidth: 6 */
   0x08, 0x00, 	/*  [    * ]  */
   0x18, 0x00, 	/*  [   ** ]  */
   0x30, 0x00, 	/*  [  **  ]  */
   0x60, 0x00, 	/*  [ **   ]  */
   0x30, 0x00, 	/*  [  **  ]  */
   0x18, 0x00, 	/*  [   ** ]  */
   0x08, 0x00, 	/*  [    * ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* '=' charwidth: 7 */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '>' charwidth: 6 */
   0x40, 0x00, 	/*  [ *    ]  */
   0x60, 0x00, 	/*  [ **   ]  */
   0x30, 0x00, 	/*  [  **  ]  */
   0x18, 0x00, 	/*  [   ** ]  */
   0x30, 0x00, 	/*  [  **  ]  */
   0x60, 0x00, 	/*  [ **   ]  */
   0x40, 0x00, 	/*  [ *    ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* '?' charwidth: 7 */
   0x78, 0x00, 	/*  [ ****  ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x18, 0x00, 	/*  [   **  ]  */
   0x30, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 	/*  [  **   ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x30, 0x00, 	/*  [  **   ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '@' charwidth: 8 */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x7E, 0x00, 	/*  [ ****** ]  */
   0x6E, 0x00, 	/*  [ ** *** ]  */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   0x3E, 0x00, 	/*  [  ***** ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'A' charwidth: 8 */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x7E, 0x00, 	/*  [ ****** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'B' charwidth: 8 */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'C' charwidth: 7 */
   0x3C, 0x00, 	/*  [  **** ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x3C, 0x00, 	/*  [  **** ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* 'D' charwidth: 8 */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'E' charwidth: 7 */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* 'F' charwidth: 7 */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* 'G' charwidth: 8 */
   0x3E, 0x00, 	/*  [  ***** ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x3E, 0x00, 	/*  [  ***** ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'H' charwidth: 8 */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x7E, 0x00, 	/*  [ ****** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'I' charwidth: 4 */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* 'J' charwidth: 7 */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x78, 0x00, 	/*  [ ****  ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* 'K' charwidth: 8 */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x78, 0x00, 	/*  [ ****   ]  */
   0x70, 0x00, 	/*  [ ***    ]  */
   0x78, 0x00, 	/*  [ ****   ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'L' charwidth: 7 */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* 'M' charwidth: 9 */
   0x41, 0x00, 	/*  [ *     * ]  */
   0x63, 0x00, 	/*  [ **   ** ]  */
   0x77, 0x00, 	/*  [ *** *** ]  */
   0x7F, 0x00, 	/*  [ ******* ]  */
   0x6B, 0x00, 	/*  [ ** * ** ]  */
   0x63, 0x00, 	/*  [ **   ** ]  */
   0x63, 0x00, 	/*  [ **   ** ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'N' charwidth: 8 */
   0x46, 0x00, 	/*  [ *   ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x76, 0x00, 	/*  [ *** ** ]  */
   0x7E, 0x00, 	/*  [ ****** ]  */
   0x6E, 0x00, 	/*  [ ** *** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x62, 0x00, 	/*  [ **   * ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'O' charwidth: 8 */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'P' charwidth: 8 */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'Q' charwidth: 8 */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x6E, 0x00, 	/*  [ ** *** ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x06, 0x00, 	/*  [     ** ]  */
   
   /* 'R' charwidth: 8 */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'S' charwidth: 6 */
   0x38, 0x00, 	/*  [  *** ]  */
   0x60, 0x00, 	/*  [ **   ]  */
   0x60, 0x00, 	/*  [ **   ]  */
   0x30, 0x00, 	/*  [  **  ]  */
   0x18, 0x00, 	/*  [   ** ]  */
   0x18, 0x00, 	/*  [   ** ]  */
   0x70, 0x00, 	/*  [ ***  ]  */
   0x00, 0x00, 	/*  [      ]  */
   
   /* 'T' charwidth: 8 */
   0x7E, 0x00, 	/*  [ ****** ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'U' charwidth: 8 */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'V' charwidth: 8 */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'W' charwidth: 9 */
   0x63, 0x00, 	/*  [ **   ** ]  */
   0x63, 0x00, 	/*  [ **   ** ]  */
   0x63, 0x00, 	/*  [ **   ** ]  */
   0x6B, 0x00, 	/*  [ ** * ** ]  */
   0x3E, 0x00, 	/*  [  *****  ]  */
   0x36, 0x00, 	/*  [  ** **  ]  */
   0x36, 0x00, 	/*  [  ** **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'X' charwidth: 8 */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'Y' charwidth: 8 */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x66, 0x00, 	/*  [ **  ** ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 	/*  [   **   ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'Z' charwidth: 7 */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x1C, 0x00, 	/*  [   *** ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x70, 0x00, 	/*  [ ***   ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '[' charwidth: 5 */
   0x70, 0x00, 	/*  [ *** ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   
   /* '\' charwidth: 7 */
   0x40, 0x00, 	/*  [ *     ]  */
   0x40, 0x00, 	/*  [ *     ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x20, 0x00, 	/*  [  *    ]  */
   0x30, 0x00, 	/*  [  **   ]  */
   0x10, 0x00, 	/*  [   *   ]  */
   0x10, 0x00, 	/*  [   *   ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* ']' charwidth: 5 */
   0x70, 0x00, 	/*  [ *** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   
   /* '^' charwidth: 7 */
   0x20, 0x00, 	/*  [  *    ]  */
   0x50, 0x00, 	/*  [ * *   ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '_' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0xF8, 0x00, 	/*  [*****   ]  */
   
   /* '`' charwidth: 5 */
   0x20, 0x00, 	/*  [  *  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x40, 0x00, 	/*  [ *   ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'a' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x38, 0x00, 	/*  [  ***   ]  */
   0x0C, 0x00, 	/*  [    **  ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'b' charwidth: 8 */
   0x60, 0x00, 	/*  [ **     ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   0x78, 0x00, 	/*  [ ****   ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x78, 0x00, 	/*  [ ****   ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'c' charwidth: 7 */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* 'd' charwidth: 8 */
   0x0C, 0x00, 	/*  [    **  ]  */
   0x0C, 0x00, 	/*  [    **  ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'e' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x38, 0x00, 	/*  [  ***   ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'f' charwidth: 5 */
   0x30, 0x00, 	/*  [  ** ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'g' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x0C, 0x00, 	/*  [    **  ]  */
   0x38, 0x00, 	/*  [  ***   ]  */
   
   /* 'h' charwidth: 8 */
   0x60, 0x00, 	/*  [ **     ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   0x78, 0x00, 	/*  [ ****   ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'i' charwidth: 4 */
   0x60, 0x00, 	/*  [ ** ]  */
   0x00, 0x00, 	/*  [    ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x60, 0x00, 	/*  [ ** ]  */
   0x00, 0x00, 	/*  [    ]  */
   
   /* 'j' charwidth: 4 */
   0x30, 0x00, 	/*  [  **]  */
   0x00, 0x00, 	/*  [    ]  */
   0x30, 0x00, 	/*  [  **]  */
   0x30, 0x00, 	/*  [  **]  */
   0x30, 0x00, 	/*  [  **]  */
   0x30, 0x00, 	/*  [  **]  */
   0x30, 0x00, 	/*  [  **]  */
   0x60, 0x00, 	/*  [ ** ]  */
   
   /* 'k' charwidth: 7 */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x78, 0x00, 	/*  [ ****  ]  */
   0x78, 0x00, 	/*  [ ****  ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* 'l' charwidth: 5 */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'm' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x7F, 0x00, 	/*  [ ******* ]  */
   0x6D, 0x80, 	/*  [ ** ** **]  */
   0x6D, 0x80, 	/*  [ ** ** **]  */
   0x6D, 0x80, 	/*  [ ** ** **]  */
   0x6D, 0x80, 	/*  [ ** ** **]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'n' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x78, 0x00, 	/*  [ ****   ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'o' charwidth: 7 */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x6C, 0x00, 	/*  [ ** ** ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* 'p' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x78, 0x00, 	/*  [ ****   ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x78, 0x00, 	/*  [ ****   ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   
   /* 'q' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x0C, 0x00, 	/*  [    **  ]  */
   0x0C, 0x00, 	/*  [    **  ]  */
   
   /* 'r' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x68, 0x00, 	/*  [ ** *   ]  */
   0x78, 0x00, 	/*  [ ****   ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   0x60, 0x00, 	/*  [ **     ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 's' charwidth: 7 */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x30, 0x00, 	/*  [  **   ]  */
   0x18, 0x00, 	/*  [   **  ]  */
   0x70, 0x00, 	/*  [ ***   ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* 't' charwidth: 5 */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x70, 0x00, 	/*  [ *** ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 	/*  [ **  ]  */
   0x30, 0x00, 	/*  [  ** ]  */
   0x00, 0x00, 	/*  [     ]  */
   
   /* 'u' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'v' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x38, 0x00, 	/*  [  ***   ]  */
   0x38, 0x00, 	/*  [  ***   ]  */
   0x10, 0x00, 	/*  [   *    ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'w' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x63, 0x00, 	/*  [ **   **]  */
   0x6B, 0x00, 	/*  [ ** * **]  */
   0x6B, 0x00, 	/*  [ ** * **]  */
   0x3E, 0x00, 	/*  [  ***** ]  */
   0x36, 0x00, 	/*  [  ** ** ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'x' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x38, 0x00, 	/*  [  ***   ]  */
   0x7C, 0x00, 	/*  [ *****  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* 'y' charwidth: 8 */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 	/*  [ ** **  ]  */
   0x3C, 0x00, 	/*  [  ****  ]  */
   0x0C, 0x00, 	/*  [    **  ]  */
   0x38, 0x00, 	/*  [  ***   ]  */
   
   /* 'z' charwidth: 7 */
   0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 	/*  [       ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x0C, 0x00, 	/*  [    ** ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x60, 0x00, 	/*  [ **    ]  */
   0x7C, 0x00, 	/*  [ ***** ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '{' charwidth: 7 */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x20, 0x00, 	/*  [  *    ]  */
   0x20, 0x00, 	/*  [  *    ]  */
   0x40, 0x00, 	/*  [ *     ]  */
   0x20, 0x00, 	/*  [  *    ]  */
   0x20, 0x00, 	/*  [  *    ]  */
   0x38, 0x00, 	/*  [  ***  ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '|' charwidth: 4 */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   0x40, 0x00, 	/*  [ *  ]  */
   
   /* '}' charwidth: 7 */
   0x70, 0x00, 	/*  [ ***   ]  */
   0x10, 0x00, 	/*  [   *   ]  */
   0x10, 0x00, 	/*  [   *   ]  */
   0x08, 0x00, 	/*  [    *  ]  */
   0x10, 0x00, 	/*  [   *   ]  */
   0x10, 0x00, 	/*  [   *   ]  */
   0x70, 0x00, 	/*  [ ***   ]  */
   0x00, 0x00, 	/*  [       ]  */
   
   /* '~' charwidth: 8 */
   0x28, 0x00, 	/*  [  * *   ]  */
   0x50, 0x00, 	/*  [ * *    ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 	/*  [        ]  */
   
   /* '? DEG_CHAR not in correct order but needed - mt charwidth: 8 */
   0x38, 0x00,     /*  [  ***  ]  */
   0x6C, 0x00,     /*  [ ** ** ]  */
   0x6C, 0x00,     /*  [ ** ** ]  */
   0x38, 0x00,     /*  [  ***  ]  */
   0x00, 0x00,     /*  [       ]  */
   0x00, 0x00,     /*  [       ]  */
   0x00, 0x00,     /*  [       ]  */
   0x00, 0x00,     /*  [       ]  */
 };
   
const unsigned char seven_dot_width_table[]  = 
 { /* widths modified by M. Thomas */
    5, /* ' ' */
    4, /* '!' */
    6, /* '"' */
    7, /* '#' */
    7, /* '$' */
    8, /* '%' */
    9, /* '&' */
    3, /* ''' */
    5, /* '(' */
    5, /* ')' */
    7, /* '*' */
    7, /* '+' */
    4, /* ',' */
    7, /* '-' */
    4, /* '.' */
    5, /* '/' */
    7, /* '0' */
    5, /* '1' */
    7, /* '2' */
    7, /* '3' */
    7, /* '4' */
    7, /* '5' */
    7, /* '6' */
    7, /* '7' */
    7, /* '8' */
    7, /* '9' */
    4, /* ':' */
    4, /* ';' */
    6, /* '<' */
    7, /* '=' */
    6, /* '>' */
    8, /* '?' */
    8, /* '@' */
    8, /* 'A' */
    8, /* 'B' */
    7, /* 'C' */
    8, /* 'D' */
    7, /* 'E' */
    7, /* 'F' */
    8, /* 'G' */
    8, /* 'H' */
    4, /* 'I' */
    7, /* 'J' */
    8, /* 'K' */
    7, /* 'L' */
    9, /* 'M' */
    8, /* 'N' */
    8, /* 'O' */
    8, /* 'P' */
    8, /* 'Q' */
    8, /* 'R' */ 
    6, /* 'S' */
    8, /* 'T' */
    8, /* 'U' */
    8, /* 'V' */
    9, /* 'W' */
    8, /* 'X' */
    8, /* 'Y' */
    7, /* 'Z' */
    5, /* '[' */
    7, /* '\' */
    5, /* ']' */
    7, /* '^' */
    8, /* '_' */
    5, /* '`' */
    7, /* 'a' */
    7, /* 'b' */
    6, /* 'c' */
    7, /* 'd' */
    7, /* 'e' */
    5, /* 'f' */
    7, /* 'g' */
    7, /* 'h' */
    4, /* 'i' */
    5, /* 'j' */
    7, /* 'k' */
    4, /* 'l' */
    10, /* 'm' */
    7, /* 'n' */
    7, /* 'o' */
    7, /* 'p' */
    7, /* 'q' */
    6, /* 'r'  was 8 */
    6, /* 's' */
    5, /* 't' */
    7, /* 'u' */
    7, /* 'v' */
    9, /* 'w' */
    7, /* 'x' */
    7, /* 'y' */
    7, /* 'z' */
    7, /* '{' */
    4, /* '|' */
    7, /* '}' */
    8, /* '~' */
    7, /* DEG_CHAR */
 };
#endif
   
#ifdef EN_NINE_DOT
const unsigned char nine_dot_glyph_table[]   = 
 { /* ' ' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '!' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x3C, 	/*  [  ****  ]  */
   0x3C, 	/*  [  ****  ]  */
   0x3C, 	/*  [  ****  ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '"' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '#' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x36, 	/*  [  ** ** ]  */
   0x36, 	/*  [  ** ** ]  */
   0x7F, 	/*  [ *******]  */
   0x36, 	/*  [  ** ** ]  */
   0x36, 	/*  [  ** ** ]  */
   0x36, 	/*  [  ** ** ]  */
   0x7F, 	/*  [ *******]  */
   0x36, 	/*  [  ** ** ]  */
   0x36, 	/*  [  ** ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '$' charwidth: 8 */
   0x08, 	/*  [    *   ]  */
   0x3E, 	/*  [  ***** ]  */
   0x6B, 	/*  [ ** * **]  */
   0x68, 	/*  [ ** *   ]  */
   0x68, 	/*  [ ** *   ]  */
   0x3E, 	/*  [  ***** ]  */
   0x0B, 	/*  [    * **]  */
   0x0B, 	/*  [    * **]  */
   0x6B, 	/*  [ ** * **]  */
   0x3E, 	/*  [  ***** ]  */
   0x08, 	/*  [    *   ]  */
   0x08, 	/*  [        ]  */
   
   /* '%' charwidth: 8 */
   0x70, 	/*  [ ***    ]  */
   0xD8, 	/*  [** **   ]  */
   0xDA, 	/*  [** ** * ]  */
   0x76, 	/*  [ *** ** ]  */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   0x6E, 	/*  [ ** *** ]  */
   0x5B, 	/*  [ * ** **]  */
   0x1B, 	/*  [   ** **]  */
   0x0E, 	/*  [    *** ]  */
   0x00, 	/*  [        ]  */
   
   /* '&' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x38, 	/*  [  ***   ]  */
   0x6C, 	/*  [ ** **  ]  */
   0x6C, 	/*  [ ** **  ]  */
   0x38, 	/*  [  ***   ]  */
   0x6C, 	/*  [ ** **  ]  */
   0xC7, 	/*  [**   ***]  */
   0xC6, 	/*  [**   ** ]  */
   0x6E, 	/*  [ ** *** ]  */
   0x3B, 	/*  [  *** **]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* ''' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '(' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x0C, 	/*  [    **  ]  */
   
   /* ')' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x30, 	/*  [  **    ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   
   /* '*' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x36, 	/*  [  ** ** ]  */
   0x1C, 	/*  [   ***  ]  */
   0x7F, 	/*  [ *******]  */
   0x1C, 	/*  [   ***  ]  */
   0x36, 	/*  [  ** ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '+' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x7E, 	/*  [ ****** ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* ',' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   0x00, 	/*  [        ]  */
   
   /* '-' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x7E, 	/*  [ ****** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '.' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '/' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x00, 	/*  [        ]  */
   
   /* '0' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x1C, 	/*  [   ***  ]  */
   0x36, 	/*  [  ** ** ]  */
   0x63, 	/*  [ **   **]  */
   0x6B, 	/*  [ ** * **]  */
   0x6B, 	/*  [ ** * **]  */
   0x6B, 	/*  [ ** * **]  */
   0x63, 	/*  [ **   **]  */
   0x36, 	/*  [  ** ** ]  */
   0x1C, 	/*  [   ***  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '1' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x0C, 	/*  [    **  ]  */
   0x1C, 	/*  [   ***  ]  */
   0x3C, 	/*  [  ****  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x3F, 	/*  [  ******]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '2' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   0x60, 	/*  [ **     ]  */
   0x7E, 	/*  [ ****** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '3' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x06, 	/*  [     ** ]  */
   0x1C, 	/*  [   ***  ]  */
   0x06, 	/*  [     ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '4' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x06, 	/*  [     ** ]  */
   0x0E, 	/*  [    *** ]  */
   0x1E, 	/*  [   **** ]  */
   0x36, 	/*  [  ** ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x7F, 	/*  [ *******]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '5' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x7E, 	/*  [ ****** ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x7C, 	/*  [ *****  ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '6' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x1C, 	/*  [   ***  ]  */
   0x30, 	/*  [  **    ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x7C, 	/*  [ *****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '7' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x7E, 	/*  [ ****** ]  */
   0x06, 	/*  [     ** ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '8' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '9' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3E, 	/*  [  ***** ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x0C, 	/*  [    **  ]  */
   0x38, 	/*  [  ***   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* ':' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* ';' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   0x00, 	/*  [        ]  */
   
   /* '<' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x06, 	/*  [     ** ]  */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   0x60, 	/*  [ **     ]  */
   0x30, 	/*  [  **    ]  */
   0x18, 	/*  [   **   ]  */
   0x0C, 	/*  [    **  ]  */
   0x06, 	/*  [     ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '=' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x7E, 	/*  [ ****** ]  */
   0x00, 	/*  [        ]  */
   0x7E, 	/*  [ ****** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '>' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x60, 	/*  [ **     ]  */
   0x30, 	/*  [  **    ]  */
   0x18, 	/*  [   **   ]  */
   0x0C, 	/*  [    **  ]  */
   0x06, 	/*  [     ** ]  */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   0x60, 	/*  [ **     ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '?' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '@' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x7E, 	/*  [ ****** ]  */
   0xC3, 	/*  [**    **]  */
   0xC3, 	/*  [**    **]  */
   0xCF, 	/*  [**  ****]  */
   0xDB, 	/*  [** ** **]  */
   0xDB, 	/*  [** ** **]  */
   0xCF, 	/*  [**  ****]  */
   0xC0, 	/*  [**      ]  */
   0x7F, 	/*  [ *******]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'A' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x7E, 	/*  [ ****** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'B' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x7C, 	/*  [ *****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x7C, 	/*  [ *****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x7C, 	/*  [ *****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'C' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'D' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x78, 	/*  [ ****   ]  */
   0x6C, 	/*  [ ** **  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x6C, 	/*  [ ** **  ]  */
   0x78, 	/*  [ ****   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'E' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x7E, 	/*  [ ****** ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x7C, 	/*  [ *****  ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x7E, 	/*  [ ****** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'F' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x7E, 	/*  [ ****** ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x7C, 	/*  [ *****  ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'G' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x6E, 	/*  [ ** *** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3E, 	/*  [  ***** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'H' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x7E, 	/*  [ ****** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'I' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'J' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'K' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x6C, 	/*  [ ** **  ]  */
   0x6C, 	/*  [ ** **  ]  */
   0x78, 	/*  [ ****   ]  */
   0x6C, 	/*  [ ** **  ]  */
   0x6C, 	/*  [ ** **  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'L' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x7E, 	/*  [ ****** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'M' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x63, 	/*  [ **   **]  */
   0x77, 	/*  [ *** ***]  */
   0x7F, 	/*  [ *******]  */
   0x6B, 	/*  [ ** * **]  */
   0x6B, 	/*  [ ** * **]  */
   0x63, 	/*  [ **   **]  */
   0x63, 	/*  [ **   **]  */
   0x63, 	/*  [ **   **]  */
   0x63, 	/*  [ **   **]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'N' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x63, 	/*  [ **   **]  */
   0x63, 	/*  [ **   **]  */
   0x73, 	/*  [ ***  **]  */
   0x7B, 	/*  [ **** **]  */
   0x6F, 	/*  [ ** ****]  */
   0x67, 	/*  [ **  ***]  */
   0x63, 	/*  [ **   **]  */
   0x63, 	/*  [ **   **]  */
   0x63, 	/*  [ **   **]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'O' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'P' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x7C, 	/*  [ *****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x7C, 	/*  [ *****  ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'Q' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x0C, 	/*  [    **  ]  */
   0x06, 	/*  [     ** ]  */
   
   /* 'R' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x7C, 	/*  [ *****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x7C, 	/*  [ *****  ]  */
   0x6C, 	/*  [ ** **  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'S' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x60, 	/*  [ **     ]  */
   0x30, 	/*  [  **    ]  */
   0x18, 	/*  [   **   ]  */
   0x0C, 	/*  [    **  ]  */
   0x06, 	/*  [     ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'T' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x7E, 	/*  [ ****** ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'U' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'V' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'W' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x63, 	/*  [ **   **]  */
   0x63, 	/*  [ **   **]  */
   0x63, 	/*  [ **   **]  */
   0x63, 	/*  [ **   **]  */
   0x6B, 	/*  [ ** * **]  */
   0x6B, 	/*  [ ** * **]  */
   0x6B, 	/*  [ ** * **]  */
   0x3E, 	/*  [  ***** ]  */
   0x36, 	/*  [  ** ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'X' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x18, 	/*  [   **   ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'Y' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'Z' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x7E, 	/*  [ ****** ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x7E, 	/*  [ ****** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '[' charwidth: 8 */
   0x3C, 	/*  [  ****  ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x3C, 	/*  [  ****  ]  */
   
   /* '\' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x00, 	/*  [        ]  */
   
   /* ']' charwidth: 8 */
   0x3C, 	/*  [  ****  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x3C, 	/*  [  ****  ]  */
   
   /* '^' charwidth: 8 */
   0x18, 	/*  [   **   ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '_' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0xFF, 	/*  [********]  */
   
   /* '`' charwidth: 8 */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'a' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x3E, 	/*  [  ***** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3E, 	/*  [  ***** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'b' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x7C, 	/*  [ *****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x7C, 	/*  [ *****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'c' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'd' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x3E, 	/*  [  ***** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3E, 	/*  [  ***** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'e' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x7E, 	/*  [ ****** ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'f' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x1C, 	/*  [   ***  ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x78, 	/*  [ ****   ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'g' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x3E, 	/*  [  ***** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3E, 	/*  [  ***** ]  */
   0x06, 	/*  [     ** ]  */
   0x7C, 	/*  [ *****  ]  */
   
   /* 'h' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x7C, 	/*  [ *****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'i' charwidth: 8 */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x78, 	/*  [ ****   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x7E, 	/*  [ ****** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'j' charwidth: 8 */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x0C, 	/*  [    **  ]  */
   0x78, 	/*  [ ****   ]  */
   
   /* 'k' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x6C, 	/*  [ ** **  ]  */
   0x78, 	/*  [ ****   ]  */
   0x6C, 	/*  [ ** **  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'l' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x78, 	/*  [ ****   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x7E, 	/*  [ ****** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'm' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x7E, 	/*  [ ****** ]  */
   0x6B, 	/*  [ ** * **]  */
   0x6B, 	/*  [ ** * **]  */
   0x6B, 	/*  [ ** * **]  */
   0x6B, 	/*  [ ** * **]  */
   0x6B, 	/*  [ ** * **]  */
   0x63, 	/*  [ **   **]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'n' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x7C, 	/*  [ *****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'o' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'p' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x7C, 	/*  [ *****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x7C, 	/*  [ *****  ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   
   /* 'q' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x3E, 	/*  [  ***** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3E, 	/*  [  ***** ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   
   /* 'r' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x7C, 	/*  [ *****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 's' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x3E, 	/*  [  ***** ]  */
   0x60, 	/*  [ **     ]  */
   0x60, 	/*  [ **     ]  */
   0x3C, 	/*  [  ****  ]  */
   0x06, 	/*  [     ** ]  */
   0x06, 	/*  [     ** ]  */
   0x7C, 	/*  [ *****  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 't' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x78, 	/*  [ ****   ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x30, 	/*  [  **    ]  */
   0x1C, 	/*  [   ***  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'u' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3E, 	/*  [  ***** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'v' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'w' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x63, 	/*  [ **   **]  */
   0x63, 	/*  [ **   **]  */
   0x63, 	/*  [ **   **]  */
   0x6B, 	/*  [ ** * **]  */
   0x6B, 	/*  [ ** * **]  */
   0x3E, 	/*  [  ***** ]  */
   0x36, 	/*  [  ** ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'x' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3C, 	/*  [  ****  ]  */
   0x18, 	/*  [   **   ]  */
   0x3C, 	/*  [  ****  ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* 'y' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x66, 	/*  [ **  ** ]  */
   0x3E, 	/*  [  ***** ]  */
   0x06, 	/*  [     ** ]  */
   0x7C, 	/*  [ *****  ]  */
   
   /* 'z' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x7E, 	/*  [ ****** ]  */
   0x06, 	/*  [     ** ]  */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   0x60, 	/*  [ **     ]  */
   0x7E, 	/*  [ ****** ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   
   /* '{' charwidth: 8 */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x0C, 	/*  [    **  ]  */
   0x00, 	/*  [        ]  */
   
   /* '|' charwidth: 8 */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x00, 	/*  [        ]  */
   
   /* '}' charwidth: 8 */
   0x30, 	/*  [  **    ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x0C, 	/*  [    **  ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x18, 	/*  [   **   ]  */
   0x30, 	/*  [  **    ]  */
   0x00, 	/*  [        ]  */
   
   /* '~' charwidth: 8 */
   0x00, 	/*  [        ]  */
   0x76, 	/*  [ *** ** ]  */
   0xDC, 	/*  [** ***  ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
   0x00, 	/*  [        ]  */
 };
#endif
   
#ifdef EN_TEN_DOT
const unsigned char ten_dot_glyph_table[]  = 
 { /* ' ' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '!' charwidth: 9 */
   0x18, 0x00, 	/*  [   **    ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '"' charwidth: 9 */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x24, 0x00, 	/*  [  *  *   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '#' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xFF, 0x00, 	/*  [******** ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xFF, 0x00, 	/*  [******** ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '$' charwidth: 9 */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x3E, 0x00, 	/*  [  *****  ]  */
   0x6B, 0x00, 	/*  [ ** * ** ]  */
   0x69, 0x00, 	/*  [ ** *  * ]  */
   0x68, 0x00, 	/*  [ ** *    ]  */
   0x3E, 0x00, 	/*  [  *****  ]  */
   0x0B, 0x00, 	/*  [    * ** ]  */
   0x0B, 0x00, 	/*  [    * ** ]  */
   0x4B, 0x00, 	/*  [ *  * ** ]  */
   0x6B, 0x00, 	/*  [ ** * ** ]  */
   0x3E, 0x00, 	/*  [  *****  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   
   /* '%' charwidth: 9 */
   0x70, 0x00, 	/*  [ ***     ]  */
   0x51, 0x00, 	/*  [ * *   * ]  */
   0x73, 0x00, 	/*  [ ***  ** ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x67, 0x00, 	/*  [ **  *** ]  */
   0xC5, 0x00, 	/*  [**   * * ]  */
   0x87, 0x00, 	/*  [*    *** ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '&' charwidth: 9 */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0xCD, 0x00, 	/*  [**  ** * ]  */
   0xC7, 0x00, 	/*  [**   *** ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7B, 0x00, 	/*  [ **** ** ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* ''' charwidth: 9 */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '(' charwidth: 9 */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* ')' charwidth: 9 */
   0x18, 0x00, 	/*  [   **    ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '*' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0xFF, 0x00, 	/*  [******** ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '+' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0xFF, 0x00, 	/*  [******** ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* ',' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   
   /* '-' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xFF, 0x00, 	/*  [******** ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '.' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '/' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x01, 0x00, 	/*  [       * ]  */
   0x03, 0x00, 	/*  [      ** ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '0' charwidth: 9 */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '1' charwidth: 9 */
   0x18, 0x00, 	/*  [   **    ]  */
   0x38, 0x00, 	/*  [  ***    ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x7E, 0x00, 	/*  [ ******  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '2' charwidth: 9 */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xFE, 0x00, 	/*  [*******  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '3' charwidth: 9 */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x1C, 0x00, 	/*  [   ***   ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '4' charwidth: 9 */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x1C, 0x00, 	/*  [   ***   ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x6C, 0x00, 	/*  [ ** **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xFE, 0x00, 	/*  [*******  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x1E, 0x00, 	/*  [   ****  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '5' charwidth: 9 */
   0xFE, 0x00, 	/*  [*******  ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xFC, 0x00, 	/*  [******   ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '6' charwidth: 9 */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xFC, 0x00, 	/*  [******   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '7' charwidth: 9 */
   0xFE, 0x00, 	/*  [*******  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '8' charwidth: 9 */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '9' charwidth: 9 */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7E, 0x00, 	/*  [ ******  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* ':' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* ';' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '<' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '=' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xFE, 0x00, 	/*  [*******  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xFE, 0x00, 	/*  [*******  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '>' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x03, 0x00, 	/*  [      ** ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '?' charwidth: 9 */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '@' charwidth: 9 */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xCE, 0x00, 	/*  [**  ***  ]  */
   0xDE, 0x00, 	/*  [** ****  ]  */
   0xDC, 0x00, 	/*  [** ***   ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'A' charwidth: 9 */
   0x10, 0x00, 	/*  [   *     ]  */
   0x38, 0x00, 	/*  [  ***    ]  */
   0x6C, 0x00, 	/*  [ ** **   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xFE, 0x00, 	/*  [*******  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'B' charwidth: 9 */
   0xFC, 0x00, 	/*  [******   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xFC, 0x00, 	/*  [******   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'C' charwidth: 9 */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xC2, 0x00, 	/*  [**    *  ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC2, 0x00, 	/*  [**    *  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'D' charwidth: 9 */
   0xF8, 0x00, 	/*  [*****    ]  */
   0x6C, 0x00, 	/*  [ ** **   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x6C, 0x00, 	/*  [ ** **   ]  */
   0xF8, 0x00, 	/*  [*****    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'E' charwidth: 9 */
   0xFE, 0x00, 	/*  [*******  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x68, 0x00, 	/*  [ ** *    ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0x68, 0x00, 	/*  [ ** *    ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xFE, 0x00, 	/*  [*******  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'F' charwidth: 9 */
   0xFE, 0x00, 	/*  [*******  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x68, 0x00, 	/*  [ ** *    ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0x68, 0x00, 	/*  [ ** *    ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0xF0, 0x00, 	/*  [****     ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'G' charwidth: 9 */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xC2, 0x00, 	/*  [**    *  ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xCE, 0x00, 	/*  [**  ***  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x3A, 0x00, 	/*  [  *** *  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'H' charwidth: 9 */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xFE, 0x00, 	/*  [*******  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'I' charwidth: 9 */
   0x7E, 0x00, 	/*  [ ******  ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x7E, 0x00, 	/*  [ ******  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'J' charwidth: 9 */
   0x1E, 0x00, 	/*  [   ****  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'K' charwidth: 9 */
   0xE6, 0x00, 	/*  [***  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x6C, 0x00, 	/*  [ ** **   ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0x70, 0x00, 	/*  [ ***     ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0x6C, 0x00, 	/*  [ ** **   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xE6, 0x00, 	/*  [***  **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'L' charwidth: 9 */
   0xF0, 0x00, 	/*  [****     ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xFE, 0x00, 	/*  [*******  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'M' charwidth: 9 */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xE7, 0x00, 	/*  [***  *** ]  */
   0xFF, 0x00, 	/*  [******** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'N' charwidth: 9 */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xE6, 0x00, 	/*  [***  **  ]  */
   0xF6, 0x00, 	/*  [**** **  ]  */
   0xDE, 0x00, 	/*  [** ****  ]  */
   0xCE, 0x00, 	/*  [**  ***  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'O' charwidth: 9 */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'P' charwidth: 9 */
   0xFC, 0x00, 	/*  [******   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0xF0, 0x00, 	/*  [****     ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'Q' charwidth: 9 */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xD6, 0x00, 	/*  [** * **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   
   /* 'R' charwidth: 9 */
   0xFC, 0x00, 	/*  [******   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x6C, 0x00, 	/*  [ ** **   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xE6, 0x00, 	/*  [***  **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'S' charwidth: 9 */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x38, 0x00, 	/*  [  ***    ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'T' charwidth: 9 */
   0xFF, 0x00, 	/*  [******** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'U' charwidth: 9 */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'V' charwidth: 9 */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'W' charwidth: 9 */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0xFF, 0x00, 	/*  [******** ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'X' charwidth: 9 */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'Y' charwidth: 9 */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'Z' charwidth: 9 */
   0xFF, 0x00, 	/*  [******** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC3, 0x00,    /*  [**    ** ]  */
   0xFF, 0x00, 	/*  [******** ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '[' charwidth: 9 */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '\' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x80, 0x00, 	/*  [*        ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xE0, 0x00, 	/*  [***      ]  */
   0x70, 0x00, 	/*  [ ***     ]  */
   0x38, 0x00, 	/*  [  ***    ]  */
   0x1C, 0x00, 	/*  [   ***   ]  */
   0x0E, 0x00, 	/*  [    ***  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x02, 0x00, 	/*  [      *  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* ']' charwidth: 9 */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '^' charwidth: 9 */
   0x08, 0x00, 	/*  [    *    ]  */
   0x1C, 0x00, 	/*  [   ***   ]  */
   0x36, 0x00, 	/*  [  ** **  ]  */
   0x63, 0x00, 	/*  [ **   ** ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '_' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xFF, 0x00, 	/*  [******** ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '`' charwidth: 9 */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'a' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0x76, 0x00, 	/*  [ *** **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'b' charwidth: 9 */
   0xE0, 0x00, 	/*  [***      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0x6C, 0x00, 	/*  [ ** **   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xDC, 0x00, 	/*  [** ***   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'c' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'd' charwidth: 9 */
   0x1C, 0x00, 	/*  [   ***   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x6C, 0x00, 	/*  [ ** **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0x76, 0x00, 	/*  [ *** **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'e' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xFE, 0x00, 	/*  [*******  ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'f' charwidth: 9 */
   0x1C, 0x00, 	/*  [   ***   ]  */
   0x36, 0x00, 	/*  [  ** **  ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'g' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x76, 0x00, 	/*  [ *** **  ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   
   /* 'h' charwidth: 9 */
   0xE0, 0x00, 	/*  [***      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x6C, 0x00, 	/*  [ ** **   ]  */
   0x76, 0x00, 	/*  [ *** **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xE6, 0x00, 	/*  [***  **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'i' charwidth: 9 */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x38, 0x00, 	/*  [  ***    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'j' charwidth: 9 */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x0E, 0x00, 	/*  [    ***  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   
   /* 'k' charwidth: 9 */
   0xE0, 0x00, 	/*  [***      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x6C, 0x00, 	/*  [ ** **   ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0x78, 0x00, 	/*  [ ****    ]  */
   0x6C, 0x00, 	/*  [ ** **   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xE6, 0x00, 	/*  [***  **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'l' charwidth: 9 */
   0x38, 0x00, 	/*  [  ***    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'm' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xE6, 0x00, 	/*  [***  **  ]  */
   0xFF, 0x00, 	/*  [******** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'n' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xDC, 0x00, 	/*  [** ***   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'o' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'p' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xDC, 0x00, 	/*  [** ***   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0xF0, 0x00, 	/*  [****     ]  */
   
   /* 'q' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x76, 0x00, 	/*  [ *** **  ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x0C, 0x00, 	/*  [    **   ]  */
   0x1E, 0x00, 	/*  [   ****  ]  */
   
   /* 'r' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xDC, 0x00, 	/*  [** ***   ]  */
   0x76, 0x00, 	/*  [ *** **  ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0xF0, 0x00, 	/*  [****     ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 's' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC0, 0x00, 	/*  [**       ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7C, 0x00, 	/*  [ *****   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 't' charwidth: 9 */
   0x10, 0x00, 	/*  [   *     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0xFC, 0x00, 	/*  [******   ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x36, 0x00, 	/*  [  ** **  ]  */
   0x1C, 0x00, 	/*  [   ***   ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'u' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0x76, 0x00, 	/*  [ *** **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'v' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'w' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0xDB, 0x00, 	/*  [** ** ** ]  */
   0xFF, 0x00, 	/*  [******** ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'x' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x3C, 0x00, 	/*  [  ****   ]  */
   0x66, 0x00, 	/*  [ **  **  ]  */
   0xC3, 0x00, 	/*  [**    ** ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* 'y' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0x7E, 0x00, 	/*  [ ******  ]  */
   0x06, 0x00, 	/*  [     **  ]  */
   0xFC, 0x00, 	/*  [******   ]  */
   
   /* 'z' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0xFE, 0x00, 	/*  [*******  ]  */
   0xCC, 0x00, 	/*  [**  **   ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x30, 0x00, 	/*  [  **     ]  */
   0x60, 0x00, 	/*  [ **      ]  */
   0xC6, 0x00, 	/*  [**   **  ]  */
   0xFE, 0x00, 	/*  [*******  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '{' charwidth: 9 */
   0x0E, 0x00, 	/*  [    ***  ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x70, 0x00, 	/*  [ ***     ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x0E, 0x00, 	/*  [    ***  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '|' charwidth: 9 */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '}' charwidth: 9 */
   0x70, 0x00, 	/*  [ ***     ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x0E, 0x00, 	/*  [    ***  ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x18, 0x00, 	/*  [   **    ]  */
   0x70, 0x00, 	/*  [ ***     ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   
   /* '~' charwidth: 9 */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x3B, 0x00, 	/*  [  *** ** ]  */
   0x6E, 0x00, 	/*  [ ** ***  ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 	/*  [         ]  */
 };
#endif
   
#ifdef EN_FIFTEEN_DOT
const unsigned char fifteen_dot_glyph_table[]  = 
 { /* ' ' charwidth: 6 */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   
   /* '!' charwidth: 6 */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   
   /* '"' charwidth: 8 */
   0x6C, 0x00, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 0x00, 	/*  [ ** **  ]  */
   0x6C, 0x00, 0x00, 	/*  [ ** **  ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   
   /* '#' charwidth: 13 */
   0x04, 0x40, 0x00, 	/*  [     *   *   ]  */
   0x04, 0x40, 0x00, 	/*  [     *   *   ]  */
   0x04, 0x40, 0x00, 	/*  [     *   *   ]  */
   0x04, 0x40, 0x00, 	/*  [     *   *   ]  */
   0x7F, 0xF0, 0x00, 	/*  [ *********** ]  */
   0x08, 0x80, 0x00, 	/*  [    *   *    ]  */
   0x08, 0x80, 0x00, 	/*  [    *   *    ]  */
   0x08, 0x80, 0x00, 	/*  [    *   *    ]  */
   0x09, 0x00, 0x00, 	/*  [    *  *     ]  */
   0x11, 0x00, 0x00, 	/*  [   *   *     ]  */
   0xFF, 0xE0, 0x00, 	/*  [***********  ]  */
   0x11, 0x00, 0x00, 	/*  [   *   *     ]  */
   0x12, 0x00, 0x00, 	/*  [   *  *      ]  */
   0x22, 0x00, 0x00, 	/*  [  *   *      ]  */
   0x22, 0x00, 0x00, 	/*  [  *   *      ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '$' charwidth: 13 */
   0x02, 0x00, 0x00, 	/*  [      *      ]  */
   0x0F, 0xC0, 0x00, 	/*  [    ******   ]  */
   0x1A, 0x60, 0x00, 	/*  [   ** *  **  ]  */
   0x32, 0x20, 0x00, 	/*  [  **  *   *  ]  */
   0x32, 0x00, 0x00, 	/*  [  **  *      ]  */
   0x3A, 0x00, 0x00, 	/*  [  *** *      ]  */
   0x1E, 0x00, 0x00, 	/*  [   ****      ]  */
   0x07, 0x00, 0x00, 	/*  [     ***     ]  */
   0x03, 0xC0, 0x00, 	/*  [      ****   ]  */
   0x02, 0xE0, 0x00, 	/*  [      * ***  ]  */
   0x02, 0x60, 0x00, 	/*  [      *  **  ]  */
   0x02, 0x60, 0x00, 	/*  [      *  **  ]  */
   0x22, 0x60, 0x00, 	/*  [  *   *  **  ]  */
   0x32, 0xC0, 0x00, 	/*  [  **  * **   ]  */
   0x1F, 0x80, 0x00, 	/*  [   ******    ]  */
   0x02, 0x00, 0x00, 	/*  [      *      ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '%' charwidth: 14 */
   0x78, 0x10, 0x00, 	/*  [ ****      *  ]  */
   0xCC, 0x10, 0x00, 	/*  [**  **     *  ]  */
   0xCC, 0x20, 0x00, 	/*  [**  **    *   ]  */
   0xCC, 0x40, 0x00, 	/*  [**  **   *    ]  */
   0xCC, 0x40, 0x00, 	/*  [**  **   *    ]  */
   0xCC, 0x80, 0x00, 	/*  [**  **  *     ]  */
   0x79, 0x00, 0x00, 	/*  [ ****  *      ]  */
   0x02, 0x00, 0x00, 	/*  [      *       ]  */
   0x02, 0xF0, 0x00, 	/*  [      * ****  ]  */
   0x05, 0x98, 0x00, 	/*  [     * **  ** ]  */
   0x09, 0x98, 0x00, 	/*  [    *  **  ** ]  */
   0x11, 0x98, 0x00, 	/*  [   *   **  ** ]  */
   0x11, 0x98, 0x00, 	/*  [   *   **  ** ]  */
   0x21, 0x98, 0x00, 	/*  [  *    **  ** ]  */
   0x40, 0xF0, 0x00, 	/*  [ *      ****  ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   
   /* '&' charwidth: 14 */
   0x07, 0x80, 0x00, 	/*  [     ****     ]  */
   0x0C, 0xC0, 0x00, 	/*  [    **  **    ]  */
   0x0C, 0xC0, 0x00, 	/*  [    **  **    ]  */
   0x0C, 0xC0, 0x00, 	/*  [    **  **    ]  */
   0x0C, 0xC0, 0x00, 	/*  [    **  **    ]  */
   0x07, 0x80, 0x00, 	/*  [     ****     ]  */
   0x0E, 0x00, 0x00, 	/*  [    ***       ]  */
   0x33, 0x3C, 0x00, 	/*  [  **  **  ****]  */
   0x63, 0x08, 0x00, 	/*  [ **   **    * ]  */
   0x61, 0x88, 0x00, 	/*  [ **    **   * ]  */
   0x61, 0xD0, 0x00, 	/*  [ **    *** *  ]  */
   0x70, 0xF0, 0x00, 	/*  [ ***    ****  ]  */
   0x78, 0x60, 0x00, 	/*  [ ****    **   ]  */
   0x3F, 0xF0, 0x00, 	/*  [  **********  ]  */
   0x0F, 0x38, 0x00, 	/*  [    ****  *** ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   
   /* ''' charwidth: 5 */
   0x60, 0x00, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 0x00, 	/*  [ **  ]  */
   0x60, 0x00, 0x00, 	/*  [ **  ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   
   /* '(' charwidth: 7 */
   0x04, 0x00, 0x00, 	/*  [     * ]  */
   0x08, 0x00, 0x00, 	/*  [    *  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x20, 0x00, 0x00, 	/*  [  *    ]  */
   0x60, 0x00, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 0x00, 	/*  [ **    ]  */
   0x60, 0x00, 0x00, 	/*  [ **    ]  */
   0x20, 0x00, 0x00, 	/*  [  *    ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x08, 0x00, 0x00, 	/*  [    *  ]  */
   0x04, 0x00, 0x00, 	/*  [     * ]  */
   
   /* ')' charwidth: 7 */
   0x40, 0x00, 0x00, 	/*  [ *     ]  */
   0x20, 0x00, 0x00, 	/*  [  *    ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x08, 0x00, 0x00, 	/*  [    *  ]  */
   0x0C, 0x00, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 0x00, 	/*  [    ** ]  */
   0x0C, 0x00, 0x00, 	/*  [    ** ]  */
   0x08, 0x00, 0x00, 	/*  [    *  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x20, 0x00, 0x00, 	/*  [  *    ]  */
   0x40, 0x00, 0x00, 	/*  [ *     ]  */
   
   /* '*' charwidth: 10 */
   0x0C, 0x00, 0x00, 	/*  [    **    ]  */
   0x0C, 0x00, 0x00, 	/*  [    **    ]  */
   0x4C, 0x80, 0x00, 	/*  [ *  **  * ]  */
   0x73, 0x80, 0x00, 	/*  [ ***  *** ]  */
   0x12, 0x00, 0x00, 	/*  [   *  *   ]  */
   0x1E, 0x00, 0x00, 	/*  [   ****   ]  */
   0x33, 0x00, 0x00, 	/*  [  **  **  ]  */
   0x33, 0x00, 0x00, 	/*  [  **  **  ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   
   /* '+' charwidth: 13 */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x7F, 0xE0, 0x00, 	/*  [ **********  ]  */
   0x7F, 0xE0, 0x00, 	/*  [ **********  ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* ',' charwidth: 6 */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x10, 0x00, 0x00, 	/*  [   *  ]  */
   0x10, 0x00, 0x00, 	/*  [   *  ]  */
   0x20, 0x00, 0x00, 	/*  [  *   ]  */
   
   /* '-' charwidth: 7 */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x7C, 0x00, 0x00, 	/*  [ ***** ]  */
   0x7C, 0x00, 0x00, 	/*  [ ***** ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   
   /* '.' charwidth: 6 */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   
   /* '/' charwidth: 11 */
   0x00, 0xC0, 0x00, 	/*  [        ** ]  */
   0x00, 0xC0, 0x00, 	/*  [        ** ]  */
   0x01, 0x80, 0x00, 	/*  [       **  ]  */
   0x01, 0x80, 0x00, 	/*  [       **  ]  */
   0x03, 0x80, 0x00, 	/*  [      ***  ]  */
   0x03, 0x00, 0x00, 	/*  [      **   ]  */
   0x03, 0x00, 0x00, 	/*  [      **   ]  */
   0x06, 0x00, 0x00, 	/*  [     **    ]  */
   0x06, 0x00, 0x00, 	/*  [     **    ]  */
   0x0C, 0x00, 0x00, 	/*  [    **     ]  */
   0x0C, 0x00, 0x00, 	/*  [    **     ]  */
   0x18, 0x00, 0x00, 	/*  [   **      ]  */
   0x18, 0x00, 0x00, 	/*  [   **      ]  */
   0x38, 0x00, 0x00, 	/*  [  ***      ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   
   /* '0' charwidth: 13 */
   0x0F, 0x00, 0x00, 	/*  [    ****     ]  */
   0x19, 0x80, 0x00, 	/*  [   **  **    ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **   ]  */
   0x20, 0x40, 0x00, 	/*  [  *      *   ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x20, 0x40, 0x00, 	/*  [  *      *   ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **   ]  */
   0x19, 0x80, 0x00, 	/*  [   **  **    ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****     ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '1' charwidth: 13 */
   0x02, 0x00, 0x00, 	/*  [      *      ]  */
   0x1E, 0x00, 0x00, 	/*  [   ****      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x1F, 0x80, 0x00, 	/*  [   ******    ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '2' charwidth: 13 */
   0x1F, 0x80, 0x00, 	/*  [   ******    ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **   ]  */
   0x20, 0x60, 0x00, 	/*  [  *      **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0xC0, 0x00, 	/*  [        **   ]  */
   0x01, 0x80, 0x00, 	/*  [       **    ]  */
   0x03, 0x00, 0x00, 	/*  [      **     ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x08, 0x00, 0x00, 	/*  [    *        ]  */
   0x18, 0x00, 0x00, 	/*  [   **        ]  */
   0x10, 0x00, 0x00, 	/*  [   *         ]  */
   0x3F, 0xE0, 0x00, 	/*  [  *********  ]  */
   0x3F, 0xE0, 0x00, 	/*  [  *********  ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '3' charwidth: 13 */
   0x1F, 0x80, 0x00, 	/*  [   ******    ]  */
   0x20, 0xC0, 0x00, 	/*  [  *     **   ]  */
   0x20, 0x60, 0x00, 	/*  [  *      **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0xC0, 0x00, 	/*  [        **   ]  */
   0x01, 0x80, 0x00, 	/*  [       **    ]  */
   0x1F, 0x00, 0x00, 	/*  [   *****     ]  */
   0x01, 0xC0, 0x00, 	/*  [       ***   ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x20, 0x60, 0x00, 	/*  [  *      **  ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **   ]  */
   0x1F, 0x80, 0x00, 	/*  [   ******    ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '4' charwidth: 13 */
   0x00, 0x40, 0x00, 	/*  [         *   ]  */
   0x00, 0xC0, 0x00, 	/*  [        **   ]  */
   0x01, 0xC0, 0x00, 	/*  [       ***   ]  */
   0x02, 0xC0, 0x00, 	/*  [      * **   ]  */
   0x04, 0xC0, 0x00, 	/*  [     *  **   ]  */
   0x08, 0xC0, 0x00, 	/*  [    *   **   ]  */
   0x10, 0xC0, 0x00, 	/*  [   *    **   ]  */
   0x10, 0xC0, 0x00, 	/*  [   *    **   ]  */
   0x20, 0xC0, 0x00, 	/*  [  *     **   ]  */
   0x7F, 0xF0, 0x00, 	/*  [ *********** ]  */
   0x7F, 0xF0, 0x00, 	/*  [ *********** ]  */
   0x00, 0xC0, 0x00, 	/*  [        **   ]  */
   0x00, 0xC0, 0x00, 	/*  [        **   ]  */
   0x00, 0xC0, 0x00, 	/*  [        **   ]  */
   0x01, 0xE0, 0x00, 	/*  [       ****  ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '5' charwidth: 13 */
   0x1F, 0xC0, 0x00, 	/*  [   *******   ]  */
   0x1F, 0xC0, 0x00, 	/*  [   *******   ]  */
   0x10, 0x00, 0x00, 	/*  [   *         ]  */
   0x10, 0x00, 0x00, 	/*  [   *         ]  */
   0x10, 0x00, 0x00, 	/*  [   *         ]  */
   0x10, 0x00, 0x00, 	/*  [   *         ]  */
   0x1F, 0x00, 0x00, 	/*  [   *****     ]  */
   0x01, 0xC0, 0x00, 	/*  [       ***   ]  */
   0x00, 0xC0, 0x00, 	/*  [        **   ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x10, 0xE0, 0x00, 	/*  [   *    ***  ]  */
   0x10, 0xC0, 0x00, 	/*  [   *    **   ]  */
   0x0F, 0x80, 0x00, 	/*  [    *****    ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '6' charwidth: 13 */
   0x0F, 0x80, 0x00, 	/*  [    *****    ]  */
   0x18, 0x40, 0x00, 	/*  [   **    *   ]  */
   0x30, 0x40, 0x00, 	/*  [  **     *   ]  */
   0x30, 0x00, 0x00, 	/*  [  **         ]  */
   0x60, 0x00, 0x00, 	/*  [ **          ]  */
   0x6F, 0x80, 0x00, 	/*  [ ** *****    ]  */
   0x70, 0xC0, 0x00, 	/*  [ ***    **   ]  */
   0x60, 0xE0, 0x00, 	/*  [ **     ***  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x20, 0x60, 0x00, 	/*  [  *      **  ]  */
   0x30, 0x40, 0x00, 	/*  [  **     *   ]  */
   0x18, 0xC0, 0x00, 	/*  [   **   **   ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****     ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '7' charwidth: 13 */
   0x3F, 0xE0, 0x00, 	/*  [  *********  ]  */
   0x3F, 0xE0, 0x00, 	/*  [  *********  ]  */
   0x00, 0x40, 0x00, 	/*  [         *   ]  */
   0x00, 0xC0, 0x00, 	/*  [        **   ]  */
   0x00, 0x80, 0x00, 	/*  [        *    ]  */
   0x01, 0x00, 0x00, 	/*  [       *     ]  */
   0x01, 0x00, 0x00, 	/*  [       *     ]  */
   0x02, 0x00, 0x00, 	/*  [      *      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x04, 0x00, 0x00, 	/*  [     *       ]  */
   0x0C, 0x00, 0x00, 	/*  [    **       ]  */
   0x0C, 0x00, 0x00, 	/*  [    **       ]  */
   0x08, 0x00, 0x00, 	/*  [    *        ]  */
   0x18, 0x00, 0x00, 	/*  [   **        ]  */
   0x18, 0x00, 0x00, 	/*  [   **        ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '8' charwidth: 13 */
   0x0F, 0x80, 0x00, 	/*  [    *****    ]  */
   0x18, 0xC0, 0x00, 	/*  [   **   **   ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x1C, 0xC0, 0x00, 	/*  [   ***  **   ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****     ]  */
   0x1F, 0x80, 0x00, 	/*  [   ******    ]  */
   0x31, 0xC0, 0x00, 	/*  [  **   ***   ]  */
   0x60, 0xE0, 0x00, 	/*  [ **     ***  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **   ]  */
   0x1F, 0x80, 0x00, 	/*  [   ******    ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '9' charwidth: 13 */
   0x0F, 0x00, 0x00, 	/*  [    ****     ]  */
   0x31, 0xC0, 0x00, 	/*  [  **   ***   ]  */
   0x20, 0xC0, 0x00, 	/*  [  *     **   ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0xE0, 0x00, 	/*  [  **    ***  ]  */
   0x1F, 0x60, 0x00, 	/*  [   ***** **  ]  */
   0x00, 0x40, 0x00, 	/*  [         *   ]  */
   0x00, 0xC0, 0x00, 	/*  [        **   ]  */
   0x00, 0xC0, 0x00, 	/*  [        **   ]  */
   0x21, 0x80, 0x00, 	/*  [  *    **    ]  */
   0x3E, 0x00, 0x00, 	/*  [  *****      ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* ':' charwidth: 6 */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   
   /* ';' charwidth: 6 */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x30, 0x00, 0x00, 	/*  [  **  ]  */
   0x10, 0x00, 0x00, 	/*  [   *  ]  */
   0x10, 0x00, 0x00, 	/*  [   *  ]  */
   0x20, 0x00, 0x00, 	/*  [  *   ]  */
   
   /* '<' charwidth: 13 */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x10, 0x00, 	/*  [           * ]  */
   0x00, 0x70, 0x00, 	/*  [         *** ]  */
   0x01, 0xE0, 0x00, 	/*  [       ****  ]  */
   0x07, 0x80, 0x00, 	/*  [     ****    ]  */
   0x1E, 0x00, 0x00, 	/*  [   ****      ]  */
   0x78, 0x00, 0x00, 	/*  [ ****        ]  */
   0x1E, 0x00, 0x00, 	/*  [   ****      ]  */
   0x07, 0x80, 0x00, 	/*  [     ****    ]  */
   0x01, 0xE0, 0x00, 	/*  [       ****  ]  */
   0x00, 0x70, 0x00, 	/*  [         *** ]  */
   0x00, 0x10, 0x00, 	/*  [           * ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '=' charwidth: 13 */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x7F, 0xF0, 0x00, 	/*  [ *********** ]  */
   0x7F, 0xF0, 0x00, 	/*  [ *********** ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x7F, 0xF0, 0x00, 	/*  [ *********** ]  */
   0x7F, 0xF0, 0x00, 	/*  [ *********** ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '>' charwidth: 13 */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x40, 0x00, 0x00, 	/*  [ *           ]  */
   0x70, 0x00, 0x00, 	/*  [ ***         ]  */
   0x3C, 0x00, 0x00, 	/*  [  ****       ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****     ]  */
   0x03, 0xC0, 0x00, 	/*  [      ****   ]  */
   0x00, 0xF0, 0x00, 	/*  [        **** ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****  ]  */
   0x0F, 0x80, 0x00, 	/*  [    *****    ]  */
   0x3E, 0x00, 0x00, 	/*  [  *****      ]  */
   0x70, 0x00, 0x00, 	/*  [ ***         ]  */
   0x40, 0x00, 0x00, 	/*  [ *           ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '?' charwidth: 10 */
   0x3E, 0x00, 0x00, 	/*  [  *****   ]  */
   0x63, 0x00, 0x00, 	/*  [ **   **  ]  */
   0x41, 0x80, 0x00, 	/*  [ *     ** ]  */
   0x01, 0x80, 0x00, 	/*  [       ** ]  */
   0x01, 0x80, 0x00, 	/*  [       ** ]  */
   0x03, 0x00, 0x00, 	/*  [      **  ]  */
   0x02, 0x00, 0x00, 	/*  [      *   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **    ]  */
   0x08, 0x00, 0x00, 	/*  [    *     ]  */
   0x18, 0x00, 0x00, 	/*  [   **     ]  */
   0x18, 0x00, 0x00, 	/*  [   **     ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x18, 0x00, 0x00, 	/*  [   **     ]  */
   0x18, 0x00, 0x00, 	/*  [   **     ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   
   /* '@' charwidth: 18 */
   0x01, 0xF8, 0x00, 	/*  [       ******     ]  */
   0x06, 0x06, 0x00, 	/*  [     **      **   ]  */
   0x0C, 0x03, 0x00, 	/*  [    **        **  ]  */
   0x10, 0xF9, 0x00, 	/*  [   *    *****  *  ]  */
   0x31, 0x98, 0x80, 	/*  [  **   **  **   * ]  */
   0x23, 0x18, 0x80, 	/*  [  *   **   **   * ]  */
   0x42, 0x18, 0x80, 	/*  [ *    *    **   * ]  */
   0x46, 0x30, 0x80, 	/*  [ *   **   **    * ]  */
   0x46, 0x30, 0x80, 	/*  [ *   **   **    * ]  */
   0x46, 0x71, 0x00, 	/*  [ *   **  ***   *  ]  */
   0x46, 0xB2, 0x00, 	/*  [ *   ** * **  *   ]  */
   0x23, 0x3C, 0x00, 	/*  [  *   **  ****    ]  */
   0x20, 0x00, 0x00, 	/*  [  *               ]  */
   0x18, 0x20, 0x00, 	/*  [   **     *       ]  */
   0x07, 0xC0, 0x00, 	/*  [     *****        ]  */
   0x00, 0x00, 0x00, 	/*  [                  ]  */
   0x00, 0x00, 0x00, 	/*  [                  ]  */
   0x00, 0x00, 0x00, 	/*  [                  ]  */
   
   /* 'A' charwidth: 14 */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x03, 0x80, 0x00, 	/*  [      ***     ]  */
   0x05, 0x80, 0x00, 	/*  [     * **     ]  */
   0x05, 0x80, 0x00, 	/*  [     * **     ]  */
   0x08, 0xC0, 0x00, 	/*  [    *   **    ]  */
   0x08, 0xC0, 0x00, 	/*  [    *   **    ]  */
   0x10, 0xE0, 0x00, 	/*  [   *    ***   ]  */
   0x10, 0x60, 0x00, 	/*  [   *     **   ]  */
   0x10, 0x60, 0x00, 	/*  [   *     **   ]  */
   0x3F, 0xF0, 0x00, 	/*  [  **********  ]  */
   0x20, 0x30, 0x00, 	/*  [  *       **  ]  */
   0x40, 0x18, 0x00, 	/*  [ *         ** ]  */
   0x40, 0x18, 0x00, 	/*  [ *         ** ]  */
   0xE0, 0x3C, 0x00, 	/*  [***       ****]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   
   /* 'B' charwidth: 12 */
   0x7F, 0x00, 0x00, 	/*  [ *******    ]  */
   0x31, 0xC0, 0x00, 	/*  [  **   ***  ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **  ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **  ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **  ]  */
   0x30, 0x80, 0x00, 	/*  [  **    *   ]  */
   0x31, 0x80, 0x00, 	/*  [  **   **   ]  */
   0x3E, 0x00, 0x00, 	/*  [  *****     ]  */
   0x31, 0xC0, 0x00, 	/*  [  **   ***  ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     ** ]  */
   0x30, 0x60, 0x00, 	/*  [  **     ** ]  */
   0x30, 0x60, 0x00, 	/*  [  **     ** ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **  ]  */
   0x7F, 0x80, 0x00, 	/*  [ ********   ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   
   /* 'C' charwidth: 14 */
   0x07, 0xF0, 0x00, 	/*  [     *******  ]  */
   0x1C, 0x18, 0x00, 	/*  [   ***     ** ]  */
   0x38, 0x08, 0x00, 	/*  [  ***       * ]  */
   0x30, 0x00, 0x00, 	/*  [  **          ]  */
   0x70, 0x00, 0x00, 	/*  [ ***          ]  */
   0x60, 0x00, 0x00, 	/*  [ **           ]  */
   0x60, 0x00, 0x00, 	/*  [ **           ]  */
   0x60, 0x00, 0x00, 	/*  [ **           ]  */
   0x60, 0x00, 0x00, 	/*  [ **           ]  */
   0x60, 0x00, 0x00, 	/*  [ **           ]  */
   0x70, 0x00, 0x00, 	/*  [ ***          ]  */
   0x30, 0x00, 0x00, 	/*  [  **          ]  */
   0x18, 0x00, 0x00, 	/*  [   **         ]  */
   0x0C, 0x08, 0x00, 	/*  [    **      * ]  */
   0x07, 0xF0, 0x00, 	/*  [     *******  ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   
   /* 'D' charwidth: 16 */
   0x7F, 0xC0, 0x00, 	/*  [ *********      ]  */
   0x30, 0x70, 0x00, 	/*  [  **     ***    ]  */
   0x30, 0x38, 0x00, 	/*  [  **      ***   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x0C, 0x00, 	/*  [  **        **  ]  */
   0x30, 0x0C, 0x00, 	/*  [  **        **  ]  */
   0x30, 0x0C, 0x00, 	/*  [  **        **  ]  */
   0x30, 0x0C, 0x00, 	/*  [  **        **  ]  */
   0x30, 0x0C, 0x00, 	/*  [  **        **  ]  */
   0x30, 0x0C, 0x00, 	/*  [  **        **  ]  */
   0x30, 0x1C, 0x00, 	/*  [  **       ***  ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x38, 0x00, 	/*  [  **      ***   ]  */
   0x30, 0x70, 0x00, 	/*  [  **     ***    ]  */
   0x7F, 0xC0, 0x00, 	/*  [ *********      ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   
   /* 'E' charwidth: 12 */
   0x7F, 0xC0, 0x00, 	/*  [ *********  ]  */
   0x30, 0x40, 0x00, 	/*  [  **     *  ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x31, 0x00, 0x00, 	/*  [  **   *    ]  */
   0x3F, 0x00, 0x00, 	/*  [  ******    ]  */
   0x31, 0x00, 0x00, 	/*  [  **   *    ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0x40, 0x00, 	/*  [  **     *  ]  */
   0x30, 0x40, 0x00, 	/*  [  **     *  ]  */
   0x7F, 0xC0, 0x00, 	/*  [ *********  ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   
   /* 'F' charwidth: 11 */
   0x7F, 0xC0, 0x00, 	/*  [ ********* ]  */
   0x30, 0x40, 0x00, 	/*  [  **     * ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x80, 0x00, 	/*  [  **    *  ]  */
   0x3F, 0x80, 0x00, 	/*  [  *******  ]  */
   0x30, 0x80, 0x00, 	/*  [  **    *  ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x78, 0x00, 0x00, 	/*  [ ****      ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   
   /* 'G' charwidth: 15 */
   0x07, 0xF0, 0x00, 	/*  [     *******   ]  */
   0x1C, 0x18, 0x00, 	/*  [   ***     **  ]  */
   0x38, 0x08, 0x00, 	/*  [  ***       *  ]  */
   0x30, 0x00, 0x00, 	/*  [  **           ]  */
   0x70, 0x00, 0x00, 	/*  [ ***           ]  */
   0x60, 0x00, 0x00, 	/*  [ **            ]  */
   0x60, 0x00, 0x00, 	/*  [ **            ]  */
   0x60, 0x00, 0x00, 	/*  [ **            ]  */
   0x60, 0x7C, 0x00, 	/*  [ **      ***** ]  */
   0x60, 0x18, 0x00, 	/*  [ **        **  ]  */
   0x70, 0x18, 0x00, 	/*  [ ***       **  ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **  ]  */
   0x18, 0x18, 0x00, 	/*  [   **      **  ]  */
   0x0C, 0x18, 0x00, 	/*  [    **     **  ]  */
   0x07, 0xF0, 0x00, 	/*  [     *******   ]  */
   0x00, 0x00, 0x00, 	/*  [               ]  */
   0x00, 0x00, 0x00, 	/*  [               ]  */
   0x00, 0x00, 0x00, 	/*  [               ]  */
   
   /* 'H' charwidth: 16 */
   0x78, 0x3C, 0x00, 	/*  [ ****     ****  ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x3F, 0xF8, 0x00, 	/*  [  ***********   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x78, 0x3C, 0x00, 	/*  [ ****     ****  ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   
   /* 'I' charwidth: 7 */
   0x78, 0x00, 0x00, 	/*  [ ****  ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x78, 0x00, 0x00, 	/*  [ ****  ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   
   /* 'J' charwidth: 9 */
   0x3E, 0x00, 0x00, 	/*  [  *****  ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x0C, 0x00, 0x00, 	/*  [    **   ]  */
   0x8C, 0x00, 0x00, 	/*  [*   **   ]  */
   0x98, 0x00, 0x00, 	/*  [*  **    ]  */
   0xF0, 0x00, 0x00, 	/*  [****     ]  */
   
   /* 'K' charwidth: 14 */
   0x78, 0xF0, 0x00, 	/*  [ ****   ****  ]  */
   0x30, 0x40, 0x00, 	/*  [  **     *    ]  */
   0x30, 0x80, 0x00, 	/*  [  **    *     ]  */
   0x30, 0x80, 0x00, 	/*  [  **    *     ]  */
   0x31, 0x00, 0x00, 	/*  [  **   *      ]  */
   0x32, 0x00, 0x00, 	/*  [  **  *       ]  */
   0x34, 0x00, 0x00, 	/*  [  ** *        ]  */
   0x3C, 0x00, 0x00, 	/*  [  ****        ]  */
   0x36, 0x00, 0x00, 	/*  [  ** **       ]  */
   0x33, 0x00, 0x00, 	/*  [  **  **      ]  */
   0x31, 0x80, 0x00, 	/*  [  **   **     ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **    ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **   ]  */
   0x30, 0x30, 0x00, 	/*  [  **      **  ]  */
   0x78, 0xF8, 0x00, 	/*  [ ****   ***** ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   
   /* 'L' charwidth: 11 */
   0x78, 0x00, 0x00, 	/*  [ ****      ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x40, 0x00, 	/*  [  **     * ]  */
   0x30, 0x40, 0x00, 	/*  [  **     * ]  */
   0x7F, 0xC0, 0x00, 	/*  [ ********* ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   
   /* 'M' charwidth: 17 */
   0x70, 0x07, 0x00, 	/*  [ ***         *** ]  */
   0x30, 0x06, 0x00, 	/*  [  **         **  ]  */
   0x38, 0x0E, 0x00, 	/*  [  ***       ***  ]  */
   0x38, 0x0E, 0x00, 	/*  [  ***       ***  ]  */
   0x3C, 0x16, 0x00, 	/*  [  ****     * **  ]  */
   0x2C, 0x16, 0x00, 	/*  [  * **     * **  ]  */
   0x2C, 0x36, 0x00, 	/*  [  * **    ** **  ]  */
   0x26, 0x26, 0x00, 	/*  [  *  **   *  **  ]  */
   0x26, 0x26, 0x00, 	/*  [  *  **   *  **  ]  */
   0x23, 0x46, 0x00, 	/*  [  *   ** *   **  ]  */
   0x23, 0x46, 0x00, 	/*  [  *   ** *   **  ]  */
   0x23, 0x86, 0x00, 	/*  [  *   ***    **  ]  */
   0x21, 0x86, 0x00, 	/*  [  *    **    **  ]  */
   0x21, 0x86, 0x00, 	/*  [  *    **    **  ]  */
   0x70, 0x0F, 0x00, 	/*  [ ***        **** ]  */
   0x00, 0x00, 0x00, 	/*  [                 ]  */
   0x00, 0x00, 0x00, 	/*  [                 ]  */
   0x00, 0x00, 0x00, 	/*  [                 ]  */
   
   /* 'N' charwidth: 15 */
   0x70, 0x1C, 0x00, 	/*  [ ***       *** ]  */
   0x30, 0x08, 0x00, 	/*  [  **        *  ]  */
   0x38, 0x08, 0x00, 	/*  [  ***       *  ]  */
   0x2C, 0x08, 0x00, 	/*  [  * **      *  ]  */
   0x2E, 0x08, 0x00, 	/*  [  * ***     *  ]  */
   0x26, 0x08, 0x00, 	/*  [  *  **     *  ]  */
   0x23, 0x08, 0x00, 	/*  [  *   **    *  ]  */
   0x21, 0x88, 0x00, 	/*  [  *    **   *  ]  */
   0x21, 0x88, 0x00, 	/*  [  *    **   *  ]  */
   0x20, 0xC8, 0x00, 	/*  [  *     **  *  ]  */
   0x20, 0x68, 0x00, 	/*  [  *      ** *  ]  */
   0x20, 0x78, 0x00, 	/*  [  *      ****  ]  */
   0x20, 0x38, 0x00, 	/*  [  *       ***  ]  */
   0x20, 0x18, 0x00, 	/*  [  *        **  ]  */
   0x70, 0x08, 0x00, 	/*  [ ***        *  ]  */
   0x00, 0x00, 0x00, 	/*  [               ]  */
   0x00, 0x00, 0x00, 	/*  [               ]  */
   0x00, 0x00, 0x00, 	/*  [               ]  */
   
   /* 'O' charwidth: 16 */
   0x07, 0xC0, 0x00, 	/*  [     *****      ]  */
   0x1C, 0x70, 0x00, 	/*  [   ***   ***    ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x1C, 0x70, 0x00, 	/*  [   ***   ***    ]  */
   0x07, 0xC0, 0x00, 	/*  [     *****      ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   
   /* 'P' charwidth: 12 */
   0x7F, 0x80, 0x00, 	/*  [ ********   ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     ** ]  */
   0x30, 0x60, 0x00, 	/*  [  **     ** ]  */
   0x30, 0x60, 0x00, 	/*  [  **     ** ]  */
   0x30, 0x60, 0x00, 	/*  [  **     ** ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **  ]  */
   0x31, 0xC0, 0x00, 	/*  [  **   ***  ]  */
   0x3F, 0x00, 0x00, 	/*  [  ******    ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x78, 0x00, 0x00, 	/*  [ ****       ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   
   /* 'Q' charwidth: 16 */
   0x07, 0xC0, 0x00, 	/*  [     *****      ]  */
   0x1C, 0x70, 0x00, 	/*  [   ***   ***    ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x60, 0x0C, 0x00, 	/*  [ **         **  ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x30, 0x18, 0x00, 	/*  [  **       **   ]  */
   0x1C, 0x70, 0x00, 	/*  [   ***   ***    ]  */
   0x07, 0xC0, 0x00, 	/*  [     *****      ]  */
   0x00, 0x30, 0x00, 	/*  [          **    ]  */
   0x00, 0x1F, 0x00, 	/*  [           *****]  */
   0x00, 0x07, 0x00, 	/*  [             ***]  */
   
   /* 'R' charwidth: 14 */
   0x7F, 0x80, 0x00, 	/*  [ ********     ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **    ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **   ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **   ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **   ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **   ]  */
   0x31, 0xC0, 0x00, 	/*  [  **   ***    ]  */
   0x3F, 0x00, 0x00, 	/*  [  ******      ]  */
   0x33, 0x00, 0x00, 	/*  [  **  **      ]  */
   0x31, 0x80, 0x00, 	/*  [  **   **     ]  */
   0x31, 0xC0, 0x00, 	/*  [  **   ***    ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **    ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **   ]  */
   0x30, 0x70, 0x00, 	/*  [  **     ***  ]  */
   0x78, 0x38, 0x00, 	/*  [ ****     *** ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   
   /* 'S' charwidth: 11 */
   0x1F, 0x80, 0x00, 	/*  [   ******  ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    ** ]  */
   0x60, 0x40, 0x00, 	/*  [ **      * ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x70, 0x00, 0x00, 	/*  [ ***       ]  */
   0x3C, 0x00, 0x00, 	/*  [  ****     ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****   ]  */
   0x03, 0x80, 0x00, 	/*  [      ***  ]  */
   0x01, 0xC0, 0x00, 	/*  [       *** ]  */
   0x00, 0xC0, 0x00, 	/*  [        ** ]  */
   0x00, 0xC0, 0x00, 	/*  [        ** ]  */
   0x40, 0xC0, 0x00, 	/*  [ *      ** ]  */
   0x61, 0x80, 0x00, 	/*  [ **    **  ]  */
   0x3F, 0x00, 0x00, 	/*  [  ******   ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   
   /* 'T' charwidth: 14 */
   0x7F, 0xF8, 0x00, 	/*  [ ************ ]  */
   0x43, 0x08, 0x00, 	/*  [ *    **    * ]  */
   0x43, 0x08, 0x00, 	/*  [ *    **    * ]  */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x03, 0x00, 0x00, 	/*  [      **      ]  */
   0x07, 0x80, 0x00, 	/*  [     ****     ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   
   /* 'U' charwidth: 15 */
   0x78, 0x38, 0x00, 	/*  [ ****     ***  ]  */
   0x30, 0x10, 0x00, 	/*  [  **       *   ]  */
   0x30, 0x10, 0x00, 	/*  [  **       *   ]  */
   0x30, 0x10, 0x00, 	/*  [  **       *   ]  */
   0x30, 0x10, 0x00, 	/*  [  **       *   ]  */
   0x30, 0x10, 0x00, 	/*  [  **       *   ]  */
   0x30, 0x10, 0x00, 	/*  [  **       *   ]  */
   0x30, 0x10, 0x00, 	/*  [  **       *   ]  */
   0x30, 0x10, 0x00, 	/*  [  **       *   ]  */
   0x30, 0x10, 0x00, 	/*  [  **       *   ]  */
   0x30, 0x10, 0x00, 	/*  [  **       *   ]  */
   0x30, 0x10, 0x00, 	/*  [  **       *   ]  */
   0x18, 0x20, 0x00, 	/*  [   **     *    ]  */
   0x1C, 0x60, 0x00, 	/*  [   ***   **    ]  */
   0x0F, 0xC0, 0x00, 	/*  [    ******     ]  */
   0x00, 0x00, 0x00, 	/*  [               ]  */
   0x00, 0x00, 0x00, 	/*  [               ]  */
   0x00, 0x00, 0x00, 	/*  [               ]  */
   
   /* 'V' charwidth: 14 */
   0x78, 0x1C, 0x00, 	/*  [ ****      ***]  */
   0x30, 0x08, 0x00, 	/*  [  **        * ]  */
   0x30, 0x08, 0x00, 	/*  [  **        * ]  */
   0x18, 0x10, 0x00, 	/*  [   **      *  ]  */
   0x18, 0x10, 0x00, 	/*  [   **      *  ]  */
   0x18, 0x10, 0x00, 	/*  [   **      *  ]  */
   0x0C, 0x20, 0x00, 	/*  [    **    *   ]  */
   0x0C, 0x20, 0x00, 	/*  [    **    *   ]  */
   0x06, 0x40, 0x00, 	/*  [     **  *    ]  */
   0x06, 0x40, 0x00, 	/*  [     **  *    ]  */
   0x06, 0x40, 0x00, 	/*  [     **  *    ]  */
   0x03, 0x80, 0x00, 	/*  [      ***     ]  */
   0x03, 0x80, 0x00, 	/*  [      ***     ]  */
   0x03, 0x80, 0x00, 	/*  [      ***     ]  */
   0x01, 0x00, 0x00, 	/*  [       *      ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   0x00, 0x00, 0x00, 	/*  [              ]  */
   
   /* 'W' charwidth: 18 */
   0x78, 0x61, 0xC0, 	/*  [ ****    **    ***]  */
   0x30, 0x60, 0x80, 	/*  [  **     **     * ]  */
   0x30, 0x60, 0x80, 	/*  [  **     **     * ]  */
   0x30, 0xE0, 0x80, 	/*  [  **    ***     * ]  */
   0x38, 0xB1, 0x00, 	/*  [  ***   * **   *  ]  */
   0x18, 0xB1, 0x00, 	/*  [   **   * **   *  ]  */
   0x19, 0x31, 0x00, 	/*  [   **  *  **   *  ]  */
   0x19, 0x39, 0x00, 	/*  [   **  *  ***  *  ]  */
   0x19, 0x1A, 0x00, 	/*  [   **  *   ** *   ]  */
   0x0D, 0x1A, 0x00, 	/*  [    ** *   ** *   ]  */
   0x0E, 0x1A, 0x00, 	/*  [    ***    ** *   ]  */
   0x0E, 0x0E, 0x00, 	/*  [    ***     ***   ]  */
   0x0E, 0x0C, 0x00, 	/*  [    ***     **    ]  */
   0x04, 0x0C, 0x00, 	/*  [     *      **    ]  */
   0x04, 0x04, 0x00, 	/*  [     *       *    ]  */
   0x00, 0x00, 0x00, 	/*  [                  ]  */
   0x00, 0x00, 0x00, 	/*  [                  ]  */
   0x00, 0x00, 0x00, 	/*  [                  ]  */
   
   /* 'X' charwidth: 13 */
   0xF8, 0x70, 0x00, 	/*  [*****    *** ]  */
   0x60, 0x20, 0x00, 	/*  [ **       *  ]  */
   0x30, 0x40, 0x00, 	/*  [  **     *   ]  */
   0x38, 0x80, 0x00, 	/*  [  ***   *    ]  */
   0x18, 0x80, 0x00, 	/*  [   **   *    ]  */
   0x0D, 0x00, 0x00, 	/*  [    ** *     ]  */
   0x0E, 0x00, 0x00, 	/*  [    ***      ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x07, 0x00, 0x00, 	/*  [     ***     ]  */
   0x0B, 0x00, 0x00, 	/*  [    * **     ]  */
   0x11, 0x80, 0x00, 	/*  [   *   **    ]  */
   0x11, 0xC0, 0x00, 	/*  [   *   ***   ]  */
   0x20, 0xC0, 0x00, 	/*  [  *     **   ]  */
   0x40, 0x60, 0x00, 	/*  [ *       **  ]  */
   0xE1, 0xF0, 0x00, 	/*  [***    ***** ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* 'Y' charwidth: 12 */
   0xF0, 0x70, 0x00, 	/*  [****     ***]  */
   0x60, 0x20, 0x00, 	/*  [ **       * ]  */
   0x30, 0x40, 0x00, 	/*  [  **     *  ]  */
   0x38, 0x40, 0x00, 	/*  [  ***    *  ]  */
   0x18, 0x80, 0x00, 	/*  [   **   *   ]  */
   0x1C, 0x80, 0x00, 	/*  [   ***  *   ]  */
   0x0D, 0x00, 0x00, 	/*  [    ** *    ]  */
   0x06, 0x00, 0x00, 	/*  [     **     ]  */
   0x06, 0x00, 0x00, 	/*  [     **     ]  */
   0x06, 0x00, 0x00, 	/*  [     **     ]  */
   0x06, 0x00, 0x00, 	/*  [     **     ]  */
   0x06, 0x00, 0x00, 	/*  [     **     ]  */
   0x06, 0x00, 0x00, 	/*  [     **     ]  */
   0x06, 0x00, 0x00, 	/*  [     **     ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****    ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   
   /* 'Z' charwidth: 13 */
   0x7F, 0xE0, 0x00, 	/*  [ **********  ]  */
   0x40, 0xE0, 0x00, 	/*  [ *      ***  ]  */
   0x00, 0xC0, 0x00, 	/*  [        **   ]  */
   0x01, 0xC0, 0x00, 	/*  [       ***   ]  */
   0x01, 0x80, 0x00, 	/*  [       **    ]  */
   0x03, 0x00, 0x00, 	/*  [      **     ]  */
   0x03, 0x00, 0x00, 	/*  [      **     ]  */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x0E, 0x00, 0x00, 	/*  [    ***      ]  */
   0x0C, 0x00, 0x00, 	/*  [    **       ]  */
   0x18, 0x00, 0x00, 	/*  [   **        ]  */
   0x18, 0x00, 0x00, 	/*  [   **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **         ]  */
   0x70, 0x20, 0x00, 	/*  [ ***      *  ]  */
   0x7F, 0xE0, 0x00, 	/*  [ **********  ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '[' charwidth: 7 */
   0x3C, 0x00, 0x00, 	/*  [  **** ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x3C, 0x00, 0x00, 	/*  [  **** ]  */
   
   /* '\' charwidth: 11 */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x38, 0x00, 0x00, 	/*  [  ***      ]  */
   0x18, 0x00, 0x00, 	/*  [   **      ]  */
   0x18, 0x00, 0x00, 	/*  [   **      ]  */
   0x0C, 0x00, 0x00, 	/*  [    **     ]  */
   0x0C, 0x00, 0x00, 	/*  [    **     ]  */
   0x06, 0x00, 0x00, 	/*  [     **    ]  */
   0x06, 0x00, 0x00, 	/*  [     **    ]  */
   0x03, 0x00, 0x00, 	/*  [      **   ]  */
   0x03, 0x00, 0x00, 	/*  [      **   ]  */
   0x03, 0x80, 0x00, 	/*  [      ***  ]  */
   0x01, 0x80, 0x00, 	/*  [       **  ]  */
   0x01, 0x80, 0x00, 	/*  [       **  ]  */
   0x00, 0xC0, 0x00, 	/*  [        ** ]  */
   0x00, 0xC0, 0x00, 	/*  [        ** ]  */
   
   /* ']' charwidth: 7 */
   0x78, 0x00, 0x00, 	/*  [ ****  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x78, 0x00, 0x00, 	/*  [ ****  ]  */
   
   /* '^' charwidth: 13 */
   0x03, 0x00, 0x00, 	/*  [      **     ]  */
   0x03, 0x00, 0x00, 	/*  [      **     ]  */
   0x07, 0x80, 0x00, 	/*  [     ****    ]  */
   0x07, 0x80, 0x00, 	/*  [     ****    ]  */
   0x0C, 0xC0, 0x00, 	/*  [    **  **   ]  */
   0x0C, 0xC0, 0x00, 	/*  [    **  **   ]  */
   0x18, 0x60, 0x00, 	/*  [   **    **  ]  */
   0x18, 0x60, 0x00, 	/*  [   **    **  ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* '_' charwidth: 10 */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x7F, 0x80, 0x00, 	/*  [ ******** ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   0x00, 0x00, 0x00, 	/*  [          ]  */
   
   /* '`' charwidth: 13 */
   0x06, 0x00, 0x00, 	/*  [     **      ]  */
   0x03, 0x00, 0x00, 	/*  [      **     ]  */
   0x01, 0x80, 0x00, 	/*  [       **    ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* 'a' charwidth: 11 */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x1F, 0x00, 0x00, 	/*  [   *****   ]  */
   0x23, 0x80, 0x00, 	/*  [  *   ***  ]  */
   0x21, 0x80, 0x00, 	/*  [  *    **  ]  */
   0x01, 0x80, 0x00, 	/*  [       **  ]  */
   0x01, 0x80, 0x00, 	/*  [       **  ]  */
   0x1F, 0x80, 0x00, 	/*  [   ******  ]  */
   0x31, 0x80, 0x00, 	/*  [  **   **  ]  */
   0x61, 0x80, 0x00, 	/*  [ **    **  ]  */
   0x61, 0x80, 0x00, 	/*  [ **    **  ]  */
   0x73, 0x80, 0x00, 	/*  [ ***  ***  ]  */
   0x3C, 0xE0, 0x00, 	/*  [  ****  ***]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   
   /* 'b' charwidth: 13 */
   0x70, 0x00, 0x00, 	/*  [ ***         ]  */
   0x30, 0x00, 0x00, 	/*  [  **         ]  */
   0x30, 0x00, 0x00, 	/*  [  **         ]  */
   0x30, 0x00, 0x00, 	/*  [  **         ]  */
   0x37, 0xC0, 0x00, 	/*  [  ** *****   ]  */
   0x38, 0x60, 0x00, 	/*  [  ***    **  ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x38, 0x60, 0x00, 	/*  [  ***    **  ]  */
   0x37, 0x80, 0x00, 	/*  [  ** ****    ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* 'c' charwidth: 11 */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****   ]  */
   0x38, 0x80, 0x00, 	/*  [  ***   *  ]  */
   0x30, 0x80, 0x00, 	/*  [  **    *  ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x38, 0x80, 0x00, 	/*  [  ***   *  ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****   ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   
   /* 'd' charwidth: 13 */
   0x00, 0xE0, 0x00, 	/*  [        ***  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x0F, 0x60, 0x00, 	/*  [    **** **  ]  */
   0x30, 0xE0, 0x00, 	/*  [  **    ***  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x30, 0xE0, 0x00, 	/*  [  **    ***  ]  */
   0x1F, 0x70, 0x00, 	/*  [   ***** *** ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* 'e' charwidth: 11 */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****   ]  */
   0x31, 0x80, 0x00, 	/*  [  **   **  ]  */
   0x20, 0xC0, 0x00, 	/*  [  *     ** ]  */
   0x60, 0xC0, 0x00, 	/*  [ **     ** ]  */
   0x7F, 0xC0, 0x00, 	/*  [ ********* ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x60, 0x00, 0x00, 	/*  [ **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x18, 0x40, 0x00, 	/*  [   **    * ]  */
   0x0F, 0x80, 0x00, 	/*  [    *****  ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   
   /* 'f' charwidth: 8 */
   0x0F, 0x00, 0x00, 	/*  [    ****]  */
   0x19, 0x00, 0x00, 	/*  [   **  *]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x7F, 0x00, 0x00, 	/*  [ *******]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x3C, 0x00, 0x00, 	/*  [  ****  ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   
   /* 'g' charwidth: 12 */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x0F, 0xF0, 0x00, 	/*  [    ********]  */
   0x19, 0x80, 0x00, 	/*  [   **  **   ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **  ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **  ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **  ]  */
   0x19, 0x80, 0x00, 	/*  [   **  **   ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****    ]  */
   0x10, 0x00, 0x00, 	/*  [   *        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x3F, 0xC0, 0x00, 	/*  [  ********  ]  */
   0x1F, 0xE0, 0x00, 	/*  [   ******** ]  */
   0x20, 0x60, 0x00, 	/*  [  *      ** ]  */
   0x60, 0xC0, 0x00, 	/*  [ **     **  ]  */
   0x3F, 0x80, 0x00, 	/*  [  *******   ]  */
   
   /* 'h' charwidth: 13 */
   0x70, 0x00, 0x00, 	/*  [ ***         ]  */
   0x30, 0x00, 0x00, 	/*  [  **         ]  */
   0x30, 0x00, 0x00, 	/*  [  **         ]  */
   0x30, 0x00, 0x00, 	/*  [  **         ]  */
   0x37, 0xC0, 0x00, 	/*  [  ** *****   ]  */
   0x38, 0x60, 0x00, 	/*  [  ***    **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x78, 0xF0, 0x00, 	/*  [ ****   **** ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* 'i' charwidth: 7 */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x70, 0x00, 0x00, 	/*  [ ***   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x78, 0x00, 0x00, 	/*  [ ****  ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   
   /* 'j' charwidth: 7 */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x38, 0x00, 0x00, 	/*  [  ***  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x10, 0x00, 0x00, 	/*  [   *   ]  */
   0xE0, 0x00, 0x00, 	/*  [***    ]  */
   
   /* 'k' charwidth: 12 */
   0x70, 0x00, 0x00, 	/*  [ ***        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0x00, 0x00, 	/*  [  **        ]  */
   0x30, 0xE0, 0x00, 	/*  [  **    *** ]  */
   0x30, 0x80, 0x00, 	/*  [  **    *   ]  */
   0x31, 0x00, 0x00, 	/*  [  **   *    ]  */
   0x32, 0x00, 0x00, 	/*  [  **  *     ]  */
   0x34, 0x00, 0x00, 	/*  [  ** *      ]  */
   0x3E, 0x00, 0x00, 	/*  [  *****     ]  */
   0x36, 0x00, 0x00, 	/*  [  ** **     ]  */
   0x33, 0x00, 0x00, 	/*  [  **  **    ]  */
   0x31, 0x80, 0x00, 	/*  [  **   **   ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **  ]  */
   0x78, 0x70, 0x00, 	/*  [ ****    ***]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   
   /* 'l' charwidth: 7 */
   0x70, 0x00, 0x00, 	/*  [ ***   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x78, 0x00, 0x00, 	/*  [ ****  ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   
   /* 'm' charwidth: 19 */
   0x00, 0x00, 0x00, 	/*  [                   ]  */
   0x00, 0x00, 0x00, 	/*  [                   ]  */
   0x00, 0x00, 0x00, 	/*  [                   ]  */
   0x00, 0x00, 0x00, 	/*  [                   ]  */
   0x77, 0xCF, 0x80, 	/*  [ *** *****  *****  ]  */
   0x38, 0x70, 0xC0, 	/*  [  ***    ***    ** ]  */
   0x30, 0x60, 0xC0, 	/*  [  **     **     ** ]  */
   0x30, 0x60, 0xC0, 	/*  [  **     **     ** ]  */
   0x30, 0x60, 0xC0, 	/*  [  **     **     ** ]  */
   0x30, 0x60, 0xC0, 	/*  [  **     **     ** ]  */
   0x30, 0x60, 0xC0, 	/*  [  **     **     ** ]  */
   0x30, 0x60, 0xC0, 	/*  [  **     **     ** ]  */
   0x30, 0x60, 0xC0, 	/*  [  **     **     ** ]  */
   0x30, 0x60, 0xC0, 	/*  [  **     **     ** ]  */
   0x78, 0xF1, 0xE0, 	/*  [ ****   ****   ****]  */
   0x00, 0x00, 0x00, 	/*  [                   ]  */
   0x00, 0x00, 0x00, 	/*  [                   ]  */
   0x00, 0x00, 0x00, 	/*  [                   ]  */
   
   /* 'n' charwidth: 13 */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x77, 0xC0, 0x00, 	/*  [ *** *****   ]  */
   0x38, 0x60, 0x00, 	/*  [  ***    **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x78, 0xF0, 0x00, 	/*  [ ****   **** ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* 'o' charwidth: 13 */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****     ]  */
   0x39, 0xC0, 0x00, 	/*  [  ***  ***   ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **   ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x30, 0xC0, 0x00, 	/*  [  **    **   ]  */
   0x39, 0xC0, 0x00, 	/*  [  ***  ***   ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****     ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* 'p' charwidth: 13 */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x77, 0xC0, 0x00, 	/*  [ *** *****   ]  */
   0x38, 0x60, 0x00, 	/*  [  ***    **  ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x30, 0x30, 0x00, 	/*  [  **      ** ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x38, 0x60, 0x00, 	/*  [  ***    **  ]  */
   0x37, 0x80, 0x00, 	/*  [  ** ****    ]  */
   0x30, 0x00, 0x00, 	/*  [  **         ]  */
   0x30, 0x00, 0x00, 	/*  [  **         ]  */
   0x30, 0x00, 0x00, 	/*  [  **         ]  */
   0x78, 0x00, 0x00, 	/*  [ ****        ]  */
   
   /* 'q' charwidth: 13 */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x0F, 0x60, 0x00, 	/*  [    **** **  ]  */
   0x30, 0xE0, 0x00, 	/*  [  **    ***  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x60, 0x60, 0x00, 	/*  [ **      **  ]  */
   0x30, 0xE0, 0x00, 	/*  [  **    ***  ]  */
   0x1F, 0x60, 0x00, 	/*  [   ***** **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0x60, 0x00, 	/*  [         **  ]  */
   0x00, 0xF0, 0x00, 	/*  [        **** ]  */
   
   /* 'r' charwidth: 9 */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x73, 0x00, 0x00, 	/*  [ ***  ** ]  */
   0x37, 0x00, 0x00, 	/*  [  ** *** ]  */
   0x39, 0x00, 0x00, 	/*  [  ***  * ]  */
   0x30, 0x00, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 0x00, 	/*  [  **     ]  */
   0x30, 0x00, 0x00, 	/*  [  **     ]  */
   0x78, 0x00, 0x00, 	/*  [ ****    ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   
   /* 's' charwidth: 11 */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****   ]  */
   0x30, 0x80, 0x00, 	/*  [  **    *  ]  */
   0x30, 0x80, 0x00, 	/*  [  **    *  ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x1C, 0x00, 0x00, 	/*  [   ***     ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****   ]  */
   0x03, 0xC0, 0x00, 	/*  [      **** ]  */
   0x00, 0xC0, 0x00, 	/*  [        ** ]  */
   0x20, 0xC0, 0x00, 	/*  [  *     ** ]  */
   0x31, 0x80, 0x00, 	/*  [  **   **  ]  */
   0x1F, 0x00, 0x00, 	/*  [   *****   ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   
   /* 't' charwidth: 8 */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x7E, 0x00, 0x00, 	/*  [ ****** ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x1E, 0x00, 0x00, 	/*  [   **** ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   
   /* 'u' charwidth: 13 */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x70, 0xE0, 0x00, 	/*  [ ***    ***  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0x60, 0x00, 	/*  [  **     **  ]  */
   0x30, 0xE0, 0x00, 	/*  [  **    ***  ]  */
   0x1F, 0x70, 0x00, 	/*  [   ***** *** ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   
   /* 'v' charwidth: 12 */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x78, 0x70, 0x00, 	/*  [ ****    ***]  */
   0x30, 0x20, 0x00, 	/*  [  **      * ]  */
   0x30, 0x20, 0x00, 	/*  [  **      * ]  */
   0x18, 0x40, 0x00, 	/*  [   **    *  ]  */
   0x18, 0x40, 0x00, 	/*  [   **    *  ]  */
   0x1C, 0x80, 0x00, 	/*  [   ***  *   ]  */
   0x0C, 0x80, 0x00, 	/*  [    **  *   ]  */
   0x0D, 0x00, 0x00, 	/*  [    ** *    ]  */
   0x07, 0x00, 0x00, 	/*  [     ***    ]  */
   0x06, 0x00, 0x00, 	/*  [     **     ]  */
   0x02, 0x00, 0x00, 	/*  [      *     ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   
   /* 'w' charwidth: 17 */
   0x00, 0x00, 0x00, 	/*  [                 ]  */
   0x00, 0x00, 0x00, 	/*  [                 ]  */
   0x00, 0x00, 0x00, 	/*  [                 ]  */
   0x00, 0x00, 0x00, 	/*  [                 ]  */
   0x78, 0xC7, 0x00, 	/*  [ ****   **   *** ]  */
   0x30, 0xC2, 0x00, 	/*  [  **    **    *  ]  */
   0x30, 0xC2, 0x00, 	/*  [  **    **    *  ]  */
   0x31, 0x62, 0x00, 	/*  [  **   * **   *  ]  */
   0x39, 0x62, 0x00, 	/*  [  ***  * **   *  ]  */
   0x1B, 0x74, 0x00, 	/*  [   ** ** *** *   ]  */
   0x1A, 0x34, 0x00, 	/*  [   ** *   ** *   ]  */
   0x1A, 0x34, 0x00, 	/*  [   ** *   ** *   ]  */
   0x0C, 0x18, 0x00, 	/*  [    **     **    ]  */
   0x0C, 0x18, 0x00, 	/*  [    **     **    ]  */
   0x0C, 0x18, 0x00, 	/*  [    **     **    ]  */
   0x00, 0x00, 0x00, 	/*  [                 ]  */
   0x00, 0x00, 0x00, 	/*  [                 ]  */
   0x00, 0x00, 0x00, 	/*  [                 ]  */
   
   /* 'x' charwidth: 12 */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x78, 0xE0, 0x00, 	/*  [ ****   *** ]  */
   0x30, 0x40, 0x00, 	/*  [  **     *  ]  */
   0x18, 0x80, 0x00, 	/*  [   **   *   ]  */
   0x1D, 0x00, 0x00, 	/*  [   *** *    ]  */
   0x0E, 0x00, 0x00, 	/*  [    ***     ]  */
   0x06, 0x00, 0x00, 	/*  [     **     ]  */
   0x07, 0x00, 0x00, 	/*  [     ***    ]  */
   0x0B, 0x80, 0x00, 	/*  [    * ***   ]  */
   0x11, 0x80, 0x00, 	/*  [   *   **   ]  */
   0x20, 0xC0, 0x00, 	/*  [  *     **  ]  */
   0x71, 0xE0, 0x00, 	/*  [ ***   **** ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   
   /* 'y' charwidth: 12 */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x78, 0x70, 0x00, 	/*  [ ****    ***]  */
   0x30, 0x20, 0x00, 	/*  [  **      * ]  */
   0x30, 0x20, 0x00, 	/*  [  **      * ]  */
   0x18, 0x40, 0x00, 	/*  [   **    *  ]  */
   0x18, 0x40, 0x00, 	/*  [   **    *  ]  */
   0x1C, 0x80, 0x00, 	/*  [   ***  *   ]  */
   0x0C, 0x80, 0x00, 	/*  [    **  *   ]  */
   0x0D, 0x00, 0x00, 	/*  [    ** *    ]  */
   0x07, 0x00, 0x00, 	/*  [     ***    ]  */
   0x06, 0x00, 0x00, 	/*  [     **     ]  */
   0x02, 0x00, 0x00, 	/*  [      *     ]  */
   0x04, 0x00, 0x00, 	/*  [     *      ]  */
   0x0C, 0x00, 0x00, 	/*  [    **      ]  */
   0x18, 0x00, 0x00, 	/*  [   **       ]  */
   
   /* 'z' charwidth: 12 */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x3F, 0xE0, 0x00, 	/*  [  ********* ]  */
   0x20, 0xE0, 0x00, 	/*  [  *     *** ]  */
   0x00, 0xC0, 0x00, 	/*  [        **  ]  */
   0x01, 0x80, 0x00, 	/*  [       **   ]  */
   0x03, 0x00, 0x00, 	/*  [      **    ]  */
   0x07, 0x00, 0x00, 	/*  [     ***    ]  */
   0x06, 0x00, 0x00, 	/*  [     **     ]  */
   0x0C, 0x00, 0x00, 	/*  [    **      ]  */
   0x18, 0x00, 0x00, 	/*  [   **       ]  */
   0x38, 0x20, 0x00, 	/*  [  ***     * ]  */
   0x3F, 0xE0, 0x00, 	/*  [  ********* ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   0x00, 0x00, 0x00, 	/*  [            ]  */
   
   /* '{' charwidth: 7 */
   0x0C, 0x00, 0x00, 	/*  [    ** ]  */
   0x10, 0x00, 0x00, 	/*  [   *   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x20, 0x00, 0x00, 	/*  [  *    ]  */
   0x40, 0x00, 0x00, 	/*  [ *     ]  */
   0x20, 0x00, 0x00, 	/*  [  *    ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **   ]  */
   0x10, 0x00, 0x00, 	/*  [   *   ]  */
   0x0C, 0x00, 0x00, 	/*  [    ** ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   
   /* '|' charwidth: 5 */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   0x30, 0x00, 0x00, 	/*  [  ** ]  */
   
   /* '}' charwidth: 7 */
   0x60, 0x00, 0x00, 	/*  [ **    ]  */
   0x10, 0x00, 0x00, 	/*  [   *   ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x08, 0x00, 0x00, 	/*  [    *  ]  */
   0x04, 0x00, 0x00, 	/*  [     * ]  */
   0x08, 0x00, 0x00, 	/*  [    *  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x18, 0x00, 0x00, 	/*  [   **  ]  */
   0x10, 0x00, 0x00, 	/*  [   *   ]  */
   0x60, 0x00, 0x00, 	/*  [ **    ]  */
   0x00, 0x00, 0x00, 	/*  [       ]  */
   
   /* '~' charwidth: 13 */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x3E, 0x10, 0x00, 	/*  [  *****    * ]  */
   0x7F, 0xF0, 0x00, 	/*  [ *********** ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****  ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
   0x00, 0x00, 0x00, 	/*  [             ]  */
 };
   
const unsigned char fifteen_dot_width_table[]   = 
 {  6, /* ' ' */
    6, /* '!' */
    8, /* '"' */
   13, /* '#' */
   13, /* '$' */
   14, /* '%' */
   14, /* '&' */
    5, /* ''' */
    7, /* '(' */
    7, /* ')' */
   10, /* '*' */
   13, /* '+' */
    6, /* ',' */
    7, /* '-' */
    6, /* '.' */
   11, /* '/' */
   13, /* '0' */
   13, /* '1' */
   13, /* '2' */
   13, /* '3' */
   13, /* '4' */
   13, /* '5' */
   13, /* '6' */
   13, /* '7' */
   13, /* '8' */
   13, /* '9' */
    6, /* ':' */
    6, /* ';' */
   13, /* '<' */
   13, /* '=' */
   13, /* '>' */
   10, /* '?' */
   18, /* '@' */
   14, /* 'A' */
   12, /* 'B' */
   14, /* 'C' */
   16, /* 'D' */
   12, /* 'E' */
   11, /* 'F' */
   15, /* 'G' */
   16, /* 'H' */
    7, /* 'I' */
    9, /* 'J' */
   14, /* 'K' */
   11, /* 'L' */
   17, /* 'M' */
   15, /* 'N' */
   16, /* 'O' */
   12, /* 'P' */
   16, /* 'Q' */
   14, /* 'R' */
   11, /* 'S' */
   14, /* 'T' */
   15, /* 'U' */
   14, /* 'V' */
   18, /* 'W' */
   13, /* 'X' */
   12, /* 'Y' */
   13, /* 'Z' */
    7, /* '[' */
   11, /* '\' */
    7, /* ']' */
   13, /* '^' */
   10, /* '_' */
   13, /* '`' */
   11, /* 'a' */
   13, /* 'b' */
   11, /* 'c' */
   13, /* 'd' */
   11, /* 'e' */
    8, /* 'f' */
   12, /* 'g' */
   13, /* 'h' */
    7, /* 'i' */
    7, /* 'j' */
   12, /* 'k' */
    7, /* 'l' */
   19, /* 'm' */
   13, /* 'n' */
   13, /* 'o' */
   13, /* 'p' */
   13, /* 'q' */
    9, /* 'r' */
   11, /* 's' */
    8, /* 't' */
   13, /* 'u' */
   12, /* 'v' */
   17, /* 'w' */
   12, /* 'x' */
   12, /* 'y' */
   12, /* 'z' */
    7, /* '{' */
    5, /* '|' */
    7, /* '}' */
   13, /* '~' */
 };
#endif
   
#ifdef EN_EIGHTEEN_DOT
const unsigned char eighteen_dot_glyph_table[]  = 
 { /* ' ' charwidth: 6 */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   0x00, 0x00, 0x00, 	/*  [      ]  */
   
   /* '!' charwidth: 8 */
   0x3F, 0x00, 0x00, 	/*  [  ******]  */
   0x3F, 0x00, 0x00, 	/*  [  ******]  */
   0x3F, 0x00, 0x00, 	/*  [  ******]  */
   0x3F, 0x00, 0x00, 	/*  [  ******]  */
   0x3F, 0x00, 0x00, 	/*  [  ******]  */
   0x3F, 0x00, 0x00, 	/*  [  ******]  */
   0x1E, 0x00, 0x00, 	/*  [   **** ]  */
   0x1E, 0x00, 0x00, 	/*  [   **** ]  */
   0x1E, 0x00, 0x00, 	/*  [   **** ]  */
   0x1E, 0x00, 0x00, 	/*  [   **** ]  */
   0x1E, 0x00, 0x00, 	/*  [   **** ]  */
   0x1E, 0x00, 0x00, 	/*  [   **** ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x1C, 0x00, 0x00, 	/*  [   ***  ]  */
   0x3E, 0x00, 0x00, 	/*  [  ***** ]  */
   0x3E, 0x00, 0x00, 	/*  [  ***** ]  */
   0x3E, 0x00, 0x00, 	/*  [  ***** ]  */
   0x1C, 0x00, 0x00, 	/*  [   ***  ]  */
   
   /* '"' charwidth: 9 */
   0x73, 0x80, 0x00, 	/*  [ ***  ***]  */
   0x73, 0x80, 0x00, 	/*  [ ***  ***]  */
   0x73, 0x80, 0x00, 	/*  [ ***  ***]  */
   0x73, 0x80, 0x00, 	/*  [ ***  ***]  */
   0x73, 0x80, 0x00, 	/*  [ ***  ***]  */
   0x73, 0x80, 0x00, 	/*  [ ***  ***]  */
   0x73, 0x80, 0x00, 	/*  [ ***  ***]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   0x00, 0x00, 0x00, 	/*  [         ]  */
   
   /* '#' charwidth: 21 */
   0x00, 0x38, 0xE0, 	/*  [          ***   ***  ]  */
   0x00, 0x70, 0xE0, 	/*  [         ***    ***  ]  */
   0x00, 0x71, 0xC0, 	/*  [         ***   ***   ]  */
   0x00, 0x71, 0xC0, 	/*  [         ***   ***   ]  */
   0x00, 0xE3, 0x80, 	/*  [        ***   ***    ]  */
   0x0F, 0xFF, 0xF8, 	/*  [    *****************]  */
   0x1F, 0xFF, 0xF8, 	/*  [   ******************]  */
   0x1F, 0xFF, 0xF0, 	/*  [   ***************** ]  */
   0x01, 0xC7, 0x00, 	/*  [       ***   ***     ]  */
   0x03, 0x87, 0x00, 	/*  [      ***    ***     ]  */
   0x03, 0x8E, 0x00, 	/*  [      ***   ***      ]  */
   0x3F, 0xFF, 0xE0, 	/*  [  *****************  ]  */
   0x3F, 0xFF, 0xE0, 	/*  [  *****************  ]  */
   0x7F, 0xFF, 0xC0, 	/*  [ *****************   ]  */
   0x07, 0x1C, 0x00, 	/*  [     ***   ***       ]  */
   0x0E, 0x1C, 0x00, 	/*  [    ***    ***       ]  */
   0x0E, 0x38, 0x00, 	/*  [    ***   ***        ]  */
   0x1C, 0x38, 0x00, 	/*  [   ***    ***        ]  */
   
   /* '$' charwidth: 16 */
   0x01, 0x80, 0x00, 	/*  [       **       ]  */
   0x01, 0x80, 0x00, 	/*  [       **       ]  */
   0x0F, 0xE0, 0x00, 	/*  [    *******     ]  */
   0x1F, 0xF0, 0x00, 	/*  [   *********    ]  */
   0x3D, 0x90, 0x00, 	/*  [  **** **  *    ]  */
   0x3D, 0x80, 0x00, 	/*  [  **** **       ]  */
   0x3F, 0x80, 0x00, 	/*  [  *******       ]  */
   0x1F, 0xE0, 0x00, 	/*  [   ********     ]  */
   0x0F, 0xF0, 0x00, 	/*  [    ********    ]  */
   0x03, 0xF8, 0x00, 	/*  [      *******   ]  */
   0x01, 0xBC, 0x00, 	/*  [       ** ****  ]  */
   0x01, 0xBC, 0x00, 	/*  [       ** ****  ]  */
   0x21, 0xBC, 0x00, 	/*  [  *    ** ****  ]  */
   0x31, 0xFC, 0x00, 	/*  [  **   *******  ]  */
   0x3F, 0xF8, 0x00, 	/*  [  ***********   ]  */
   0x1F, 0x80, 0x00, 	/*  [   ********     ]  */
   0x01, 0xE0, 0x00, 	/*  [       **       ]  */
   0x01, 0x80, 0x00, 	/*  [       **       ]  */
   
   /* '%' charwidth: 22 */
   0x1F, 0x01, 0xC0, 	/*  [   *****       ***    ]  */
   0x3F, 0x81, 0x80, 	/*  [  *******      **     ]  */
   0x71, 0xC3, 0x00, 	/*  [ ***   ***    **      ]  */
   0x71, 0xC7, 0x00, 	/*  [ ***   ***   ***      ]  */
   0x71, 0xC6, 0x00, 	/*  [ ***   ***   **       ]  */
   0x71, 0xCC, 0x00, 	/*  [ ***   ***  **        ]  */
   0x71, 0xDC, 0x00, 	/*  [ ***   *** ***        ]  */
   0x3F, 0x98, 0x00, 	/*  [  *******  **         ]  */
   0x1F, 0x30, 0x00, 	/*  [   *****  **          ]  */
   0x00, 0x33, 0xE0, 	/*  [          **  *****   ]  */
   0x00, 0x67, 0xF0, 	/*  [         **  *******  ]  */
   0x00, 0xCE, 0x38, 	/*  [        **  ***   *** ]  */
   0x00, 0xCE, 0x38, 	/*  [        **  ***   *** ]  */
   0x01, 0x8E, 0x38, 	/*  [       **   ***   *** ]  */
   0x03, 0x8E, 0x38, 	/*  [      ***   ***   *** ]  */
   0x03, 0x0E, 0x38, 	/*  [      **    ***   *** ]  */
   0x06, 0x07, 0xF0, 	/*  [     **      *******  ]  */
   0x0E, 0x03, 0xE0, 	/*  [    ***       *****   ]  */
   
   /* '&' charwidth: 21 */
   0x07, 0xE0, 0x00, 	/*  [     ******          ]  */
   0x0F, 0xF0, 0x00, 	/*  [    ********         ]  */
   0x1F, 0xF8, 0x00, 	/*  [   **********        ]  */
   0x1F, 0x38, 0x00, 	/*  [   *****  ***        ]  */
   0x1F, 0x38, 0x00, 	/*  [   *****  ***        ]  */
   0x1F, 0xF8, 0x00, 	/*  [   **********        ]  */
   0x0F, 0xF0, 0x00, 	/*  [    ********         ]  */
   0x07, 0xE0, 0x00, 	/*  [     ******          ]  */
   0x0F, 0xF0, 0x00, 	/*  [    ********         ]  */
   0x1F, 0xF8, 0x00, 	/*  [   **********        ]  */
   0x3E, 0xFC, 0x00, 	/*  [  ***** ******       ]  */
   0x7C, 0x7F, 0x00, 	/*  [ *****   *******     ]  */
   0x7C, 0x3F, 0x80, 	/*  [ *****    *******    ]  */
   0x7C, 0x1F, 0xC0, 	/*  [ *****     *******   ]  */
   0x7E, 0x1F, 0xE0, 	/*  [ ******    ********  ]  */
   0x3F, 0xFF, 0xF8, 	/*  [  *******************]  */
   0x1F, 0xF3, 0xF8, 	/*  [   *********  *******]  */
   0x0F, 0xE0, 0xF8, 	/*  [    *******     *****]  */
   
   /* ''' charwidth: 5 */
   0x70, 0x00, 0x00, 	/*  [ *** ]  */
   0x70, 0x00, 0x00, 	/*  [ *** ]  */
   0x70, 0x00, 0x00, 	/*  [ *** ]  */
   0x70, 0x00, 0x00, 	/*  [ *** ]  */
   0x70, 0x00, 0x00, 	/*  [ *** ]  */
   0x70, 0x00, 0x00, 	/*  [ *** ]  */
   0x70, 0x00, 0x00, 	/*  [ *** ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   0x00, 0x00, 0x00, 	/*  [     ]  */
   
   /* '(' charwidth: 11 */
   0x01, 0x80, 0x00, 	/*  [       **  ]  */
   0x03, 0x80, 0x00, 	/*  [      ***  ]  */
   0x07, 0x00, 0x00, 	/*  [     ***   ]  */
   0x07, 0x00, 0x00, 	/*  [     ***   ]  */
   0x0E, 0x00, 0x00, 	/*  [    ***    ]  */
   0x0E, 0x00, 0x00, 	/*  [    ***    ]  */
   0x1E, 0x00, 0x00, 	/*  [   ****    ]  */
   0x1E, 0x00, 0x00, 	/*  [   ****    ]  */
   0x1E, 0x00, 0x00, 	/*  [   ****    ]  */
   0x1E, 0x00, 0x00, 	/*  [   ****    ]  */
   0x1E, 0x00, 0x00, 	/*  [   ****    ]  */
   0x1E, 0x00, 0x00, 	/*  [   ****    ]  */
   0x0E, 0x00, 0x00, 	/*  [    ***    ]  */
   0x0E, 0x00, 0x00, 	/*  [    ***    ]  */
   0x07, 0x00, 0x00, 	/*  [     ***   ]  */
   0x07, 0x00, 0x00, 	/*  [     ***   ]  */
   0x03, 0x80, 0x00, 	/*  [      ***  ]  */
   0x01, 0x80, 0x00, 	/*  [       **  ]  */
   
   /* ')' charwidth: 11 */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   0x38, 0x00, 0x00, 	/*  [  ***      ]  */
   0x1C, 0x00, 0x00, 	/*  [   ***     ]  */
   0x1C, 0x00, 0x00, 	/*  [   ***     ]  */
   0x0E, 0x00, 0x00, 	/*  [    ***    ]  */
   0x0E, 0x00, 0x00, 	/*  [    ***    ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****   ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****   ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****   ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****   ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****   ]  */
   0x0F, 0x00, 0x00, 	/*  [    ****   ]  */
   0x0E, 0x00, 0x00, 	/*  [    ***    ]  */
   0x0E, 0x00, 0x00, 	/*  [    ***    ]  */
   0x1C, 0x00, 0x00, 	/*  [   ***     ]  */
   0x1C, 0x00, 0x00, 	/*  [   ***     ]  */
   0x38, 0x00, 0x00, 	/*  [  ***      ]  */
   0x30, 0x00, 0x00, 	/*  [  **       ]  */
   
   /* '*' charwidth: 16 */
   0x03, 0xC0, 0x00, 	/*  [      ****      ]  */
   0x09, 0xA0, 0x00, 	/*  [    *  ** *     ]  */
   0x0D, 0xB0, 0x00, 	/*  [    ** ** **    ]  */
   0x1F, 0x78, 0x00, 	/*  [   ***** ****   ]  */
   0x03, 0xC0, 0x00, 	/*  [      ****      ]  */
   0x03, 0xC0, 0x00, 	/*  [      ****      ]  */
   0x1F, 0x78, 0x00, 	/*  [   ***** ****   ]  */
   0x0D, 0xB0, 0x00, 	/*  [    ** ** **    ]  */
   0x09, 0xA0, 0x00, 	/*  [    *  ** *     ]  */
   0x03, 0xC0, 0x00, 	/*  [      ****      ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   0x00, 0x00, 0x00, 	/*  [                ]  */
   
   /* '+' charwidth: 22 */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x1F, 0xFF, 0xF0, 	/*  [   *****************  ]  */
   0x1F, 0xFF, 0xF0, 	/*  [   *****************  ]  */
   0x1F, 0xFF, 0xF0, 	/*  [   *****************  ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x38, 0x00, 	/*  [          ***         ]  */
   0x00, 0x00, 0x00, 	/*  [                      ]  */
   
   /* ',' charwidth: 8 */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x38, 0x00, 0x00, 	/*  [  ***   ]  */
   0x7C, 0x00, 0x00, 	/*  [ *****  ]  */
   0x7C, 0x00, 0x00, 	/*  [ *****  ]  */
   0x7C, 0x00, 0x00, 	/*  [ *****  ]  */
   0x3C, 0x00, 0x00, 	/*  [  ****  ]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x60, 0x00, 0x00, 	/*  [ **     ]  */
   
   /* '-' charwidth: 11 */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x1F, 0xC0, 0x00, 	/*  [   ******* ]  */
   0x3F, 0x80, 0x00, 	/*  [  *******  ]  */
   0x7F, 0x00, 0x00, 	/*  [ *******   ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   0x00, 0x00, 0x00, 	/*  [           ]  */
   
   /* '.' charwidth: 8 */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x00, 0x00, 0x00, 	/*  [        ]  */
   0x38, 0x00, 0x00, 	/*  [  ***   ]  */
   0x7C, 0x00, 0x00, 	/*  [ *****  ]  */
   0x7C, 0x00, 0x00, 	/*  [ *****  ]  */
   0x7C, 0x00, 0x00, 	/*  [ *****  ]  */
   0x38, 0x00, 0x00, 	/*  [  ***   ]  */
   
   /* '/' charwidth: 8 */
   0x06, 0x00, 0x00, 	/*  [     ** ]  */
   0x06, 0x00, 0x00, 	/*  [     ** ]  */
   0x0E, 0x00, 0x00, 	/*  [    *** ]  */
   0x0C, 0x00, 0x00, 	/*  [    **  ]  */
   0x0C, 0x00, 0x00, 	/*  [    **  ]  */
   0x1C, 0x00, 0x00, 	/*  [   ***  ]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x18, 0x00, 0x00, 	/*  [   **   ]  */
   0x38, 0x00, 0x00, 	/*  [  ***   ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x30, 0x00, 0x00, 	/*  [  **    ]  */
   0x70, 0x00, 0x00, 	/*  [ ***    ]  */
   0x60, 0x00, 0x00, 	/*  [ **     ]  */
   0x60, 0x00, 0x00, 	/*  [ **     ]  */
   0xE0, 0x00, 0x00, 	/*  [***     ]  */
   0xC0, 0x00, 0x00, 	/*  [**      ]  */
   0xC0, 0x00, 0x00, 	/*  [**      ]  */
   0xC0, 0x00, 0x00, 	/*  [**      ]  */
   
   /* '0' charwidth: 16 */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x0F, 0xF8, 0x00, 	/*  [    *********   ]  */
   0x1F, 0xFC, 0x00, 	/*  [   ***********  ]  */
   0x3E, 0x3E, 0x00, 	/*  [  *****   ***** ]  */
   0x3E, 0x3E, 0x00, 	/*  [  *****   ***** ]  */
   0x3C, 0x1E, 0x00, 	/*  [  ****     **** ]  */
   0x7C, 0x1F, 0x00, 	/*  [ *****     *****]  */
   0x7C, 0x1F, 0x00, 	/*  [ *****     *****]  */
   0x7C, 0x1F, 0x00, 	/*  [ *****     *****]  */
   0x7C, 0x1F, 0x00, 	/*  [ *****     *****]  */
   0x7C, 0x1F, 0x00, 	/*  [ *****     *****]  */
   0x7C, 0x1F, 0x00, 	/*  [ *****     *****]  */
   0x3C, 0x1F, 0x00, 	/*  [  ****     *****]  */
   0x3E, 0x3E, 0x00, 	/*  [  *****   ***** ]  */
   0x3E, 0x3E, 0x00, 	/*  [  *****   ***** ]  */
   0x1F, 0xFC, 0x00, 	/*  [   ***********  ]  */
   0x0F, 0xF8, 0x00, 	/*  [    *********   ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   
   /* '1' charwidth: 16 */
   0x00, 0x20, 0x00, 	/*  [          *     ]  */
   0x00, 0x60, 0x00, 	/*  [         **     ]  */
   0x00, 0xE0, 0x00, 	/*  [        ***     ]  */
   0x07, 0xE0, 0x00, 	/*  [     ******     ]  */
   0x1F, 0xE0, 0x00, 	/*  [   ********     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x0F, 0xF8, 0x00, 	/*  [    *********   ]  */
   
   /* '2' charwidth: 16 */
   0x07, 0xE0, 0x00, 	/*  [     ******     ]  */
   0x1F, 0xF8, 0x00, 	/*  [   **********   ]  */
   0x1F, 0xFC, 0x00, 	/*  [   ***********  ]  */
   0x38, 0xFC, 0x00, 	/*  [  ***   ******  ]  */
   0x20, 0x7C, 0x00, 	/*  [  *      *****  ]  */
   0x00, 0x7C, 0x00, 	/*  [         *****  ]  */
   0x00, 0xFC, 0x00, 	/*  [        ******  ]  */
   0x00, 0xF8, 0x00, 	/*  [        *****   ]  */
   0x01, 0xF8, 0x00, 	/*  [       ******   ]  */
   0x01, 0xF0, 0x00, 	/*  [       *****    ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x07, 0xE0, 0x00, 	/*  [     ******     ]  */
   0x07, 0xC0, 0x00, 	/*  [     *****      ]  */
   0x0F, 0x80, 0x00, 	/*  [    *****       ]  */
   0x1F, 0x82, 0x00, 	/*  [   ******     * ]  */
   0x1F, 0xFE, 0x00, 	/*  [   ************ ]  */
   0x3F, 0xFC, 0x00, 	/*  [  ************  ]  */
   0x7F, 0xFC, 0x00, 	/*  [ *************  ]  */
   
   /* '3' charwidth: 16 */
   0x1F, 0xC0, 0x00, 	/*  [   *******      ]  */
   0x3F, 0xF0, 0x00, 	/*  [  **********    ]  */
   0x3F, 0xF8, 0x00, 	/*  [  ***********   ]  */
   0x21, 0xF8, 0x00, 	/*  [  *    ******   ]  */
   0x00, 0xF8, 0x00, 	/*  [        *****   ]  */
   0x00, 0xF8, 0x00, 	/*  [        *****   ]  */
   0x01, 0xF0, 0x00, 	/*  [       *****    ]  */
   0x1F, 0xE0, 0x00, 	/*  [   ********     ]  */
   0x1F, 0xF0, 0x00, 	/*  [   *********    ]  */
   0x1F, 0xF8, 0x00, 	/*  [   **********   ]  */
   0x00, 0xFC, 0x00, 	/*  [        ******  ]  */
   0x00, 0x7C, 0x00, 	/*  [         *****  ]  */
   0x00, 0x7C, 0x00, 	/*  [         *****  ]  */
   0x00, 0x7C, 0x00, 	/*  [         *****  ]  */
   0x00, 0xF8, 0x00, 	/*  [        *****   ]  */
   0x7F, 0xF8, 0x00, 	/*  [ ************   ]  */
   0x3F, 0xF0, 0x00, 	/*  [  **********    ]  */
   0x3F, 0xC0, 0x00, 	/*  [  ********      ]  */
   
   /* '4' charwidth: 16 */
   0x01, 0xF8, 0x00, 	/*  [       ******   ]  */
   0x01, 0xF8, 0x00, 	/*  [       ******   ]  */
   0x03, 0xF8, 0x00, 	/*  [      *******   ]  */
   0x03, 0xF8, 0x00, 	/*  [      *******   ]  */
   0x06, 0xF8, 0x00, 	/*  [     ** *****   ]  */
   0x06, 0xF8, 0x00, 	/*  [     ** *****   ]  */
   0x0C, 0xF8, 0x00, 	/*  [    **  *****   ]  */
   0x1C, 0xF8, 0x00, 	/*  [   ***  *****   ]  */
   0x18, 0xF8, 0x00, 	/*  [   **   *****   ]  */
   0x38, 0xF8, 0x00, 	/*  [  ***   *****   ]  */
   0x30, 0xF9, 0x00, 	/*  [  **    *****  *]  */
   0x7F, 0xFF, 0x00, 	/*  [ ***************]  */
   0x7F, 0xFE, 0x00, 	/*  [ ************** ]  */
   0xFF, 0xFE, 0x00, 	/*  [*************** ]  */
   0x00, 0xF8, 0x00, 	/*  [        *****   ]  */
   0x00, 0xF8, 0x00, 	/*  [        *****   ]  */
   0x00, 0xF8, 0x00, 	/*  [        *****   ]  */
   0x01, 0xFC, 0x00, 	/*  [       *******  ]  */
   
   /* '5' charwidth: 16 */
   0x1F, 0xF8, 0x00, 	/*  [   **********   ]  */
   0x1F, 0xF8, 0x00, 	/*  [   **********   ]  */
   0x3F, 0xF8, 0x00, 	/*  [  ***********   ]  */
   0x3E, 0x08, 0x00, 	/*  [  *****     *   ]  */
   0x3E, 0x00, 0x00, 	/*  [  *****         ]  */
   0x3E, 0x00, 0x00, 	/*  [  *****         ]  */
   0x3F, 0xE0, 0x00, 	/*  [  *********     ]  */
   0x3F, 0xF0, 0x00, 	/*  [  **********    ]  */
   0x3F, 0xF8, 0x00, 	/*  [  ***********   ]  */
   0x21, 0xFC, 0x00, 	/*  [  *    *******  ]  */
   0x00, 0x7C, 0x00, 	/*  [         *****  ]  */
   0x00, 0x7C, 0x00, 	/*  [         *****  ]  */
   0x00, 0x7C, 0x00, 	/*  [         *****  ]  */
   0x00, 0x7C, 0x00, 	/*  [         *****  ]  */
   0x01, 0xF8, 0x00, 	/*  [       ******   ]  */
   0x7F, 0xF8, 0x00, 	/*  [ ************   ]  */
   0x3F, 0xF0, 0x00, 	/*  [  **********    ]  */
   0x1F, 0xC0, 0x00, 	/*  [   *******      ]  */
   
   /* '6' charwidth: 16 */
   0x00, 0x78, 0x00, 	/*  [         ****   ]  */
   0x01, 0xF0, 0x00, 	/*  [       *****    ]  */
   0x07, 0xE0, 0x00, 	/*  [     ******     ]  */
   0x0F, 0x80, 0x00, 	/*  [    *****       ]  */
   0x1F, 0x00, 0x00, 	/*  [   *****        ]  */
   0x3E, 0x00, 0x00, 	/*  [  *****         ]  */
   0x3E, 0xF0, 0x00, 	/*  [  ***** ****    ]  */
   0x3D, 0xFC, 0x00, 	/*  [  **** *******  ]  */
   0x7D, 0xFC, 0x00, 	/*  [ ***** *******  ]  */
   0x7C, 0x7E, 0x00, 	/*  [ *****   ****** ]  */
   0x7C, 0x3E, 0x00, 	/*  [ *****    ***** ]  */
   0x7C, 0x3E, 0x00, 	/*  [ *****    ***** ]  */
   0x7C, 0x3E, 0x00, 	/*  [ *****    ***** ]  */
   0x7C, 0x3E, 0x00, 	/*  [ *****    ***** ]  */
   0x3E, 0x7C, 0x00, 	/*  [  *****  *****  ]  */
   0x1F, 0xFC, 0x00, 	/*  [   ***********  ]  */
   0x0F, 0xF8, 0x00, 	/*  [    *********   ]  */
   0x07, 0xE0, 0x00, 	/*  [     ******     ]  */
   
   /* '7' charwidth: 16 */
   0x3F, 0xFE, 0x00, 	/*  [  ************* ]  */
   0x3F, 0xFC, 0x00, 	/*  [  ************  ]  */
   0x7F, 0xFC, 0x00, 	/*  [ *************  ]  */
   0x40, 0x7C, 0x00, 	/*  [ *       *****  ]  */
   0x00, 0x78, 0x00, 	/*  [         ****   ]  */
   0x00, 0xF8, 0x00, 	/*  [        *****   ]  */
   0x00, 0xF8, 0x00, 	/*  [        *****   ]  */
   0x01, 0xF0, 0x00, 	/*  [       *****    ]  */
   0x01, 0xF0, 0x00, 	/*  [       *****    ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x03, 0xE0, 0x00, 	/*  [      *****     ]  */
   0x07, 0xC0, 0x00, 	/*  [     *****      ]  */
   0x07, 0xC0, 0x00, 	/*  [     *****      ]  */
   0x0F, 0x80, 0x00, 	/*  [    *****       ]  */
   0x0F, 0x80, 0x00, 	/*  [    *****       ]  */
   0x0F, 0x80, 0x00, 	/*  [    *****       ]  */
   0x1F, 0xC0, 0x00, 	/*  [   *******      ]  */
   
   /* '8' charwidth: 16 */
   0x07, 0xF0, 0x00, 	/*  [     *******    ]  */
   0x0F, 0xF8, 0x00, 	/*  [    *********   ]  */
   0x1F, 0xFC, 0x00, 	/*  [   ***********  ]  */
   0x3E, 0x3E, 0x00, 	/*  [  *****   ***** ]  */
   0x3E, 0x1E, 0x00, 	/*  [  *****    **** ]  */
   0x3E, 0x1E, 0x00, 	/*  [  *****    **** ]  */
   0x3F, 0x9C, 0x00, 	/*  [  *******  ***  ]  */
   0x1F, 0xF8, 0x00, 	/*  [   **********   ]  */
   0x0F, 0xF8, 0x00, 	/*  [    *********   ]  */
   0x1F, 0xFC, 0x00, 	/*  [   ***********  ]  */
   0x3C, 0xFE, 0x00, 	/*  [  ****  ******* ]  */
   0x78, 0x3F, 0x00, 	/*  [ ****     ******]  */
   0x78, 0x1F, 0x00, 	/*  [ ****      *****]  */
   0x78, 0x1F, 0x00, 	/*  [ ****      *****]  */
   0x7C, 0x3F, 0x00, 	/*  [ *****    ******]  */
   0x3F, 0xFE, 0x00, 	/*  [  ************* ]  */
   0x1F, 0xFC, 0x00, 	/*  [   ***********  ]  */
   0x07, 0xF0, 0x00, 	/*  [     *******    ]  */
   
   /* '9' charwidth: 16 */
   0x07, 0xE0, 0x00, 	/*  [     ******     ]  */
   0x1F, 0xF0, 0x00, 	/*  [   *********    ]  */
   0x3F, 0xF8, 0x00, 	/*  [  ***********   ]  */
   0x3E, 0x7C, 0x00, 	/*  [  *****  *****  ]  */
   0x7C, 0x3E, 0x00, 	/*  [ *****    ***** ]  */
   0x7C, 0x3E, 0x00, 	/*  [ *****    ***** ]  */
   0x7C, 0x3E, 0x00, 	/*  [ *****    ***** ]  */
   0x7C, 0x3E, 0x00, 	/*  [ *****    ***** ]  */
   0x7E, 0x3E, 0x00, 	/*  [ ******   ***** ]  */
   0x3F, 0xBE, 0x00, 	/*  [  ******* ***** ]  */
   0x3F, 0xBE, 0x00, 	/*  [  ******* ***** ]  */
   0x0F, 0x7C, 0x00, 	/*  [    **** *****  ]  */
   0x00, 0x7C, 0x00, 	/*  [         *****  ]  */
   0x00, 0xF8, 0x00, 	/*  [        *****   ]  */
   0x01, 0xF0, 0x00, 	/*  [       *****    ]  */
   0x07, 0xE0, 0x00, 	/*  [     ******     ]  */
   0x0F, 0x80, 0x00, 	/*  [    *****       ]  */
   0x1E, 0x00, 0x00, 	/*  [   ****         ]  */
   
    /* ':' charwidth: 8 - added by mth */
   0x00, 0x00, 0x00,       /*  [        ]  */
   0x00, 0x00, 0x00,       /*  [        ]  */
   0x00, 0x00, 0x00,       /*  [        ]  */
   0x00, 0x00, 0x00,       /*  [        ]  */
   (0x38>>1), 0x00, 0x00,  /*  [  ***   ]  */
   (0x7C>>1), 0x00, 0x00,  /*  [ *****  ]  */
   (0x7C>>1), 0x00, 0x00,  /*  [ *****  ]  */
   (0x7C>>1), 0x00, 0x00,  /*  [ *****  ]  */
   (0x38>>1), 0x00, 0x00,  /*  [  ***   ]  */
   0x00, 0x00, 0x00,       /*  [        ]  */
   0x00, 0x00, 0x00,       /*  [        ]  */
   (0x38>>1), 0x00, 0x00,  /*  [  ***   ]  */
   (0x7C>>1), 0x00, 0x00,  /*  [ *****  ]  */
   (0x7C>>1), 0x00, 0x00,  /*  [ *****  ]  */
   (0x7C>>1), 0x00, 0x00,  /*  [ *****  ]  */
   (0x38>>1), 0x00, 0x00,  /*  [  ***   ]  */
   0x00, 0x00, 0x00,       /*  [        ]  */
   0x00, 0x00, 0x00,       /*  [        ]  */
 };
   
   
const unsigned char eighteen_dot_width_table[]  = 
 {  6, /* ' ' */
    8, /* '!' */
    9, /* '"' */
   21, /* '#' */
   16, /* '$' */
   22, /* '%' */
   21, /* '&' */
    5, /* ''' */
   11, /* '(' */
   11, /* ')' */
   16, /* '*' */
   22, /* '+' */
    8, /* ',' */
   11, /* '-' */
    8, /* '.' */
    8, /* '/' */
   16, /* '0' */
   16, /* '1' */
   16, /* '2' */
   16, /* '3' */
   16, /* '4' */
   16, /* '5' */
   16, /* '6' */
   16, /* '7' */
   16, /* '8' */
   16, /* '9' */
   8,  /* ':' */
 };
#endif

