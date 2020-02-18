#ifndef T6963_H
#define T6963_H


#ifdef T6963_C
// ASCII 字符控制代码解释定义
#define STX 0x02
#define ETX 0x03
#define EOT 0x04
#define ENQ 0x05
#define BS 0x08
#define CR 0x0D
#define LF 0x0A
#define DLE 0x10
#define ETB 0x17
#define SPACE 0x20
#define COMMA 0x2C
#define TRUE 1
#define FALSE 0
#define HIGH 1
#define LOW 0
 
#define LCD_OUT    PORTB
#define LCD_IN     PINB
#define dir_datap  DDRB

#define dir_conp   DDRA
#define port_conp  PORTA

#define LCM 2  //数据寄存器、控制寄存器选择
#define RD 1
#define WR 0

// T6963C 命令定义
#define LC_CUR_POS 0x21 // 光标位置设置
#define LC_CGR_POS 0x22 // CGRAM 偏置地址设置
#define LC_ADD_POS 0x24 // 地址指针位置
#define LC_TXT_STP 0x40 // 文本区首址
#define LC_TXT_WID 0x41 // 文本区宽度
#define LC_GRH_STP 0x42 // 图形区首址
#define LC_GRH_WID 0x43 // 图形区宽度
#define LC_MOD_OR 0x80 // 显示方式逻辑或
#define LC_MOD_XOR 0x81 // 显示方式逻辑异或
#define LC_MOD_AND 0x82 // 显示方式逻辑与
#define LC_MOD_TCH 0x83 // 显示方式文本特征
#define LC_DIS_SW 0x90 // 显示开关D0=1/0:光标闪烁启用/禁用
// D1=1/0:光标显示启用/禁用
// D2=1/0:文本显示启用/禁用
// D3=1/0:图形显示启用/禁用
#define LC_CUR_SHP 0xA0 // 光标形状选择0xA0-0xA7表示光标占的行数
#define LC_AUT_WR 0xB0 // 自动写设置
#define LC_AUT_RD 0xB1 // 自动读设置
#define LC_AUT_OVR 0xB2 // 自动读/写结束
#define LC_INC_WR 0xC0 // 数据一次写地址加1
#define LC_INC_RD 0xC1 // 数据一次读地址加1
#define LC_DEC_WR 0xC2 // 数据一次写地址减1
#define LC_DEC_RD 0xC3 // 数据一次读地址减1
#define LC_NOC_WR 0xC4 // 数据一次写地址不变
#define LC_NOC_RD 0xC5 // 数据一次读地址不变
#define LC_SCN_RD 0xE0 // 屏读
#define LC_SCN_CP 0xE8 // 屏拷贝
#define LC_BIT_OP 0xF0 // 位操作

#define NOP() asm("nop")
// D0-D2 定义D0-D7 位D3 1置位0 清除
const  unsigned char uPowArr[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
// ASCII 字模宽度及高度定义
#define ASC_CHR_WIDTH 8
#define ASC_CHR_HEIGHT 16
// ASCII 字模显示为8*16
const unsigned char  turnf[8] = {7,6,5,4,3,2,1,0};
unsigned char gCurRow,gCurCol; // 当前行列存储行高16 点列宽8点

/**************************************************************


typedef struct         // 汉字字模数据结构
	{
  	char Index[2];
  	char Msk[32];
	}typFNT_GB16;

**************************************************************/

/* 取当前行数据*/
unsigned char fnGetRow(void);
/* 取当前列数据*/
unsigned char fnGetCol(void);
/* 读状态*/
unsigned char RD_LCM(void);
/* 读数据*/
unsigned char RD_DAT(void);
/* 写数据*/
void WR_DAT(unsigned char val);
/* 写命令*/
void WR_CMD(unsigned char val);
/************************************************/
/* 状态位STA1,STA0判断读写指令和读写数据*/
/* 在读写数据或者写入命令前必须保证均为1 */
/************************************************/
unsigned char fnSTA01(void);
/********************************************/
/* 检查STA2,如果STA2=1 为自动读状态*/
/********************************************/
unsigned char fnSTA2(void);
/***********************************************/
/* 状态位STA3 判断STA3 = 1 数据自动写状态*/
/***********************************************/
unsigned char fnSTA3(void);
/************************************************/
/* 状态位STA6 判断STA6 =1 屏读/屏拷贝状态*/
/************************************************/
unsigned char fnSTA6(void);
/************************/
/* 写双参数的指令*/
/************************/
unsigned char fnPR1(unsigned char uCmd,unsigned char uPar1,unsigned char uPar2);
/************************/
/* 写单参数的指令*/
/************************/
unsigned char fnPR11(unsigned char uCmd,unsigned char uPar1);
/************************/
/* 写无参数的指令*/
/************************/
unsigned char fnPR12(unsigned char uCmd);
/************************/
/* 写数据*/
/************************/
unsigned char fnPR13(unsigned char uData);
/************************/
/* 读数据*/
/************************/
unsigned char fnPR2(void);
/************************/
/* 设置当前地址*/
/************************/
void fnSetPos(unsigned char urow, unsigned char ucol);
/**********************/
/* 设置当前显示行列*/
/**********************/
void cursor(unsigned char uRow, unsigned char uCol);
/*********************/
/* 清屏*/
/*********************/
void cls(void);
/************************/
/* LCM 初始化*/
/************************/
char fnLCMInit(void);
/********************************************************/
/* ASCII(8*16) 及 汉字(16*16) 显示函数      */
/********************************************************/
unsigned char dprintf(unsigned char x,unsigned char y, char *ptr);
/*======================*/
/* 延时*/
/*======================*/
void shortdelay(unsigned int tt);
/*************************/
/* 画点*/
/*************************/
void point(unsigned char x,unsigned char y,unsigned char s);
/************************************************/
/* 画线任意方向的斜线,直线数学方程aX+bY=1 */
/************************************************/
void Linexy(unsigned char x0,unsigned char y0,unsigned char xt,unsigned char yt,unsigned char s);
/********************************************/
/* 画圆数学方程(X-Ox)^2+(Y-Oy)^2=Rx^2 */
/********************************************/
void circle(unsigned char Ox,unsigned char Oy,unsigned char Rx,unsigned char s);

#else

/* 取当前行数据*/
extern unsigned char fnGetRow(void);
/* 取当前列数据*/
extern unsigned char fnGetCol(void);
/* 读状态*/
extern unsigned char RD_LCM(void);
/* 读数据*/
extern unsigned char RD_DAT(void);
/* 写数据*/
extern void WR_DAT(unsigned char val);
/* 写命令*/
extern void WR_CMD(unsigned char val);
/************************************************/
/* 状态位STA1,STA0判断读写指令和读写数据*/
/* 在读写数据或者写入命令前必须保证均为1 */
/************************************************/
extern unsigned char fnSTA01(void);
/********************************************/
/* 检查STA2,如果STA2=1 为自动读状态*/
/********************************************/
extern unsigned char fnSTA2(void);
/***********************************************/
/* 状态位STA3 判断STA3 = 1 数据自动写状态*/
/***********************************************/
extern unsigned char fnSTA3(void);
/************************************************/
/* 状态位STA6 判断STA6 =1 屏读/屏拷贝状态*/
/************************************************/
extern unsigned char fnSTA6(void);
/************************/
/* 写双参数的指令*/
/************************/
extern unsigned char fnPR1(unsigned char uCmd,unsigned char uPar1,unsigned char uPar2);
/************************/
/* 写单参数的指令*/
/************************/
extern unsigned char fnPR11(unsigned char uCmd,unsigned char uPar1);
/************************/
/* 写无参数的指令*/
/************************/
extern unsigned char fnPR12(unsigned char uCmd);
/************************/
/* 写数据*/
/************************/
extern unsigned char fnPR13(unsigned char uData);
/************************/
/* 读数据*/
/************************/
extern unsigned char fnPR2(void);
/************************/
/* 设置当前地址*/
/************************/
extern void fnSetPos(unsigned char urow, unsigned char ucol);
/**********************/
/* 设置当前显示行列*/
/**********************/
extern void cursor(unsigned char uRow, unsigned char uCol);
/*********************/
/* 清屏*/
/*********************/
extern void cls(void);
/************************/
/* LCM 初始化*/
/************************/
extern char fnLCMInit(void);
/********************************************************/
/* ASCII(8*16) 及 汉字(16*16) 显示函数      */
/********************************************************/
extern unsigned char dprintf(unsigned char x,unsigned char y, char *ptr);
/*======================*/
/* 延时*/
/*======================*/
extern void shortdelay(unsigned int tt);
/*************************/
/* 画点*/
/*************************/
extern void point(unsigned char x,unsigned char y,unsigned char s);
/************************************************/
/* 画线任意方向的斜线,直线数学方程aX+bY=1 */
/************************************************/
extern void Linexy(unsigned char x0,unsigned char y0,unsigned char xt,unsigned char yt,unsigned char s);
/********************************************/
/* 画圆数学方程(X-Ox)^2+(Y-Oy)^2=Rx^2 */
/********************************************/
extern void circle(unsigned char Ox,unsigned char Oy,unsigned char Rx,unsigned char s);
#endif
#endif
