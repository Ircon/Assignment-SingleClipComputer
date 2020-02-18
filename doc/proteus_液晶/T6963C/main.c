#include "AVR\IO.H"
#include "T6963C.H"
int main ()
{
	unsigned int i,a=0,c=0;
	unsigned char b=0;
	Lcd_Init();
	Show_Bmp_240x64(bmp);
	TCCR1A=0xff;	
	TCCR1B=0x0A;
	TCNT1=0X0000;
	TIMSK=0X00;
	ETIMSK=0X00;
	DDRB=0xff;
	while(1)
	{
		if(b==0)
		{
			a=a+1;
			if(a>0x3ef)
			{
				b=1;
			};
		}
		else
		{
			a=a-1;
			if(a<2)
			{
				b=0;
			};
		}
		OCR1A=a;
		OCR1B=0x03ff-a;
		OCR1C=OCR1C+1;
		c++;
		OCR1C=c;
		if(c>=0x03f0)
		{c=0;};
		
		i=500;
		while(i--);
	}
}
