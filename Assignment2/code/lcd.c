#include <reg51.h>
#include "math.h"
#define test

#define DATA_PORT (P0)
#define OUT_PORT (P1)

#define OPEN_SHOW_WORD (0x3F)
#define CLOSE_SHOW_WORD (0x3E)

typedef unsigned int uint;
typedef unsigned char uchar;

sbit CS1 = P2 ^ 0;
sbit CS2 = P2 ^ 1;
sbit RW = P2 ^ 2;
sbit RS = P2 ^ 3;
sbit E = P2 ^ 4;

sbit LED1 = P2 ^ 5;
sbit LED2 = P2 ^ 6;
sbit LED3 = P2 ^ 7;
//������ʱ������LCD��д����,�Լ���ʼ��
void delay_us(uint t)							//us����ʱ����
{
	for( ;t>0;t--);								
}

uchar code num[][16]=		//????:??12,?x?=8x16,????,????, ??
{
	0xFF,0x1F,0xEF,0xF7,0xF7,0xEF,0x1F,0xFF,0xFF,0xF0,0xEF,0xDF,0xDF,0xEF,0xF0,0xFF,//0,0
	0xFF,0xEF,0xEF,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xDF,0xDF,0xC0,0xDF,0xDF,0xFF,0xFF,//1,1
	0xFF,0x8F,0xF7,0xF7,0xF7,0x77,0x8F,0xFF,0xFF,0xCF,0xD7,0xDB,0xDD,0xDE,0xCF,0xFF,//2,2
	0xFF,0xCF,0xF7,0x77,0x77,0xB7,0xCF,0xFF,0xFF,0xE7,0xDF,0xDF,0xDF,0xEE,0xF1,0xFF,//3,3
	0xFF,0xFF,0x3F,0xDF,0xEF,0x07,0xFF,0xFF,0xFF,0xF8,0xFB,0xDB,0xDB,0xC0,0xDB,0xFF,//4,4
	0xFF,0x07,0xF7,0x77,0x77,0xF7,0xF7,0xFF,0xFF,0xE6,0xDE,0xDF,0xDF,0xEE,0xF1,0xFF,//5,5
	0xFF,0x1F,0xEF,0x77,0x77,0xE7,0xFF,0xFF,0xFF,0xF0,0xEE,0xDF,0xDF,0xEE,0xF1,0xFF,//6,6
	0xFF,0xC7,0xF7,0xF7,0x37,0xC7,0xF7,0xFF,0xFF,0xFF,0xFF,0xC0,0xFF,0xFF,0xFF,0xFF,//7,7
	0xFF,0x8F,0x77,0xF7,0xF7,0x77,0x8F,0xFF,0xFF,0xE3,0xDD,0xDE,0xDE,0xDD,0xE3,0xFF,//8,8
	0xFF,0x1F,0xEF,0xF7,0xF7,0xEF,0x1F,0xFF,0xFF,0xFF,0xCE,0xDD,0xDD,0xEE,0xF0,0xFF,//9,9
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//?10
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,//-,11
	0xFF,0xFF,0xFF,0x3F,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xCF,0xCF,0xFF,0xFF,0xFF,//:,12
	/*--  文字:  1  --*/
	/*--  宋体12;  此字体下对应的点阵为：宽x�?8x16   --*/
	0x00,0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,//13
	/*--  文字:  2  --*/
	/*--  宋体12;  此字体下对应的点阵为：宽x�?8x16   --*/
	0x00,0x70,0x08,0x08,0x08,0x08,0xF0,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//14

	/*--  文字:  3  --*/
	/*--  宋体12;  此字体下对应的点阵为：宽x�?8x16   --*/
	0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x20,0x21,0x21,0x22,0x1C,0x00,//15

	/*--  文字:  4  --*/
	/*--  宋体12;  此字体下对应的点阵为：宽x�?8x16   --*/
	0x00,0x00,0x80,0x40,0x30,0xF8,0x00,0x00,0x00,0x06,0x05,0x24,0x24,0x3F,0x24,0x24,//16

	/*--  文字:  5  --*/
	/*--  宋体12;  此字体下对应的点阵为：宽x�?8x16   --*/
	0x00,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x20,0x20,0x20,0x11,0x0E,0x00,//17
};

//״̬��飬LCD�Ƿ�æ
void check_busy()		
{
	uchar dat;									//״̬��Ϣ���ж��Ƿ�æ��
	RS=0; 								//ѡ��ָ��Ĵ��� 
	RW=1; 								//ѡ���ȡ��ʽ 
	do										//�����²���
	{
		DATA_PORT=0x00;							//����������0
		E=1;							//ʹ�ܶ���,״ֵ̬������
		delay_us(2);							//��ʱ,�ȴ������ȶ�
		dat=DATA_PORT;							//��ȡ���߲��������
		E=0;							//ֹͣ��ȡ
		dat=0x80 & dat; 						//������7λΪ0ʱ�ſɲ���(�б�busy�ź�)
	}
	while(!(dat==0x00));						//��busy��Ϊ0,��ѭ��;Ϊ0�����
}

//д����cmdcode��LCD����
void write_com(uchar cmdcode)
{
	check_busy();								//���LCD�Ƿ�æ
	RS=0;								//ѡ��ָ��Ĵ���
	RW=0;								//ѡ��д�뷽ʽ
	P0=cmdcode;								//ָ��������
	delay_us(2);								//��ʱ,��ָ�������ȶ�
	E=1;								//ʹ��д��
	delay_us(2);								//��ʱ��ȷ��д������
	E=0;								//ֹͣд��
}

//д���ݵ�LCD����
void write_DATA_PORT(uchar lcdDATA_PORT)
{
	check_busy();								//���LCD�Ƿ�æ
	RS=1;								//ѡ�����ݼĴ���
	RW=0;								//ѡ��д�뷽ʽ
	P0=lcdDATA_PORT;								//����������
	delay_us(2);								//��ʱ,�������ȶ�
	E=1;								//ʹ��д��
	delay_us(2);								//��ʱ,ȷ��д������
	E=0;								//ֹͣд��
}

//��LCD������
uchar read_DATA_PORT()
{
	uchar lcdat;								//�����������
	check_busy();								//���LCD�Ƿ�æ
	RS=1;								//ѡ�����ݼĴ���
	RW=1;								//ѡ���ȡ��ʽ
	E=0;								//׼����
	E=1;								//ʹ�����
	delay_us(2);								//��ʱ,�������ȶ�
	lcdat=DATA_PORT;								//��ȡ�˿�ֵ
	delay_us(2);								//��ʱ,ȷ��������
	E=0;								//ֹͣY��ȡ
	return lcdat;								//��������ֵ
}

//������ѡ��,
void select_scr(uchar ss)
{
	switch(ss)								//����ѡ�������ж�
	{
		case 0: CS1=0;CS2=0;break;		//ѡ��ȫ������
		case 1: CS1=0;CS2=1;break;		//ѡ����������
		case 2: CS1=1;CS2=0;break;		//ѡ����������
		case 3: CS1=1;CS2=1;break;		//�������޲���
	}
}

//���LCD�ڴ����,
void Clr_Scr(uchar ss)							//������Ļ��ʾ
{
	uchar j,k;									//����ҳ���б���
	select_scr(ss);								//ѡ����Ļ
 	for(k=0;k<8;k++)							//����ҳ��0-7����8ҳ
 	{
		write_com(0xb8+k);					//д��ҳ��ַ
  		for(j=0;j<64;j++)						//��������0-63���е�ַ�Զ���1
		{
      		write_com(0x40+j);				//д���е�ַ
			write_DATA_PORT(0x00);					//д��հ����ݣ�
		}
	}
}

//��ʼ��LCD����ͬ��Ӧ�ã���ʼ��������ͬ��
void init_lcd(uchar ss)							//LCD��ʼ������
{
	check_busy();								//���LCD�Ƿ�æ
	select_scr(ss);								//ѡ����Ļ
	Clr_Scr(ss);								//LCD����Ļ��ʾ
	write_com(0x3e);			  				//����ʾ
	write_com(0xb8+0);				  		//����:��ʼҳ��ַ
	write_com(0xc0+0);			  			//����:��ʼ�е�ַ
	write_com(0x40+0);			  			//����:��ʼ�е�ַ
	write_com(0x3f);				  			//����ʾ
}

//��ʾ������

//ָ����ַ������ʾ��������hzk[][32]��������1~4������(16*16)										
//����:	ss=0~2; page=0~7; col=0~63; hzk����hzk[][32]; n=1~4
void hz_Disp16(uchar ss,uchar page,uchar column,uchar hzk[][32],uchar n)	
{
	uchar k,p,c;								//����:������,ҳ,��
	select_scr(ss);								//ѡ����Ļ
	for(k=0;k<n;k++)							//�����ָ���ѭ��д��
	{
		for(p=0;p<2;p++)						//ÿ���ַ�ռ 2 ҳ,����д2��
		{
			write_com(0xb8+page+p);		   	//д����ʾҳ��ַ(page,page+1)
			write_com(0x40+column+16*k);		//д����ʾ�е�ַ(��ʼ��,0~16~32~48)
			for(c=0;c<16;c++)					//����16��(��,�°��ָ�16������)
			write_DATA_PORT(hzk[k][16*p+c]);			//ÿҳд��16���ֽڵĺ��ֱ�������
		}
	}
}

//�ظ���ʾ��ά����szk[][16]����ͬ�ַ�1~8��
void sz_Disp1(uchar ss,uchar page,uchar column,uchar szk[][16],uchar n)
{											//����: ��,ҳ���С��롢�ַ���
	uchar i=0,j=0,k=0;				   			//����: ҳ,��,�ַ���
	select_scr(ss);								//ѡ����Ļ
	for(k=0;k<n;k++)
	{
		for(i=0;i<2;i++)					   	//ÿ���ַ�ռ 2 ҳ,����д2��
		{
			write_com(0xb8+page+i);		   	//д����ʾҳ��ַ(pag,pag+1)
			write_com(0x40+column+8*k);		//д����ʾ�е�ַ(��ʼ��)
			for(j=0;j<8;j++) 				   	//����8��
			write_DATA_PORT(szk[0][8*i+j]);			//д��8���ֽ�����
		}
	}
}

//����ͼ����ʾ����������������,ҳ,��,���ݣ�
void picture_disp12(uchar ss,uchar page,uchar column,uchar table[8][64])
{
	uchar i,j;									//ҳ,�б���
	select_scr(ss);								//ѡ����
  	if(column>=64)							//����е�ַ���ڵ���64
	column=column-64; 						//���׵�ַ�Դ�0x40��ʼ��ʾ
    for(i=0;i<8;i++)							//��8ҳд��
    {  
		write_com(0xb8+page+i);				//дҳ��ַ
        write_com(0x40+column);				//д�е�ַ
      	for(j=0;j<64;j++)						//����д��0~63��
        {
           write_DATA_PORT(table[i][j]);				//дͼ������
        }
    }
}

//��ͼ������

//���׹��ܴ�㺯��,����ָ�����껭��	color=1���ڣ�color=0����
void LCD_DrawDot(uchar x,uchar y,uchar color)//xˮƽ���꣬y��ֱ���꣬color��ʾ��ɫ 
{	
	char dat[8],Byte[8];  						//�������ֽ�����
	uchar j;  								//ѭ������
 	for(j=0;j<8;j++) dat[j]=0x00;     			//����Ԫ�ظ���ֵ 0x00

	if(x<64)									//������С��64
	select_scr(1);								//ѡ�������  
	else  									//��������ڵ���64
	{select_scr(2);x-=64;}						//ѡ���Ұ���,���������Ϊ0~63
	for(j=1;j<7;j++)  							//ÿ��8ҳ(0~7),8���ֽ�
	{
		write_com(0xb8+j);    				//дҳ��ַ
		write_com(0x40+x);    				//д�е�ַ
		read_DATA_PORT();							//�ն�һ��
		Byte[j]=read_DATA_PORT();					//����һ�ֽ�����
	 
		if(color)								//���Ҫ��ʾ��
		{
			dat[y>>3]|=0x01<<(y&0x07);		//yֵ����ҳ��Ӧ�ֽ�λ�� 1
			Byte[j]|=dat[j];					//����ԭ����("��"����)
		}					
		else 								//���Ҫ������
		{
			dat[y>>3]&=~(0x01<<(y&0x07));		//yֵ����ҳ��Ӧ�ֽ�λ�� 0
			Byte[j]&=dat[j];					//����ԭ����("��"����)
		}
		write_com(0x40+x);    				//��д�е�ַ(��������ı�)
		write_DATA_PORT(Byte[j]); 					//д����������ֽ�����
	}
}


void dynamic_display()							//��̬��ʾ����ʱ���µ���ֵ��
{
	sz_Disp1(2,0, 0,num[1],1);			//����0ҳ�� 0����ʾ'��ʮλ'
	//sz_Disp1(2,0, 8,num[2],1);			//����0ҳ�� 8����ʾ'���λ'
	//sz_Disp1(2,0,24,num[3],1);			//����0ҳ��24����ʾ'��ʮλ'
	//sz_Disp1(2,0,32,num[4],1);			//����0ҳ��32����ʾ'�¸�λ'
	//sz_Disp1(2,0,48,num[5],1);			//����0ҳ��48����ʾ'��ʮλ'
	//sz_Disp1(2,0,56,num[6],1);			//����0ҳ��56����ʾ'�ո�λ'
	/*				
	sz_Disp1(2,4, 0,num[7],1);			//����4ҳ�� 0����ʾ'ʱʮλ'
	sz_Disp1(2,4, 8,num[8],1);			//����4ҳ�� 8����ʾ'ʱ��λ'
	sz_Disp1(2,4,24,num[9],1);			//����4ҳ��24����ʾ'��ʮλ'
	sz_Disp1(2,4,32,num[1],1);			//����4ҳ��32����ʾ'�ָ�λ'
	sz_Disp1(2,4,48,num[2],1);			//����4ҳ��48����ʾ'��ʮλ'
	sz_Disp1(2,4,56,num[3],1);			//����4ҳ��56����ʾ'���λ'
	*/
}
void fixed_display() 							//�̶���ʾ(����ʾ��Ļ��ҳ,��,����,������λ)
{
	sz_Disp1(1,0,16,num[11],1);					//����0ҳ��16����ʾ'-'
	sz_Disp1(1,0,40,num[11],1);					//����0ҳ��40����ʾ'-'
	//hz_Disp16(1,2,0,HZ[8],2);					//����2ҳ��0����ʾ"����"
	sz_Disp1(1,2,32,num[10],2);					//����2ҳ��32�����ڿ�"��" 2��
	sz_Disp1(1,4,16,num[12],1);					//����4ҳ��16����ʾ':'
	sz_Disp1(1,4,40,num[12],1);					//����4ҳ��40����ʾ':'
	//hz_Disp16(1,6,0,HZ[10],2);					//����6ҳ��0����ʾ"����"
	//hz_Disp16(1,6,48,HZ[0],1);					//����6ҳ��48����ʾ"��"
	
	//hz_Disp16(2,0,0,HZ[18],4);					//������һ��ʱ������
	//hz_Disp16(2,0,0,HZ[20],2);					//����1
	sz_Disp1(2,0,34,num[13],1);
	//hz_Disp16(2,2,0,HZ[20],2);					//����2
	sz_Disp1(2,2,34,num[14],1);
	//hz_Disp16(2,4,0,HZ[20],2);					//����3
	sz_Disp1(2,4,34,num[15],1);
}
void main(void){
	init_lcd(0);
	wait_busy();
		//dynamic_display();
		
	while(1){		
		fixed_display();
	}
	return;
}