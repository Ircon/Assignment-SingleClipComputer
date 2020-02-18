#define MAIN_C
/************************************************************************
;连线图: 液晶屏分为4 行*15列汉字使用间接操作方式
;*LCM--------------------------ATmega16*  
;*D0～D7                       PB0～PB7*
*CD                            PA2*
*RD                            PA1*
*WR                            PA0*
;************************************************************************/
/******************************************************/
/* 本程序在晓奇程序的基础上改编*/
/* Email: mymach@tom.com */
/******************************************************/

#include "..\includes\includes.h"
#include <util/delay.h>
#define TRUE 1
#define FALSE 0

volatile unsigned char NYRyang[]="2010年03月04日 星期六";
volatile unsigned char NYRyin[]="1999年11月25日";
volatile unsigned char Time[]="12时00分00秒";
volatile unsigned char SetTime[]="时间设置: 秒";
volatile unsigned char Timechange=FALSE;
volatile unsigned char Displaychange=0;
volatile unsigned char DisplayTime=TRUE;
volatile unsigned char DisplayYear=TRUE;
volatile signed char Second=0;
volatile signed char Minute=0;
volatile signed char Hour=12;
volatile signed int Year=0;
volatile signed char Month=0;
volatile signed char Day=0;
volatile unsigned char Setmodebit=0;
#define leapYear_Sign ((Year%4==0)&&(Year%100!=0)||(Year%400==0))
void getweekday(unsigned char weekday)
{
	switch(weekday){
		case 0:	NYRyang[19]=('日'&0xff00)>>8;
				NYRyang[20]=('日'&0x00ff);
				break;
		case 1:	NYRyang[19]=('一'&0xff00)>>8;
				NYRyang[20]=('一'&0x00ff);
				break;
		case 2:	NYRyang[19]=('二'&0xff00)>>8;
				NYRyang[20]=('二'&0x00ff);
				break;
		case 3:	NYRyang[19]=('三'&0xff00)>>8;
				NYRyang[20]=('三'&0x00ff);
				break;
		case 4:	NYRyang[19]=('四'&0xff00)>>8;
				NYRyang[20]=('四'&0x00ff);
				break;	
		case 5:	NYRyang[19]=('五'&0xff00)>>8;
				NYRyang[20]=('五'&0x00ff);
				break;
		case 6:	NYRyang[19]=('六'&0xff00)>>8;
				NYRyang[20]=('六'&0x00ff);
				break;
		default:break;
		}
}
void GetYinLiNian(void)
{
	unsigned char NLyear[4];
	GetChinaCalendar(Year,Month,Day,NLyear);  //NLyear={20,06,12,21}
	NYRyin[0]=NLyear[0]/10+'0';
	NYRyin[1]=NLyear[0]%10+'0';
	NYRyin[2]=NLyear[1]/10+'0';
	NYRyin[3]=NLyear[1]%10+'0';
	NYRyin[6]=NLyear[2]/10+'0';
	NYRyin[7]=NLyear[2]%10+'0';
	NYRyin[10]=NLyear[3]/10+'0';
	NYRyin[11]=NLyear[3]%10+'0';
}
void time2_RTCinit(void)
{
	TCCR2=0X07;
	ASSR=0X08;
	TCNT2=0Xdf;
	while(ASSR&0X05);
	TIMSK=0X40;
	asm("sei");
}
void key_deal(void)
{
	if(PIND&0X0f){
			_delay_ms(50);
		if(PIND&0X0f){			
			if(PIND&0X01){
				Setmodebit++;
				if(	Setmodebit>6)
					Setmodebit=1;
				Timechange=TRUE;				
				
			}
			else if((PIND&0X02)&&(Timechange==TRUE)){
				Displaychange=2;
				if(Setmodebit==1){
					Second++;
					if(Second==60)
						Second=0;
				}
				else if(Setmodebit==2){		
					Minute++;
					if(Minute==60)
						Minute=0;
				}
				else if(Setmodebit==3){
					Hour++;
					if(Hour==24)
						Hour=0;
				}
				else if(Setmodebit==4){
					if((Month==2)&&((leapYear_Sign==0)&&(Day>=29)))
						Day=0;
					if((Month==2)&&((leapYear_Sign==1)&&(Day>=28)))
						Day=0;
					if(((Month==1)||(Month==3)||(Month==5)||(Month==7)||(Month==8)||(Month==10)||(Month==12))&&(Day>=31))
						Day=0;
					if(((Month==4)||(Month==6)||(Month==9)||(Month==11))&&(Day>=30))
						Day=0;
					Day++;
					
				}
				else if(Setmodebit==5){
					if(Month==12)
						Month=1;
					Month++;
				}
				else{
					if(Year==2099)
						Year=1900;
					Year++;
				}
			}
			else if((PIND&0X04)&&(Timechange==TRUE)){
				Displaychange=2;
				if(Setmodebit==1){
					Second--;
					if(Second<0)
						Second=59;
				}
				else if(Setmodebit==2){		
					Minute--;
					if(Minute<0)
						Minute=59;
				}
				else if(Setmodebit==3){
					Hour--;
					if(Hour<0)
						Hour=23;
				}
				else if(Setmodebit==4){					
					if((Month==2)&&((leapYear_Sign==0)&&(Day==1)))
						Day=30;
					if((Month==2)&&((leapYear_Sign==1)&&(Day==1)))
						Day=29;
					if(((Month==1)||(Month==3)||(Month==5)||(Month==7)||(Month==8)||(Month==10)||(Month==12))&&(Day==1))
						Day=32;
					if(((Month==4)||(Month==6)||(Month==9)||(Month==11))&&(Day==1))
						Day=31;
					Day--;
				}
				else if(Setmodebit==5){
					if(Month==1)
						Month=13;
					Month--;
				}
				else{
					if(Year==1900)
						Year=2099;
					Year--;
				}
			}
			else{
				cls();
				Setmodebit=0;
				Timechange=FALSE;
			}
			DisplayTime=TRUE;
			DisplayYear=TRUE;
		}
	}
}
void DiaplaySetTime(void)
{
	if(Timechange==FALSE) return;
	switch(Setmodebit){
		case 1: 	SetTime[10]=('秒'&0xff00)>>8;
				SetTime[11]=('秒'&0x00ff);
				break;
		case 2:	SetTime[10]=('分'&0xff00)>>8;
				SetTime[11]=('分'&0x00ff);
				break;
		case 3:	SetTime[10]=('时'&0xff00)>>8;
				SetTime[11]=('时'&0x00ff);
				break;			
		case 4:	SetTime[10]=('日'&0xff00)>>8;
				SetTime[11]=('日'&0x00ff);
				break;
		case 5:	SetTime[10]=('月'&0xff00)>>8;
				SetTime[11]=('月'&0x00ff);
				break;
		case 6:	SetTime[10]=('年'&0xff00)>>8;
				SetTime[11]=('年'&0x00ff);
				break;
		default:	break;
	}
	dprintf(0,1,SetTime);	
			
}
int main(void) // 测试用
{
    	unsigned char i;
	unsigned char week;	
	
	unsigned char data[3];
	unsigned char JQdate;	
	char str[15];
	
	DDRC=0X3F;
    	DDRA = 0xFF;
    	PORTA = 0xff;
	
	Year=(NYRyang[0]-'0')*1000+(NYRyang[1]-'0')*100+(NYRyang[2]-'0')*10+(NYRyang[3]-'0');
	Month=(NYRyang[6]-'0')*10+(NYRyang[7]-'0');
	Day=(NYRyang[10]-'0')*10+(NYRyang[11]-'0');	
    	shortdelay(1200);
    	fnLCMInit();
    	cls();
    	cursor(0,0);
	time2_RTCinit();	
	
	while(1){
		key_deal();
		DiaplaySetTime();
//		_delay_ms(220);
//		Hour+=23;
//		Second=60;
//		Minute=59;
		//Second+=60;
		if(Second==60){
			Second=0;
			Minute++;			
			if(Minute==60){
				Minute=0;
				Hour++;
				if(Hour>=24){
					DisplayYear=TRUE;
					Hour=0;
					if(((Month==2)&&(((leapYear_Sign==0)&&(Day==29))||((leapYear_Sign==1)&&(Day==28))))||
						(((Month==1)||(Month==3)||(Month==5)||(Month==7)||(Month==8)||(Month==10)||(Month==12))&&(Day==31))||
						(((Month==4)||(Month==6)||(Month==9)||(Month==11))&&(Day==30))){
						Day=1;
						if(Month<12)
							Month++;
						else{
							Month=1;
							Year++;
						}
					}
					else
						Day++;
				}
			}
			
		}
		if(DisplayTime==TRUE){
			DisplayTime=FALSE;
			Time[9]=Second%10+'0';
			Time[8]=Second/10+'0';
			Time[5]=Minute%10+'0';
			Time[4]=Minute/10+'0';
			Time[1]=Hour%10+'0';
			Time[0]=Hour/10+'0';	
			dprintf(0,3,Time);
		}
		if(DisplayYear==TRUE){	
			DisplayYear=FALSE;
			NYRyang[0]=Year/1000+'0';
			NYRyang[1]=Year%1000/100+'0';
			NYRyang[2]=Year%1000%100/10+'0';
			NYRyang[3]=Year%1000%100%10+'0';
			NYRyang[6]=Month/10+'0';
			NYRyang[7]=Month%10+'0';
			NYRyang[10]=Day/10+'0';
			NYRyang[11]=Day%10+'0';
			GetYinLiNian();
			GetChinaCalendarStr(Year,Month,Day,str);      //str   ={"丙戌年腊月廿一"}
			GetWeek(Year,Month,Day,&week);   //week  =4
			getweekday(week);
			GetJieQi(Year,Month,Day,&data[1]); //JQdate=4 意思为本月的4号是个节气
			
			dprintf(0,4,NYRyang);
			dprintf(0,5,NYRyin);		
			dprintf(0,6,str);
			GetJieQiStr(Year,Month,Day,str);     //str   ={"离雨水还有11天"}
			dprintf(0,7,str);
		}
	}	

}
ISR(TIMER2_OVF_vect)
{
	TCNT2=0Xdf;
	while(ASSR&0X04);
	if(Timechange==FALSE)
		Second++;
	else
		Displaychange++;
	DisplayTime=TRUE;
}
