#include <LANG.h>
#include <LCD12864.h>
#include <DS1302.h>
#include <FONT.h>
sbit UP = P0 ^ 0;
sbit DOWN = P0 ^ 1;
sbit LEFT = P0 ^ 2;
sbit RIGHT = P0 ^ 3;
sbit LED = P2 ^ 0;

uchar time_data[7];
void init_fun(){
	LCD_Init();
}
// 日 期 年*2 月*2 日*2
uchar code tim_loc_x[] = {
	0,0,0,0,0,0,0,0,
	
	16,16,16,16,16,16,16,16,
	
	32,32
};
uchar code tim_loc_y[] = {
	0,1,2,3,4,5,6,7,
	
	0,1,2,3,4,5,6,7,
	
	0,1
};
uchar load_time[] = {10,20,40};
uchar* tim_font_arr[] = {
	HanZi_32[2],HanZi_32[3],0,0,0,0,0,0,
	
	HanZi_32[7],HanZi_32[8],0,0,0,0,0,0,
	
	HanZi_32[0],HanZi_32[1]
};

uchar code tim_loc_xl[] = {
	0,0,0,0,0,0,0,0
};
uchar code tim_loc_yl[] = {
	0,1,2,3,4,5,6,7
};

uchar* tim_font_arrl[] = {
	HanZi_32[2],HanZi_32[3],0,0,0,0,0,0
};
uchar set_pos = 0;
uchar code mod_num[] = {24,60,60};
void display_all(){
	//display_font16(0,0,HanZi_32[2])
	uchar i;
	for(i = 0 ; i < sizeof(tim_loc_x) / sizeof(uchar); ++i){
		display_font16(tim_loc_x[i],tim_loc_y[i],tim_font_arr[i]);
	}
	
	for(i = 0 ; i < sizeof(tim_loc_xl) / sizeof(uchar); ++i){
		display_font16_l(tim_loc_xl[i],tim_loc_yl[i],tim_font_arrl[i]);
	}
	for(i = 0; i < 3 ; ++i){
		if(i == set_pos){
			display_font16_l(16,i*2 + 2,HanZi_32[9]);
			display_font16_l(16,i*2 + 3,HanZi_32[9]);
		}else{
			display_font16_l(16,i*2 + 2,HanZi_32[8]);
			display_font16_l(16,i*2 + 3,HanZi_32[8]);
		}
	}
	
}
void updat_time(){
	tim_font_arr[2] = num_32[time_data[0]/10%10];
	tim_font_arr[3] = num_32[time_data[0]%10];
	tim_font_arr[4] = num_32[time_data[2]/10%10];
	tim_font_arr[5] = num_32[time_data[2]%10];
	tim_font_arr[6] = num_32[time_data[3]/10%10];
	tim_font_arr[7] = num_32[time_data[3]%10];
	
	tim_font_arr[10] = num_32[time_data[4]/10%10];
	tim_font_arr[11] = num_32[time_data[4]%10];
	tim_font_arr[12] = num_32[time_data[5]/10%10];
	tim_font_arr[13] = num_32[time_data[5]%10];
	tim_font_arr[14] = num_32[time_data[6]/10%10];
	tim_font_arr[15] = num_32[time_data[6]%10];
	
	tim_font_arrl[2] = num_32[load_time[0]/10%10];
	tim_font_arrl[3] = num_32[load_time[0]%10];
	tim_font_arrl[4] = num_32[load_time[1]/10%10];
	tim_font_arrl[5] = num_32[load_time[1]%10];
	tim_font_arrl[6] = num_32[load_time[2]/10%10];
	tim_font_arrl[7] = num_32[load_time[2]%10];
}
void scan_key(){
	if(UP == 0 || DOWN == 0 || LEFT == 0 || RIGHT == 0){
		dely_US(50);
		if(UP == 0){
			load_time[set_pos] = (load_time[set_pos] + 1) % mod_num[set_pos];
		}else if(DOWN == 0){
			load_time[set_pos] = (load_time[set_pos] + mod_num[set_pos] - 1) % mod_num[set_pos];
		}else if(LEFT == 0){
			set_pos = (set_pos + 2) % 3;
		}else if(RIGHT == 0){
			set_pos = (set_pos + 1) % 3;
		}
		
	}
}
void main(void){
	uchar buf,add = 0;
	init_fun();
//	test_disply3();
	while(1){
		read_time(time_data);
/*
		add++;
		if(add == 10)time_data[6]++;
	*/	
		add %= 10;
		scan_key();
		updat_time();
		buf = load_time[0]*60;
		buf += load_time[1];
		buf *= 60;
		buf += load_time[2];
		
		buf -= (time_data[4]*260 + time_data[5]*60 + time_data[6]);
		if(buf <= 0 && buf >= -5)
			LED = 0;
		else
			LED = 1;
		
		display_all();
		//dely_US(300);
	}

}