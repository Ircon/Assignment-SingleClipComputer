#define T6963_C
#include "..\includes\includes.h"
/* 
******************************************************************************** 
*                                             LCD Test Software 
* 
* 
* File     : Mega16LCDyxd240128T6963c.c 
* By       : WangXinJia 
* Version  : V0.0 
* Data : 2006-11-25 
* HardWare : -MCU- |-Atmega16 -|-Ext 8MHz -| 
  -LCD- |-YXD-240128V601G| 
 -LCD Chip- |-T6963C -|-TOSHIBA -| 
* Function :1 Second Change Text(Intern) ; 
* Bug : No Display Once in a while    
* Link : -DIR-DB-  |-PA0 
_______________           ______________ 
|              |      --1|FG            | 
|              |      --2|GND           | 
|              |      --3|+5V           | 
|              |      --4|V0            | 
|           PA0|--------5|WR            | 
|           PA1|--------6|RD            | 
|              |     ---7|CE            | 
|           PA2|--------8|CD            | 
|CPU           |     ---9|RST           | 
|ATmmega16L PB0|-------10|D0   LCD      | 
|           PB1|-------11|D1  240128    | 
|           PB2|-------12|D2            | 
|           PB3|-------13|D3            | 
|           PB4|-------14|D4            | 
|           PB5|-------15|D5            | 
|           PB6|-------16|D6            | 
|           PB7|-------17|D7            | 
|           PD7|-------18|FS            | 
|              |     --19|A             | 
|              |     --20|K             | 
|              |     --21|-VOUT         | 
|______________|         |______________| 
******************************************************************************** 
*/ 

/************************************************/
/* 取当前行数据*/
/************************************************/
unsigned char fnGetRow(void)
{
    return(gCurRow);
}
/************************************************/
/* 取当前列数据*/
/************************************************/
unsigned char fnGetCol(void)
{
    return(gCurCol);
}
/************************************************/
/* 读状态*/
/************************************************/
unsigned char RD_LCM(void)
{
    unsigned char temp;
    dir_datap=0x00;
    port_conp  |= (1 <<LCM ); //LCM=LCMCW;
    port_conp &= ~(1 << RD);  //RD=0;
    NOP();
    temp=LCD_IN;
    port_conp |= (1<<RD);     //RD=1;
    dir_datap = 0xff;
    return(temp);
}
/************************************************/
/* 读数据*/
/************************************************/
unsigned char RD_DAT(void)
{
    unsigned char temp;
    dir_datap = 0x00;
    port_conp &= ~(1<<LCM);     //LCM=LCMDW;
    port_conp &= ~(1 << RD);    //RD=0;
    NOP();
    temp = LCD_IN;
    port_conp |= (1<<RD);       //RD=1;
    dir_datap = 0xff;
    return(temp);
}
/************************************************/
/* 写数据*/
/************************************************/
void WR_DAT(unsigned char val)
{
    dir_datap = 0xff;
    port_conp &= ~(1<<LCM);     //LCM=LCMDW;
    LCD_OUT = val;
    port_conp &= ~(1<<WR);      //WR=0;
    NOP();
    NOP();
    port_conp |= (1<<WR);       //WR=1;
}
/************************************************/
/* 写命令*/
/************************************************/
void WR_CMD(unsigned char val)
{
    dir_datap = 0xff;
    port_conp  |= (1 << LCM);    //LCM=LCMCW;
    LCD_OUT = val;
    port_conp &= ~(1<<WR);       //WR=0;
    NOP();
    NOP();
    port_conp |= (1<<WR);        //WR=1;
}
/************************************************/
/* 状态位STA1,STA0判断读写指令和读写数据*/
/* 在读写数据或者写入命令前必须保证均为1 */
/************************************************/
unsigned char fnSTA01(void)
{
    unsigned char i;
    for(i=10;i>0;i--)
    {
        if((RD_LCM() & 0x03) == 0x03) // 读取状态
        {
            break;
        }
    }
    return(i); // 若返回零说明错误
}
/********************************************/
/* 检查STA2,如果STA2=1 为自动读状态*/
/********************************************/
unsigned char fnSTA2(void)
{
    unsigned char i;
    for(i=10;i>0;i--)
    {
        if((RD_LCM() & 0x04) == 0x04)
        {
            break;
        }
    }
    return(i); // 若返回零说明错误
}
/***********************************************/
/* 状态位STA3 判断STA3 = 1 数据自动写状态*/
/***********************************************/
unsigned char fnSTA3(void)
{
    unsigned char i;
    for(i=10;i>0;i--)
    {
        if((RD_LCM() & 0x08) == 0x08)
        {
            break;
        }
    }
    return(i); // 若返回零说明错误
}
/************************************************/
/* 状态位STA6 判断STA6 =1 屏读/屏拷贝状态*/
/************************************************/
unsigned char fnSTA6(void)
{
    unsigned char i;
    for(i=10;i>0;i--)
    {
        if((RD_LCM() & 0x40) == 0x40)
        {
            break;
        }
    }
    return(i); // 若返回零说明错误
}
/************************/
/* 写双参数的指令*/
/************************/
unsigned char fnPR1(unsigned char uCmd,unsigned char uPar1,unsigned char uPar2)
{
    if(fnSTA01() == 0)
    {
        return 1;
    }
    WR_DAT(uPar1);
    if(fnSTA01() == 0)
    {
        return 2;
    }
    WR_DAT(uPar2);
    if(fnSTA01() == 0)
    {
        return 3;
    }
    WR_CMD(uCmd);
    return(0); // 返回0 成功
}
/************************/
/* 写单参数的指令*/
/************************/
unsigned char fnPR11(unsigned char uCmd,unsigned char uPar1)
{
    if(fnSTA01() == 0)
    {
        return 1;
    }
    WR_DAT(uPar1);
    if(fnSTA01() == 0)
    {
        return 2;
    }
    WR_CMD(uCmd);
    return(0); // 返回0 成功 
}
/************************/
/* 写无参数的指令*/
/************************/
unsigned char fnPR12(unsigned char uCmd)
{   
    if(fnSTA01() == 0)
    {
        return 1;
    }
    WR_CMD(uCmd);
    return(0); // 返回0 成功
}
/************************/
/* 写数据*/
/************************/
unsigned char fnPR13(unsigned char uData)
{
    if(fnSTA3() == 0)
    {
        return 1;
    }
    WR_DAT(uData);
    return(0); // 返回0 成功
}
/************************/
/* 读数据*/
/************************/
unsigned char fnPR2(void)
{
    unsigned char temp;
    if(fnSTA01() == 0)
    {
         return 1; // 获取状态如果状态错
    }
    temp=RD_DAT();
    return(temp); // 返回数据
}
/************************/
/* 设置当前地址*/
/************************/
void fnSetPos(unsigned char urow, unsigned char ucol)
{
    unsigned int iPos;
    iPos = (unsigned int)urow * 30 + ucol;
    fnPR1(LC_ADD_POS,iPos & 0xFF,iPos / 256);
    gCurRow = urow;
    gCurCol = ucol;
}
/**********************/
/* 设置当前显示行列*/
/**********************/
void cursor(unsigned char uRow, unsigned char uCol)
{
    fnSetPos(uRow * 16, uCol);
}
/*********************/
/* 清屏*/
/*********************/
void cls(void)
{
    unsigned int i;
    fnPR1(LC_ADD_POS,0x00,0x00); // 置地址指针为从零开始
    fnPR12(LC_AUT_WR); // 自动写
    for(i=0;i<240*128/8;i++) // 清一屏
    {
        fnSTA3();
        fnPR13(0x0); // 写数据实际使用时请将0x55 改成0x0
    }
    fnPR12(LC_AUT_OVR); // 自动写结束
    fnPR1(LC_ADD_POS,0x00,0x00); // 重置地址指针
    gCurRow = 0; // 置地址指针存储变量
    gCurCol = 0;
}
/************************/
/* LCM 初始化*/
/************************/
char fnLCMInit(void)
{
    if(fnPR1(LC_TXT_STP,0x00,0x00) != 0) // 文本显示区首地址
    {
        return (0xff);
    }
    fnPR1(LC_TXT_WID,0x1E,0x00); // 文本显示区宽度
    fnPR1(LC_GRH_STP,0x00,0x00); // 图形显示区首地址
    fnPR1(LC_GRH_WID,0x1E,0x00); // 图形显示区宽度
    fnPR12(LC_CUR_SHP | 0x01); // 光标形状
    fnPR12(LC_MOD_OR); // 显示方式设置
    fnPR12(LC_DIS_SW | 0x08); // 显示开关设置
    return(0);
}
/********************************************************/
/* ASCII(8*16) 及 汉字(16*16) 显示函数      */
/********************************************************/
unsigned char dprintf(unsigned char x,unsigned char y, char *ptr)
{
    unsigned char c1,c2,cData;
    unsigned char i,j,uLen,uRow,uCol;
    unsigned int  k;
    uLen=0;
    i=0;
    uRow = y;
    uCol = x;
//   ptrGB_16=(ptrtypFNT_GB16)GB_16t;
    fnSetPos(uRow*16,uCol);      //起点定位
    while (ptr[uLen]!=0) //探测字串长度
    {
        uLen++;
    }	
    while(i<uLen)
    {
    	c1 = ptr[i];
    	c2 = ptr[i+1];
        //ascii字符与汉字内码的区别在于128做分界，大于界线的为汉字码
    	uRow = fnGetRow();    
    	uCol = fnGetCol();
    	if(c1 <=128)	// ASCII
      	{
            for(j=0;j<16;j++) //写16行
            {
        	fnPR12(LC_AUT_WR);    			// 写数据(命令)
        	if (c1 >= 0x20)
          	{
            	    fnPR13( pgm_read_byte(ASC_MSK+(c1-0x20)*ASC_CHR_HEIGHT+j-(16-ASC_CHR_HEIGHT)) );//pgm_read_byte(ASC_MSK+(c1-0x20)*ASC_CHR_HEIGHT+j-(16-ASC_CHR_HEIGHT))
          	}
        	else
        	{
          	    fnPR13(cData);
          	}
         	fnPR12(LC_AUT_OVR);  			//写数据结束
                fnSetPos(uRow+j+1,uCol);
             }
            if(c1 != BS)  	// 非退格
            {
                uCol++;   // 列数加1
            }
          			   				
        }
    	else    	// 中文
      	{
            for(j=0;j<sizeof(GB_16ptr)/2;j++)  	// 查找定位
            {
                if(c1 == GB_16ptr[j][0] && c2 == GB_16ptr[j][1])
                {
                    break;
                }
          				
            }
            for(k=0;k<16;k++)
            {
                fnSetPos(uRow+k,uCol);
        	fnPR12(LC_AUT_WR);      		// 写数据
        	if(j < sizeof(GB_16ptr)/2)
          	{
          	    fnPR13(pgm_read_byte(GB_16+j*32+k*2));
          	    fnPR13(pgm_read_byte(GB_16+j*32+k*2+1));
          	}
        	else   		// 未找到该字
          	{
          	    if(k < 4)
            	    {
                        fnPR13(0x00);
            		fnPR13(0x00);
            	     }
         	    else
            	    {
            		fnPR13(0xff);
			fnPR13(0xff);
            	    }
                }
        	fnPR12(LC_AUT_OVR);
            }
      	    uCol += 2;
      	    i++;
        };
    	if(uCol >= 30)      					// 光标后移
      	{
      	    uRow += 16;
            if(uRow < 0x80)
            {
                uCol -= 30;
            }
      	    else
            {
        	uRow = 0;
        	uCol = 0;
            }
      	 }
         fnSetPos(uRow,uCol);
    	 i++;
    }
    return uLen;        //返回字串长度，汉字按2字节计算
}  
/*======================*/
/* 延时*/
/*======================*/
void shortdelay(unsigned int tt)
{
    unsigned char i;
    while (tt)
    {
        i=100;
        while (i)
        {
            i--;
        }
    tt--;
    }
}
/*************************/
/* 画点*/
/*************************/
void point(unsigned char x,unsigned char y,unsigned char s)
{
    unsigned char x1;
    x1 = x >> 3; // 取Y方向分页地址
    fnSetPos(y,x1); // 起点定位
    x1 = turnf[ x & 0x07 ];
    x1 = 0xF0 | x1 | s; // 字节内位置计算
    fnPR12(x1); // 画上屏幕S显示属性8 画点0 擦除点
}
/************************************************/
/* 画线任意方向的斜线,直线数学方程aX+bY=1 */
/************************************************/
void Linexy(unsigned char x0,unsigned char y0,unsigned char xt,unsigned char yt,unsigned char s)
{
    register unsigned char t;
    int xerr = 0,yerr = 0;
    int delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x = xt-x0; // 计算坐标增量
    delta_y = yt-y0;
    uRow = x0;
    uCol = y0;
    if(delta_x>0)
    {
        incx = 1; // 设置单步方向
    }
    else if( delta_x==0 )
    {
        incx=0; // 垂直线
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
     
    }
    if(delta_y>0)
    {
        incy=1;
    }
    else if( delta_y==0 ) 
    {
        incy=0; // 水平线
    }
    else 
    {
        incy=-1;
        delta_y=-delta_y;
    }
    if( delta_x > delta_y ) 
    {
        distance=delta_x; // 选取基本增量坐标轴
    }
    else 
    {
        distance=delta_y;
    }
    for( t=0;t <= distance+1; t++ )   // 画线输出
    { 
        point(uRow,uCol,s); // 画点
        xerr += delta_x;
        yerr += delta_y;
        if( xerr > distance )
        {
            xerr -= distance;
            uRow += incx;
        }
        if( yerr > distance )
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}
/********************************************/
/* 画圆数学方程(X-Ox)^2+(Y-Oy)^2=Rx^2 */
/********************************************/
void circle(unsigned char Ox,unsigned char Oy,unsigned char Rx,unsigned char s)
{
    unsigned int xx,rr;
    unsigned int xt,yt;
    unsigned int rs,row,col;
    yt = Rx;
    rr = (unsigned int)Rx*Rx+1; // 补偿1 修正方形
    rs = (yt+(yt>>1))>>1; // (*0.75)分开1/8圆弧来画
    for (xt=0;xt<=rs;xt++)
    {
        xx = xt*xt;
        while ((yt*yt)>(rr-xx))
        {
            yt--;
        }
        row = Ox+xt; // 第一象限
        col = Oy-yt;
        point(row,col,s);
        row = Ox-xt; // 第二象限
        point(row,col,s);
        col = Oy+yt; // 第三象限
        point(row,col,s);
        row = Ox+xt; // 第四象限
        point(row,col,s);
        /***************45度镜象画另一半***************/
        row = Ox+yt; // 第一象限
        col = Oy-xt;
        point(row,col,s);
        row = Ox-yt; // 第二象限
        point(row,col,s);
        col = Oy+xt; // 第三象限
        point(row,col,s);
        row = Ox+yt; // 第四象限
        point(row,col,s);
    }
}
