#include<reg51.h>         
#include<intrins.h>
typedef unsigned char uchar;
// 引脚设置
sbit SDA 	= P1^0;         
sbit SCL 	= P1^1;          
sbit STCP 	= P2^0;
sbit SHCP 	= P2^1;
sbit DS 	= P2^2;
sbit MADA 	= P0 ^ 0;
sbit UP 	= P0^1;
sbit DOWN 	= P0 ^ 2;

// 全局变量
sbit ON_OFF = 0;
uchar LIM = 100,COUNT = 0,FLAG = 0;


uchar code LED_DIG[]={
	0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90
};

void Dely_US(uchar tim){
	while(tim--);
}
                     
void start(){	
	SDA=1;   
	_nop_(); 
	SCL=1;   
	_nop_();  
	SDA=0;   
	_nop_(); 
	SCL=0;    
	_nop_();
}

void stop(){
	SDA=0;                   
	_nop_();                
	SCL=1;                  
	_nop_();               
	SDA=1;                  
	_nop_();               
}

void checkACK(){
	SCL=1;                        
	_nop_();                      
	while(SDA==1);                
	SCL=0;                      
	_nop_();                     
}

void sendACK(bit ACK){
    if(ACK)SDA=1;           
      else SDA=0;           	
    SCL=1;                   
	_nop_();                
	SCL=0;                   
	_nop_();                
}

void send_byte(uchar date){
	uchar i,temp;                
	temp=date;                  
	for(i=0;i<8;i++){
        if(temp&0x80)SDA=1;      
          else SDA=0;           
	    _nop_();                
		SCL=1;                  
		_nop_();                  
		_nop_();              
		_nop_();                 
		_nop_();                 
		_nop_();                
		SCL=0;                  
	    _nop_();                
		_nop_();              
		_nop_();                 
		_nop_();                 
		_nop_();  
		temp=temp<<1;          
	}
	checkACK();                 
}

uchar receive_byte(){
	uchar i,temp;           
	SDA=1;                  
	_nop_();                
	for(i=0;i<8;i++){
		SCL=1;                         
		_nop_();             
		_nop_();              
		_nop_();                 
		_nop_();                 
		_nop_();
		temp=(temp<<1)|SDA; 
		SCL=0;               
	    _nop_();             
		_nop_();              
		_nop_();                 
		_nop_();                 
		_nop_();       
	}
    sendACK(0);              
	return temp;            
}
uchar Read_AD(uchar chn){
   uchar  ad_data;          
   start();                
   send_byte(0x90);         
   send_byte(0x40|chn);     
   
   start();                 
   send_byte(0x91);        

   ad_data=receive_byte();  
   sendACK(1);             
   stop();                 
   return(ad_data);
}
void HC595(uchar x){
	uchar i;
	for(i=0;i<8;i++){
		SHCP=0;
		if((x<<i)&0x80)
			DS=1;
		else
		DS=0;
		SHCP=1;
	}
}
void send_num(uchar f,uchar s){
	STCP=0; 
	HC595(f);
	HC595(s);
	STCP=1; 
	Dely_US(36);
}
void LED(uchar n){
	send_num(0x20,LED_DIG[n/100]);
	send_num(0x20,0xff);
	
	send_num(0x40,LED_DIG[(n%100)/10]);
	send_num(0x20,0xff);
	
	send_num(0x40,LED_DIG[(n%100)/10]);
	send_num(0x40,0xff);

	
	send_num(0x80,LED_DIG[n%10]);
	send_num(0x80,0xff); 
}
void key_scan(){
	if(UP == 0 || DOWN == 0){
		dely_US(30);
		if(UP == 0){
			LIM = (LIM + 1) % 1000;
		}else if(DONW == 0){
			LIM = (LIM + 999) % 1000;
		}
	}
}
void run_some(uchar buf){
	if(FLAG){
		++COUNT;
		if(COUNT == 532){
			COUNT = 0;
			FLAG = 1;
			MADA = 1;
		}else{
			MADA = 0;
		}
	}else{
		if(buf > TIM && ON_OFF)
			FLAG = 1;
		else
			FLAG = 0;
	}
}
void main(){
	uchar temp;
	MADA = 1;
	while(1){
		temp=Read_AD(0);             
		LED(temp);
		key_scan();
		run_some(buf);
	}
}
