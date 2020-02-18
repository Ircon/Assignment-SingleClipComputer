#ifndef _HEAD_DS18B20_
#define _HEAD_DS18B20_

sbit DQ = P1^1;                				

void DS18_delay(int useconds) 
{
	int s;
	for (s=0; s<useconds;s++);
}

unsigned char ow_reset(void) 
{
	unsigned char presence;
	DQ = 0;
	DS18_delay(29);
	DQ = 1;
	DS18_delay(3);
	presence = DQ;
	DS18_delay(25);
	return(presence);
}

void write_bit(char bitval) 
{
	DQ = 0;
	if(bitval==1) DQ =1;
	DS18_delay(5);
	DQ = 1;
}

void ds18write_byte(char val) 
{
	unsigned char i;
	unsigned char t;
	for (i=0; i<8; i++)
	{
		t =val & 0x01;
		write_bit(t); 
		val>>= 1;
	}
	DS18_delay(5);
}

unsigned char read_bit(void) 
{
	unsigned char i;
	DQ = 0;
	DQ = 1;
	for (i=0; i<3; i++);
	return(DQ);
}

unsigned char DSread_byte(void) 
{
	unsigned char i;
	unsigned char value = 0;
	for (i=0;i<8;i++)
	{
		if(read_bit())
		value|=0x01<<i;
		DS18_delay(6);
	}
	return(value);
}

unsigned int ReadTemperature(void) 
{
	unsigned char get[10];
	unsigned char temp_msb,temp_lsb;
	unsigned int t;
	unsigned char k;
	ow_reset();
	ds18write_byte(0xCC);
	ds18write_byte(0x44);
	DS18_delay(5);
	ow_reset();
	ds18write_byte(0xCC);
	ds18write_byte(0xBE);
	for (k=0;k<2;k++)
	{get[k]=DSread_byte();}

	temp_msb = get[1];
	temp_lsb = get[0];
	t=temp_msb*256+temp_lsb;
	t=t&0x0ff0;
	if(t<0xff&&t>0xf0)
	t=(-1)*t;
	return t>>4;
}

#endif