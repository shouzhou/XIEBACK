//*************************************************************************//
//* Name				:   display_cal.h
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic 电子称 显存计算文件
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
#ifndef _DISPLAY_CAL_H_
#define _DISPLAY_CAL_H_

//*************************************************************************//
//----------  变量定义 ---------------
//*************************************************************************//

//------------------------------------------------------------------
//--- 定义LED显存字段  --
//---    SEG 8  7   6   5   4   3   2   1
//--- LED    D  C   E   H   B   G   A   F
#define SEG_A 0x04
#define SEG_B 0x08
#define SEG_C 0x20
#define SEG_D 0x40
#define SEG_E 0x80
#define SEG_F 0x02
#define SEG_G 0x01
#define SEG_H 0x10
//------------------------------------------------------------------
//--- 定义LCD显存字段  --
//---    SEG 8  7   6   5   4   3   2   1
//--- LCD    H  G   F   E   D   C   B   A
#define SCG_A 0x01
#define SCG_B 0x02
#define SCG_C 0x04
#define SCG_D 0x08
#define SCG_E 0x10
#define SCG_F 0x20
#define SCG_G 0x40
#define SCG_H 0x80



//------------------------------------------------------------------
//电子称 显示位数: 从【重量最高位】- 【金额最低位】为[0 - 15]
//--- 定义LED显存分布  --
//--即使 【显示位转换】也不要更改顺序
//重量区显存分布  
#define  LED_WEIGHT_6 	0
#define  LED_WEIGHT_5 	1
#define  LED_WEIGHT_4 	2
#define  LED_WEIGHT_3 	3
#define  LED_WEIGHT_2 	4
#define  LED_WEIGHT_1 	5

//#define  LED_WEIGHT_6 	2
//#define  LED_WEIGHT_5 	3
//#define  LED_WEIGHT_4 	4
//#define  LED_WEIGHT_3 	5
//#define  LED_WEIGHT_2 	6
//#define  LED_WEIGHT_1 	7

//LED 符号 区显 存分布
#define  LED_LABLE_1 6
//#define  LED_LABLE_1 15
//#define  LED_LABLE_2 	15

//--- 定义LCD显存分布  --
//--即使 【显示位转换】也不要更改顺序
//重量区显存分布
#define  LCD_WEIGHT_6 	0
#define  LCD_WEIGHT_5 	1
#define  LCD_WEIGHT_4 	2
#define  LCD_WEIGHT_3 	3
#define  LCD_WEIGHT_2 	4
#define  LCD_WEIGHT_1 	5
#define  LCD_WEIGHT_0 	6


//#define  LCD_WEIGHT_6 	2
//#define  LCD_WEIGHT_5 	3
//#define  LCD_WEIGHT_4 	4
//#define  LCD_WEIGHT_3 	5
//#define  LCD_WEIGHT_2 	6
//#define  LCD_WEIGHT_1 	7
//#define  LCD_WEIGHT_0 	0




//LCD 符号 区显 存分布
//#define  LCD_LABLE_1 	15
//#define  LCD_LABLE_2 	15
//--- 定义显示 相关变量  --
EXTERN unsigned char Led_bright;	//显示亮度变量
EXTERN bit Price_light_symbol; 		// = 1 表示背光关闭 = 0 表示背光开启

//--- 显示更新 相关变量  --
EXTERN bit Led_dsp_updata;			// 更新显示 LED 标志位
EXTERN bit Led_dsp_updata_2;		// 更新显示 LED 标志位
EXTERN bit Dsp_en;					// = 1 表示液晶打开显示

#ifdef LOW_DSP_UPDATA
	EXTERN bit Low_speed_updata;	// = 1 表示需要在低速下更新显示
#endif

//--- 指示灯  --
EXTERN bit Lamp_tape;				// tape  指示灯
EXTERN bit Lamp_accum;				// accum 指示灯


//*************************************************************************//
//----------  函数原型 ---------------
//*************************************************************************//
void display_cal(bit dsp_mode);

#endif