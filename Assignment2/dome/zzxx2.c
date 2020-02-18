#include"Hzimo.h"
#include"KS0108.h"
#include<reg52.h>	

void fixed_display() 							
{
	sz_Disp1(2,0,16,num[11],1);
	sz_Disp1(2,0,40,num[11],1);
	hz_Disp16(2,2,0,HZ[8],2);
	sz_Disp1(2,2,32,num[10],2);
	sz_Disp1(2,4,16,num[12],1);
	sz_Disp1(2,4,40,num[12],1);
}

void dynamic_display()
{
	sz_Disp1(2,0, 0,num[1],1);
	sz_Disp1(2,0, 8,num[9],1);
	sz_Disp1(2,0,24,num[1],1);
	sz_Disp1(2,0,32,num[2],1);
	sz_Disp1(2,0,48,num[1],1);
	sz_Disp1(2,0,56,num[6],1);
	hz_Disp16(2,2,48,HZ[1],1);
	
	sz_Disp1(2,4, 0,num[1],1);
	sz_Disp1(2,4, 8,num[2],1);
	sz_Disp1(2,4,24,num[1],1);
	sz_Disp1(2,4,32,num[2],1);
	sz_Disp1(2,4,48,num[3],1);
	sz_Disp1(2,4,56,num[4],1);
}
void delayms(uint tim){
	uint x,y;
	for(x = 0 ; x < tim ; ++x)
		for(y = 0 ; y < 212 ; ++y);
}
void main(void)
{

	init_lcd(0);								
	Clr_Scr(0);									
	while(1)									
	{
		fixed_display();							
		dynamic_display();
	}
}