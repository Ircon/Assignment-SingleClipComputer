STCP_595 EQU P2.0
SHCP_595 EQU P2.1
DS_595   EQU P2.2
HOUR  EQU 30H;存放小时变量
MIN   EQU 31H;存放分钟变量
SEC	  EQU 32H;存放秒钟变量
COUNT EQU 33H;5ms计数加1
ADDR  EQU 34H;位选
DAT   EQU 35H;段选
KEY   EQU 36H;键值
ORG 0000H
LJMP MAIN
ORG 0003H
LJMP X0_INT
ORG 000BH   
LJMP T0_INT
ORG 0100H
MAIN:
	  LCALL UNIT_TIMER
START:	 		   ;初始化一次
      LCALL DISPLAY
	  LJMP  START
UNIT_TIMER:
     MOV  TMOD,#01H;定时器0工作方式1
	 MOV  TH0, #60;赋初值
	 MOV  TL0, #176  
	 SETB TR0;启动定时器0
	 SETB ET0;打开定时器0开关
	 SETB EX0
	 SETB IT0
	 MOV COUNT,#0
	 MOV SEC, #0
	 MOV MIN, #0
	 MOV HOUR,#0
	 MOV R1,  #0
	 MOV KEY, #0
	 SETB EA;打开总开关 
	 RET ;返回    
T0_INT:	;中断服务程序 (获取 时 分 秒)
	 PUSH ACC;
	 PUSH PSW
	 MOV  TH0, #60;赋初值
	 MOV  TL0, #176
	 INC  COUNT;每隔50ms加1
	 POP PSW
	 POP ACC
	 RETI
X0_INT:
      PUSH ACC
	  PUSH PSW
	  LCALL DELAY//延时12ms消抖
	  LCALL DELAY
	  JB INT0,RECOVER1;MOV C,F2H;
	  ;CJNE C,#0,RECOVER 
	  INC  KEY
	  MOV A,KEY
	  CJNE A,#100,RECOVER1 ;CJNE A,#64H,RECOVER
	  MOV KEY,#0
RECOVER1:  
      POP  PSW
	  POP  ACC
	  RETI
DISPLAY:
     MOV  A,COUNT
     CJNE A,#20,RECOVER
	 MOV  COUNT,#0
	 INC  SEC
	 MOV  A, SEC
	 CJNE A, #60,RECOVER
	 MOV  SEC,#0
	 INC MIN
	 MOV A,MIN
	 CJNE A,#60,RECOVER
     MOV MIN,#0
	 INC HOUR
	 MOV A,HOUR
	 CJNE A,#24,RECOVER
	 MOV SEC,#0    ;秒钟变量清除为0
     MOV MIN,#0    ;分钟变量清除为0
     MOV HOUR,#0   ;小时变量清除为0
	 MOV A,KEY
	 ;CJNE A,#100,RECOVER ;CJNE A,#64H,RECOVER    不能放在这 否则出现乱码
	 MOV KEY,#0 
RECOVER:			   //每时每刻都显示
     MOV ADDR,#05H		  
	 MOV A,SEC				   ;显示个位的秒
	 MOV B,#10	;出错，与10H区别 10H表示16
	 DIV AB
	 MOV R1,A
	 MOV DAT,B
	 LCALL SEND_LED
	 
	 MOV ADDR,#04H				;显示十位的秒
	 MOV DAT,R1
	 LCALL SEND_LED
	 
     MOV ADDR,#03H				;显示个位的分
	 MOV A,MIN
	 MOV B,#10
	 DIV  AB
	 MOV R1,A
	 MOV DAT,B
	 LCALL SEND_LED
     MOV ADDR,#02H				;显示十位的分
	 MOV DAT,R1
	 LCALL SEND_LED
	
     MOV ADDR,#01H				;显示个位的时
	 MOV A,HOUR
	 MOV B,#10		  ;有误
	 DIV AB
	 MOV R1,A
	 MOV DAT,B
	 LCALL SEND_LED
     MOV ADDR,#00H				;显示十位的时
	 MOV DAT,R1
	 LCALL SEND_LED	  
	
	 MOV ADDR,#07H				;显示个位的按键数
	 MOV A,KEY
	 MOV B,#10
	 DIV AB
	 MOV DAT,B
	 MOV R1,A
	 LCALL SEND_LED
	 
	 MOV ADDR,#06H				;显示十位的按键数
	 MOV DAT,R1
	 LCALL SEND_LED 
	 RET
SEND_LED:
     MOV DPTR, #SEG_ADDR
	 MOV A,ADDR
	 MOVC A,@A+DPTR
	 LCALL WR_595
	 MOV DPTR, #SEG_DAT
	 MOV A,DAT
	 MOVC A,@A+DPTR
	 LCALL WR_595
	 LCALL OUT_595
	 RET  
OUT_595: 
         CLR	STCP_595	    //;存储寄存器输入 
		 NOP 
		 NOP 
		 SETB   STCP_595 
		 RET  
WR_595:	
	   	MOV	R5,#8H		  	 ;数码管的8位 
LOOP1: 	
		RLC	A 			;带进位位左移
		MOV	DS_595,C	   	//;送数据进595 	//;移位寄存器时钟，根据DATASHEET，送数据进去要一个SCK时钟周期。	
		CLR	SHCP_595
	    NOP 
		NOP  
	    SETB SHCP_595 
	  	DJNZ R5,LOOP1	  	 //;8位数据是否送完 DJNZ只能处理寄存器和地址
	   	RET
DELAY:;12MS:
MOV R5,#48
DEL:
MOV R6,#250
DJNZ R6,$
DJNZ R5,DEL
RET
/*CLR_595:
	MOV A,#0FFH
	LCALL WR_595
	MOV A,#0FFH
	LCALL WR_595
	LCALL OUT_595
RET*/ 
SEG_DAT	: DB 0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H 
SEG_ADDR: DB 01H,02H,04H,08H,10H,20H,40H,80H	
END	
