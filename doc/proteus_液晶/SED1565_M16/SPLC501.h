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
{//����dc=0ʱ��ʾ�����dc=1ʱ��ʾ��������
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
void LCD_Init(void) //LCD��ʼ������
{unsigned char nc;
 
 SPLC501_RES=0;  //LCD(�͵�ƽ)��λ
 for(nc=100;nc>0;nc--);
 SPLC501_RES=1;     // RES�ø�
 for(nc=100;nc>0;nc--);
 SPLC501_SPIwrite(0xa2,0);// LCDƫѹ���ã�1/9 BIAS
 SPLC501_SPIwrite(0xa1,0);//�е�ַѡ��Ϊ���ҵ����Ӧ0~127;0xa1:�е�ַ�����ң�0xa0:�е�ַ���ҵ���
 SPLC501_SPIwrite(0xc0,0);// �е�ַ���ϵ���Ϊ0~63;0xc0:�е�ַ���ϵ��£�0xc8:�е�ַ���µ���
 SPLC501_SPIwrite(0x26,0);// V5��ѹ�ڲ������������
 SPLC501_SPIwrite(0x81,0);//���ȵ�������0~63(������)
 SPLC501_SPIwrite(0x10,0);   //���ȵ���Ϊ˫�ֽ�����,ǰһ�����������ֵ���ǳ
 SPLC501_SPIwrite(0x2f,0); //�ϵ���ƣ��򿪵�ѹ������ѹ���͵�ѹ����
 SPLC501_SPIwrite(0xaf,0); //set display off��0xaf:set display on
 SPLC501_SPIwrite(0x60,0);//��ʵ������

}
//-------------------------------------------------------------------------------

void Test(unsigned char a,unsigned char b)//ȫ������
{
  unsigned int i,j,page=0xb0;//ָ����ҳ����
  for(i=0;i<8;i++)
   { SPLC501_SPIwrite(page++,0);//ָ���Ӧҳ
     SPLC501_SPIwrite(0x10,0);//SET COLUMN(��) ADDRESS MSB(��4λ)Ϊ0
     SPLC501_SPIwrite(0x04,0);//SET COLUMN ADDRESS LSB(��4λ)Ϊ0
     for(j=0;j<64;j++) 
      {SPLC501_SPIwrite(a,1);
       SPLC501_SPIwrite(b,1);
      } 
   } 
}
/*
//------------------------------------------------------------------------------
void Frame_dis(unsigned char *p)//��ʾ128��64ͼ������ָ��pָ��ͼƬ�����׵�ַ
{ unsigned char i,j,page=0xb0;//ָ����ҳ����
  for(i=0;i<8;i++)         //��8ҳ
   { SPLC501_SPIwrite(page++,0);
     SPLC501_SPIwrite(0x10,0);   //�趨�е�ַ��4λΪ0
     SPLC501_SPIwrite(0x04,0);   //�趨�е�ַ��4λΪ0
     for(j=0;j<128;j++)    //��128��
       SPLC501_SPIwrite(*p++,1);
   } 
}
*/
//------------------------------------------------------------------------------
void clrscr(unsigned char page_f, //��ʾһ���ֻ����ֺ���
    unsigned char page_sum,unsigned char column_f,unsigned char column_sum )//x*8
{//������ָ�����׵�ַ����ʼҳ��ռ����ҳ����ʼ�У�ռ������
  unsigned char i,j,page=0xb0;//ָ����ҳ����
  for(i=page_f;i<page_sum+page_f;i++)
   { SPLC501_SPIwrite(page+i,0); 
     SPLC501_SPIwrite(0x10|((0xf0&column_f)>>4),0);//�趨�е�ַ��4λΪ0
     SPLC501_SPIwrite(0x04|(0x0f&column_f),0);    //�趨�е�ַ��4λΪ0
     for(j=0;j<column_sum;j++)
      {
	   SPLC501_SPIwrite(0x00,1);
	  }
   }
}  
 
//------------------------------------------------------------------------------
void Display_Words(unsigned char *p,unsigned char page_f, //��ʾһ���ֻ����ֺ���
    unsigned char page_sum,unsigned char column_f,unsigned char column_sum )//x*8
{//������ָ�����׵�ַ����ʼҳ��ռ����ҳ����ʼ�У�ռ������
  unsigned char i,j,page=0xb0;//ָ����ҳ����
  for(i=page_f;i<page_sum+page_f;i++)
   { SPLC501_SPIwrite(page+i,0); 
     SPLC501_SPIwrite(0x10|((0xf0&column_f)>>4),0);//�趨�е�ַ��4λΪ0
     SPLC501_SPIwrite(0x04|(0x0f&column_f),0);    //�趨�е�ַ��4λΪ0
     for(j=0;j<column_sum;j++)
      {
	   SPLC501_SPIwrite(*p++,1);
	  }
   }
}  
//------------------------------------------------------------------------------
void Plot(unsigned char *P)//pָ����������
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
