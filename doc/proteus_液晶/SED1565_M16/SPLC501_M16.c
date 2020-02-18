#include <avr/io.h>
#include <util/delay.h>
#include "SPLC501.h"
#include<avr/interrupt.h>
#include<avr/pgmspace.h>
#define BIG 40
#define	uchar unsigned char	

volatile unsigned char IIC_STATE; //IIC 通讯状态机
//TWI 状态定义
#define STATE_IIC_ADDR 0xC3
#define STATE_IIC_WDATA 0xA5
#define STATE_IIC_RDATA 0x5A
#define STATE_IIC_STOP 0
volatile unsigned char IIC_ADDR;
volatile unsigned char IIC_DATA;  //当前数据
#define SLA_Device_Addr 0xa0 //定义器件地址
//从机方式中断响应状态码
#define SR_SLA_ACK 0x60
#define SR_ALL_ACK 0x70
#define SR_DATA_ACK 0x80
#define SR_DATA_NOACK 0x88
#define SR_ALL_DATA_ACK 0x90
#define SR_ALL_DATA_NOACK 0x98
#define SR_STOP_RESTART 0xA0
#define ST_SLA_ACK 0xA8
#define ST_DATA_ACK 0xB8
#define ST_DATA_NOACK 0xC0
#define ST_LAST_DATA_ACK 0xC8
//常用TWI操作（从模式写和从模式读）
#define Twi_STATE() (TWSR&0xf8)
#define SLA_autoACK() (TWCR=(1<<TWEA)|(1<<TWINT)|(1<<TWEN)|(1<<TWIE))
#define SLA_Send8Bit(x) {TWCR=(x);TWCR=(1<<TWEA)|(1<<TWINT)|(1<<TWEN)|(1<<TWIE);}
#define SLA_Resume() (TWCR=(1<<TWEA)|(1<<TWSTO)|(1<<TWINT)|(1<<TWEN)|(1<<TWIE))
#define uchar unsigned char
#define uint unsigned int
unsigned char Buffer[100]={0};
unsigned char size=0;


unsigned char BMP[]=
{
/*--  文字:  测  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x08,0x31,0x86,0x60,0x00,0xFE,0x02,0xF2,0x02,0xFE,0x00,0xF8,0x00,0x00,0xFF,0x00,
0x04,0xFC,0x03,0x00,0x80,0x47,0x30,0x0F,0x10,0x67,0x00,0x07,0x40,0x80,0x7F,0x00,

/*--  文字:  试  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x40,0x42,0xDC,0x08,0x00,0x90,0x90,0x90,0x90,0x90,0xFF,0x10,0x12,0x1C,0x10,0x00,
0x00,0x00,0x7F,0x20,0x10,0x20,0x20,0x1F,0x10,0x10,0x01,0x06,0x18,0x20,0x78,0x00,
};
//------------------------------------------------------------------------------
void Draw_word(unsigned int d_where,unsigned char page_f, //显示一个字或多个字函数
    unsigned char page_sum,unsigned char column_f,unsigned char column_sum )//x*8
{//参数：指向字首地址，起始页，占多少页，起始列，占多少列
  unsigned char i,j,page=0xb0;//指向首页命令
  d_where=d_where*32;
  for(i=page_f;i<page_sum+page_f;i++)
   { SPLC501_SPIwrite(page+i,0); 
     SPLC501_SPIwrite(0x10|((0xf0&column_f)>>4),0);//设定列地址高4位为0
     SPLC501_SPIwrite(0x04|(0x0f&column_f),0);    //设定列地址低4位为0
     for(j=0;j<column_sum;j++)
      {
	   SPLC501_SPIwrite(BMP[d_where],1);
       d_where++;
	  }
   }
}

//------------------------------------------------------------------------------
int main(void)
{
unsigned char i=0;
 DDRC=0xff;
 DDRD=0xff;

 LCD_Init();
 clrscr(0,8,0,128);
 Draw_word(0,4,2,24,16);
 Draw_word(1,4,2,40,16);

while(1);

}


