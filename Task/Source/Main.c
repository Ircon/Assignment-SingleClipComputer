#include <Lang.h>
#include <DT3.h>
#include <DS1302.h>

/// ==== PIN脚配置 ====
sbit P_LED = P1 ^ 0;
sbit P_UP = P0 ^ 0;
sbit P_DOWN = P0 ^ 1;
sbit P_LEFT = P0 ^ 2;
sbit P_RIGHT = P0 ^ 3;
sbit P_OK = P0 ^ 4;

/// ==== 宏变量 ====
// 计时器0 参数
#define _TH_NUM ((0xFF + 1 - 1000) / (0xFF + 1))
#define _HL_NUM ((0xFF + 1 - 1000) % (0xFF - 1))

// 按键代码
#define _BT_LONGDOWN 0
#define _BT_UPNOW 1
#define _BT_DOWN 2
#define _BT_NOACT 3

/// ==== 全局变量 ====
uchar TIME_INFO[8];
uchar PMW_GAP;
uchar PNW_SWITCH;
uchar LIGHT_VAL;

sbit *KEY_ARR[] =
{
	&P_UP, &P_DOWN, &P_LEFT, &P_RIGHT, &P_OK
};
bit LAST_VAL[5];

// 0 长按 1 抬起 2 按下 3 没有按
uchar KEY_STATE[5];

/// ==== 函数原型 ====
void Init();
void Key_Scan();

int main()
{
	Init();
	while(1)
	{
		// 键盘监听
		Key_Scan();
		
		// 时间刷新
		read_time(TIME_INFO);
		
		// 温度指数
		LIGHT_VAL = Read_AD(0);
		
		// 屏幕显示
	}
	return 0;
}

void Init()
{
	int i;
	EA = 1;
	
	// 定时器
	TMOD = 0x02;
	TH0 = _TH_NUM;
	TL0 = _TL_NUM;
	ET0 = 1;
	TR0 = 1;
	
	// 按键记录初始值
	for(i = 0 ; i < sizeof(LAST_VAL) / sizeof(bit) ; ++i)
	{
		LAST_VAL[i] = *(KEY_ARR[i]);
	}
	
}

void Key_Scan()
{
	int i;
	bit tem[5];
	for(i = 0 ; i < sizeof(KEY_ARR) / sizeof(sbit*) ; ++i)
	{
		tem[i] = *(KEY_ARR[i]);
	}
	for(i = 0 ; i < sizeof(KEY_ARR) / sizeof(sbit*) ; ++i)
	{
		if(tem[i] != *(KEY_ARR[i])) 
			continue;
		
		uchar buf = *(KEY_ARR[i]);
		buf <<= 1;
		buf += LAST_VAL[i];
		KEY_STATE[i] = buf;
		/*
		switch(buf)
		{
			case 0: // 长按
			break;
			
			case 1: // 抬起
			break;
			
			case 2: // 按下
			break;
			
			case 3: // 没有按
			break;
		}*/
	}
	
	for(i = 0 ; i < sizeof(KEY_ARR) / sizeof(sbit*) ; ++i)
	{
		LAST_VAL[i] = *(KEY_ARR[i]);
	}
}
void inter_t0() interrupt 1
{
	static cont = 0;
	
	if(PMW_SWITCH == 0)
	{
		cont = 0;
		P_LED = 1;
	}
	else
	{
		++cont;
		cont %= PMW_GAP;
		
		if(cont == 0)
			P_LED = ~P_LED;
	}
	
	TH0 = _TH_NUM;
	TL0 = _TL_NUM;
	TR0 = 1;
}