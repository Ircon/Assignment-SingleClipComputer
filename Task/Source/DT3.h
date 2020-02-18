#include<Lang.h>         
#include<intrins.h>
typedef unsigned char uchar;
// “˝Ω≈…Ë÷√
sbit SDA 	= P1^0;         
sbit SCL 	= P1^1;          
sbit MADA 	= P0 ^ 0;

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