#include"reg52.h"							 		//52系列单片机头文件
#define uchar unsigned char							//无符号字符变量宏定义

//按键引脚定义
sbit key1=P1^5;  									//设置键
sbit key2=P1^6;  									//加键
sbit key3=P1^7;  									//减键
sbit key4=P3^2;									//确认键
sbit key5=P3^3;									//暂停键
sbit beep=P1^0;									//蜂鸣器
uchar key_s;
uchar key_z;
uchar key1n;										//调整功能选择
uchar DT[]=										//DS1302读写数据缓存
{	0x00,0x58,0x12,0x16,0x02,0x07,0x14	};			//秒分时日月星期年
uchar Temp;										//DS18B20实时温度读取值变量

//延时函数
void delayms(uint t)								//ms延时函数，有参函数
{
	uint x,y;
	for(x=t;x>0;x--)
	for(y=110;y>0;y--);
}


//按键音函数
void key_sound(uchar t)						   		//参数t控制发音频率
{
 	uchar i;
	for(i=0;i<20;i++)						   		//循环10次
	{
	 	beep= ~beep;
		delayms(t);
	}
	beep= 1;								   		//关闭按键音
}
void int0() interrupt 0
{
		key_s=1;
}
void int1() interrupt 1
{
		key_z=1;
}
// 键盘扫描函数
void keyscan()										//键盘扫描程序
{
	uchar miao,fen,shi,xinqi,ri,yue,nian;				//调整对象:秒分时,星期,日月年
	uchar BCD;									//调整值的BCD码
		//（设置键）功能键key1:
		if(key1==0)									//功能键key1被按下？
		{
			delayms(10);								//延时消抖动
			if(key1==0)								//确认key1键按下
			 {
				key_sound(1);							//蜂鸣器发声
				while(!key1);							//等待Key1键松开
				Clr_Scr(0);
				 if(key1n==0)
				 {
					 hz_Disp16(1,2,0,HZ[22],4);					//显示智能电子表
					hz_Disp16(2,2,0,HZ[26],4);
					hz_Disp16(1,6,0,HZ[30],2);					//显示确认、取消
					hz_Disp16(2,6,32,HZ[32],2); 
					//while((key_s==0&&key_z==1)||(key_s==1&&key_z==0)||(key_s==0&&key_z==0));
					 while(key_s==0);
				 }
				if(key_z==1)
				{
					key_s=0;
					key_z=0;
					return;
				}
				Clr_Scr(0);	
				
				/*if(key3==0)
					return;			
				else if(key2==0&&key1n==0)
				{
					while(!key2);
					key1n++;
				}
				if(key1n!=0)*/
				key1n++;							//计数(选择调整对象)
				hz_Disp16(2,6,0,adj[0],2);
				hz_Disp16(2,6,48,adj[3],1);
				if(key1n==8)key1n=0;					//8个功能循环完毕,重置0
				switch(key1n)							//根据计数值选择
						{
					case 1: 							//设置键key1 按动 1 次时
					hz_Disp16(2,6,32,adj[2],1);			//右屏6页第 0列显示"调整年数"
						break;
					case 2:  							//设置键key1 按动 2 次时
								hz_Disp16(2,6,32,adj[4],1);			//右屏6页16列处显示"月"
					break;
					case 3:  							//设置键key1 按动 3 次时
					hz_Disp16(2,6,32,adj[5],1);			//右屏6页16列处显示"日"
					break;
					case 4:  							//设置键key1 按动 4 次时
					hz_Disp16(2,6,32,adj[6],1);			//右屏6页16列处显示"周"
					break;
					case 5:  							//设置键key1 按动 5 次时
					hz_Disp16(2,6,32,adj[7],1);			//右屏6页16列处显示"时"
					break;
					case 6:  							//设置键key1 按动 6 次时
					hz_Disp16(2,6,32,adj[8],1);			//右屏6页16列处显示"分"
					break;
					case 7:  							//设置键key1 按动 7 次时
					hz_Disp16(2,6,32,adj[9],1);			//右屏6页16列处显示"秒"
					break;
						}
				}
		}
		if(key1n!=0)									//只有按下K1后
		{											//再按K2,K3键才有效
			//加键key2:
			if(key2==0)								//上调键被按下？
			{
				delayms(20);							//延时消抖
				if(key2==0)							//确认
					key_sound(2);							//蜂鸣器发声
				while(!key2);							//等待键松开
				switch(key1n)							//根据key1按键次数选择
				{
					case 7:							//调整秒数
					miao=DT[0];miao++;				//每按一次K2,秒数 +1
					if(miao==60)miao=0;				//若秒数等于60重置 0
					BCD=miao/10*16+miao%10;		//十进制转BCD码
						Write_1302(0x8e,0x00); 			//开启允许写，禁止写保护 
						Write_1302(0x80,BCD); 			//调整秒数BCD码写入80H地址
						Write_1302(0x8e,0x80); 			//启用写保护
					break;							//跳出

					case 6:							//调整分钟
					fen=DT[1],fen++;					//每按一次K2,分数 +1
					if(fen==60)fen=0;					//若分数等于60重置 0
					BCD=fen/10*16+fen%10;			//十进制转BCD码
						Write_1302(0x8e,0x00);				//开启允许写，禁止写保护 
						Write_1302(0x82,BCD);				//调整分数BCD码写入82H地址
						Write_1302(0x8e,0x80);				//打开写保护
					break;							//跳出
					
					case 5:							//调整小时
					shi=DT[2];shi++;					//每按一次K2,时数 +1
					if(shi==24)shi=0;					//若时数等于24重置 0
					BCD=shi/10*16+shi%10;			//十进制转BCD码
						Write_1302(0x8e,0x00);				//允许写，禁止写保护 
						Write_1302(0x84,BCD);				//调整时数BCD码写入84H地址
						Write_1302(0x8e,0x80);				//打开写保护
					break;							//跳出
					
					case 4:							//调整日期
					xinqi=DT[5];xinqi++;				//每按一次K2,周数 +1
					if(xinqi==8)xinqi=1;				//若周数等于8 重置 1
								BCD=xinqi/10*16+xinqi%10;		//十进制转BCD码
						Write_1302(0x8e,0x00);				//允许写，禁止写保护 
						Write_1302(0x8a,BCD);				//调整周数BCD码写入8aH地址
						Write_1302(0x8e,0x80);				//打开写保护
					break;							//跳出

					case 3:							//调整日期
					ri=DT[3];ri++;						//每按一次K2,日数 +1
					if(ri==32)ri=1;						//若日数等于32重置 1
					BCD=ri/10*16+ri%10;				//十进制转BCD码
						Write_1302(0x8e,0x00);				//允许写，禁止写保护
						Write_1302(0x86,BCD);				//调整日数BCD码写入86H地址
						Write_1302(0x8e,0x80);				//打开写保护
					break;							//跳出

					case 2:							//调整月份
					yue=DT[4];yue++;					//每按一次K2,月数 +1
					if(yue==13)yue=1;					//若月数等于13重置 1
					BCD=yue/10*16+yue%10;			//十进制转BCD码
						Write_1302(0x8e,0x00);				//允许写，禁止写保护
						Write_1302(0x88,BCD);				//调整月数BCD码写入88H地址
						Write_1302(0x8e,0x80);				//打开写保护
					break;							//跳出
					
					case 1:							//调整年份
					nian=DT[6];nian++;				//每按一次K2,年数 +1
					if(nian==100)nian=0;				//若年数加到100重置 0
								BCD=nian/10*16+nian%10;			//十进制转BCD码
						Write_1302(0x8e,0x00);				//允许写，禁止写保护
						Write_1302(0x8c,BCD);				//调整年数BCD码写入8cH地址
						Write_1302(0x8e,0x80);				//打开写保护
					break;							//跳出
				}
			}
			//减键key3
			if(key3==0)
			{
				delayms(10);							//调延时，消抖动
				if(key3==0)
					key_sound(2);							//蜂鸣器发声
				while(!key3);							//等待键松开
				switch(key1n)							//根据key1功能键选择
				{
					case 7:							//调整秒数
					miao=DT[0];miao--;				//每按一次K3, 秒数 -1
					if(miao==-1)miao=59;				//当秒数据减到-1时重置59
								BCD=miao/10*16+miao%10;		//十进制转BCD码
						Write_1302(0x8e,0x00); 			//允许写，禁止写保护 
						Write_1302(0x80,BCD); 			//调整秒数BCD码写入80H地址
						Write_1302(0x8e,0x80); 			//打开写保护
					break;
				
					case 6:							//调整分数
					fen=DT[1];fen--;					//每按一次K3,分数 -1
					if(fen==-1)fen=59;					//分数据减到-1时重置 59
					BCD=fen/10*16+fen%10;			//十进制转BCD码
						Write_1302(0x8e,0x00);				//允许写，禁止写保护 
						Write_1302(0x82,BCD);				//调整分数BCD码写入82H地址
						Write_1302(0x8e,0x80);				//打开写保护
					break;

					case 5:							//调整时数
					shi=DT[2];shi--;					//每按一次K3,时数 -1
					if(shi==-1)shi=23;					//当时数减到-1时重置23
					BCD=shi/10*16+shi%10;			//十进制转BCD码
						Write_1302(0x8e,0x00);				//允许写，禁止写保护 
						Write_1302(0x84,BCD);				//调整时数BCD码写入84H地址
						Write_1302(0x8e,0x80);				//打开写保护
					break;
				
					case 4:							//调整周数
					xinqi=DT[5];xinqi--;				//每按一次K3,星期数-1
					if(xinqi==0)xinqi=7;				//当星期数减到 0 时重置 7
					BCD=xinqi/10*16+xinqi%10;		//十进制转BCD码
						Write_1302(0x8e,0x00);				//允许写，禁止写保护 
						Write_1302(0x8a,BCD);				//调整周数BCD码写入8aH地址
						Write_1302(0x8e,0x80);				//打开写保护
					break;
				
					case 3:							//调整日期
					ri=DT[3];ri--;						//每按一次K3,日期数-1
					if(ri==0)ri=31;						//当日期数减到 0 时重置 31
					BCD=ri/10*16+ri%10;				//十进制转BCD码
						Write_1302(0x8e,0x00);				//允许写，禁止写保护
						Write_1302(0x86,BCD);				//调整日数BCD码写入86H地址
						Write_1302(0x8e,0x80);				//打开写保护
					break;
				
					case 2:							//调整月份
					yue=DT[4];yue--;					//每按一次K3,月数-1
					if(yue==0)yue=12;					//当月数减到 0 时重置 12
					BCD=yue/10*16+yue%10;			//十进制转BCD码
						Write_1302(0x8e,0x00);				//允许写，禁止写保护
						Write_1302(0x88,BCD);				//调整月数BCD码写入88H地址
						Write_1302(0x8e,0x80);				//打开写保护
					break;	
				
					case 1:							//调整年份
					nian=DT[6];nian--;					//每按一次K3,年数-1
					if(nian==-1)nian=99;				//当月数减到 -1 时重置 99
							BCD=(nian)/10*16+(nian)%10;		//十进制转BCD码
						Write_1302(0x8e,0x00);				//允许写，禁止写保护
						Write_1302(0x8c,BCD);				//调整年数BCD码写入8cH地址
						Write_1302(0x8e,0x80);				//打开写保护
					break;
				}
			}
		}
	
}