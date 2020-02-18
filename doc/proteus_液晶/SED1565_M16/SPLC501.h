#ifndef	__SPLC501_H
#define	__SPLC501_H
//---------------------------------------------------------------------
typedef struct xxx
{	unsigned char bit0:1;
	unsigned char bit1:1;
	unsigned char bit2:1;
	unsigned char bit3:1;
	unsigned char bit4:1;
	unsigned char bit5:1;
	unsigned char bit6:1;
	unsigned char bit7:1;
}bit_field;

#define PORTA_addr   0x3B
#define PORTC_addr   0x35
#define SPLC501_CLK  (*((volatile bit_field*)(PORTC_addr))).bit0
#define SPLC501_DIN  (*((volatile bit_field*)(PORTC_addr))).bit1
#define PORTD_addr   0x32
#define SPLC501_A0   (*((volatile bit_field*)(PORTD_addr))).bit7
#define SPLC501_CS   (*((volatile bit_field*)(PORTD_addr))).bit4
#define SPLC501_RES  (*((volatile bit_field*)(PORTD_addr))).bit5

//------------------------------------------------------------------------------
void SPLC501_SPIwrite(unsigned char data_command,unsigned char dc) 
{//参数dc=0时表示发命令，dc=1时表示发送数据
  unsigned char nc;
  SPLC501_A0=dc;
  SPLC501_CS=0;  
  for(nc=0;nc<8;nc++)
  {
   SPLC501_CLK=0;
   SPLC501_DIN=(data_command&0x80)==0?0:1;
   data_command<<=1;
   SPLC501_CLK=1;
  }
  SPLC501_CS=1;  
}
//------------------------------------------------------------------------------
void LCD_Init(void) //LCD初始化函数
{unsigned char nc;
 
 SPLC501_RES=0;  //LCD(低电平)复位
 for(nc=100;nc>0;nc--);
 SPLC501_RES=1;     // RES置高
 for(nc=100;nc>0;nc--);
 SPLC501_SPIwrite(0xa2,0);// LCD偏压设置：1/9 BIAS
 SPLC501_SPIwrite(0xa1,0);//列地址选择为从右到左对应0~127;0xa1:列地址从左到右；0xa0:列地址从右到左
 SPLC501_SPIwrite(0xc0,0);// 行地址从上到下为0~63;0xc0:行地址从上到下；0xc8:行地址从下到上
 SPLC501_SPIwrite(0x26,0);// V5电压内部电阻调整设置
 SPLC501_SPIwrite(0x81,0);//亮度调整命令0~63(暗到亮)
 SPLC501_SPIwrite(0x10,0);   //亮度调节为双字节命令,前一个参数控制字的深浅
 SPLC501_SPIwrite(0x2f,0); //上电控制，打开调压器、稳压器和电压跟随
 SPLC501_SPIwrite(0xaf,0); //set display off。0xaf:set display on
 SPLC501_SPIwrite(0x60,0);//其实行设置

}
//-------------------------------------------------------------------------------

void Test(unsigned char a,unsigned char b)//全屏测试
{
  unsigned int i,j,page=0xb0;//指向首页命令
  for(i=0;i<8;i++)
   { SPLC501_SPIwrite(page++,0);//指向对应页
     SPLC501_SPIwrite(0x10,0);//SET COLUMN(列) ADDRESS MSB(高4位)为0
     SPLC501_SPIwrite(0x04,0);//SET COLUMN ADDRESS LSB(低4位)为0
     for(j=0;j<64;j++) 
      {SPLC501_SPIwrite(a,1);
       SPLC501_SPIwrite(b,1);
      } 
   } 
}
/*
//------------------------------------------------------------------------------
void Frame_dis(unsigned char *p)//显示128×64图像函数，指针p指向图片数组首地址
{ unsigned char i,j,page=0xb0;//指向首页命令
  for(i=0;i<8;i++)         //共8页
   { SPLC501_SPIwrite(page++,0);
     SPLC501_SPIwrite(0x10,0);   //设定列地址高4位为0
     SPLC501_SPIwrite(0x04,0);   //设定列地址低4位为0
     for(j=0;j<128;j++)    //共128列
       SPLC501_SPIwrite(*p++,1);
   } 
}
*/
//------------------------------------------------------------------------------
void clrscr(unsigned char page_f, //显示一个字或多个字函数
    unsigned char page_sum,unsigned char column_f,unsigned char column_sum )//x*8
{//参数：指向字首地址，起始页，占多少页，起始列，占多少列
  unsigned char i,j,page=0xb0;//指向首页命令
  for(i=page_f;i<page_sum+page_f;i++)
   { SPLC501_SPIwrite(page+i,0); 
     SPLC501_SPIwrite(0x10|((0xf0&column_f)>>4),0);//设定列地址高4位为0
     SPLC501_SPIwrite(0x04|(0x0f&column_f),0);    //设定列地址低4位为0
     for(j=0;j<column_sum;j++)
      {
	   SPLC501_SPIwrite(0x00,1);
	  }
   }
}  
 
//------------------------------------------------------------------------------
void Display_Words(unsigned char *p,unsigned char page_f, //显示一个字或多个字函数
    unsigned char page_sum,unsigned char column_f,unsigned char column_sum )//x*8
{//参数：指向字首地址，起始页，占多少页，起始列，占多少列
  unsigned char i,j,page=0xb0;//指向首页命令
  for(i=page_f;i<page_sum+page_f;i++)
   { SPLC501_SPIwrite(page+i,0); 
     SPLC501_SPIwrite(0x10|((0xf0&column_f)>>4),0);//设定列地址高4位为0
     SPLC501_SPIwrite(0x04|(0x0f&column_f),0);    //设定列地址低4位为0
     for(j=0;j<column_sum;j++)
      {
	   SPLC501_SPIwrite(*p++,1);
	  }
   }
}  
//------------------------------------------------------------------------------
void Plot(unsigned char *P)//p指向曲线数组
{ volatile unsigned char i,j,x,x1;//,page,temp,move_bit,*p_map_bit;
  //unsigned char dot;
  //unsigned int d,p1,max=0;
  unsigned int d;
 //volatile double c;
  //Test(0,0);
  SPLC501_SPIwrite(0x10,0);//SET COLUMN ADDRESS MSB
  SPLC501_SPIwrite(0x04,0);   //SET COLUMN ADDRESS LSB,
  
  /*for(i=0;i<128;i++)
   { 
     //d=((*P++)<<8)|(*P++);
	 d=(*P++)|((*P++)<<8);
     if(d>max)max=d;
    }
   c=(double)60/max;
    P=p1;  */
  for(i=0;i<128;i++)
   { //d=(*P++)|((*P++)<<8);
     //d=(double)d*c;
	 d=*P;
	 if(d>63)d=63;
	 x=7-d/8;
	 x1=7-(d&7);
	 for(j=0;j<8;j++)
	  {
	   SPLC501_SPIwrite(0xb0+j,0);//
       SPLC501_SPIwrite(0x10|((0xf0&i)>>4),0);//SET COLUMN ADDRESS MSB
       SPLC501_SPIwrite(0x00|(0x0f&i),0);   //SET COLUMN ADDRESS LSB,
	   if(j!=x)
	    SPLC501_SPIwrite(0,1);
	   else 
	    SPLC501_SPIwrite(1<<x1,1); //
	  }
    }
   SPLC501_SPIwrite(0x40,0); //start h
   //SPLC501_SPIwrite(0x0af,0);//set display on
}

//------------------------------------------------------------------------------

#endif
