//******************************************************************************
//                                 本程序介绍
//                                          ---北京交通大学电气学院 杨罡 2008.8
//
//       编译环境ICC6.31，
//       使用ATMEGA32单片机 & KS0108B/HD61202控制器的128*64液晶
//
//   1、 本程序实现了基本的GUI绘图函数
//       void lcd_fill( uint8_t pattern);全屏幕填充函数
//       void lcd_erase(void);           擦除全屏幕的函数
//       void lcd_update_all(void);      把缓冲区显示到液晶上的刷新函数
//       void lcd_dot( uint8_t x,  uint8_t y,  uint8_t mode); 任意坐标画点的函数
//       void lcd_line( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode );
//       任意（x1,y1）（x2,y2）两点之间的画直线函数，这个函数化很短的斜线时会出问题，一般的长斜线无问题
//       void lcd_rect(  uint8_t x, uint8_t y, uint8_t width, uint8_t height,  uint8_t mode);
//       画以（x,y）为左上角，高height，长width 的矩形的函数
//       void lcd_box( uint8_t x,  uint8_t y, uint8_t width,  uint8_t height,  uint8_t mode);
//       画以（x,y）为左上角，高height，长width 的矩形并且用mode方式填充的函数
//       void lcd_circle( uint8_t xCenter,  uint8_t yCenter, uint8_t radius,  uint8_t mode);
//       画（xCenter,yCenter）为圆心，半径为radius的函数，圆心可以在屏幕外面，所以可以画弧:)
//    2、本程序实现多种字体
//         可以根据需求去font.h文件选择你需要的字体，提供了5点6点7点9点10点15点18点
//       的英文字体供用户选择，中文字库太大没有实现，不过一般英文就公用了吧
//       void lcd_text(uint8_t left, uint8_t top, uint8_t font,   char *str);
//       显示在坐标为（left,top）处显示字体为 font 的即时字符串 str
//       void lcd_text_p(uint8_t left, uint8_t top, uint8_t font,  const char *str);	
//       显示在坐标为（left,top）处显示字体为 font 的存放在flash中的字符串 str
//       font 参见font.h中的定义，需要先选择后使用
//    3、移植性
//         液晶很多，这个只是其中一种的程序，但是本程序是可以移植的，你只需根据具体
//       具体的液晶控制器去重写 LCD_DRIVER.H 和LCD_DRIVER.C 就可以了 
//    4、使用                                           
//         由于使用显示缓存区的缘故所以RAM容量在1K一下的AVR器件将无法使用本程序
//       建议使用ATMEGA32或者ATMEGA64等大容量的片子，或者你课以将缓存区去掉，直接使用
//       液晶的显示缓存读写，那么就可以用于任何AVR了:)
//       !!!!!!!!值得注意的是：当你下载到片子运行时，请把LCD_DRIVER.C文件中的这一行注释掉
//               #define debug_protues     
//       否则将产生显示混乱。！！！！！！！！！！！！！！！！！！！！！！！    
//     5、关于demo
//        demo程序放在了main函数中，一共三个画面，可以改动其中的函数来熟悉以上介绍
//       函数。调入位图的函数没有在demo中运行，你可以试着调用，但是需要加位图数据。                             
//******************************************************************************
#include "LCD_DRIVER.h"
#include "m32_init.c"
#include  "connection.h"
#include  "font.h"
#include  "bitmap.h"
#include  "graphic.h"
#include  "m32macro.h"
void project_init(void)
{ 
  init_devices();
  PORTB=0x00;
  lcd_init();
}

void main()
{
 project_init();
  while(1){
 //**********************************************************
 //welcome to beijing olympic games logo :)
 //**********************************************************
 lcd_text(15,13,FONT_SIX_DOT,"welcome to beijing olympic");	
 lcd_rect(0,0,128,64,LCD_MODE_SET);
 lcd_circle(48,31,10,LCD_MODE_SET);
 lcd_circle(63,31,10,LCD_MODE_SET);
 lcd_circle(78,31,10,LCD_MODE_SET);
 lcd_circle(58,46,10,LCD_MODE_SET);
 lcd_circle(70,46,10,LCD_MODE_SET);
 lcd_update_all();
 //**********************************************************
  _delay_ms(3000);
  lcd_erase(); 
 lcd_update_all();
 
 //**********************************************************
 //rectangle , box ,six dot font ,nine dot font 
 //**********************************************************
  lcd_rect(10,10,10,10,LCD_MODE_SET);
  lcd_box(30,10,10,10,LCD_MODE_SET);
  lcd_text(10,25,FONT_SIX_DOT,"yanggang 12:00:00");
  lcd_text(10,40,FONT_NINE_DOT,"yanggang");
  lcd_update_all();
 //**********************************************************
  _delay_ms(3000);
  lcd_erase(); 
  lcd_update_all();
  
  //********************************************************
  // a simple window but it has no function to response any
  // operation
  //********************************************************
  lcd_text(30,2,FONT_SIX_DOT,"yanggang's window");
  lcd_box(0,0,128,11,LCD_MODE_XOR);
  lcd_rect(0,0,128,64,LCD_MODE_SET);
  lcd_text(20,30,FONT_SIX_DOT,"welcome to my window");
  lcd_text(80,50,FONT_SIX_DOT,"ok");
  lcd_text(100,50,FONT_SIX_DOT,"exit");
  lcd_box(79,48,15,10,LCD_MODE_XOR);
  lcd_box(99,48,20,10,LCD_MODE_XOR);
  lcd_update_all();
  //********************************************************
  _delay_ms(3000);
  lcd_erase(); 
  lcd_update_all();
  
  }
}