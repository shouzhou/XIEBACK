//*************************************************************************//
//* Name				:   ext_data.h
//* Vision				:	V-Nov07_12
//* Project				:   电子计价秤 系列方案
//* Function			:   全局变量 头文件
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
#ifndef	__DATA__
#define	__DATA__

#ifndef  EXTERN 
#define  EXTERN extern
#else 
	#define EXTERN 
#endif

//************************************************************************************************************************
// -- 定义 预编译 选项 部分 --
//************************************************************************************************************************

//------------------------------------------------------------------
//--- 选择 功能模块 --
//------------------------------------------------------------------
//#define PERCENTAGE_FUN_EN			//启用改宏，表示编译 "百分比功能"

//#define THREE_CAL_FUN_EN			//启用改宏，表示编译 "三段标定功能"

//--- 去抖动 ---
#define GLITCH_FILTER_FUN_EN		//启用改宏，表示编译 "整体畜牧抗震"
#define GLITCH_DETECT_EN			//(要增加9个字节内存)启用改宏，表示编译 "传感器稳定性跟踪"
//#define ADC_DE_PULSE_FUN_EN		//启用改宏，表示编译 "ADC内码抗脉冲"
//#define ADC_DE_GLITCH_FUN_EN		//启用改宏，表示编译 "ADC内码抗抖动"

//--- 按键关机 ---
//#define KEY_PRESS_PD_FUN_EN			//启用该宏，表示编译 "按键关机功能"
#define SDI5219_HALF_SPEED			//SDI5219采用1/2分频运行
#define SDI5219_CAL_DSP_UPDATA_SLOW			//SDI5219 标定状态下 刷新显示 频率降低 "用来提高标定和非标定时的一致，减弱干扰"

//温度检测
//温度补偿
//时间显示

//--- 内部重量处理 ---(通常不需要变动)---
#define INTER_WEIGHT_LOCK			//启用改宏，表示编译 "内部重量锁定"
//-强制归零-
#define FORCE_WEIGHT_ZERO			//启用改宏，表示编译 "强制归零"
//#define FORCE_UP_ZERO				//注意：改宏需要和强制归零一起启动才有效，启用改宏，表示编译 "强制归零时增强一定时间的追零"

//--条件编译: 三段标定 -- begin --
	//("3段标定情况下请关闭线性补偿")
#ifndef THREE_CAL_FUN_EN
	#define W_LINE_CAL_EN				//启用改宏，表示编译 "简单线性修正"	
#endif
//--条件编译: 三段标定 -- end --

//------------------------------------------------------------------
//--- 选择显示驱动 --
//------------------------------------------------------------------

//控制-"./_dsp_driver/display_driver.c" 中使用的LCD 和 LED驱动
//#define LCD_DRIVER_1621		
#define LED_DRIVER_1640
//#define LED_DRIVER_6932
//#define LED_DRIVER_1616

//控制是否采用 显示位转换 查找表 (一般不打开)
#define  DSP_BIT_CHX_EN				//启用改宏，表示编译 "显示位转换"	

//------------------------------------------------------------------
//--- 定义 高精度ADC的输出频率 --
// -- 因为本程序中很多时间定义是通过 SDI0819的输出数据个数进行的，所以事先要设定SDI0819的输出频率：20Hz 或者 10Hz；
// -- 其他 频率需要 重新 添加 修改 所有具有   "(时间属性)"   的变量
//------------------------------------------------------------------
#define ADC_OUT_20Hz	//使用 该宏定义 。定义 SDI0819 的输出频率：20Hz
//#define ADC_OUT_10Hz	//使用 该宏定义 。定义 SDI0819 的输出频率：10Hz

//------------------------------------------------------------------
//--- 显示更新速度 ---(通常不需要变动)---
#ifdef ADC_OUT_20Hz  //只在20Hz的时候有效
	//#define LOW_DSP_UPDATA				//用于降低更新功耗，表示 处理两次ADC数据才更新一次显示
#endif

//------------------------------------------------------------------
//--- 其他 --
//------------------------------------------------------------------



//************************************************************************************************************************
// -- MCU IO 接口定义定义 部分 --
//************************************************************************************************************************
//------------ 配置adc的接口 ------------
//sbit DRDY_SDO = P0^0;
//sbit SCLK = P0^2;
//------------ 配置蜂鸣器的接口 ------------
sbit BUZZER_P = P1^5;
//sbit BUZZER_N = P1^7;
//------------ 配置LED的接口 ------------
//--1640--
//sbit LED_DIN = P0^4;
//sbit LED_SCLK = P0^5;
sbit LED_DIN = P1^3;
sbit LED_SCLK = P1^4;
//--6932--
//sbit LED_DIN = P0^3;
//sbit LED_SCLK = P0^5;
//sbit LED_STB = P1^1;
//------------ 配置LCD的接口 ------------
//sbit LCD_DATA = P0^3;
//sbit LCD_WR_N = P0^4;
//sbit LCD_CS_N = P0^5;
sbit LCD_DATA = P0^3;
sbit LCD_WR_N = P1^3;
sbit LCD_CS_N = P1^4;
//------------ 配置LCD-背光的接口 ------------
sbit LCD_LAMP = P1^5;
EXTERN bit Lcd_lamp_symbol;		//用于 确定液晶背光灯是否开启
EXTERN unsigned char Lcd_bright;	//显示亮度变量
#define  LCD_LAMP_B4		0xff	//最高亮度(持续为高)
#define  LCD_LAMP_B3		0x60	//3亮度	
#define  LCD_LAMP_B2		0x30	//2亮度
#define  LCD_LAMP_B1		0x10	//1亮度
//#define  LCD_LAMP_ON		{LCD_LAMP = 1;Lcd_lamp_symbol = 1; PWMCON  |= 0x04;}		// (通过PWM控制背光灯)背光灯打开  (用三极管和不用三极管,逻辑正好是反的,定义宏方便修改)
//#define  LCD_LAMP_OFF		{LCD_LAMP = 0;Lcd_lamp_symbol = 0; PWMCON  &= 0xfb;}		// (通过PWM控制背光灯)背光灯关闭
#define  LCD_LAMP_ON		{LCD_LAMP = 1;Lcd_lamp_symbol = 1;}		// (通过PWM控制背光灯)背光灯打开  (用三极管和不用三极管,逻辑正好是反的,定义宏方便修改)
#define  LCD_LAMP_OFF		{LCD_LAMP = 0;Lcd_lamp_symbol = 0;}		// (通过PWM控制背光灯)背光灯关闭
//------------ 配置 交流充电检测的接口 ------------
sbit CHARGE_SYMBOL = P1^5;
//------------ 配置 电池电压检测的接口 ------------
sbit BAT_DET_PIN = P1^6;
#define  BAT_DET_PIN_HIGH		(BAT_DET_PIN = 1)		// 电压检测 口线 变高
#define  BAT_DET_PIN_LOW		(BAT_DET_PIN = 0)		// 电压检测 口线 变低
//------------ 配置 键盘的接口 ------------
//sbit KEY_PIN1 = P1^1;	
//sbit KEY_PIN2 = P0^1;	
//sbit KEY_PIN3 = P1^2;	
//sbit KEY_PIN4 = P0^0;	
//sbit KEY_PIN5 = P1^3;	//BUZZER_N = P1^3;	
//sbit KEY_PIN6 = P0^4;	//LCD_WR = P0^4;		
//sbit KEY_PIN7 = P1^5;	//CHARGE_SYMBOL = P0^2;
//sbit KEY_PIN8 = P0^3;	//LCD_DATA = P0^3;	
//sbit KEY_PIN9 = P0^5;	//LCD_CS_N = P0^5;	
  //sbit KEY_PIN10 = P0^0;

sbit KEY_PIN1 = P0^1;
sbit KEY_PIN3 = P0^0;
sbit KEY_PIN5 = P1^1;
sbit KEY_PIN7 = P1^2;

//------------ IO口操作(宏定义) ------------
//关键管脚的IO配置
//-- P1^5(CHARGE_SYMBOL)		集电极开路 
//-- P1^6(BAT_DET_PIN)			纯输入
//-- P1^4(LCD_LAMP) 			强驱动
//-- P1^3(BUZZER_P) 			强驱动
//#define  IO_MODE_RESTORE_A_EN {P1M0 |= 0x38;P1M0 &= 0xbf;P1M1 &= 0xe7;P1M1 |= 0x60;};
#define  IO_MODE_RESTORE_A_EN {P1M0 &= 0xbf;P1M1 |= 0x40;};

//开启显示IO正常 //P0.3(LCD_DATA-KEY_PIN9) P0.4(LCD_WR_N) P0.5(LCD_CS_N) 强驱动;
//#define  IO_MODE_DSP_EN	{P0M0 |= 0x38;P0M1 &= 0xc7;}
#define  IO_MODE_DSP_EN	{P0M0 |= 0x18;P0M1 &= 0xe7;}	
//关闭显示IO,设定显示IO //
#define  IO_MODE_DSP_DIS {LCD_CS_N = 1;LCD_WR_N = 1;LCD_DATA = 1;}
//#define  IO_MODE_DSP_DIS {LED_DIN = 1;LED_SCLK = 1;}	

//开启 键盘IO正常(按键检测部分) // P1^1(KEY_PIN1),P0^1(KEY_PIN2),P1^2(KEY_PIN3),P0^0(KEY_PIN4) 普通上拉;	
#define  IO_MODE_KEY_A_EN	 {P0M0 &= 0xfc;P0M1 &= 0xfc;P1M0 &= 0xf9;P1M1 &= 0xf9;}	
// KEY_PIN5-KEY_PIN10 强驱动	
#define  IO_MODE_KEY_B_EN	 {P0M0 |= 0x38;P0M1 &= 0xc7;P1M0 |= 0x28;P1M1 &= 0xd7;}	
//关闭 键盘IO // KEY_PIN1-KEY_PIN10配置为纯模拟输入,输出1; 
//例外: (CHARGE_SYMBOL集电极开路) (BUZZER_P不能配置为1和模拟输入)  (和显示复用的线不能配置为模拟输入)
#define  IO_MODE_KEY_DIS {P0M0 &= 0xfc;P0M1 |= 0x03;P1M0 &= 0xf9;P1M1 |= 0x06;KEY_PIN1 = 1;KEY_PIN3 = 1;KEY_PIN5 = 1;KEY_PIN7 = 1;}	
//---定义SG-ADC的工作模式--
//--5219--
//#define SGADCON_DEFAULT	0xc9	//定义 "SGADCON" 的默认值
#define SGADCON_DEFAULT	0xc9	//定义 "SGADCON" 的默认值
#define PD_CON_DEFAULT  0x7e	//定义 "PD_CON" 的默认值 (锁定P2 锁定sgadcon，sgadcon2)
//--5216--
//#define SGADCON_DEFAULT	0x34	//(关闭SOC-SGADC,温度通道,160Hz)定义 "SGADCON" 的默认值
//#define PD_CON_DEFAULT  0x18	//(锁定sgadcon，sgadcon2)(关闭ALDO,关闭温度)定义 "PD_CON" 的默认值 
//************************************************************************************************************************
// -- 系统配置变量 --
// -- EEPROM 空间  --
//************************************************************************************************************************
//--- EEPROM 块定义 --
#define ADDR_CAL_DATA 0x6800		// (26区)用于存放 核心标定数据
#define ADDR_CAL_BACKUP 0x6c00		// (27区) 该区用于 核心标定数据 备份 
#define ADDR_SYS_DATA 0x7000		// (28区) 该区用于存放   系统密码 厂家信息 等
#define ADDR_PRICE_DATA_A 0x7400	// (29区) 用于存放价格数据 PRICE_M1 - PRICE_M8

//------------------------------------------------------------------
//--- EEPROM 空间 -- 
//--条件编译: 三段标定 -- end --
#define  FLASH_DATA_LENTH_INIT 21				// flash 数据的 字节数

//--条件编译: 三段标定 -- begin --
#ifdef THREE_CAL_FUN_EN
	#define  FLASH_DATA_LENTH (FLASH_DATA_LENTH_INIT + 4)
#else
	#define  FLASH_DATA_LENTH FLASH_DATA_LENTH_INIT
#endif

//--- 
//内部24万点时，【Follow_zero_d】 【Cal_d】 最好配置成 【int】
//--
union	eedata_pattern_a	//定义联合体,数据可以采用字节和字两种方式访问；
{
	struct
	{  
		unsigned char Sys_seting_1;									//	配置1 +　系统标志
		unsigned long Cal_point_code;								//	标定内码值 = (标定点ADC内码 - 零点ADC内码)
	//--条件编译: 三段标定 -- begin --
	#ifdef THREE_CAL_FUN_EN
		int	Cal_delta_1;		//线性度补偿1
		int Cal_delta_2;		//线性度补偿2
	#endif
	//--条件编译: 三段标定 -- end --
		unsigned int Follow_zero_d;//(内部24万点时，最好配成int) 	// 定义内码追零大小
		unsigned char Follow_zero_num;								// 定义追零的范围（ 0.5d的多少倍）
		unsigned int Cal_d; //(内部24万点时，最好配成int)			//显示分度 d(内部重量)
		unsigned int Cal_max_weight; 								//定义最大称重
		unsigned char Intel_cal_d; 									//定义1个分度对应的内码大小
		unsigned long Cal_inter_weight;								//---标定砝码 (内部)重量--------
		unsigned char Sys_seting_2;									//	系统标志
		unsigned char TAPE_NUM;  									//去皮百分比
		unsigned char ZERO_NUM;  									//置零百分比
		unsigned char Sys_seting_3;									//	系统标志
	} ee_struct;
	unsigned char b[FLASH_DATA_LENTH];
};
EXTERN union eedata_pattern_a idata Core_data;

//------------------------------------------------------------------
//--- 系统配置变量 -- (说明) --
//Sys_seting_1; 
		///第0,1,2位:	(WEIGHT_POINT: = 000 表示重量区显示0个零; = 001 显示1个零; = 010 显示2个零; = 011 显示3个零; = 100 显示4个零) 
		//第3位: 		(DISPLAY_MODE: = 0 表示显示一个"0";   = 1 表示显示一个"o";)
 		//第4、5、6、7位 为: 0110 表示 系统 各设定已经写入 相关值
//Sys_seting_2; 	
		//第0,1位 	(DSP_SPEED 用来 表示 4级 显示 速度)
		//第2,3,4位 (GLITCH_REMOVE_BIT 用来表示 抗震滤波 模式)
		//第5,6位   ()
		//第7位     (INIT_ZERO_SET = 1 用来表示 开机重量受限)
//Sys_seting_3; 
		//第0位     (POWER_6V_BIT 用来表示6V供电 POWER_4V_BIT用来表示4V供电)
        //第2,3位   (POWER_MODE 来表示亮度设置)

#define  SYS_SETING_1_DEFAULT	(Core_data.ee_struct.Sys_seting_1 = 0x03);
#define  SYS_SETING_2_DEFAULT	(Core_data.ee_struct.Sys_seting_2 = 0x01);
#define  SYS_SETING_3_DEFAULT	(Core_data.ee_struct.Sys_seting_3 = 0x04);
//-- 重量小数点 标志位 --- SYSTEM ---(这两位必须是字节的最低位，否则赋值会出错)	
#define  WEIGHT_POINT_LOAD		(Core_data.ee_struct.Sys_seting_1)						// 数值
#define  WEIGHT_POINT_NUM		(Core_data.ee_struct.Sys_seting_1 & 0x07)				// 数值
#define  WEIGHT_POINT_NUM_ADD	(Core_data.ee_struct.Sys_seting_1++)					// ++操作
#define  WEIGHT_POINT_NUM_DEC	(Core_data.ee_struct.Sys_seting_1--)					// --操作
#define  WEIGHT_POINT_FULL		(WEIGHT_POINT_NUM >= 4)									// 达到最大值
#define  WEIGHT_POINT_ZERO		(WEIGHT_POINT_NUM == 0)									// 达到最大值
#define  WEIGHT_POINT_CLR		(Core_data.ee_struct.Sys_seting_1 &= 0xf8)				// 设定为 0位
#define  WEIGHT_POINT_1_SET		(Core_data.ee_struct.Sys_seting_1 |= 0x01)				// 设定为 1位
#define  WEIGHT_POINT_2_SET		(Core_data.ee_struct.Sys_seting_1 |= 0x02)				// 设定为 2位
#define  WEIGHT_POINT_3_SET		(Core_data.ee_struct.Sys_seting_1 |= 0x03)				// 设定为 3位
#define  WEIGHT_POINT_4_SET		(Core_data.ee_struct.Sys_seting_1 |= 0x04)				// 设定为 4位

//--数码休眠显示闪烁标识--------
#define  DISPLAY_MODE_1         ((Core_data.ee_struct.Sys_seting_1 & 0x08) == 0x00)     // 显示"0"
#define  DISPLAY_MODE_2         ((Core_data.ee_struct.Sys_seting_1 & 0x08) == 0x08)     // 显示"o"
#define  DISPLAY_MODE_CLK		(Core_data.ee_struct.Sys_seting_1 &= 0xf7)
#define  DISPLAY_MODE_SET		(Core_data.ee_struct.Sys_seting_1 |= 0x08)

#define  SYSTEM_CAL_SYMBOL		((Core_data.ee_struct.Sys_seting_1 & 0xf0) == 0x60)		//系统设定过
#define  SYSTEM_CAL_SYMBOL_CLR	(Core_data.ee_struct.Sys_seting_1 &= 0x0f)				//系统设定
#define  SYSTEM_CAL_SYMBOL_SET	(Core_data.ee_struct.Sys_seting_1 |= 0x60)				//系统设定

//-- 显示速度 标志位 --- USER ---	
#define  DSP_SPEED_LOAD			(Core_data.ee_struct.Sys_seting_2)						// 数值
#define  DSP_SPEED_NUM			(Core_data.ee_struct.Sys_seting_2 & 0x03)				// 数值
#define  DSP_SPEED_NUM_ADD		(Core_data.ee_struct.Sys_seting_2 ++)				// ++操作
#define  DSP_SPEED_NUM_DEC		(Core_data.ee_struct.Sys_seting_2 --)				// --操作
#define  DSP_SPEED_FULL			(DSP_SPEED_NUM >= 0x02)									// 达到最大值
#define  DSP_SPEED_ZERO			(DSP_SPEED_NUM == 0x00)									// 达到最小值
#define  DSP_SPEED_CLR			(Core_data.ee_struct.Sys_seting_2 &= 0xfc)				// 设定为 0位
#define  DSP_SPEED_0_SET		(Core_data.ee_struct.Sys_seting_2 &= 0xfc)				// 设定为 1位
#define  DSP_SPEED_1_SET		(Core_data.ee_struct.Sys_seting_2 |= 0x01)				// 设定为 2位
#define  DSP_SPEED_2_SET		(Core_data.ee_struct.Sys_seting_2 |= 0x02)				// 设定为 3位
#define  DSP_SPEED_0			((Core_data.ee_struct.Sys_seting_2 & 0x03) == 0x00)		
#define  DSP_SPEED_1			((Core_data.ee_struct.Sys_seting_2 & 0x03) == 0x01)	
#define  DSP_SPEED_2			((Core_data.ee_struct.Sys_seting_2 & 0x03) == 0x02)	

//-- 如果抗震标志位 需要储存 --
#define  GLITCH_REMOVE_BIT_EN	((Core_data.ee_struct.Sys_seting_2 & 0x1C) != 0x00)		// 标志判断 (= 1表示 开启)
#define  GLITCH_REMOVE_BIT_DIS	((Core_data.ee_struct.Sys_seting_2 & 0x1C) == 0x00)		// 标志判断 (= 1表示 关闭)
#define  GLITCH_REMOVE_MODE0	((Core_data.ee_struct.Sys_seting_2 & 0x1C) == 0x04)		// 标志判断 ()
#define  GLITCH_REMOVE_MODE1	((Core_data.ee_struct.Sys_seting_2 & 0x1C) == 0x08)		// 标志判断 ()
#define  GLITCH_REMOVE_MODE2	((Core_data.ee_struct.Sys_seting_2 & 0x1C) == 0x0c)		// 标志判断 ()
#define  GLITCH_REMOVE_MODEA	((Core_data.ee_struct.Sys_seting_2 & 0x1C) == 0x10)		// 标志判断 ()
#define  GLITCH_REMOVE_BIT_CLR	(Core_data.ee_struct.Sys_seting_2 &= 0xe3)				// 关闭
#define  GLITCH_REMOVE_MODE0_SET 	{Core_data.ee_struct.Sys_seting_2 &= 0xe3;Core_data.ee_struct.Sys_seting_2 |= 0x04;}				// 打开
#define  GLITCH_REMOVE_MODE1_SET	{Core_data.ee_struct.Sys_seting_2 &= 0xe3;Core_data.ee_struct.Sys_seting_2 |= 0x08;}				// 标志位取反
#define  GLITCH_REMOVE_MODE2_SET	{Core_data.ee_struct.Sys_seting_2 &= 0xe3;Core_data.ee_struct.Sys_seting_2 |= 0x0c;}
#define  GLITCH_REMOVE_MODEA_SET	{Core_data.ee_struct.Sys_seting_2 &= 0xe3;Core_data.ee_struct.Sys_seting_2 |= 0x10;}
#define  GLITCH_REMOVE_BIT_ADD	(Core_data.ee_struct.Sys_seting_2 += 0x04)
#define  GLITCH_REMOVE_BIT_NUM	(Core_data.ee_struct.Sys_seting_2 & 0x1C)				// 标志判断 ()
#define  GLITCH_REMOVE_BIT_FULL	(GLITCH_REMOVE_BIT_NUM >= 0x10)							// 标志判断 ()

//-- 开机重量受限 标志位 --- USER ---
#define  INIT_ZERO_SET_EN		((Core_data.ee_struct.Sys_seting_2 & 0x80) == 0x80)		// 开机重量受限
#define  INIT_ZERO_SET_DIS		((Core_data.ee_struct.Sys_seting_2 & 0x80) == 0x00)		// 
#define  INIT_ZERO_SET_CLR		(Core_data.ee_struct.Sys_seting_2 &= 0x7f)				// 
#define  INIT_ZERO_SET_SET		(Core_data.ee_struct.Sys_seting_2 |= 0x80)				// 
#define  INIT_ZERO_SET_XOR		(Core_data.ee_struct.Sys_seting_2 ^= 0x80)				// 


#define FORCE_ZERO_BIT_NUM 1

//#define POWER_6V_BIT_EN 0

#define POWER_6V_BIT_EN        ((Core_data.ee_struct.Sys_seting_3 & 0x01) == 0x01)
#define POWER_4V_BIT_EN        ((Core_data.ee_struct.Sys_seting_3 & 0x01) == 0x00)
#define POWER_BIT_CLK          (Core_data.ee_struct.Sys_seting_3 &= 0xfe) 
#define POWER_BIT_SET          (Core_data.ee_struct.Sys_seting_3 |= 0x01)

#define POWER_MODE		   	   (Core_data.ee_struct.Sys_seting_3 & 0x06)
#define POWER_MODE_1		   ((Core_data.ee_struct.Sys_seting_3 & 0x06) == 0x00)
#define POWER_MODE_2		   ((Core_data.ee_struct.Sys_seting_3 & 0x06) == 0x02)
#define POWER_MODE_3		   ((Core_data.ee_struct.Sys_seting_3 & 0x06) == 0x04)
#define POWER_MODE_ADD		   (Core_data.ee_struct.Sys_seting_3 += 0x02)
#define POWER_MODE_CLK		   (Core_data.ee_struct.Sys_seting_3 &= 0xf9)
//------------------------------------------------------------------
//---用来存放 需要 经常 变动的 用户变量 --- 

//EXTERN unsigned char User_seting_1;
//--- Xdata ---
//--- SYS DATA 相关定义 ---
#define  SYS_DATA_LENTH 21		// SYS 数据的 字节数
union	SYS_pattern	//定义联合体,数据可以采用字节和字两种方式访问；
{
	struct
	{  
		unsigned char User_seting_1;
		unsigned long Weight_HH;
		unsigned long Weight_LL;
		unsigned long Counter_sample_num;
		unsigned long Counter_sample_weight;
		unsigned long Per_sap_w;
	} sys_struct;
	unsigned char b[SYS_DATA_LENTH];
};
EXTERN union SYS_pattern xdata System_data;									//	配置1
//---用来存放 需要 经常 变动的 变量 --- 
//User_seting_1; 	
				//第0,1位:	(WARRING_TYPE: = 00 不报警; = 01 范围内报警; = 10 范围外报警; = 11 范围外报警)
				//第2位 	(WARRING_COUNT: = 0 重量报警 = 1数量报警)
				//第3位		(WARRING_SET: = 1 设定过报警值)	
 				//第4位		(COUNT_SET: = 1 设定过 计数)
				//第5位		(PERCENT_SET: = 1 设定过 百分比)
				//第6,7位	(LCD_LAMP_MODE: = 00 背光灯关闭 01背光灯打开 02背光灯自动)

#define  USER_SETING_1_DEFAULT	(0x80)

//-- 重量报警--方式 标志位 --- USER ---(这两位必须是字节的最低位，否则赋值会出错)	
#define  WARRING_TYPE_LOAD		(System_data.sys_struct.User_seting_1)						// 数值
#define  WARRING_TYPE_NUM		(System_data.sys_struct.User_seting_1 & 0x03)				// 数值
#define  WARRING_TYPE_NUM_ADD	(System_data.sys_struct.User_seting_1++)					// ++操作
#define  WARRING_TYPE_NUM_DEC	(System_data.sys_struct.User_seting_1--)					// --操作
#define  WARRING_TYPE_NO		(WARRING_TYPE_NUM == 0x00)			// 
#define  WARRING_TYPE_IN		(WARRING_TYPE_NUM == 0x01)			// 
#define  WARRING_TYPE_OUT		(WARRING_TYPE_NUM == 0x02)			// 
#define  WARRING_TYPE_FULL		(WARRING_TYPE_NUM >= 2)				// 达到最大值
#define  WARRING_TYPE_ZERO		(WARRING_TYPE_NUM == 0)				// 达到最小值
#define  WARRING_TYPE_NO_SET	(System_data.sys_struct.User_seting_1 &= 0xfc)				// 设定为
#define  WARRING_TYPE_IN_SET	(System_data.sys_struct.User_seting_1 |= 0x01)				// 设定为
#define  WARRING_TYPE_OUT_SET	(System_data.sys_struct.User_seting_1 |= 0x02)				// 设定为 

//-- 数量报警 标志位 --- USER ---
#define  WARRING_COUNT_EN		((System_data.sys_struct.User_seting_1 & 0x04) == 0x04)		// 数量报警
#define  WARRING_COUNT_DIS		((System_data.sys_struct.User_seting_1 & 0x04) == 0x00)		// 重量报警
#define  WARRING_COUNT_CLR		(System_data.sys_struct.User_seting_1 &= 0xfb)				// 设定为 0
#define  WARRING_COUNT_SET		(System_data.sys_struct.User_seting_1 |= 0x04)				// 设定为 1
#define  WARRING_COUNT_XOR		(System_data.sys_struct.User_seting_1 ^= 0x04)				// 标志位取反

//-- 数量设定过 标志位 --- USER ---
#define  WARRING_SET_EN			((System_data.sys_struct.User_seting_1 & 0x08) == 0x08)	// 设定过报警
#define  WARRING_SET_DIS		((System_data.sys_struct.User_seting_1 & 0x08) == 0x00)	// 没有
#define  WARRING_SET_CLR		(System_data.sys_struct.User_seting_1 &= 0xf7)				// 
#define  WARRING_SET_SET		(System_data.sys_struct.User_seting_1 |= 0x08)				// 
#define  WARRING_SET_XOR		(System_data.sys_struct.User_seting_1 ^= 0x08)				// 

//-- 数量设定过 标志位 --- USER ---
#define  COUNT_SET_EN			((System_data.sys_struct.User_seting_1 & 0x10) == 0x10)		// 设定过数量
#define  COUNT_SET_DIS			((System_data.sys_struct.User_seting_1 & 0x10) == 0x00)		// 没有 设定过数量
#define  COUNT_SET_CLR			(System_data.sys_struct.User_seting_1 &= 0xef)				// 
#define  COUNT_SET_SET			(System_data.sys_struct.User_seting_1 |= 0x10)				// 
#define  COUNT_SET_XOR			(System_data.sys_struct.User_seting_1 ^= 0x10)				// 

//-- 百分比设定过 标志位 --- USER ---
#define  PERCENT_SET_EN			((System_data.sys_struct.User_seting_1 & 0x20) == 0x20)		// 设定过百分比
#define  PERCENT_SET_DIS		((System_data.sys_struct.User_seting_1 & 0x20) == 0x00)		// 没有 设定过百分比
#define  PERCENT_SET_CLR		(System_data.sys_struct.User_seting_1 &= 0xdf)				// 
#define  PERCENT_SET_SET		(System_data.sys_struct.User_seting_1 |= 0x20)				// 
#define  PERCENT_SET_XOR		(System_data.sys_struct.User_seting_1 ^= 0x20)				// 

//-- 背光灯--方式 标志位 --- USER ---(这两位必须是字节的最低位，否则赋值会出错)	
#define  LCD_LAMP_MODE_LOAD		(System_data.sys_struct.User_seting_1)						// 数值
#define  LCD_LAMP_MODE_NUM		(System_data.sys_struct.User_seting_1 & 0xc0)				// 数值
#define  LCD_LAMP_MODE_NUM_ADD	(System_data.sys_struct.User_seting_1 += 0x40)				// ++操作
#define  LCD_LAMP_MODE_NUM_DEC	(System_data.sys_struct.User_seting_1 -= 0x40)				// --操作
#define  LCD_LAMP_OFF_MODE		(LCD_LAMP_MODE_NUM == 0x00)		// 设定为 
#define  LCD_LAMP_ON_MODE		(LCD_LAMP_MODE_NUM == 0x40)			// 设定为 
#define  LCD_LAMP_AUTO_MODE		(LCD_LAMP_MODE_NUM == 0x80)			// 设定为 
#define  LCD_LAMP_MODE_FULL		(LCD_LAMP_MODE_NUM >= 0x80)			// 达到最大值
#define  LCD_LAMP_MODE_ZERO		(LCD_LAMP_MODE_NUM == 0x00)			// 达到最小值
#define  LCD_LAMP_OFF_SET		(System_data.sys_struct.User_seting_1 &= 0x3f)				// 设定为 
#define  LCD_LAMP_ON_SET		(System_data.sys_struct.User_seting_1 |= 0x40)				// 设定为 
#define  LCD_LAMP_AUTO_SET		(System_data.sys_struct.User_seting_1 |= 0x80)				// 设定为 


//*************************************************************************//
//----------  时间相关 (宏定义) (变量)-- (时间属性) ------ 
//*************************************************************************//

//------------------------------------------------------------------
//--- (取零位-标定点)长时间读取ADC的相关门限  -- (时间属性) ---
#ifdef ADC_OUT_20Hz
	#define LONG_FILTER_NUM 10			// (时间属性)读取ADC的稳定个数
	#define LONG_FILTER_NUM_2 20		// (时间属性)读取ADC的稳定个数
	#define LONG_FILTER_ACCUM_NUM 40	// (时间属性)累计读取ADC的个数
#endif
#ifdef ADC_OUT_10Hz
	#define LONG_FILTER_NUM 5			// (时间属性)读取ADC的稳定个数
	#define LONG_FILTER_NUM_2 10		// (时间属性)读取ADC的稳定个数
	#define LONG_FILTER_ACCUM_NUM 20	// (时间属性)累计读取ADC的个数
#endif

//------------------------------------------------------------------
//-- "重量稳定，休眠" 时间变量  		-- (时间属性) ---
//-- "电压显示" 时间变量  				-- (时间属性) ---
//-- "亮度等提示字符串显示" 时间变量  	-- (时间属性) ---
EXTERN unsigned char Weight_stable_counter;  //重量稳定计数器；当重量稳次数为"Weight_stable_num"时，LED面板"单价区"、"金额区"休眠显示；
EXTERN bit Weight_sleep; 					// ==1 重量在休眠模式下 显示 "-"; == 0 不显示
EXTERN bit Sleep_dsp_mode; 					// == 1 LED 面板休眠显示
EXTERN bit Vol_state; 						// ==1 表示需要显示电池电压
//EXTERN bit Lp_dsp_symbol; 				// = 1 单价显示亮度提示

//-- 定义 重量零点 或者 重量稳定时，LED面板 休眠显示时间
#ifdef ADC_OUT_20Hz
	#define WEIGHT_ZERO_NUM 50			// (时间属性)"零点 休眠显示时间"，20Hz(2.5s)
	#define WEIGHT_STABLE_NUM 200		// (时间属性)"非零点 休眠显示时间"，20Hz(10s)
	#define WEIGHT_SLEEP_NUM 8			// (时间属性)"休眠时 数码闪动的时间"，20Hz(0.4s)
	#define VOL_DSP_NUM 40				// (时间属性)"电压显示的时间"，20Hz(2s)
	#define LP_DSP_NUM 10				// (时间属性)"电压显示的时间"，20Hz(0.5s)
#endif
#ifdef ADC_OUT_10Hz
	#define WEIGHT_ZERO_NUM 25			// (时间属性)"零点 休眠显示时间"，10Hz(2.5s)
	#define WEIGHT_STABLE_NUM 100		// (时间属性)"非零点 休眠显示时间"，10Hz(10s)
	#define WEIGHT_SLEEP_NUM 4			// (时间属性)"休眠时 数码闪动的时间"，10Hz(0.4s)
	#define VOL_DSP_NUM 20				// (时间属性)"电压显示的时间"，10Hz(2s)
	#define LP_DSP_NUM 5				// (时间属性)"电压显示的时间"，10Hz(0.5s)
#endif

//-----------------------------------------
//-- 蜂鸣器报警 时间变量  -- (时间属性) ---
EXTERN unsigned char Warring_counter;  // 蜂鸣器报警 计数器
EXTERN bit Lamp_lowv_a; // 电压过低，控制蜂鸣器报警
EXTERN bit Lamp_lowv; // 电压过低，显示 LB 重量为0 且非休眠时报警
//--
#ifdef ADC_OUT_20Hz	
	#define WARRING_NUM_LOWV_1 250		//(时间属性)
	#define WARRING_NUM_LOWV_2 20		//(时间属性)
	#define WARRING_NUM_FULL 4			//(时间属性)
	//---
	#define WARRING_NUM_HIGH 2			//(时间属性)
	#define WARRING_NUM_LOW 6			//(时间属性)
	#define WARRING_NUM_FULL 4			//(时间属性)
#endif
#ifdef ADC_OUT_10Hz
	#define WARRING_NUM_LOWV_1 125		//(时间属性)
	#define WARRING_NUM_LOWV_2 10		//(时间属性)
	#define WARRING_NUM_FULL 2			//(时间属性)
	//---
	#define WARRING_NUM_HIGH 1			//(时间属性)
	#define WARRING_NUM_LOW 3			//(时间属性)
	#define WARRING_NUM_FULL 2			//(时间属性)
#endif

//-----------------------------------------
//-- 显示重量稳定 时间变量  -- (时间属性) ---
EXTERN unsigned char Stable_light_counter;
EXTERN bit Stable_light_symbol; 			// =1 表示显示重量稳定
//--
#ifdef ADC_OUT_20Hz	
	#define STABLE_LIGHT_NUM 15				//(时间属性)
#endif
#ifdef ADC_OUT_10Hz
	#define STABLE_LIGHT_NUM 7				//(时间属性)
#endif

//-----------------------------------------
//-- 电池电压检测 时间变量  -- (时间属性) ---
EXTERN unsigned char Vol_auto_det_counter;
EXTERN bit Power_det_mode; 					//== 1 表示 检测电压时，不关闭显示
//--
#ifdef ADC_OUT_20Hz	
	#define VOL_AUTO_DET_NUM 240			//(时间属性)
#endif
#ifdef ADC_OUT_10Hz
	#define VOL_AUTO_DET_NUM 120			//(时间属性)
#endif

//-----------------------------------------
//-- 动态充电 闪动 时间变量  -- (时间属性) ---
EXTERN bit Charte_bit; // = 1 表示 连接交流电充电
EXTERN bit Charte_bit_dis; // = 1 表示 连接交流电充电充电完毕
EXTERN unsigned char idata Charge_counter; // 动态充电计数器
//--
#ifdef ADC_OUT_20Hz	
	#define CHARGE_COUNTER_NUM 10			//(时间属性)
#endif
#ifdef ADC_OUT_10Hz
	#define CHARGE_COUNTER_NUM 5			//(时间属性)
#endif

//-----------------------------------------
//-- 长时间按键处理 时间变量  -- (时间属性) ---
//-- 通过单片机计时器进行的时间控制 
//#define Press_key_counter_num 15
#define PRESS_KEY_COUNTER_NUM 0xe0
#define PRESS_KEY_COUNTER_NUM_2 0x30
//------------------------
//--将 stable_light_counter 和 Press_key_counter 合并
//--Press_key_counter 使用 stable_light_counter 的 高4位
//EXTERN unsigned char Press_key_counter; 
EXTERN bit Key_counter_en; 					//== 1 表示需要检测长时间按键

//*************************************************************************//
//----------  基本秤重变量 ------ 
//*************************************************************************//

//------------------------------------------------------------------
//--- ADC 内码 相关 --
EXTERN bit ADready;									// ==1 表示 ADC 有数据更新
EXTERN unsigned long idata	ADbuf[4];				//用于 ADC 输出的第一级 COMB^3(2)的滤波缓存;
EXTERN unsigned long idata	ADcode;					//经过滤波，最终用来计算重量的 ADC内码
EXTERN unsigned long idata	Zero_code;				//零点的内码
EXTERN bit Sign_code; 								//内码的符号(大于或者小于零位内码)
EXTERN bit Code_zero; 								// 内码处于零点  CODE_ZERO 和 WEIGHT_ZERO 的区别 在于后者要考虑TAPE(去皮)
EXTERN bit Cal_zero_en; 							// == 1 表示需要进行置零操作
EXTERN bit ADCcmd;

//------------------------------------------------------------------
//--- ADC 内码 平均取样操作 --
EXTERN bit Ave_step_en; 	// = 1 表示需要取样第一步：开始取样
													// == 0 表示需要经过一定的延时才能进入 平均取样操作
//--平均取样 前面丢弃的ADC个数
//-- 注意 -- : AVE_DIS_NUM 必须小于 LONG_FILTER_ACCUM_NUM
#ifdef ADC_OUT_20Hz	
	#define AVE_DIS_NUM	10
#endif
#ifdef ADC_OUT_10Hz 
	#define AVE_DIS_NUM	5
#endif


//--- ADC 内码 抗脉冲 --
#ifdef ADC_DE_PULSE_FUN_EN
	EXTERN unsigned char Ad_depulse_count_1;
#endif
//--- ADC 内码 去抖动 --
#ifdef ADC_DE_GLITCH_FUN_EN
	EXTERN unsigned char Ad_depulse_count_2;
	EXTERN bit Filter_en;
#endif
//------------------------------------------------------------------
//--- 内部重量 --
EXTERN unsigned long idata	Weight_internal_last;			//内部重量(没有进行温度等补偿前的内部重量)
EXTERN int Zero_weight_accum;								//记录累计 重量追零
EXTERN bit Startup_zero_symbol;								//开机强制增大追零范围
//------------------------------------------------------------------
//--- 称重变量 --
EXTERN unsigned long idata	Weight;				//显示重量
EXTERN unsigned long xdata	Tape_weight;		//皮重的重量
EXTERN bit Weight_zero;							//显示重量于零点标志位,用于LED面板"重量区"休眠显示；
EXTERN bit Sign;								//定义重量的符号位
EXTERN bit Weight_overflow; 					// 重量过大 蜂鸣器 以 1Hz 的频率报警
EXTERN bit Tape_en; 							// ==1 表示需要进行去皮操作

//------------------------------------------------------------------
// 计重报警门限变量
//为了兼容计价秤的"filter_cal_Vc"文件，
//将原来Weight_HH用Money名字来替代,将原来Weight_LL用Money_accum.w名字来替代。
EXTERN unsigned long idata Money; 			//Weight_HH;	//计重报警上限
EXTERN union  ADpattern idata Money_accum; 	//Weight_LL;	//计重报警下限

//------------------------------------------------------------------
//---计数功能 --
//EXTERN unsigned long idata Counter_sample_weight;
//EXTERN unsigned int idata Counter_sample_num;

//--平均取样 前面丢弃的 内部重量的个数
//-- 注意 -- : AVE_IW_DIS_NUM 必须小于 AVE_IW_ACCUM_NUM
#ifdef ADC_OUT_20Hz	
	#define AVE_IW_DIS_NUM		10
	#define AVE_IW_NUM			10
	#define AVE_IW_ACCUM_NUM	40
#endif
#ifdef ADC_OUT_10Hz 
	#define AVE_IW_DIS_NUM		5
	#define AVE_IW_NUM			5
	#define AVE_IW_ACCUM_NUM	20
#endif

//------------------------------------------------------------------
//---百分比功能 --
//#ifdef PERCENTAGE_FUN_EN
//	EXTERN unsigned long idata Per_sap_w;	//百分比取样重量;
//#endif


//*************************************************************************//
//----------  操作 控制 变量 ------ 
//*************************************************************************//

//------------------------------------------------------------------
//--- 校正控制变量 --
EXTERN bit Data_error; 								// 核心标定数据错误标志位: == 1 表示 核心数据错误
EXTERN bit Weight_updata_en; 						//输入标定点重量时 = 1,要求无视锁定,立即更新重量
EXTERN bit Start_up_end;						//==0 表示处于开机步骤，==1表示开机结束
EXTERN bit Save_user_data; 						//== 1 表示需要存储 用户设置
EXTERN bit Get_cal_data; 						//== 1 表示需要 执行标定结束操作
//------------------------------------------------------------------
//-- 计重计数 变量 --
EXTERN unsigned char idata Active_mode;

//Active_mode 用来标志 称 的不同状态 : 直接关系 称得显示 和 键盘响应
// bit7 - bit6: 
// 00:	计重模式
// 01:	计数模式
// 10:  百分比模式
// bit5:
//	 	== 1: 标定
// bit4 - bit0: 共32个状态,分别用于各个模式下面的操作

//工作模式 = 0 计重 = 1 计数 = 2 百分比
// 10 - 29 用于 标定 模式
// 30 - 49 用于 计重设定
// 50 - 69 用于 计数设定
// 40 - 89 用于 百分比设定


//------------------------------------------------------------------
// 箭头按键 输入变量
//A_input_num,为了兼容计价秤的"filter_cal_Vc"文件，将原来A_input_num用Price名字来替代
EXTERN unsigned long idata Price;	//A_input_num
EXTERN unsigned char idata A_cycle_num;
EXTERN unsigned char idata A_input_setp;
EXTERN bit A_input_symbol;	//==1表示正在 进行 箭头按键输入
EXTERN bit A_input_num_symbol;	//==1表示正在 进行 数字显示

EXTERN bit Cal_temp_bit; //== 1 表示 进入循环选择

//------------------------------------------------------------------
// 显示状态控制变量
EXTERN unsigned char idata Dsp_state_con;
//bit7: = 0 表示 不需要显示数字
//		= 1 表示 需要现实数字(数字不一定将字符覆盖)
//bit6: = 1 表示 需要交替显示 数字 和 字符(此时，数字时纯数字)
//bit0-bit5(64个状态):
//用于显示不同的字符(dsp_pattern)
EXTERN bit Dsp_state_num_state; //		 = 0 表示 交替显示（字符）
								//		= 1 表示 交替显示（数字）

//------------------------------------------------------------------
//-- 按键操作标志 --
EXTERN bit Cal_zero_en; 	//==1 表示需要 进行置零 操作
EXTERN bit Cal_set_en; 		//==1 
EXTERN bit Cal_mode_en; 	//==1 
EXTERN bit Cal_unit_en; 	//==1 
EXTERN bit Cal_tape_en; 	//==1 

//------------------------------------------------------------------
//-- 系统警告标志 --
EXTERN bit Weight_ul_error;		//==1表示重量报警门限设置有问题
EXTERN bit Weight_warning_h; 	// = 1 重量报警
EXTERN bit Weight_warning_l; 	// = 1 重量报警
EXTERN bit Weight_warning_in; 	// = 1 重量报警
EXTERN bit Sys_txt_warning_a; 	// = 1 表示需要 强制显示 系统级 警告
EXTERN bit Sys_txt_warning_b; 	// = 1 表示需要 强制显示 系统级 警告 但 一会退出（ Press_key_num_a）
EXTERN bit Weight_on_error; 	// = 1 表示开机 零位 过高





//*************************************************************************//
//----------  功能模块 变量 ------ 
//*************************************************************************//

//-----------------------------------------
//-- 电池电压检测定义  ---

//-- 4.3v电池门限 ---
//#define V_LOW_FULL_H 505 	//充满门限-高，电压>=于此值 表示充电充满（4.3v供电）
//#define V_LOW_FULL_L 495 	//充满门限-低，电压>=于此值 表示充电充满（4.3v供电）
#define V_LOW_FULL_H 490 	//充满门限-高，电压>=于此值 表示充电充满（4.3v供电）
#define V_LOW_FULL_L 480 	//充满门限-低，电压>=于此值 表示充电充满（4.3v供电）
#define V_LOW_1_CH 470		
#define V_LOW_2_CH 450

#define V_LOW_0 420 		//电量第一格门限，电压>=于此值 显示电池4格（4.3v供电）
#define V_LOW_1 390 		//电量第二格门限，电压>=于此值 显示电池3格（4.3v供电）
#define V_LOW_2 370 		//电量第三格门限，电压>=于此值 显示电池2格（4.3v供电）
#define V_LOW_3 360 		//电量第四格门限，电压>=于此值 显示电池1格（4.3v供电）

#define V_CO_PRE  984 	//电压系数 = VDD * 3 如 稳压管输出 3.3,则 V_CO = 990 
						// 3 为 VCC通过 分压电阻系数.//如:150k / 50k =>  = 3
						// 
//#define V_CO_PRE  670 	

//#define V_HIG_FULL_H 705 //7.05V   //电压>=于此值 表示充电充满（6.3v供电）
//#define V_HIG_FULL_L 695 //6.95V   //电压>=于此值 表示充电充满（6.3v供电）
#define V_HIG_FULL_H 690 //690V   //电压>=于此值 表示充电充满（6.3v供电）
#define V_HIG_FULL_L 680 //6.80V   //电压>=于此值 表示充电充满（6.3v供电）
#define V_HIG_1_CH 670
#define V_HIG_2_CH 650

#define V_HIG_0 620 //6.20V   ////电压>=于此值 显示电池全格（6v供电）
#define V_HIG_1 590 //5.90V  //电压>=于此值 显示电池低一格（6v供电）
#define V_HIG_2 570 //5.70V  //电压低于此值 欠压灯亮 蜂鸣器响  电压>=于此值 显示电池框（6v供电）
#define V_HIG_3 560 //5.70V  //电压低于此值 欠压灯亮 蜂鸣器响  电压>=于此值 显示电池框（6v供电）

EXTERN bit	Batter_a;	// = 1 显示 电池最大的一格
EXTERN bit	Batter_b;	// = 1 显示 电池中间的一格
EXTERN bit	Batter_c;	// = 1 显示 电池最小的一格


EXTERN bit	Batter_b_ch;	// = 1 显示 电池中间的一格
EXTERN bit	Batter_c_ch;	// = 1 显示 电池最小的一格

//-----------------------------------------
//-- 按键关机  ---
#ifdef KEY_PRESS_PD_FUN_EN
	EXTERN bit Mcu_stop_pre; //== 1 表示准备进入关机状态
	EXTERN bit Mcu_stop_rec; //== 1 表示处于关机状态
#endif
//-----------------------------------------
//-- 开机零位检测  ---
//EXTERN bit Init_load_over; //== 1 表示 开机 零位过高（>满量程的 15%左右）

//-----------------------------------------
//SDI5219 标定状态下 刷新显示 频率降低 "用来提高标定和非标定时的一致，减弱干扰"
//-- 标定状态下 刷新显示 频率降低 时间变量  -- (时间属性) ---
#ifdef SDI5219_CAL_DSP_UPDATA_SLOW	
	EXTERN unsigned char xdata Cal_dsp_slow_counter;
#endif
//--
#ifdef ADC_OUT_20Hz	
	#define CAL_DSP_SLOW_NUM	4
#else
	#define CAL_DSP_SLOW_NUM	2
#endif


//*************************************************************************//
//----------  厂家特殊功能 变量 ------ 
//*************************************************************************//
//EXTERN unsigned char xdata Imperial_num; //==0,公制 ==1英镑  ==2 VISS

EXTERN bit Imperial_bit; //==0,公制 ==1英制

EXTERN bit Warning_low;

EXTERN bit Warning_high;

EXTERN bit Warning_in;

//------------------功能设置标志位----------------
EXTERN unsigned char idata  change_num_a;
EXTERN unsigned char idata  change_num_b;
//*************************************************************************//
//----------  新增 变量 ------ 
//*************************************************************************//

EXTERN bit Data_sycle_symbol;		//用于 定义是否是数据周期, = 1表示 新的数据周期

//-----------------------------------------
//-- (强制 扩大追零 标志位) ---
#ifdef FORCE_UP_ZERO
	EXTERN bit Force_up_zero_symbol;			// = 1 强制 扩大追零范围
	EXTERN unsigned char Force_up_zero_counter;	// 强制扩大追零计数器
	#ifdef ADC_OUT_20Hz	
		#define FORCE_UP_ZERO_NUM 140			//	
	#endif
	#ifdef ADC_OUT_10Hz
		#define FORCE_UP_ZERO_NUM 70			//
	#endif
#endif

EXTERN bit Double_counter_symbol; //增加长按键的时间1倍  ==1 才开始计数

#endif

