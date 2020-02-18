//ICC-AVR application builder : 2008-8-10 22:07:54
// Target : M32
// Crystal: 1.0000Mhz

#include <iom32v.h>
#include <macros.h>

void port_init(void)
{
 PORTA = 0x00;
 DDRA  = 0xFF;
 PORTB = 0x00;
 DDRB  = 0xFF;
 PORTC = 0x00; 
 DDRC  = 0xFF;
 PORTD = 0x00;
 DDRD  = 0xFF;
}

//call this routine to initialize all peripherals
void init_devices(void)
{
 //stop errant interrupts until set up
 CLI(); //disable all interrupts
 port_init();

 MCUCR = 0x00;
 GICR  = 0x00;
 TIMSK = 0x00; //timer interrupt sources
 SEI(); //re-enable interrupts
 //all peripherals are now initialized
}

