//*************************************************************************//
//* Name				:   main.c
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic 电子称 主文件
//* $Author				:   superc
//*
//* Copyright 2012 by solidic
//*
//* Recode:
//*						1: 2012-11-07  First Creat By Superc
//*
//*
//*
//*************************************************************************//
#pragma code symbols debug oe
#define EXTERN
#include <REG52.H>
#include <math.h>
//**************************************************************************
// -- 全部头文件 集中管理 ---- ，版本修改文件名会变动 ---  必须修改 这个文件
//**************************************************************************
#include "./_solidic/head_file_version.h"


main()
{
//************************************************************************************************************************
// -- 初始化系统 --
//************************************************************************************************************************
	P0 = 0xfb;	//P0^2(BUZZER_P) = 0
	P1 = 0xef;	//P1.4(LCD_LAMP) = 0;
	P2 = 0xff;		
//--初始化PWM--
	PWMF_H  = 0x00;
	PWMF_L  = 0xff;
	PWM0  	= LCD_LAMP_B4;
	PWM1  	= 0xff;	
	PWMCON  = 0x04;	//PWM0-P1.4(LCD_LAMP)输出波形(当PWM0=0xff时,输出高电平)

//--配置IO模式 
	//配置P0 为 纯模拟输入
	P0M0 = 0x00;
	P0M1 = 0xff;
	//配置P1 为 纯模拟输入
	P1M0 = 0x00;
	P1M1 = 0xff;
//---配置Sigma_delta ADC---
	//配置P2.0 P2.1 只输入
	P2M0 = 0x00;
	P2M1 = 0xff;
//--关键管脚的IO配置
	IO_MODE_RESTORE_A_EN;
	//--配置ADC---
	EIE |= 0x04;		//打开SG ADC 中断
	//SGADCON = SGADCON_DEFAULT; 	//20Hz
	//SGADCON2 = 0x30;	//Pga-1/2斩波 Adc-1/2斩波
	//SGADCON2 |= 0x05;	//Pga-1/4斩波 Adc-1/4斩波
	//锁定P2口为纯模拟输入，锁定SGADCON,SGADCON2
	//开启ALDO同步SG-ADC,关闭Temp;
	PD_CON = PD_CON_DEFAULT;	
//--选择CPU频率
//-- 条件编译: 2分频时钟模式 -- begin --
#ifdef SDI5219_HALF_SPEED
	CKCON |= 0x01;
#endif
//-- 条件编译: 2分频时钟模式 -- end --
//--初始化定时器
	TMOD = 0x11;	//定时器0、1都工作在16位状态下				
	TH0 = 0x80;                       
	TL0 = 0x00;
	//TH1 = 0x00;                       
	//TL1 = 0x00;
	//TR0  = 1;	//开计数器0,用于控制键盘扫描					
	//TR1  = 1;	//开计数器1,用于时间控制			
//--配置中断
	IT0 = 0;		//设定外部中断0 采用低电平触发

	IE   = 0x80;	
	//--配置中断优先级
	IP = 0x05;		//外部中断0,1为高优先级
	EIP = 0x06;		//低频唤醒，24位ADC中断为高优先级
//---变量初始化---
	Led_bright = 0x03;
	System_data.sys_struct.Counter_sample_num = 10;
//-----关闭显示---------------
	LCD_LAMP_OFF; // 关闭背光灯	
	//--IO口配置--
	IO_MODE_DSP_EN;
	dsp_updata(1); //关闭显示
	//--初始化LCD模式
	write_lcd(0,0x01);
	write_lcd(0,0x18);
	write_lcd(0,0x29);
	write_lcd(0,0x02); //关闭液晶显示
	write_lcd(0,0x40); //蜂鸣器频率4K
	write_lcd(0,0xe3); //正常工作模式
	//--IO口配置--
	IO_MODE_DSP_DIS;
//--延时，保证电压检测以及EEPROM操作准确
	delay_ms(500);
//--- 初始化看门狗 --------
	//---打开看门狗
	EA = 0;
	WD_TA = 0x05;
	WD_TA = 0x0a;
	WDCON = 0xbf;	//(SDI5216)看门狗时间 4s钟,(关闭)低频唤醒0.2s
	EA = 1;
	//--打开低频唤醒中断
	EIE |= 0x02;	//(SDI5216)不开启低频唤醒中断
//----初始化秤的标定点----
	init_cal_point();
//----初始化显示----
	Price_light_symbol = 1; //根据设置 决定 开机 是否开启背光
	LCD_LAMP_ON; // 打开背光灯	
	buzzer(0);
	Led_dsp_updata = 1;
	Tape_weight = 0;
//************************************************************************************************************************
// -- 处理主循环 --
//************************************************************************************************************************
	while(1)
	{
//--SDI5219---
		//--关键管脚的IO配置
		IO_MODE_RESTORE_A_EN;
		//************************************************
		//---喂狗--及其他安全处理-begin-
		//************************************************
		EA = 0;
		WD_TA = 0x05;
		WD_TA = 0x0a;
		WDCON = 0xbf;	//(SDI5216)看门狗时间 4s钟,(关闭)低频唤醒0.2s
		EA = 1;

		//************************************************
		//---按键处理-----------------begin---------------
		//************************************************
		if( Key_press && (!Key_response) )
		{	
			key_scan();
			//Flash 安全验证码 A
			NRM_securty_a = 0xaa;	//Flash 安全验证码 A
			if(ADbuf_init.b[0] != NONE)
			{
				Key_counter_en = 1; //开始按键时间计数 

				buzzer(0);
				//处理 显示休眠
				if(Sleep_dsp_mode)
				{
					Weight_stable_counter = 0;
					//任意按键 关闭休眠显示功能
					Sleep_dsp_mode = 0;
				}
				Led_dsp_updata = 1; //LED显示需要更新

				//---处理 "ZERO" 功能键 ------------	
				if(ADbuf_init.b[0] == ZERO)
					Cal_zero_en = 1;
/*				//---处理 "UNIT_KEY" 功能键 ------------
				if(ADbuf_init.b[0] == UNIT_KEY)
					Cal_unit_en = 1;
				//---处理 "MODE_KEY" 功能键 ------------
				if(ADbuf_init.b[0] == MODE_KEY)
					Cal_mode_en = 1;*/
				//---处理 "SET_KEY" 功能键 ------------
				if(ADbuf_init.b[0] == SET_KEY)
					Cal_set_en = 1;
				//---处理 "TAPE" 功能键 ------------
				if(ADbuf_init.b[0] == TAPE)
					Cal_tape_en = 1;
				//---处理 "ARROW_KEY" 功能键 ------------
				if(ADbuf_init.b[0] == ARROW_KEY) // ARROW_KEY
				{
					unsigned char set_step;
					set_step = Active_mode & 0x1f;	//得到 设定 标定 的 步骤

					if((Active_mode & 0x20) == 0x20)	//重量标定模式
					{
						switch(set_step)
						{
							case 1:		if(Core_data.ee_struct.Cal_d == (1 << INTER_WEIGHT_NUM))
											Core_data.ee_struct.Cal_d = (2 << INTER_WEIGHT_NUM);	//分度为 2g
										else
										if(Core_data.ee_struct.Cal_d == (2 << INTER_WEIGHT_NUM))
											Core_data.ee_struct.Cal_d = (5 << INTER_WEIGHT_NUM);	//分度为 5g
										else
										if(Core_data.ee_struct.Cal_d == (5 << INTER_WEIGHT_NUM))
											Core_data.ee_struct.Cal_d = (10 << INTER_WEIGHT_NUM);	//分度为 10g
										else
										if(Core_data.ee_struct.Cal_d == (10 << INTER_WEIGHT_NUM))
											Core_data.ee_struct.Cal_d = (20 << INTER_WEIGHT_NUM);	//分度为 20g
										else
										if(Core_data.ee_struct.Cal_d >= (20 << INTER_WEIGHT_NUM))
											Core_data.ee_struct.Cal_d = (1 << INTER_WEIGHT_NUM);//分度为 1g
										follow_zero_cal();	
										break;
							case 2:		if(WEIGHT_POINT_FULL)	//处理重量小数点	
											WEIGHT_POINT_CLR;
										else
											WEIGHT_POINT_NUM_ADD;
										break;
/*							case 3:		if(Core_data.ee_struct.Follow_zero_num >= 10)	//确定追零范围
											Core_data.ee_struct.Follow_zero_num = 1;
										else
											Core_data.ee_struct.Follow_zero_num ++;
										follow_zero_cal();
										break;*/
							case 4:		Price = 0;	//-- 箭头按键 输入数据清零
										A_cycle_num = 0;	//-- 箭头按键 状态 (0-9)清零
 										A_input_setp = 1;	//-- 箭头按键 输入位置初始化
										Active_mode ++;	//-- 进入 箭头按键 输入
										break;	
							case 5:		a_input_analyse();	//进行键盘按键输入处理
										break;
							case 6:		a_input_analyse();	//进行键盘按键输入处理
										break;
							case 7:		Price = 0;	//-- 箭头按键 输入数据清零
										A_cycle_num = 0;	//-- 箭头按键 状态 (0-9)清零
 										A_input_setp = 1;	//-- 箭头按键 输入位置初始化
										Active_mode --;		//-- 进入 箭头按键 输入
										break;										
							case 0x0b:	if(!Cal_temp_bit) 	// Filt-- 
											Active_mode ++;	//-- 进入 箭头按键 输入
										else				//-- 控制显示 速度
										{
											if(DSP_SPEED_FULL)
												DSP_SPEED_CLR;
											else
												DSP_SPEED_NUM_ADD;
										}
										break;
							case 0x0c:	if(!Cal_temp_bit) 	// Da----
											Active_mode += 2;	//-- 进入 箭头按键 输入
										else				//-- 抗震幅度				
										{
											if(GLITCH_REMOVE_BIT_FULL)
												GLITCH_REMOVE_BIT_CLR;
											else
												GLITCH_REMOVE_BIT_ADD;
										}
										break;
							case 0x0d:	
										break;
							case 0x0e:	if(!Cal_temp_bit) 	// B----
											Active_mode ++;	//-- 进入 箭头按键 输入
										else				//-- 背光灯--方式
										{
											if(LCD_LAMP_MODE_FULL)
												LCD_LAMP_OFF_SET;
											else
												LCD_LAMP_MODE_NUM_ADD;
										}
										break;
							case 0x0f:	if(!Cal_temp_bit) 	// ON----
										{
											Active_mode &= 0xc0;	
											Active_mode |= 0x2b;	//进入标定模式 的第11个状态（Filt--）
										}
										else				//-- 开机重量受限				
										{
											if(INIT_ZERO_SET_EN)
												INIT_ZERO_SET_CLR;
											else
												INIT_ZERO_SET_SET;
										}
										break;
							case 0x15:	Price = 0;	//-- 箭头按键 输入数据清零
										A_cycle_num = 0;	//-- 箭头按键 状态 (0-9)清零
 										A_input_setp = 1;	//-- 箭头按键 输入位置初始化
										Active_mode ++;	//-- 进入 箭头按键 输入
										break;

							case 0x16:	a_input_analyse();	//进行键盘按键输入处理
										break;
							case 0x17:	Price = 0;	//-- 箭头按键 输入数据清零
										A_cycle_num = 0;	//-- 箭头按键 状态 (0-9)清零
 										A_input_setp = 1;	//-- 箭头按键 输入位置初始化
										Active_mode ++;	//-- 进入 箭头按键 输入
										break;	
							case 0x18:	a_input_analyse();	//进行键盘按键输入处理
										break;	
							case 0x19:	if(WARRING_TYPE_FULL)	//箭头按键 设定 报警模式
											WARRING_TYPE_NO_SET;
										else
											WARRING_TYPE_NUM_ADD;
										break;
							case 0x1d:	WARRING_COUNT_XOR; //切换重量报警 和 数量报警
										break;
							default:	break;
						}//--switch(set_step)
					}//--if((Active_mode & 0x20) == 0x20)	//重量标定模式
					else
					if((Active_mode & 0xe0) == 0x40)	//计数模式下的取样设定
					{
						switch(set_step)
						{
							case 1:		if(System_data.sys_struct.Counter_sample_num <= 10)	
											System_data.sys_struct.Counter_sample_num = 20;
										else
										if(System_data.sys_struct.Counter_sample_num == 20)	
											System_data.sys_struct.Counter_sample_num = 50;
										else
										if(System_data.sys_struct.Counter_sample_num == 50)	
											System_data.sys_struct.Counter_sample_num = 100;
										else
										if(System_data.sys_struct.Counter_sample_num == 100)	
											System_data.sys_struct.Counter_sample_num = 200;
										else
										if(System_data.sys_struct.Counter_sample_num == 200)	
											System_data.sys_struct.Counter_sample_num = 500;
										else
										if(System_data.sys_struct.Counter_sample_num == 500)	
											System_data.sys_struct.Counter_sample_num = 1000;
										else
										if(System_data.sys_struct.Counter_sample_num >= 1000)	
											System_data.sys_struct.Counter_sample_num = 10;
										break;	
							default:	break;
						}//--switch(set_step)
					}//--if((Active_mode & 0xe0) == 0x40)	//计数模式下的取样设定
					else
					if((Active_mode & 0xe0) == 0x80)	//模式下的设定
					{
						switch(change_num_a)
							{
							    case 1:		if(Core_data.ee_struct.Follow_zero_num >= 5)
												Core_data.ee_struct.Follow_zero_num = 1;
											else
												Core_data.ee_struct.Follow_zero_num ++;
											change_num_b = Core_data.ee_struct.Follow_zero_num;
											follow_zero_cal();
											break;
								case 2:     if(Core_data.ee_struct.ZERO_NUM == 4)
								            {
								               Core_data.ee_struct.ZERO_NUM = 10;
	                                           change_num_b = 2;
											}
											else
											if(Core_data.ee_struct.ZERO_NUM == 10)
								            {
								               Core_data.ee_struct.ZERO_NUM = 15;
	                                           change_num_b = 3;
											}
											else
											if(Core_data.ee_struct.ZERO_NUM == 15)
								            {
								               Core_data.ee_struct.ZERO_NUM = 20;
	                                           change_num_b = 4;
											}
											else
											if(Core_data.ee_struct.ZERO_NUM == 20)
								            {
								               Core_data.ee_struct.ZERO_NUM = 30;
	                                           change_num_b = 5;
											}
											else
											if(Core_data.ee_struct.ZERO_NUM == 30)
								            {
								               Core_data.ee_struct.ZERO_NUM = 4;
	                                           change_num_b = 1;
											}
											break;										
	                            case 3:     if(Core_data.ee_struct.TAPE_NUM == 30)
								            {
								               Core_data.ee_struct.TAPE_NUM = 50;
	                                           change_num_b = 2;
											}
											else
											if(Core_data.ee_struct.TAPE_NUM == 50)
								            {
								               Core_data.ee_struct.TAPE_NUM = 60;
	                                           change_num_b = 3;
											}
											else
											if(Core_data.ee_struct.TAPE_NUM == 60)
								            {
								               Core_data.ee_struct.TAPE_NUM = 80;
	                                           change_num_b = 4;
											}
											else
											if(Core_data.ee_struct.TAPE_NUM == 80)
								            {
								               Core_data.ee_struct.TAPE_NUM = 100;
	                                           change_num_b = 5;
											}
											else
											if(Core_data.ee_struct.TAPE_NUM == 100)
								            {
								               Core_data.ee_struct.TAPE_NUM = 30;
	                                           change_num_b = 1;
											}
											break;
								case 4:     if(POWER_MODE_3)
								            {
											    POWER_MODE_CLK;
	                                            change_num_b = 1;
											}
											else
	                                        if(POWER_MODE_1)
								            {
											    POWER_MODE_ADD;
	                                            change_num_b = 2;
											}
	                                        else
											if(POWER_MODE_2)
								            {
											    POWER_MODE_ADD;
	                                            change_num_b = 3;
											}
											break;
								case 5:     if(DISPLAY_MODE_1)
								            {	
												DISPLAY_MODE_SET;										    
											    change_num_b = 2;
											}										
											else
											if(DISPLAY_MODE_2)
											{
												DISPLAY_MODE_CLK;
											    change_num_b = 1;
											}
											break;
								case 6:     if(POWER_6V_BIT_EN)
								            {
											    POWER_BIT_CLK;
											    change_num_b = 2;
											}
											else
											if(POWER_4V_BIT_EN)
											{
											    if(POWER_BIT_SET);
											    change_num_b = 1;
											}
											break;
	//							case 2:		//--输入 百分比取样 重量
	//										a_input_analyse();	//进行键盘按键输入处理
	//										break;									
								default:	break;
							}//--switch(set_step)
					}//--if((Active_mode & 0xe0) == 0x80)	//百分比模式下的取样设定
				}//--if(KEY_CODE == ARROW_KEY) // ARROW_KEY
				// --- 设定 按键已处理 标志位 ----
				Key_response = 1;
			}//--if(ADbuf_init.b[0] != NONE)
		}//--if( Key_press && (!Key_response) )

		if(!Key_press && (Cal_zero_en || Cal_unit_en || Cal_mode_en || Cal_set_en || Cal_tape_en))
		{
				Led_dsp_updata = 1; //LED显示需要更新
				//---处理 "ZERO" 功能键 ------------	
				if(Cal_zero_en)
				{
					if(A_input_setp == 0)	//处理追零 (临时征用Counter_sample_num，减少内存消耗，注意恢复)
					{	
						if(Weight_internal_last < (15000 << INTER_WEIGHT_NUM)) //一定小于内部的 10% (3000 / 30000)
						{
							if(Sign_code)
								System_data.sys_struct.Counter_sample_num = labs((long)(Zero_weight_accum) - (long)(Weight_internal_last));
							else
								System_data.sys_struct.Counter_sample_num = labs((long)(Zero_weight_accum) + (long)(Weight_internal_last));
					//--- 当计算为外部重量时，由于存在Core_data.ee_struct.Cal_d的可能放大效果，可能溢出
					//--- 同时，计重有6位数，Cal_max_weight 最大可为999，同样可能在Cal_max_weight * (30 << INTER_WEIGHT_NUM))溢出
					//--- 1000 * 30 * 8 = 24万，已经溢出了
					//--- 1000 * 30 * 4 = 12万，溢出
					//--- 1000 * 30 * 2 = 6万，无溢出
					//--- 需要将所有 INTER_WEIGHT_NUM 转换到 6万内码来判断追零

					//--- 条件编译: 内码模式 -- begin ---
					#if INTER_WEIGHT_NUM >= 2
							System_data.sys_struct.Counter_sample_num >>= (INTER_WEIGHT_NUM - 1);
					#endif
					//--- 条件编译: 内码模式 -- end ---
							if(Core_data.ee_struct.Intel_cal_d < 0x80)
							{
								System_data.sys_struct.Counter_sample_num /= Core_data.ee_struct.Intel_cal_d;
							}
							else
							{	
								unsigned char j;
								j = Core_data.ee_struct.Intel_cal_d & 0x7f; 
								System_data.sys_struct.Counter_sample_num *= j;
							}
							
							System_data.sys_struct.Counter_sample_num += (Core_data.ee_struct.Cal_d / 2);
							System_data.sys_struct.Counter_sample_num /= Core_data.ee_struct.Cal_d;	
							System_data.sys_struct.Counter_sample_num *= Core_data.ee_struct.Cal_d;					

							if(System_data.sys_struct.Counter_sample_num <= (Core_data.ee_struct.Cal_max_weight * (30 << 1)) )
							{
								Zero_code = ADcode;
								Weight_lock_symbol = 0;
								if(Sign_code)
									Zero_weight_accum = Zero_weight_accum - (int)(Weight_internal_last);
								else
									Zero_weight_accum = Zero_weight_accum + (int)(Weight_internal_last);
								Weight_internal_last = 0;
							
							}	
						 	//--恢复Counter_sample_num
							init_cal_point();
						}				
						//if(Weight_zero)
						//	Price_light_symbol = !Price_light_symbol;
					}
					else
//					if((Active_mode & 0x20) == 0x20)	//重量标定模式
					{
						Price = 0;	//-- 箭头按键 输入数据清零
						A_cycle_num = 0;	//-- 箭头按键 状态 (0-9)清零
 						A_input_setp = 1;	//-- 箭头按键 输入位置初始化
					}
					if((Active_mode & 0x80) == 0x80)
					{
					    Active_mode = 0;
                        change_num_a = 0;
						change_num_b = 0;
						Dsp_state_con = 0;
//						core_memory_write();
						init_cal_point();
					}
					Cal_zero_en = 0;
				}

/*				//---处理 "UNIT_KEY" 功能键 ------------
				if(Cal_unit_en)
				{
					if((Active_mode & 0x20) != 0x20)
						Imperial_bit = !Imperial_bit;
					Cal_unit_en = 0;
				}

				//---处理 "MODE_KEY" 功能键 ------------
				if(Cal_mode_en)
				{
					unsigned char i;
					if((Active_mode & 0x80) == 0x80)	//在"标定"模式下,直接退出标定模式
					{
						Active_mode = 0x00;
						change_num_a = 0;
						change_num_b = 0;
						Dsp_state_con = 0;
						init_cal_point();			//此处增加 读取 之前的 EEPROM 用于恢复 设定为成功破坏的数据
					}
					else
					if((Active_mode & 0x20) == 0x20)	//在"标定"模式下,直接退出标定模式
					{
						if((Active_mode & 0x1f) <= 4) // 需要储存
						{
							//--------- 将 标定 参数 存入EEPROM -------------------------------
							core_memory_write();
							delay_ms(200);
							buzzer(0);
						}
						Active_mode = 0x00;
						init_cal_point();			//此处增加 读取 之前的 EEPROM 用于恢复 设定为成功破坏的数据
					}
					else
					{
						//非标定模式的话，则在 计重 - 百分比 模式 间切换
						Active_mode += 0x40;
					//-- 条件编译: 百分比 -- begin --
					#ifdef PERCENTAGE_FUN_EN
						if((Active_mode & 0xc0) == 0xc0)
							Active_mode &= 0x3f;
					#else
						if((Active_mode & 0xc0) == 0x80)
							Active_mode &= 0x3f;
				  	#endif
					//-- 条件编译: 百分比 -- end --	
						if((Active_mode & 0xc0) != 0x40)
						{
							//--读取 用户 数据--
							for(i=0;i<SYS_DATA_LENTH;i++)
								System_data.b[i] = nvm_data_read_byte(ADDR_SYS_DATA + i);
						}
					}
//					if((Active_mode & 0xc0) == 0x40)
//                   	Active_mode |= 0x01;
					Active_mode &= 0xc0;
					Imperial_bit = 0; //每次按下模式键 都清楚 英制
					Cal_mode_en = 0;
				}
*/
				//---处理 "SET_KEY" 功能键 ------------
				if(Cal_set_en)
				{
					if((Active_mode & 0x20) == 0x20) 	//在"标定"模式下,直接退出标定模式
					{
						if((Active_mode & 0x1f) <= 4) // 需要储存
						{
							//--------- 将 标定 参数 存入EEPROM -------------------------------
							core_memory_write();
							delay_ms(200);
							buzzer(0);
						}
						Active_mode = 0x00;
						init_cal_point();//此处增加 读取 之前的 EEPROM 用于恢复 设定为成功破坏的数据
					}
/*					else
					if(Active_mode== 0x40)
					{
						Active_mode++;
					}*/
					else
					if((Active_mode & 0xc0) == 0x80)
					{
					    if(change_num_a < 6)
						   change_num_a ++;
						else
						   change_num_a = 1;
					}
					else
					{
						if((Active_mode & 0x01) == 0x01)
						    Active_mode = 0;
						else
						{
							//非标定模式的话，则在 计重 - 计数模式 间切换
//                       Active_mode &= 0x40;
//							Active_mode += 0x40;
							if((Active_mode & 0xc0) == 0x40)
								Active_mode &= 0x3f;
							else
							if((Active_mode & 0xc0) == 0x00)
								Active_mode += 0x40;
						}	
					}
					if((Active_mode & 0xc0) == 0x40)
                    	Active_mode |= 0x01;
//					Active_mode &= 0xc0;
					Imperial_bit = 0; //每次按下模式键 都清楚 英制
					Cal_set_en = 0;
				}

				//---处理 "TAPE" 功能键 ------------
				if(Cal_tape_en)
				{
					if((Active_mode & 0x1f) == 0x00)
					{
						Tape_en = 1;
					}
					else
					{
						unsigned char set_step;
						set_step = Active_mode & 0x1f;		//得到 设定 标定 的 步骤

						if((Active_mode & 0x20) == 0x20)	//重量标定模式
						{
							switch(set_step)	//注意：标定模式下，31步(0x1f)用来取零点，30步(0x1e)用来取标定点
							{
								case 1:	 	Active_mode ++;break;// 分度	确认
								case 2:		Active_mode += 2;break;// 小数点 确认
								//case 3:		// 追零 确认
								//			Active_mode ++;	//直接进入 下一标定阶段
								//			break;
								case 4:		Active_mode = 0x3f;		//直接进入 零点 设定 (0x3f)
											Warring_counter = 0;
											Ave_step_en = 1;
											break;
								case 5:		//--输入 最大 重量
											if(A_input_setp == 6)
											{
												A_input_setp = 0;
												Core_data.ee_struct.Cal_max_weight = Price / 1000;//将 箭头按键输入的数据 取出到具体变量的语句
												Active_mode = 0x3f;		//直接进入 零点 设定 (0x3f)
												Warring_counter = 0;
												Ave_step_en = 1;
												Price = data_trans_out(Core_data.ee_struct.Cal_inter_weight); //显示标定砝码
											}
											else
											{
												A_input_setp ++;	//处理 箭头按键 数字输入;
												A_cycle_num = 0;
											}
											break;
							    case 6:		//--输入 标定点 重量
											if(A_input_setp == 6)
											{
												A_input_setp = 0;
												Core_data.ee_struct.Cal_inter_weight = Price; //存储标定砝码 用于恢复显示
												//--此处没有 break,将直接进入 case 7 进入 标定点取样
											}
											else
											{
												A_input_setp ++;	//处理 箭头按键 数字输入;
												A_cycle_num = 0;
												break;
											}
								case 7:		Active_mode = 0x3e;		//直接进入 标定点 设定 (0x3e)
											Warring_counter = 0;
											Ave_step_en = 0;
										//	Core_data.ee_struct.Cal_inter_weight = Price;
											break;
/*								case 8:		Active_mode = 0x20 + 0x1c;		//直接进入 标定点2 设定 (0x3c)
											Warring_counter = 0;
											Ave_step_en = 0;
											break;
								case 9:		Active_mode = 0x20 + 0x1b;		//直接进入 标定点3 设定 (0x3b)
											Warring_counter = 0;
											Ave_step_en = 0;
											break;*/
								case 0x0b:
								case 0x0c:	
								case 0x0d:	
								case 0x0e:	
								case 0x0f:	if(!Cal_temp_bit) // Filt--
												Cal_temp_bit = 1;
											else
											{
												Cal_temp_bit = 0;
												core_memory_write(); //需要保存
												delay_ms(100);
											//	Save_user_data = 1;
												buzzer(0);
											}
											break;	
								case 0x15:	Active_mode += 2;	//直接进入下限频率显示 set_step == 3
											break;
								case 0x16:	//--输入 报警上限
											if(A_input_setp == 6)
											{
												Active_mode ++;			//直接进入下限频率显示 set_step == 3
												A_input_setp = 0;
												Money = Price;	//设定上限 //将原来We	ight_HH用Money名字来替代
											}
											else
											{
												A_input_setp ++;	//处理 箭头按键 数字输入;
												A_cycle_num = 0;
											}
											break;
								case 0x17:	Active_mode += 2;	//直接进入 报警范围设定
											break;
								case 0x18:	//--输入 报警下限
											if(A_input_setp == 6)
											{
												Active_mode ++;	//直接进入 报警范围设定
												A_input_setp = 0;
												Money_accum.w = Price;	//设定下限 //将原来Weight_LL用Money_accum.w名字来替代
											}
											else
											{
												A_input_setp ++;	//处理 箭头按键 数字输入;
												A_cycle_num = 0;
											}
											break;
								case 0x19:	//--完成 报警设定
											Save_user_data = 1; 		//存储系统设置
											Active_mode &= 0xc0;
											WARRING_SET_SET;		//设定报警设定标志位
											break;
								case 0x1d:	Active_mode &= 0xe0;
											Active_mode += 0x15;
											break;										
								default:	break;
							}//--switch(set_step)
						}//--if((Active_mode & 0x20) == 0x20)	//重量标定模式
						else
						if((Active_mode & 0xe0) == 0x40)	//计数模式下的取样设定
						{
							switch(set_step)
							{
								case 1:		Active_mode ++;
											break;	
								case 2:		//取样品重量及分析
											if( Sign || (Weight_internal_last == 0))
											{
												Active_mode ++;
											}
											else
											{
												Active_mode = 0x5f;	
												Warring_counter = 0;
												Ave_step_en = 0;
											}
											break;
								case 3:		Active_mode = 0x00; 	//返回正常计重模式
											init_cal_point();		//此处增加 读取 之前的 EEPROM 用于恢复 设定为成功破坏的数据
											break;
								default:	break;
							}//--switch(set_step)
						}//--if((Active_mode & 0xe0) == 0x40)	//计数模式下的取样设定
						else
						if((Active_mode & 0x80) == 0x80)
						{	
						    core_memory_write();
							delay_ms(200);
							buzzer(0);
						}
					}//---else---if((Active_mode & 0x1f) == 0x00)
					Cal_tape_en = 0;
				}//---if(Cal_tape_en)
		} //---if(!KEY_PRESS && (Cal_zero_en || Cal_unit_en || Cal_mode_en || Cal_set_en || Cal_tape_en))


		//************************************************
		//---按键处理-----------------end---------------
		//************************************************

		//************************************************
		//---长时间按键处理-----------------begin---------
		//************************************************
		if(Key_counter_en && ((Active_mode & 0x3f) == 0) ) // 防止和 开机按小数点 还原参数 冲突
		{
			unsigned char j;
			Led_dsp_updata = 1; //LED显示需要更新
			//--条件编译: 按键关机 --begin--
			#ifdef KEY_PRESS_PD_FUN_EN
				if(Mcu_stop_rec || Mcu_stop_pre)
					j = PRESS_KEY_COUNTER_NUM_2;
				else
			#endif
			//--条件编译: 按键关机 --end--
					j = PRESS_KEY_COUNTER_NUM;

			if( (ADbuf_init.b[0] == TAPE) && (Stable_light_counter >= j) )
			{
				Key_counter_en = 0;
				Stable_light_counter &= 0x0f;
				//非标定模式的话:
				if((Active_mode== 0x00)||(Active_mode== 0x40))	//如果在 计重/计数 正常显示状态的话，进入上下限设定
				{
					Active_mode += (0x20 + 0x1d); // 0x20 + 0x1d
				}
				buzzer(0);
				Cal_tape_en = 0;
			}

			if( (ADbuf_init.b[0] == ZERO) && (Stable_light_counter >= j) )//标定
			{
				Key_counter_en = 0;
				Stable_light_counter &= 0x0f;
				if((Active_mode & 0x3f) == 0)	// 非 标定设定状态下
				{
					Active_mode |= 0x21;
					Active_mode &= 0xe1;		//进入标定模式 的第一个状态
					Data_error = 0;
					Imperial_bit = 0; //每次进入标定都清楚英制
				}
				buzzer(0);
				Cal_zero_en = 0;
			}
			if( (ADbuf_init.b[0] == SET_KEY) && (Stable_light_counter >= j) )
			{
				Key_counter_en = 0;
				Stable_light_counter &= 0x0f;
				Cal_tape_en = 0;
				if((Active_mode & 0x3f) == 0)// 非标定设定状态下
				{
					Active_mode = 0x88;
					change_num_a = 1;				
					Cal_temp_bit = 0;
					Data_error = 0;
				}
				buzzer(0);
				Cal_set_en = 0;
			}
		}
		//************************************************
		//---长时间按键处理-----------------end---------
		//************************************************

		//---------------------------------------------------------------
		//-- 模式控制 --
		if((Active_mode & 0x3f) == 0x00) //正常 计重 计数 百分比 判断
		{
			if(((Active_mode & 0xc0) == 0x40) && COUNT_SET_DIS)		//计数模式,且没有进行计数取样
				Active_mode |= 0x01;	
		//-- 条件编译: 百分比 -- begin --
		#ifdef PERCENTAGE_FUN_EN
			if(((Active_mode & 0xc0) == 0x80) && PERCENT_SET_DIS)	//百分比模式,且没有进行百分比取样
				Active_mode |= 0x01;
	  	#endif
		//-- 条件编译: 百分比 -- end --
		}
		//---------------------------------------------------------------
		//-- 存储用户数据 ---
		if(Save_user_data)
		{
			System_data.sys_struct.Weight_HH = Money;					//将原来Weight_HH用Money名字来替代
			System_data.sys_struct.Weight_LL = Money_accum.w;			//将原来Weight_LL用Money_accum.w名字来替代
			uer_memory_write();
			Save_user_data = 0;
			init_cal_point();
			delay_ms(100);
			buzzer(0);
		}

		//************************************************
		//---ADC数据处理-----------------begin---------
		//************************************************
	   	if(ADready)
		{
			EA = 0; //数据处理期间 必须 -- 关闭中断
			Filter_ADdata();
           	if(Start_up_end)		//非开机步骤
		   	{
				internal_weight_cal(); //重量段位 显示重量
				ext_weight_cal();	
			}
			else		//开机步骤 用"A_input_setp"来表示开机步骤
           	if(A_input_setp < 0x0a)
		   	{
				Follow_zero_counter ++;
				if(Follow_zero_counter >= 7)
				{
				    Follow_zero_counter = 0;
				    A_input_setp++;	
					Led_dsp_updata = 1;
					if(A_input_setp == 0x0a)
					{
						Vol_state = 1;	
						Zero_weight_accum = power_detect();
					}
		           	//buzzer(0);
				}
			}						

			//-----------动态取内码------------begin------------
			//Warring_counter = 0;  //进入零点获取时，初始化计数器
			//注意，在动态取内码 时 要使用到 【Counter_sample_weight】
			if( (!Start_up_end&&(A_input_setp==0x0a)) || (Active_mode == 0x3f) || (Active_mode == 0x3e))
			{
				unsigned char j;
				bit average_end;
				average_end = 0;
				if((Active_mode == 0x3f) || (Active_mode == 0x3e))
					j = LONG_FILTER_NUM;
				else
					j = LONG_FILTER_NUM_2;

				if(Warring_counter == 0) //刚进入的时候，将 Price 清空
				{	
					Price = 0;
					Vol_auto_det_counter = 0;
					System_data.sys_struct.Counter_sample_weight = ADcode;
				}	
				Warring_counter ++;			
				if(Ave_step_en)		//开始执行采样
				{
					if(labs(ADcode - System_data.sys_struct.Counter_sample_weight) <= 7) //如果内码稳定在一定范围内的话
					{
						Price += ADcode;
						Vol_auto_det_counter ++;
					}
					else  //变动较大的内码重新取
					{
						Price = 0;
						Vol_auto_det_counter = 0;
						System_data.sys_struct.Counter_sample_weight = ADcode;
					}
				}
				else	//采样前丢弃的数据---防止按键时抖动--(可通过设定 Sample_step_en 来设定是否需要去抖动)
				if(	Warring_counter >= AVE_DIS_NUM )	 //-- 注意 -- : SAMPLE_DIS_NUM 必须小于 LONG_FILTER_ACCUM_NUM
				{
					Warring_counter = 0;
					Ave_step_en = 1;
				}

				if(Vol_auto_det_counter >= j) //取多个稳定内码，并平均
				{   
					Price /= j;
					average_end = 1; //用Per_cal = 0;表示取稳定内码结束
				}
				else
				if(Warring_counter >= LONG_FILTER_ACCUM_NUM) //取40个数还不能达到稳定的情况下，直接退出
				{   
					Price = ADcode;					
					average_end = 1; //用Per_cal = 0;表示取稳定内码结束
				}

				if(average_end)
				{
					if(!Start_up_end&&(A_input_setp==0x0a))
					{
					    Zero_code = Price;		//确定零点的内码
						A_input_setp = 0;
						Vol_state = 0;
						Weight = 0;
						Price = 0;																																																																																																														  
						//--处理背光灯---
						//if(PRICE_ZERO_DSP_DIS)
						//{
						//	Price_light_symbol = 0; //根据设置 决定 开机 是否开启背光
						//	LCD_LAMP_OFF; 			// 关闭背光灯	
						//}
						//--开启 开机 追零增大范围
						Startup_zero_symbol = 1;
						Zero_weight_accum = 0;
						Start_up_end = 1;
						Active_mode = 0x00;
						//恢复 取零位时破坏的数据
						init_cal_point();
						Led_dsp_updata = 1;
						buzzer(0);
						//--开机零位检测
						if(INIT_ZERO_SET_EN && (Zero_code >= 2220000)) //零位 加上 盖子 可能在 2100000左右
							Weight_on_error = 1;
					}
					else
					if(Active_mode == 0x3f) //取得零点
					{
					    Zero_code = Price; 	// 标定 第一步,确定零点内码;
						Active_mode = 0x27;	// = 0x20 + 7
						Zero_weight_accum = 0;
						Price = data_trans_out(Core_data.ee_struct.Cal_inter_weight); //显示标定砝码
					}
				//--条件编译: 三段标定 -- begin --
				#ifdef THREE_CAL_FUN_EN
					else
					if(Active_mode == 0x3e) //取得标定点 1
					{
						if(Price > Zero_code)
						{
							Core_data.ee_struct.Cal_point_code = Price - Zero_code;  //得到第一个标定点	
							Active_mode = 0x28;	// = 0x20 + 8
							buzzer(1);
						}
						else
						{
							buzzer(1);
							buzzer(1);
							init_cal_point();  //其他地方也需要注意 【Cal_point_code】的大小
							Active_mode &= 0xdf;	//退出标定模式
							Active_mode &= 0xc0;		
						}																																																			
					}
					else
					if(Active_mode == 0x3c) //取得标定点 2 的补偿量
					{
						if(Price > Zero_code)
						{
							Price -= Zero_code;  //得到第二个标定点	
							Money = Core_data.ee_struct.Cal_point_code << 1;
							if(labs(Price - Money) > (Core_data.ee_struct.Cal_point_code / 4))
							{
								Core_data.ee_struct.Cal_delta_1 = 0;
								Get_cal_data = 1;
							}
							else
							if(labs(Price - Money) > 20000)
							{
								Core_data.ee_struct.Cal_delta_1 = 0;
								Get_cal_data = 1;
							}
							else
							{							
								Core_data.ee_struct.Cal_delta_1 = Price - Money;
								Active_mode = 0x29;	// = 0x20 + 9
								buzzer(1);
							}
						}
						else
						{
							buzzer(1);
							buzzer(1);
							init_cal_point();  //其他地方也需要注意 【Cal_point_code】的大小
							Active_mode &= 0xdf;	//退出标定模式
							Active_mode &= 0xc0;
							Led_dsp_updata = 1;		
						}																																																			
					}
					else
					if(Active_mode == 0x3b) //取得标定点 3 的补偿量 / 完成标定
					{
						if(Price > Zero_code)
						{
							Price -= Zero_code;  //得到第二个标定点	
							Money = Core_data.ee_struct.Cal_point_code * 3;
							if(labs(Price - Money) > (Core_data.ee_struct.Cal_point_code / 2))
							{
								Core_data.ee_struct.Cal_delta_2 = 0;
							}
							else
							if(labs(Price - Money) > 20000)
							{
								Core_data.ee_struct.Cal_delta_2 = 0;
							}
							else
							{
								Core_data.ee_struct.Cal_delta_2 = Price - Money;
								Core_data.ee_struct.Cal_delta_2 -= 	Core_data.ee_struct.Cal_delta_1; //要加两次				
								Core_data.ee_struct.Cal_delta_2 -= 	Core_data.ee_struct.Cal_delta_1;
							}
							Get_cal_data = 1;
						}
						else
						{
							buzzer(1);
							buzzer(1);
							init_cal_point();  //其他地方也需要注意 【Cal_point_code】的大小
							Active_mode &= 0xdf;	//退出标定模式
							Active_mode &= 0xc0;
							Led_dsp_updata = 1;	
						}
					}
				#else
					else
					if(Active_mode == 0x3e) //取得标定点 1
					{
						if(Price > Zero_code)
						{
							Core_data.ee_struct.Cal_point_code = Price - Zero_code;  //得到第一个标定点
							Get_cal_data = 1;
						}
						else
						{
							buzzer(1);
							buzzer(1);
							init_cal_point();  //其他地方也需要注意 【Cal_point_code】的大小
							Active_mode &= 0xdf;	//退出标定模式
							Active_mode &= 0xc0;
							Led_dsp_updata = 1;		
						}																																																			
					}
				#endif
				//--条件编译: 三段标定 -- end --
					
					if(Get_cal_data)
					{
						Get_cal_data = 0;
						//做相关 取标定点 操作
						Price = Core_data.ee_struct.Cal_inter_weight;
						Money = Core_data.ee_struct.Cal_point_code;
						//---------处理标定数据---------
						//标定内码 必须放在 MONEY 中
						//标定重量 必须放到 Price 中
						//会用到中间变量 Money_accum.w
						//如果有误 data_error = 1;
						//标定正确的话 得到正确的"Core_data.ee_struct.Cal_point_code"
						cal_data_process();
						//--
						Money = 0;
						Money_accum.w = 0;

						if(Data_error) //标定有误，报警退出；
						{
							buzzer(1);
							buzzer(1);
						}
						else	//开始 计算 三段修正量
						{
							if(Active_mode == 0x3b)
								Price *= 3;
							Weight_internal_last = data_trans_into();
							Weight = Price;		
							//SYSTEM_CAL_SYMBOL_CLR;
							//SYSTEM_CAL_SYMBOL_SET;
							follow_zero_cal();
							core_memory_write();
							//Weight_updata_en = 1;
						}
						//如果此处不及时回复正确的标定值，【Cal_point_code】的大小，否则容易造成【循环次数过长】死机
						//恢复 【Counter_sample_weight】
						Data_error = 0;
						init_cal_point();  //其他地方也需要注意 【Cal_point_code】的大小
						Active_mode &= 0xdf;	//退出标定模式
						Active_mode &= 0xc0;
						delay_ms(100);
						buzzer(0);					
						Led_dsp_updata = 1;
					}
				}
			}			
			//-----------动态取内码------------end------------	
			//处于 设定方式 时 每次都需要更新
			//内码查看 需要每次更新 
			//百分比 输入重量时每次都更新
			if( ((Active_mode & 0x20) == 0x20) || (Active_mode == 0x0a) || (Active_mode == 0x82) )
				Led_dsp_updata = 1;
//--SDI5219---
			if((Active_mode & 0x20) == 0x20)	//重量标定模式
			{
			//-- 条件编译: 标定状态下 刷新显示 频率降低 -- begin --
			#ifdef SDI5219_CAL_DSP_UPDATA_SLOW	
				if (Cal_dsp_slow_counter >= CAL_DSP_SLOW_NUM) // 前次价格输入起，超过6秒后，再次输入价格按键将和前面价格不关联；
				{
					Cal_dsp_slow_counter = 0;		
					Led_dsp_updata = 1;
				}
				else
				{
					Cal_dsp_slow_counter ++; //价格关联计数器 按照 ADC数据个数 计时
				}
			#else
				Led_dsp_updata = 1;
			#endif
			//-- 条件编译: 标定状态下 刷新显示 频率降低 -- end --	
			}
			//Dsp_en = 1;
			EA = 1;  //数据处理期间 必须 -- 关闭中断
		}
		//************************************************
		//---ADC数据处理-----------------end---------
		//************************************************


		//************************************************
		//---(时间相关功能块)-----------------begin-------
		//************************************************
//--SDI5219---
		if(Data_sycle_symbol)
		{
		Data_sycle_symbol = 0;
		//----- 自动电池电压检测 ------
		//----- 报警 ---------
		//正常非设定状态才检测电池，才报警。开机 或者 设定时 "Vol_auto_det_counter" "Warring_counter" 可复用	
		if(Start_up_end && ((Active_mode & 0x1f) == 0x00))	
		{
			//注意，标设定状态下，不检测电池电压，这样变量 Vol_auto_det_counter 可以用在标设定时的其他地方
			if(Vol_state)
			{
				Vol_auto_det_counter = 0;	
			}
			else
			if(Vol_auto_det_counter >= VOL_AUTO_DET_NUM) //电池电压检测
			{
				power_detect();
				Vol_auto_det_counter = 0;
				Led_dsp_updata = 1; //第一次稳定时，需要更新显示
			}
			else
			{
				Vol_auto_det_counter ++;
			}

			//注意，标设定状态下，不报警，这样变量 Warring_counter 可以用在标设定时的其他地方
		//-- 条件编译: 按键关机 -- begin --
		#ifdef KEY_PRESS_PD_FUN_EN	
			if( (Weight_overflow || Lamp_lowv || Lamp_lowv_a || Weight_warning_h || Weight_warning_l || Weight_warning_in ) && !Mcu_stop_pre && !Mcu_stop_rec )//----- 蜂鸣器报警处理,按键关机的情况下不处理报警（因为充电时，单片机实际上时启动的
		#else
			if( Weight_overflow || Lamp_lowv || Lamp_lowv_a || Weight_warning_h || Weight_warning_l || Weight_warning_in )//----- 蜂鸣器报警处理,按键关机的情况下不处理报警（因为充电时，单片机实际上时启动的
	    #endif
		//-- 条件编译: 按键关机 -- end --
			{
				Warring_counter++;
				if(Lamp_lowv_a)
				{
					if(Warring_counter >= WARRING_NUM_LOWV_2)
					{
						Warring_counter = 0;
						buzzer(0);
					}
				}
				else
				if(Lamp_lowv)
				{
					if(Warring_counter >= WARRING_NUM_LOWV_1)
					{
						Warring_counter = 0;
						buzzer(0);
					}
				}
				else
				if(Weight_warning_h)
				{
					if(Warring_counter >= WARRING_NUM_HIGH)
					{
						Warring_counter = 0;
						buzzer(0);
					}
				}
				else
				if(Weight_warning_l)
				{
					if(Warring_counter >= WARRING_NUM_LOW)
					{
						Warring_counter = 0;
						buzzer(0);
					}
				}
				else
				{
					if(Warring_counter >= WARRING_NUM_FULL)
					{
						Warring_counter = 0;
						buzzer(0);
					}
				}
			}
			else
			{
				Warring_counter = 0;
			}
		}

		//----- 数码休眠闪动 --- begin ---
		if(Sleep_dsp_mode) //如果 休眠
		{
			if(Weight_stable_counter >= WEIGHT_SLEEP_NUM)
			{
				Weight_sleep = !Weight_sleep;	//重量交替休眠显示
				Weight_stable_counter = 0;
				Led_dsp_updata_2 = 1;
			}
			else
			{
				Weight_stable_counter ++;	
			}	
		}
		//----- 数码休眠闪动 --- end ---

		//----- 检测长时间按键 --- begin ---	
		if(Key_counter_en)
		{
			if(Stable_light_counter < PRESS_KEY_COUNTER_NUM)
			{
				if(Double_counter_symbol)
				{
					Stable_light_counter += 0x10;
					Double_counter_symbol = 0;
			   	}
				else
					Double_counter_symbol = 1;
			}
		}
		//----- 检测长时间按键 --- end ---

		//----- 动态充电控制 --- begin ---
		{
			unsigned char j;
			//--1/4间歇时 调节 计时长度

		//--条件编译: 按键关机 --begin--
		#ifdef KEY_PRESS_PD_FUN_EN
			if(Mcu_stop_rec || Mcu_stop_pre)
				j = CHARGE_COUNTER_NUM / 5;
			else
	   	#endif
		//--条件编译: 按键关机 --end--
				j = CHARGE_COUNTER_NUM;
			//--如果检测到充电--
			if(Charte_bit)	//--如果检测到充电 且充电未完成--
			{
				if(Charte_bit_dis)
				{
					Batter_c = 1;		
					Batter_b = 1;		
					Batter_a = 1;
					Led_dsp_updata = 1;
					Dsp_en = 1;
				}
				else
				if (Charge_counter >= j) //动态充电
				{
					Charge_counter = 0;

					if(!Batter_c && !Batter_c_ch)
					{
						Batter_c = 1;		
					}
					else
					if(!Batter_b && !Batter_b_ch)
					{
						Batter_b = 1;		
					}
					else
					if(!Batter_a)
					{
						Batter_a = 1;		
					}
					else
					{
						if(!Batter_c_ch)
							Batter_c = 0;		
						if(!Batter_b_ch)
							Batter_b = 0;				
						Batter_a = 0;
					}
					Led_dsp_updata = 1;
					Dsp_en = 1;
				}
				else
				{
					Charge_counter ++;
				}
			}
			else
			{
				Charge_counter = 0;
			}	
	  	}
		//----- 动态充电控制 --- end ---
		}
//--SDI5219---
		//************************************************
		//---(设定 存储步骤 9)-----------------end-------
		//************************************************

		//************************************************
		//--- 显示处理 -----------------begin-------
		//************************************************
	#ifdef KEY_PRESS_PD_FUN_EN
		if( Charte_bit || !Mcu_stop_rec)
	#endif
		{	
		//--- 条件编译: 低速显示更新 -- begin ---
		#ifdef LOW_DSP_UPDATA
			if(Led_dsp_updata && Low_speed_updata)
		#else
			if(Led_dsp_updata)
		#endif
		//--- 条件编译: 低速显示更新 -- end ---
			{
				//--液晶--
				display_cal(1); //计算重量
				//--- 防止液晶 死掉 ---
				//--IO口配置--
				IO_MODE_DSP_EN;
				//初始化LCD模式
				write_lcd(0,0x01);
				write_lcd(0,0x03);
				write_lcd(0,0x18);
				write_lcd(0,0x29);
				write_lcd(0,0xe3); //正常工作模式
				//---				
				write_lcd(4,0);	// 更新LED驱动
				//--IO口配置--
				IO_MODE_DSP_DIS;
				//数码
				display_cal(0); //计算重量
				//--IO口配置--
				IO_MODE_DSP_EN;
				dsp_updata(0);// 更新LED驱动
		//--- 条件编译: 数码1616驱动 -- begin ---
		//--- 1616驱动是,dsp_updata(0)不具备打开显示功能，所以需要额外的一句
		#ifdef LED_DRIVER_1616
				dsp_updata(2);// 更新LED驱动
		#endif
		//--- 条件编译: 数码1616驱动 -- end ---
				//--IO口配置--
				IO_MODE_DSP_DIS;
				Led_dsp_updata = 0; //重量显示区更新完成
				Led_dsp_updata_2 = 0;
			}
			else
			if(Led_dsp_updata_2)
			{
				if(Sleep_dsp_mode) //在没有间歇供电的休眠情况下的数码闪动				
				{
					//--IO口配置--
					IO_MODE_DSP_EN;
					if(!Weight_sleep)
						dsp_updata(2);// 打开数码	
					else
					if(!Charte_bit)
						dsp_updata(1);// 关闭数码
					//--IO口配置--
					IO_MODE_DSP_DIS;
				}
				Led_dsp_updata_2 = 0; //重量显示区更新完成
			}
		}

		//************************************************
		//--- 显示处理 ----------------- end -------
		//************************************************

//------ 清除 Flash 安全码 -- beign --
		//注意:
		//如果动态取内码时会清零NRM_securty_a,其和标定写入直接又没有按键设置NRM_securty_a，会导致无法写入
		//解决需要增加 条件判断
		//如:if((Cal_step != 6)&&(!Sample_step_2))等
		if( (Active_mode != 0x3b) && (Active_mode != 0x3c) && (Active_mode != 0x3e) && (Active_mode != 0x5f) && (Active_mode != 0x9f)  && (!Save_user_data) && (!Key_press))
			NRM_securty_a = 0x00;	//Flash 安全验证码 A
		NRM_securty_b = 0x00;	//Flash 安全验证码 B
//------ 清除 Flash 安全码 -- end --

		//************************************************
		//---  休眠处理 ----------------- begin -------
		//************************************************
//--SDI5219---
		//--STOP前的IO配置
		//CHARGE_SYMBOL = 0;		//(CHARGE_SYMBOL)必须为0,防止消耗电流
		//--- 只有在"//充电状态下，或者没有按键关机的情况下"才进入休眠
	//--条件编译: 按键关机 --begin--
	#ifdef KEY_PRESS_PD_FUN_EN
		if((!Mcu_stop_pre && !Mcu_stop_rec) || Charte_bit)	//充电状态下，或者没有按键关机的情况下
   	#endif
	//--条件编译: 按键关机 --end--
		{	
			//--关闭时的IO配置--
			//--电压检测,交流检测 应该度要设定为纯输入模式,防止内部消耗电流	
			//---关闭看门狗
			EA = 0;
			WD_TA = 0x05;
			WD_TA = 0x0a;
			WDCON = 0xaf;	//看门狗时间 4s钟,(关闭)低频唤醒0.2s
			EA = 1;	
			_nop_();
			_nop_();
			//************************************************
			//--- 进入 STOP ----------------- end -------
			//************************************************
			//--只是切断MCU的时钟，SG-ADC和主震荡处于工作状态
			//--低频唤醒 和 SG-ADC 都能唤醒MCU
			PCON |= 0x02; //STOP1 ----MCU将停在此处
			//---打开看门狗
			EA = 0;
			WD_TA = 0x05;
			WD_TA = 0x0a;
			WDCON = 0xbf;	//(SDI5216)看门狗时间 4s钟,(关闭)低频唤醒0.2s
			EA = 1;	
		}
		//************************************************
		//---  休眠处理 ----------------- end -------
		//************************************************
//--SDI5219---
		//************************************************
		//---  键盘以及IO检测 ----------------- begin ----
		//************************************************
		EA = 0;
		//--IO口配置--
		IO_MODE_KEY_A_EN;
		// 防止出错，将 KEY_PIN1 - KEY_PIN4 写1

		KEY_PIN1 = 1;
		KEY_PIN3 = 1;
		// 检测是否有健按下
		KEY_PIN5 = 1;
		KEY_PIN7 = 1;
		//KEY_PIN9 = 0;

		delay_ms(0);

		// 如果 KEY_1,KEY_2,KEY_3,KEY_4为1,则说明没有键被按下
		if(KEY_PIN1 && KEY_PIN3 && KEY_PIN5 && KEY_PIN7)
	//	if(KEY_PIN1 && KEY_PIN4)
		{
			Key_press = 0;
			Key_response = 0;
			Key_counter_en = 0;
			Stable_light_counter &= 0x0f;
		}
		else
	 	{
			Key_press = 1;															
	   	}
		//--IO口配置--
		IO_MODE_KEY_DIS;
		//--测试 交流充电
		CHARGE_SYMBOL = 1;
		//CHARGE_SYMBOL = 0;	//屏蔽交流检测
		if(CHARGE_SYMBOL)
		{
			Charte_bit = 1;
			//--消除低压报警
			Lamp_lowv_a = 0;
		}
		else
		{	
			if(Charte_bit) //退出动态充电时自动检测一下电池电压
			{
				if((Active_mode & 0x1f) == 0x00)  
					Vol_auto_det_counter = VOL_AUTO_DET_NUM;
				Power_det_mode = 1;
			}
			Charte_bit = 0;
			Charte_bit_dis = 0;
			Batter_b_ch=0;
			Batter_c_ch=0;
		}
		EA = 1;
		//************************************************
		//---  键盘以及IO检测 ----------------- end ----
		//************************************************																								
	}//---while(1)
}//----main()

// -- 箭头按键 输入数据 处理函数
void a_input_analyse(void)
{
	unsigned long step;

	switch(A_input_setp)
	{
		case 1:	step = 100000;
				break;
		case 2:	step = 10000;
				break;
		case 3:	step = 1000;
				break;
		case 4:	step = 100;
				break;
		case 5:	step = 10;
				break;
		case 6:	step = 1;
				break;
		default: break;
	}

	if(A_cycle_num == 9)
	{
		Price -= (9 * step);
		A_cycle_num = 0;
	}
	else
	{
		Price += step;
		A_cycle_num ++;
	}	
}

//--------------------------------
//----- 延时函数,num=1,延时1ms ---
//--SDI5219---
void delay_ms(unsigned int num)
{ 	
	unsigned int i,j;
  	for(i= 0;i<num;i++)	//(SDI5219) 延时1ms
//-- 条件编译: 2分频时钟模式 -- begin --
#ifdef SDI5219_HALF_SPEED
		for(j=0;j<96;j++)
			;
#else
		for(j=0;j<164;j++)
			;
#endif
//-- 条件编译: 2分频时钟模式 -- end --
}

//--------------------------------
//----- 蜂鸣器函数 ---
//--SDI5219---
void buzzer(unsigned char mode)
{
	unsigned char j;
	unsigned char i=0;
	//--注意，蜂鸣器中调用 液晶驱动 ，内存消耗 可能变大2字节
	//write_lcd(0,0x09);	//蜂鸣器
	EA = 0; //禁止按键中断
	//BUZZER_P = 1;
	//BUZZER_N = 1;
//-- 条件编译: 2分频时钟模式 -- begin --
#ifdef SDI5219_HALF_SPEED
	while(i<200)	//(SDI5219)2KHz 持续50ms
	{
		BUZZER_P = !BUZZER_P; //PB2 取反;
		for(j=0;j<57;j++)
		{
			//_nop_();
			//_nop_();
			//_nop_();
			//_nop_();
		}
		i++;
	}
#else
	while(i<200)	//(SDI5219)2KHz 持续50ms
	{
		BUZZER_P = !BUZZER_P; //PB2 取反;
		for(j=0;j<111;j++)
		{
			//_nop_();
			//_nop_();
			//_nop_();
			//_nop_();
		}
		i++;
	}	
#endif
//-- 条件编译: 2分频时钟模式 -- end --
	//------------
	BUZZER_P = 0;
	//BUZZER_N = 0;
	//--注意，蜂鸣器中调用 液晶驱动 ，内存消耗 可能变大2字节
	//write_lcd(0,0x08);	//蜂鸣器
	EXIF &= 0xbf;	//清除ADC 中断标志位
	EA = 1; //禁止按键中断
}

//------------------------------------------------
//----- 读取EEPROM的标定点数据，初始标定电子秤 ---
void init_cal_point(void)
{	
	unsigned char Per_d_code,i;
	bit data_error_a;

	data_error_a = 0;
	Core_data.ee_struct.Sys_seting_1 = nvm_data_read_byte(ADDR_CAL_DATA + 0); // 第0个字节用于存放 标定标值位

	//如果已经设置过相关属性，则读取设定值；未进行设置，则写入默认值
	if( ((Core_data.ee_struct.Sys_seting_1 & 0xf0) == 0x60) ) // 如果 不是 初次开机
	{
		//---读取核心标定数据---
		for(i=0;i<FLASH_DATA_LENTH;i++)
			Core_data.b[i] = nvm_data_read_byte(ADDR_CAL_DATA + i);
	
		//---核心标定数据 异常处理--
		if(Core_data.ee_struct.Cal_point_code >= 600000)
			data_error_a = 1;
		else
		if(Core_data.ee_struct.Cal_point_code < 10000)
			data_error_a = 1;
		else
		if((Core_data.ee_struct.Intel_cal_d & 0x7f) > 64)
			data_error_a = 1;
		else
		{
			Per_d_code = (Core_data.ee_struct.Cal_point_code << INTER_WEIGHT_NUM) / CAL_POINT_W;//计算 1个内部分度对应的内码
			if((Per_d_code < LOW_CAL_D_CODE)||(Per_d_code > UP_CAL_D_CODE))
			{
				data_error_a = 1;
			}
		}
		//--读取 用户 数据--
		for(i=0;i<SYS_DATA_LENTH;i++)
			System_data.b[i] = nvm_data_read_byte(ADDR_SYS_DATA + i);
	}
	else
	{
		data_error_a = 1;
	}
	
	if(data_error_a) // 如果 初次开机 或者数据错误
	{
		SYS_SETING_1_DEFAULT;
		SYS_SETING_2_DEFAULT;
		SYS_SETING_3_DEFAULT;
		//--- for 0819 ---
		Core_data.ee_struct.Follow_zero_d = 6 << (INTER_WEIGHT_NUM-1); 		//默认3g追零
		Core_data.ee_struct.Follow_zero_num = 2; //默认0.5d追零
		Core_data.ee_struct.Cal_d = 1 << INTER_WEIGHT_NUM;				//默认分度1g (1g << INTER_WEIGHT_NUM)
		Core_data.ee_struct.Cal_max_weight = 30; 						//初始默认量程为（30kg）
		//--如 INTER_WEIGHT_NUM = 1(60000内部重量分度),1个内部分度对应10个内码
		//--CAL_POINT_W * 10 = 120000
		Core_data.ee_struct.Cal_point_code = (10 * (unsigned long)(CAL_POINT_W)) >> (INTER_WEIGHT_NUM - 1);	
		Core_data.ee_struct.Intel_cal_d = 1;
		Core_data.ee_struct.Cal_inter_weight = (unsigned long)(10000) << INTER_WEIGHT_NUM;		//没什么作用，完全是单纯的初始化
	//--条件编译: 三段标定 -- begin --
	#ifdef THREE_CAL_FUN_EN
		Core_data.ee_struct.Cal_delta_1 = 0;
		Core_data.ee_struct.Cal_delta_2 = 0;
	#endif
	//--条件编译: 三段标定 -- end --
		//--用户数据--
		System_data.sys_struct.User_seting_1 = USER_SETING_1_DEFAULT;
		System_data.sys_struct.Weight_HH = 0;
		System_data.sys_struct.Weight_LL = 0;
		System_data.sys_struct.Counter_sample_num = 10;	
		System_data.sys_struct.Counter_sample_weight = (unsigned long)(10000) << INTER_WEIGHT_NUM;
		System_data.sys_struct.Per_sap_w = (unsigned long)(10000) << INTER_WEIGHT_NUM;	
		Core_data.ee_struct.ZERO_NUM = 30;
		Core_data.ee_struct.TAPE_NUM = 100;
		//core_memory_write();
	}

	if(Core_data.ee_struct.Follow_zero_num < 1)
		Core_data.ee_struct.Follow_zero_num = 1;
	
//---处理 背光 --- 
	if(LCD_LAMP_OFF_MODE)
		Price_light_symbol = 0;
	else
		Price_light_symbol = 1;
//---为了兼容825程序,将上下报警门限放到 Money 和 Money_accum.w 中
	Money = System_data.sys_struct.Weight_HH;
	Money_accum.w = System_data.sys_struct.Weight_LL;
	//--
	if((Money < Money_accum.w)&&(!WARRING_TYPE_NO))
	{
		Weight_ul_error = 1;
	}
	else
	{
		Weight_ul_error = 0;
	}

	Weight_stable_counter = 0;

//--条件编译: 三段标定 -- begin --
#ifdef THREE_CAL_FUN_EN
	//---防止三段补偿出错导致死机
	if((Core_data.ee_struct.Cal_delta_1 > 20000) || (Core_data.ee_struct.Cal_delta_1 < -20000))
	{
		Core_data.ee_struct.Cal_delta_1 = 0;
	}
	if((Core_data.ee_struct.Cal_delta_2 > 20000) || (Core_data.ee_struct.Cal_delta_2 < -20000))
	{
		Core_data.ee_struct.Cal_delta_2 = 0;
	}
#endif
//--条件编译: 三段标定 -- end --
}

//写核心存储器函数
void core_memory_write(void)
{
	unsigned char i,j,temp;
	bit k;

	j = 0; //记录一次循环的擦写次数
	k = 1;
	//Flash 安全验证码 B
	NRM_securty_b = 0x55;	//Flash 安全验证码 B
	//--------------------
	if(!SYSTEM_CAL_SYMBOL) // 如果 是 初次
	{
		SYSTEM_CAL_SYMBOL_CLR;
		SYSTEM_CAL_SYMBOL_SET;
		//-- 存用户数据区
		uer_memory_write();	
	}	
	//--------------------
	while(k)
	{
		FLASH_ENA = 0x05;	//(放到函数外)
		e2rom_erase(ADDR_CAL_DATA); //擦除
		j++;
		k = 0; //准备退出
		for(i=0;i<FLASH_DATA_LENTH;i++)
		{
			FLASH_ENA = 0x05;	//nvm_data_write_byte 函数中无这句话
			nvm_data_write_byte(ADDR_CAL_DATA + i,Core_data.b[i]);
			temp = nvm_data_read_byte(ADDR_CAL_DATA + i); //数据校验
			if(temp != Core_data.b[i]) //如果发现关键数据没有写对，则擦除重写，尝试3次
			{
				k = 1;
			}
		}
		if(j>5)
		{
			buzzer(1);
			buzzer(1);
			k=0;
		}
	}	
}

//写 用户数据 存储器函数
void uer_memory_write(void)
{
	unsigned char i,j,temp;
	bit k;
	j = 0; //记录一次循环的擦写次数
	k = 1;
	//Flash 安全验证码 B
	NRM_securty_b = 0x55;	//Flash 安全验证码 B
	while(k)
	{
		FLASH_ENA = 0x05;	//(放到函数外)
		e2rom_erase(ADDR_SYS_DATA); //擦除
		j++;
		k = 0; //准备退出
		for(i=0;i<SYS_DATA_LENTH;i++)
		{
			FLASH_ENA = 0x05;	//nvm_data_write_byte 函数中无这句话
			nvm_data_write_byte(ADDR_SYS_DATA + i,System_data.b[i]);
			temp = nvm_data_read_byte(ADDR_SYS_DATA + i); //数据校验
			if(temp != System_data.b[i]) //如果发现关键数据没有写对，则擦除重写，尝试3次
				k = 1;
		}
		if(j>5)
		{
			k=0;
			buzzer(1);
			buzzer(1);
		}
	}
}

//--追零范围计算
void follow_zero_cal(void)
{
	//-- 设定追零范围 ----
	Core_data.ee_struct.Follow_zero_d = Core_data.ee_struct.Cal_d * Core_data.ee_struct.Follow_zero_num;
	if(Core_data.ee_struct.Intel_cal_d < 0x80)
	{
		Core_data.ee_struct.Follow_zero_d = Core_data.ee_struct.Follow_zero_d * Core_data.ee_struct.Intel_cal_d / 2;
	}
	else
	{	unsigned char i;
		i = Core_data.ee_struct.Intel_cal_d & 0x7f; 
		Core_data.ee_struct.Follow_zero_d = Core_data.ee_struct.Follow_zero_d / 2 / i;
	}
//	Core_data.ee_struct.Follow_zero_d *= Core_data.ee_struct.Follow_zero_num;
	//if(Core_data.ee_struct.Follow_zero_d < (7 << (INTER_WEIGHT_NUM-1)))
	//	Core_data.ee_struct.Follow_zero_d = (7 << (INTER_WEIGHT_NUM-1)); //默认每次标定后，对于0.5d < 3.5g的，按照3.5g追零
}


//----------------------------------------------------------
//---   检测电池电压   ----
unsigned int power_detect(void)
{
	unsigned int v;
	EA = 0;
//--- 用于检测电压时 关闭显示 --
//	if(!Power_det_mode)
//		dsp_updata(1); //关闭显示
//	Power_det_mode = 0;
//--- 相关IO口配置 ----
	//-- 如果 电压检测管脚和键盘管脚公用的话，需要将键盘所有检测IO变为 [仅输入模式]
//--- 
	BAT_DET_PIN_HIGH; // 电池电压检测脚变高, 否则电池电压检测将不正常
	//--测试通道1的电压
	SARCON  = 0x0b;  //通道1 速度最快 打开Sar-ADC
	delay_ms(1);
	
	if(!(SARCON & 0x04)) // ADC不忙则开始转换
	{
		SARCON |= 0x04;
		while(SARCON & 0x04) // 等待转换结束
		{
		}
	}

//--- 用于检测电压时 关闭显示 --
//-- 条件编译: 按键关机 -- begin --
//#ifdef KEY_PRESS_PD_FUN_EN
//	if((!Mcu_stop_pre && !Mcu_stop_rec) || Charte_bit)
//	{
//		dsp_updata(2); //打开显示
//	}
//#else
//	dsp_updata(2); //打开显示
//#endif
//-- 条件编译: 按键关机 -- end --

	v = SARDATA;
	v = (unsigned long)v * (unsigned long)V_CO_PRE / 0xff; // 计算电池电压

//--- 恢复 相关IO口配置 ----
	//-- 如果 电压检测管脚和键盘管脚公用的话，需要将键盘所有检测IO变为 [仅输入模式]

	SARCON  &= 0xf7;; //关闭ADC
	EA = 1;

	if(v >= 20)
	{
		if(!Charte_bit)
		{
			if(POWER_6V_BIT_EN) //6v供电
			{
				if(v  >= V_HIG_0) // 电池低于规定电压时，欠压灯亮
					Batter_a=1;	// = 1 显示 电池最大的一格			
				else
					Batter_a=0;	// = 1 显示 电池最大的一格	
		
				if(v  >= V_HIG_1)
					Batter_b=1;	// = 1 显示 电池中间的一格
				else
					Batter_b=0;	// = 1 显示 电池中间的一格
		
				if(v  >= V_HIG_2)
					Lamp_lowv = 0;			
				else
					Lamp_lowv = 1;// 重量窗 显示 -- LB -- 			
		
				if(v  >= V_HIG_3)
				{
					Batter_c=1;	// = 1 显示 电池最小的一格
					Lamp_lowv_a = 0;
				}
				else
				{
					Batter_c=0;	// = 1 显示 电池最小的一格
					Lamp_lowv_a = 1;	//重量窗 显示 -- LB -- 并锁死，蜂鸣器报警
				}				
			}
			else
			{
				if(v  >= V_LOW_0) // 电池低于规定电压时，欠压灯亮
					Batter_a=1;	// = 1 显示 电池最大的一格			
				else
					Batter_a=0;	// = 1 显示 电池最大的一格	
		
				if(v  >= V_LOW_1)
					Batter_b=1;	// = 1 显示 电池中间的一格
				else
					Batter_b=0;	// = 1 显示 电池中间的一格
		
				if(v  >= V_LOW_2)
					Lamp_lowv = 0;
				else
					Lamp_lowv = 1;// 重量窗 显示 -- LB -- 
		
				if(v  >= V_LOW_3)
				{
					Batter_c=1;	// = 1 显示 电池最小的一格
					Lamp_lowv_a = 0;
				}
				else
				{
					Batter_c=0;	// = 1 显示 电池最小的一格
					Lamp_lowv_a = 1;	//重量窗 显示 -- LB -- 并锁死，蜂鸣器报警
				}
			}
		}
		else
		{
			if(POWER_6V_BIT_EN) //6v供电
			{
				if(v  >= V_HIG_1_CH)
				{
					Batter_b_ch=1;	// = 1 显示 电池中间的一格
					Batter_b = 1;	// = 1 显示 电池中间的一格
				}
				else
				{
					Batter_b_ch=0;	// = 1 显示 电池中间的一格
				}
		
				if(v  >= V_HIG_2_CH)
				{
					Batter_c_ch=1;	// = 1 显示 电池最小的一格	
					Batter_c =1;	// = 1 显示 电池最小的一格		
				}
				else
				{
					Batter_c_ch=0;	// = 1 显示 电池最小的一格	
				}
			}
			else
			{
				if(v  >= V_LOW_1_CH)
				{
					Batter_b_ch=1;	// = 1 显示 电池中间的一格
					Batter_b = 1;	// = 1 显示 电池中间的一格
				}
				else
				{
					Batter_b_ch=0;	// = 1 显示 电池中间的一格
				}
		
				if(v  >= V_LOW_2_CH)
				{
					Batter_c_ch=1;	// = 1 显示 电池最小的一格	
					Batter_c =1;	// = 1 显示 电池最小的一格		
				}
				else
				{
					Batter_c_ch=0;	// = 1 显示 电池最小的一格	
				}
			}
		}
	}

	//--检测是否电池充满
	if(POWER_6V_BIT_EN) //6v供电
	{
		if(Charte_bit_dis)
		{
			if(v  <= V_HIG_FULL_L) // 电压<=于此值 表示充电充满
				Charte_bit_dis=0;
		}
		else
		{
			if(v  >= V_HIG_FULL_H) // 电压>=于此值 表示充电充满
				Charte_bit_dis=1;
		}
	}
	else
	{
		if(Charte_bit_dis)
		{
			if(v  <= V_LOW_FULL_L) // 电压<=于此值 表示充电充满
				Charte_bit_dis = 0;
		}
		else
		{
			if(v  >= V_LOW_FULL_H) // 电压>=于此值 表示充电充满
				Charte_bit_dis=1;
		}
	}
	return(v);
}

//低频唤醒中断；*****************
static void wu_serve(void) interrupt 6
{		
	EA = 0;	 // 禁止中断
//-- 条件编译: 按键关机 -- begin --
#ifdef KEY_PRESS_PD_FUN_EN
	if(Mcu_stop_rec)
	{
		ADready = 0;
		Led_dsp_updata = 0;
		//ET0 = 1;	//打开计时器中断0
		//TR0 = 1;	//运行计时器0
		//---打开看门狗
		WD_TA = 0x05;
		WD_TA = 0x0a;
		WDCON = 0xbf;	//看门狗时间 4s钟,低频唤醒0.2s
	}
	else
	if(Mcu_stop_pre) //确保准备进入按键关机时 ADC不会被中断冲洗启动;
	{
	}
	else
#endif
//-- 条件编译: 按键关机 -- end --
	Data_sycle_symbol = 1;	//新的数据周期
	EXIF &= 0xdf; //清除 低频唤醒中断标志位
	EA = 1;  // 打开中断
}
//内部 24bit SG ADC 中断,读取adc数据；*****************
static void sgadc_serve(void) interrupt 7
{		
	EA = 0;	 // 禁止中断

	data_receive(); // 读取 SDI0818 的 转换数据
	ADcode_pre &= 0x00ffffff;
	ADcode_pre >>= 2;
		
	ADready = 1;	// SDI0818 数据 读取 Buffer 更新 标志位 置 1	
	if(ADready)
	{
		//--(comb2)^3--begin--	
		{
		ADbuf[3]=ADbuf[2];
		ADbuf[2]=ADbuf[1];
		ADbuf[1]=ADbuf[0];
    	ADbuf[0] = ADcode_pre;
    	//ADbuf[0] &= 0x00ffffff;	 
    	//ADbuf[0] >>= 2;	 
		ADcode_pre = ADbuf[0] + 3 * ADbuf[1] + 3 * ADbuf[2] + ADbuf[3];
		ADcode_pre >>= 3;
		}
		//--(comb2)^3--end--
	//--- 条件编译: 低速显示更新 -- begin ---
	#ifdef LOW_DSP_UPDATA
		Low_speed_updata = !Low_speed_updata;
	#endif
	//--- 条件编译: 低速显示更新 -- end ---
	}	
			
	Data_sycle_symbol = 1;	//新的数据周期
	EXIF &= 0xbf;
	EA = 1;  // 打开中断
}



