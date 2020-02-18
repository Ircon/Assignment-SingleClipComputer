//******************************************************************************
//                                 ���������
//                                          ---������ͨ��ѧ����ѧԺ ��� 2008.8
//
//       ���뻷��ICC6.31��
//       ʹ��ATMEGA32��Ƭ�� & KS0108B/HD61202��������128*64Һ��
//
//   1�� ������ʵ���˻�����GUI��ͼ����
//       void lcd_fill( uint8_t pattern);ȫ��Ļ��亯��
//       void lcd_erase(void);           ����ȫ��Ļ�ĺ���
//       void lcd_update_all(void);      �ѻ�������ʾ��Һ���ϵ�ˢ�º���
//       void lcd_dot( uint8_t x,  uint8_t y,  uint8_t mode); �������껭��ĺ���
//       void lcd_line( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode );
//       ���⣨x1,y1����x2,y2������֮��Ļ�ֱ�ߺ���������������̵ܶ�б��ʱ������⣬һ��ĳ�б��������
//       void lcd_rect(  uint8_t x, uint8_t y, uint8_t width, uint8_t height,  uint8_t mode);
//       ���ԣ�x,y��Ϊ���Ͻǣ���height����width �ľ��εĺ���
//       void lcd_box( uint8_t x,  uint8_t y, uint8_t width,  uint8_t height,  uint8_t mode);
//       ���ԣ�x,y��Ϊ���Ͻǣ���height����width �ľ��β�����mode��ʽ���ĺ���
//       void lcd_circle( uint8_t xCenter,  uint8_t yCenter, uint8_t radius,  uint8_t mode);
//       ����xCenter,yCenter��ΪԲ�ģ��뾶Ϊradius�ĺ�����Բ�Ŀ�������Ļ���棬���Կ��Ի���:)
//    2��������ʵ�ֶ�������
//         ���Ը�������ȥfont.h�ļ�ѡ������Ҫ�����壬�ṩ��5��6��7��9��10��15��18��
//       ��Ӣ�����幩�û�ѡ�������ֿ�̫��û��ʵ�֣�����һ��Ӣ�ľ͹����˰�
//       void lcd_text(uint8_t left, uint8_t top, uint8_t font,   char *str);
//       ��ʾ������Ϊ��left,top������ʾ����Ϊ font �ļ�ʱ�ַ��� str
//       void lcd_text_p(uint8_t left, uint8_t top, uint8_t font,  const char *str);	
//       ��ʾ������Ϊ��left,top������ʾ����Ϊ font �Ĵ����flash�е��ַ��� str
//       font �μ�font.h�еĶ��壬��Ҫ��ѡ���ʹ��
//    3����ֲ��
//         Һ���ܶ࣬���ֻ������һ�ֵĳ��򣬵��Ǳ������ǿ�����ֲ�ģ���ֻ����ݾ���
//       �����Һ��������ȥ��д LCD_DRIVER.H ��LCD_DRIVER.C �Ϳ����� 
//    4��ʹ��                                           
//         ����ʹ����ʾ��������Ե�����ԣSRAM������1Kһ�µ�AVR�������޷�ʹ�ñ�����
//       ����ʹ��ATMEGA32����ATMEGA64�ȴ�������Ƭ�ӣ���������Խ�������ȥ����ֱ��ʹ��
//       Һ������ʾ�����д����ô�Ϳ��������κ�AVR��:)
//       !!!!!!!!ֵ��ע����ǣ��������ص�Ƭ������ʱ�����LCD_DRIVER.C�ļ��е���һ��ע�͵�
//               #define debug_protues     
//       ���򽫲�����ʾ���ҡ�����������������������������������������������    
//     5������demo
//        demo���������main�����У�һ���������棬���ԸĶ����еĺ�������Ϥ���Ͻ���
//       ����������λͼ�ĺ���û����demo�����У���������ŵ��ã�������Ҫ��λͼ���ݡ�                             
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