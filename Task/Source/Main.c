#include <Lang.h>
#include <DT3.h>
#include <DS1302.h>

/// ==== PIN������ ====
sbit P_LED = P1 ^ 0;
sbit P_UP = P0 ^ 0;
sbit P_DOWN = P0 ^ 1;
sbit P_LEFT = P0 ^ 2;
sbit P_RIGHT = P0 ^ 3;
sbit P_OK = P0 ^ 4;

/// ==== ����� ====
// ��ʱ��0 ����
#define _TH_NUM ((0xFF + 1 - 1000) / (0xFF + 1))
#define _HL_NUM ((0xFF + 1 - 1000) % (0xFF - 1))

// ��������
#define _BT_LONGDOWN 0
#define _BT_UPNOW 1
#define _BT_DOWN 2
#define _BT_NOACT 3

/// ==== ȫ�ֱ��� ====
uchar TIME_INFO[8];
uchar PMW_GAP;
uchar PNW_SWITCH;
uchar LIGHT_VAL;

sbit *KEY_ARR[] =
{
	&P_UP, &P_DOWN, &P_LEFT, &P_RIGHT, &P_OK
};
bit LAST_VAL[5];

// 0 ���� 1 ̧�� 2 ���� 3 û�а�
uchar KEY_STATE[5];

/// ==== ����ԭ�� ====
void Init();
void Key_Scan();

int main()
{
	Init();
	while(1)
	{
		// ���̼���
		Key_Scan();
		
		// ʱ��ˢ��
		read_time(TIME_INFO);
		
		// �¶�ָ��
		LIGHT_VAL = Read_AD(0);
		
		// ��Ļ��ʾ
	}
	return 0;
}

void Init()
{
	int i;
	EA = 1;
	
	// ��ʱ��
	TMOD = 0x02;
	TH0 = _TH_NUM;
	TL0 = _TL_NUM;
	ET0 = 1;
	TR0 = 1;
	
	// ������¼��ʼֵ
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
			case 0: // ����
			break;
			
			case 1: // ̧��
			break;
			
			case 2: // ����
			break;
			
			case 3: // û�а�
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