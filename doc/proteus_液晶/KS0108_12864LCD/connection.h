//************************************************
//本头文件定义了AVR和KS0108B/HD61202的管脚连接
//可以根据自己的硬件连接修改
//         ---北京交通大学电气学院 杨罡 2008.8
//*************************************************
#ifndef CONNECTION_H
#define CONNECTION_H

//*************************************************
//definition of the ports connected to LED
//************************************************
#define CS1          PB0         
#define CS2          PB1
#define D_I          PB2
#define R_W          PB3
#define E            PB4
#define DATA_PORT    PORTA
#define LCDCTRLPORT  PORTB
//************************************************

#endif