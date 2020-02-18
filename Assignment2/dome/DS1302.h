
#include"reg52.h"								
#define uchar unsigned char						
#define uint unsigned int							

sbit T_IO=P3^5;								
sbit T_CLK=P3^6;							
sbit T_RST=P3^7;							
sbit ACC0=ACC^0;							
sbit ACC7=ACC^7;							

void RTI_1302_1Byte(uchar d)					
{
	uchar i;							
	ACC=d;								
	for(i=8;i>0;i--)			
	{
		T_IO=ACC0;					
		T_CLK=1;						
		T_CLK=0;						
		ACC=ACC>>1;					
	}
}

uchar RTO_1302_1Byte()		
{											
	uchar i;								
	for(i=8;i>0;i--)				
	{
		ACC=ACC>>1;						
		ACC7=T_IO;						
		T_CLK=1;							
		T_CLK=0;							
	}
	return(ACC);						
}

void Write_1302(uchar ucAddr,uchar ucDa)			
{
	T_RST=0;								
	T_CLK=0;								
	T_RST=1;								
	RTI_1302_1Byte(ucAddr);	
	RTI_1302_1Byte(ucDa);		
	T_CLK=1;								
	T_RST=0;								
}

uchar Read_1302(uchar ucAddr)				
{
	uchar ucData;						
	T_RST=0;								
	T_CLK=0;								
	T_RST=1;								
	RTI_1302_1Byte(ucAddr);	
	ucData=RTO_1302_1Byte();
	T_CLK=1;								
	T_RST=0;								
	return(ucData);					
}


void Set_1302(uchar *pClock)					
{
	uchar i;									//??
	uchar ucAddr=0x80;						
	Write_1302(0x8e,0x00);				??
	
	for(i=7;i>0;i--)							
	{
		Write_1302(ucAddr,*pClock);	
		pClock++;							//????
		ucAddr+=2;							//DS
	}
	Write_1302(0x8e,0x80);				
}

uchar BCD_Decimal(uchar bcd)		
{
	uchar Decimal;							
	Decimal=bcd>>4;							
	Decimal=Decimal*10+(bcd&=0x0F);			//??????
	return Decimal;							//?????
}

void Get_1302(uchar ucCurtime[])					
{
	uchar i;
	uchar ucAddr=0x81;					
	for(i=0;i<7;i++)						
	{
		ucCurtime[i]=							
			BCD_Decimal(Read_1302(ucAddr));	//??????	
		ucAddr+=2;							//??????,???2
	}
}