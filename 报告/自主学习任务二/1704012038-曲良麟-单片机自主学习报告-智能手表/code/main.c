#include <KS0108.h>
#include <lang.h>
#include <FONT.h>
#include <DS1302.h>
#include <DS18B20.h>

sbit SOUND = P1 ^ 0;
sbit UP = P1 ^ 2;
sbit DOWN = P1 ^ 3;
sbit LEFT = P1 ^ 4;
sbit RIGHT = P1 ^ 5;
sbit EXIT = P1 ^ 6;
sbit LED = P2 ^ 7;
uchar SWITCH = 0;
uchar res[7] = {
	0,29,17,17,12,2,19
};
uchar POI = 0;
uchar LOC[6] = {0,2,3,4,5,6};
uchar cla_[3][3];
code uchar XLOC[] = {0,1,2,0,1,2,0,1,2};
code uchar YLOC[] = {0,0,0,1,1,1,2,2,2};
code uchar MAX_LOC[] = {24,60,60};
uchar X_POI = 0;
uchar Y_POI = 0;
uchar XY_POI = 0;
void show_first_word(){

	show_font_16(1,32,3,HanZi_16[0]);
	show_font_16(1,48,3,HanZi_16[1]);
	show_font_16(2,0,3,HanZi_16[2]);
	show_font_16(2,16,3,HanZi_16[3]);
	dely_ms(2500);
	cls_disy(3);

}

void show_time(){
	uchar buf;
	// 第一行
	// 年
	buf = res[0];
	show_font_8(1,8*1,0,NUM[buf%10]);
	buf /= 10;
	show_font_8(1,8*0,0,NUM[buf%10]);
	
	show_font_8(1,8*2,0,Sing[1]);
	//月
	buf = res[2];
	show_font_8(1,8*4,0,NUM[buf%10]);
	buf /= 10;
	show_font_8(1,8*3,0,NUM[buf%10]);
	
	show_font_8(1,8*5,0,Sing[1]);
	
	//日
	buf = res[3];
	show_font_8(1,8*7,0,NUM[buf%10]);
	buf /= 10;
	show_font_8(1,8*6,0,NUM[buf%10]);
	
	//第二行
	// 时
	buf = res[4];
	show_font_8(1,8*1,2,NUM[buf%10]);
	buf /= 10;
	show_font_8(1,8*0,2,NUM[buf%10]);
	
	show_font_8(1,8*2,2,Sing[0]);
	//分
	buf = res[5];
	show_font_8(1,8*4,2,NUM[buf%10]);
	buf /= 10;
	show_font_8(1,8*3,2,NUM[buf%10]);
	
	show_font_8(1,8*5,2,Sing[0]);
	
	//秒
	buf = res[6];
	show_font_8(1,8*7,2,NUM[buf%10]);
	buf /= 10;
	show_font_8(1,8*6,2,NUM[buf%10]);
	
	//第三行
	buf = res[1];
	show_font_16(1,0,4,HanZi_16[4]);
	show_font_16(1,16,4,HanZi_16[5]);
	show_font_16(1,16*2,4,HZ_Num[buf]);
}

void show_del(){
	uchar buf;
	//第一行
	// 时
	buf = cla_[0][0];
	show_font_8f(2,8*1,0,NUM[buf%10],(X_POI == 0 && Y_POI == 0));
	buf /= 10;
	show_font_8f(2,8*0,0,NUM[buf%10],(X_POI == 0 && Y_POI == 0));
	
	show_font_8(2,8*2,0,Sing[0]);
	//分
	buf = cla_[0][1];
	show_font_8f(2,8*4,0,NUM[buf%10],(X_POI == 1 && Y_POI == 0));
	buf /= 10;
	show_font_8f(2,8*3,0,NUM[buf%10],(X_POI == 1 && Y_POI == 0));
	
	show_font_8(2,8*5,0,Sing[0]);
	
	//秒
	buf = cla_[0][2];
	show_font_8f(2,8*7,0,NUM[buf%10],(X_POI == 2 && Y_POI == 0));
	buf /= 10;
	show_font_8f(2,8*6,0,NUM[buf%10],(X_POI == 2 && Y_POI == 0));	
	
	//第二行
	// 时
	buf = cla_[1][0];
	show_font_8f(2,8*1,2,NUM[buf%10],(X_POI == 0 && Y_POI == 1));
	buf /= 10;
	show_font_8f(2,8*0,2,NUM[buf%10],(X_POI == 0 && Y_POI == 1));
	
	show_font_8(2,8*2,2,Sing[0]);
	//分
	buf = cla_[1][1];
	show_font_8f(2,8*4,2,NUM[buf%10],(X_POI == 1 && Y_POI == 1));
	buf /= 10;
	show_font_8f(2,8*3,2,NUM[buf%10],(X_POI == 1 && Y_POI == 1));
	
	show_font_8(2,8*5,2,Sing[0]);
	
	//秒
	buf = cla_[1][2];
	show_font_8f(2,8*7,2,NUM[buf%10],(X_POI == 2 && Y_POI == 1));
	buf /= 10;
	show_font_8f(2,8*6,2,NUM[buf%10],(X_POI == 2 && Y_POI == 1));
	
	//第二行
	// 时
	buf = cla_[2][0];
	show_font_8f(2,8*1,4,NUM[buf%10],(X_POI == 0 && Y_POI == 2));
	buf /= 10;
	show_font_8f(2,8*0,4,NUM[buf%10],(X_POI == 0 && Y_POI == 2));
	
	show_font_8(2,8*2,4,Sing[0]);
	//分
	buf = cla_[2][1];
	show_font_8f(2,8*4,4,NUM[buf%10],(X_POI == 1 && Y_POI == 2));
	buf /= 10;
	show_font_8f(2,8*3,4,NUM[buf%10],(X_POI == 1 && Y_POI == 2));
	
	show_font_8(2,8*5,4,Sing[0]);
	
	//秒
	buf = cla_[2][2];
	show_font_8f(2,8*7,4,NUM[buf%10],(X_POI == 2 && Y_POI == 2));
	buf /= 10;
	show_font_8f(2,8*6,4,NUM[buf%10],(X_POI == 2 && Y_POI == 2));
	
}
void drmatic_display(){
	show_time();
	show_del();
}

uchar on_time(){
	uchar ans,x;
	ans = 0;
	for(x = 0 ; x < 3 ; ++x){
		if(
			(cla_[x][0] == res[2]) &&
			cla_[x][1] == res[1] &&
			cla_[x][2] == res[0]
		){
			ans = 1;
			break;
		}
	}
	return ans;
}
uint last_val = 0;
void show_temper(){
	
	uint te = ReadTemperature();
	
	if(last_val == te) return;
	last_val = te;
	show_font_16(1,16*0,6,HanZi_16[6]);
	show_font_16(1,16*1,6,HanZi_16[7]);
	show_font_16(1,16*2,6,HanZi_16[8]);
	
	show_font_8(1,16*2,6,NUM[te/10]);
	show_font_8(1,16*2+8,6,NUM[te%10]);
	
	show_font_16(1,16*2+8+8,6,HanZi_16[9]);
}



void set_view(uchar opt){
	uchar buf;
	
	
	if(opt != 0)
	{
		if(opt == 1){
			++res[LOC[POI]];
		}else if(opt == 2){
			--res[LOC[POI]];
		}else if(opt == 3){
			POI = (POI + 5) % 6;
		}else if(opt == 4){
			POI = (POI + 1) % 6;
		}
		if(POI == 1){
			if(res[LOC[POI]] == 0) res[LOC[POI]] = 12;
			else if(res[LOC[POI]] > 12) res[LOC[POI]] = 1;
		}else if(POI == 2){
			if(res[LOC[POI]] == 0) res[LOC[POI]] = 30;
			else if(res[LOC[POI]] > 30) res[LOC[POI]] = 1;
		}else if(POI == 3){
			if(res[LOC[POI]] == 0) res[LOC[POI]] = 24;
			else if(res[LOC[POI]] > 24) res[LOC[POI]] = 1;
		}else if(POI == 4){
			if(res[LOC[POI]] == 0) res[LOC[POI]] = 60;
			else if(res[LOC[POI]] > 60) res[LOC[POI]] = 1;
		}else if(POI == 5){
			if(res[LOC[POI]] == 0) res[LOC[POI]] = 60;
			else if(res[LOC[POI]] > 60) res[LOC[POI]] = 1;
		}
		cls_disy(3);
	}
	
	if(POI < 3){
		show_font_16(1,16*POI,0,HanZi_16[10]);
	}else{
		show_font_16(2,16*(POI-3),0,HanZi_16[10]);
	}
	
	// 年 月 日 时 分 秒
	show_font_16(1,16*0,2,RiZi[0]);
	show_font_16(1,16*1,2,RiZi[1]);
	show_font_16(1,16*2,2,RiZi[2]);
	show_font_16(2,16*0,2,RiZi[3]);
	show_font_16(2,16*1,2,RiZi[4]);
	show_font_16(2,16*2,2,RiZi[5]);
	
	
	// 第一行
	// 年
	buf = res[0];
	show_font_8(1,8*1,4,NUM[buf%10]);
	buf /= 10;
	show_font_8(1,8*0,4,NUM[buf%10]);
	
	
	//月
	buf = res[2];
	show_font_8(1,8*3,4,NUM[buf%10]);
	buf /= 10;
	show_font_8(1,8*2,4,NUM[buf%10]);
	
	
	//日
	buf = res[3];
	show_font_8(1,8*5,4,NUM[buf%10]);
	buf /= 10;
	show_font_8(1,8*4,4,NUM[buf%10]);
	
	//第二行
	// 时
	buf = res[4];
	show_font_8(2,8*1,4,NUM[buf%10]);
	buf /= 10;
	show_font_8(2,8*0,4,NUM[buf%10]);
	
	//分
	buf = res[5];
	show_font_8(2,8*3,4,NUM[buf%10]);
	buf /= 10;
	show_font_8(2,8*2,4,NUM[buf%10]);
	
	
	//秒
	buf = res[6];
	show_font_8(2,8*5,4,NUM[buf%10]);
	buf /= 10;
	show_font_8(2,8*4,4,NUM[buf%10]);
}



void key_scan(){
	LED = 0;
	if(UP == 0 || DOWN == 0 || LEFT == 0 || RIGHT == 0){
		dely_ms(50);
		if(UP == 0 || DOWN == 0 || LEFT == 0 || RIGHT == 0){
			if(UP == 0){
				cla_[Y_POI][X_POI] =
					(cla_[Y_POI][X_POI] + 1) % MAX_LOC[X_POI];
			}else if(DOWN == 0){
				cla_[Y_POI][X_POI] = (cla_[Y_POI][X_POI] + MAX_LOC[X_POI] - 1) % MAX_LOC[X_POI];
			}else if(LEFT == 0){
				XY_POI = (XY_POI + 8) % 9;
				X_POI = XLOC[XY_POI];
				Y_POI = YLOC[XY_POI];
			}else if(RIGHT == 0){
				XY_POI = (XY_POI + 1) % 9;
				X_POI = XLOC[XY_POI];
				Y_POI = YLOC[XY_POI];
			}
		}
	}
}

void main(void){
	uchar x= 16,y,z=0,flag;
	uchar buf_time[3];
	int buf;
	
	flag = 0;
	// 初始化 显示器
	select_side(3);
	led_init();	
	show_first_word();
	
	// 中断
	IT0 = 1;	//设置外部中断1的触发方式为下降沿触发。
	EX0 = 1;	//开启外部中断1
	EA = 1;		//中断总开关
	
	while(1){
		
		if(SWITCH){
			if(UP == 0 || DOWN == 0 || LEFT == 0 || RIGHT == 0 || EXIT == 0){
			
				dely_ms(100);
				if(UP == 0 || DOWN == 0 || LEFT == 0 || RIGHT == 0 || EXIT == 0){
					if(UP == 0)
						set_view(1);
					else if(DOWN == 0)
						set_view(2);
					else if(LEFT == 0)
						set_view(3);
					else if(RIGHT == 0)
						set_view(4);
					else if(EXIT == 0){
						SWITCH = 0;
						EA = 1;
						LED = 1;
						last_val = 0;
						// 写日期
						//for(y = 0 ; y < 7 ; ++y)
						//	res[y] = Dec_BCD(res[y]);
						write_time(res);
					}
				}
			}else{
				set_view(0);
			}
			
		}else{
			read_time(res);
			//for(y = 0 ; y < 7 ; ++y)
				//res[y] = BCD_Decimal(res[y]);
			key_scan();
			drmatic_display();
			show_temper();
			if(on_time()){
				flag = 1;
				buf_time[2] = res[2];
				buf_time[1] = res[1];
				buf_time[0] = res[0];
			}
			if(flag){
				buf = (res[2]*60*60 + res[1]*60 + res[0]) 
					- (buf_time[2]*60*60 + buf_time[1]*60 + buf_time[0]);
				if(buf<0) buf = -buf;
				if(buf <= 5){
					SOUND = 0;
				}else {
					SOUND =1;
					flag = 0;
				}	
			}
			dely_ms(50);
		}
	}
		
	return;
}


void int0_fun(void) interrupt 0 {
	EA = 0;
	set_view(0);
	cls_disy(3);
	LED = 0;
	SWITCH = 1;
}