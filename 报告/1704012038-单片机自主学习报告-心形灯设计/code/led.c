#include <reg51.h>
sbit LED1 = P1 ^ 0;
sbit LED2 = P1 ^ 1;

sbit P0_0 = P0 ^ 0;
sbit P0_1 = P0 ^ 1;
sbit P0_2 = P0 ^ 2;
sbit P0_3 = P0 ^ 3;
sbit P1_7 = P1 ^ 7;
unsigned char SPEED = 0;
unsigned char IS_OK = 0;
unsigned char CNT = 0;
// fun

void dely_1s();
void frist_1();
void dely();



main(){
	frist_1();
	while(1){
		
		if(!P0_0){ // 开始
			IS_OK = 1;
		}else if(!P0_1){ // 暂停
			IS_OK = 0;
			LED1 = 1;
			LED2 = 1;
		}
		P0_2 = P0_0;
		P0_3 = P0_1;
		P1_7 = IS_OK;
		
		P2 = SPEED;
	}
}

void frist_1(){
	
	int i;
		
	LED1 = 0;
	LED2 = 0;
	for(i = 0 ; i< 5 ; ++i)
		dely_1s();
	
	LED1 = 1;
	LED2 = 1;
	for(i = 0 ; i < 5 ; ++i)
		dely_1s();
#pragma asm
	MOV  TMOD,#01H	;定时器0工作方式1
	MOV  TH0, #60H	;赋初值
	MOV  TL0, #176  
	SETB TR0		;启动定时器0
	SETB ET0		;打开定时器0开关
	SETB EX0
	SETB IT0
	SETB EX1
	SETB IT1
	SETB EA 		;总开关
#pragma endasm
}
void dely(){
	int i ;
	for(i = 0 ; i < 10000 ; ++i);
}
void dely_1s(){
// djnz 2 个机器周期
// PUSH 2
// POP  2
#pragma asm
	PUSH ACC
	PUSH PSW
	
	MOV ACC,#4
	
LOP3:
	PUSH ACC
	MOV ACC,#249
LOP2:	

	PUSH ACC
	MOV ACC,#248
LOP1:
	DJNZ ACC,LOP1
	POP ACC
	
	DJNZ ACC,LOP2
	POP ACC
	
	DJNZ ACC,LOP3
	
	POP PSW
	POP ACC
	
#pragma endasm
}

// 中断
void INT0_INC_SPEED(void) interrupt 0 {
	++SPEED;
	SPEED %= 5;
}
void TIMER(void) interrupt 1 {
	if(IS_OK != 0){
#pragma asm
		MOV  TH0, #60H	;赋初值
		MOV  TL0, #176 
#pragma endasm
		LED1 = 1;
		LED2 = 1;
		return;
	}
			
	if(CNT > SPEED){
		CNT = 0;
		if(SPEED == 0){
			LED1 = 0;
			LED2 = 0;
		}else if(SPEED == 1){
			LED1 = !LED1;
		}else if(SPEED == 2){
			LED2 = !LED2;
		}else if(SPEED == 5){
			LED1 = 0;
			LED2 = 0;
		}else{
			LED1 = !LED1;
			LED2 = !LED2;
		}
	}else{
		++CNT;
	}
#pragma asm
	MOV  TH0, #60hH	;赋初值
	MOV  TL0, #176 
#pragma endasm
}
void INT0_DEC_SPEED(void) interrupt 2 {
	--SPEED;
	SPEED %= 5;
}