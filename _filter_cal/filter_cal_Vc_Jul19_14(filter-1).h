//*************************************************************************//
//* Name				:   display_cal.h
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic 电子称 滤波及重量、数量处理
//* $Author				:   superc
//*
//* Copyright 2012 by solidic
//*
//* Recode:
//*						1: 2013-01-10  First Creat By Superc
//*
//*
//*
//*************************************************************************//
#ifndef _FILTER_CAL_H_
#define _FILTER_CAL_H_

//*************************************************************************//
//----------  预编译选项 ------ 手动调整 部分 ---------
//*************************************************************************//

//------------------------------------------------------------------
//-- 定义 内部重量 的 分度
// 1:	内部重量 6万分度
// 2:	内部重量 12万分度
// 3:	内部重量 24万分度
#define INTER_WEIGHT_MODE 1
//-- 定义 内部重量进入锁定的门限(通常取值如下)
// 1:	内部重量 6万分度
// 2:	内部重量 12万分度
// 4:	内部重量 24万分度
#define INTER_WEIGHT_LOCK_TH 1	
	
//------------------------------------------------------------------
//-- 定义 数据处理的相关门限 --
#define OUT_ZERO_TH 14		//定义内部重量从0跳到非零的最小门限	
#define INTO_ZERO_TH 7		//定义内部重量从0跳到非零的最小门限	
#define DE_JETTER_TH 4		//去抖动门限
//-- 如果环境差，或者不好的传感器方案可以加大零点起跳
//#define OUT_ZERO_TH 14		//定义内部重量从0跳到非零的最小门限	
//#define INTO_ZERO_TH 7		//定义内部重量从0跳到非零的最小门限	
//#define DE_JETTER_TH 4		//去抖动门限

//------------------------------------------------------------------
//--定义 是否启用 "简单线性修正" --
//--- 目前传感器(2mV/V)，(24为ADC输出去掉低2位后)，满量程的内码大概60万
//--- 如:30kg量程，1g对应20内码左右
//--- 同理:6kg量程，0.2g对应20内码左右
#ifdef W_LINE_CAL_EN			//一般 30kg 满量程补偿 1.5g就行了
	#define  BEGIN_CAL_CODE 	300000 // 开始修正点 15kg
	#define  SLOPE_CAL_CODE 	10000 // (600000 - 300000 ) / 30 ; ( 开始修正点 15kg - 结束修正点 30kg) / // 总修正量 1.5g
	//#define  SLOPE_CAL_CODE 	2500 // (600000 - 300000 ) / 120 ; ( 开始修正点 15kg - 结束修正点 30kg) / // 总修正量 6g	
#endif

//------------------------------------------------------------------
//--定义 是否启用 "锁定功能"  -- (时间属性) ---
//-- 一般加载时的解锁，主要是通过"快速解锁"和"累加解锁"
//-- "持续偏移解锁"用在漂移大的时候，此时"快速解锁"和"累加解锁"门限设定大，通过持续内码漂移判断是否加载
//-- "持续偏移解锁"目前使用的很少，而且其和滤波强度有关系，使用时要具体测试

//---------   快速解锁  -------
//-- 实际上,当加载东西的时候,大部分都是通过这种方式解锁
//-- 测试，当设定为8时，加载0.5g的东西基本上都能解锁
#ifdef INTER_WEIGHT_LOCK
	#define QUICK_UNLOCK_W 	10 							// 快速解除重量锁定 的ADC内码门限

//---------   持续偏移解锁  -------	
//-- 实际上,当加载东西的时候,很难连续往一个方向跳动7次,
//-- 而且一般有4次降采样的话，就要在连续的24次数据中，滤波数值数值很难连续往一个方向跳动7次
//-- 尤其还是要在30次数据内，"经测试，如果设定为(7 -- 30),即使加载7g，有时候也可能无法跳出来"
//-- 设定为 " UNLOCK_NUM(7) - UNLOCK_TIME_NUM(30)" 的话，一般没什么用，还不如去掉不用
//--
//-- 0819正常不加载的时候，这个数值大概在3左右，所以设定为5的时候一般加载1g能很好的通过这种方式解锁
//-- 所以，正常不加电阻的时候应该设定为 " UNLOCK_NUM(5) - UNLOCK_TIME_NUM(40)"
//-- 如果为了保证抗蠕变好点，也就是不要频繁推出锁定的话，设定为 " UNLOCK_NUM(7) - UNLOCK_TIME_NUM(30)"
//--
//-- Filter-R滤波设定为 " UNLOCK_NUM(7) - UNLOCK_TIME_NUM(30)" 的话，一般没什么用，主要是通过"快速解锁"和"累加解锁"
	
	#ifdef ADC_OUT_20Hz	
		#define UNLOCK_NUM 			7 					// (时间属性)解除重量锁定 的ADC内码门限
		#define UNLOCK_TIME_NUM 	30					// (时间属性)衡量解锁判断的时间 1.5s  (20Hz ADC输出)
	#endif
	#ifdef ADC_OUT_10Hz
		#define UNLOCK_NUM 			4 					// (时间属性)解除重量锁定 的ADC内码门限
		#define UNLOCK_TIME_NUM 	15					// (时间属性)衡量解锁判断的时间 1.5s  (10Hz ADC输出)
	#endif
	//------------------------
	EXTERN char idata	ADcode_unlock_buf[2]; 			//用于判断重量解锁的 内码变量
#endif

EXTERN unsigned char idata Weight_lock_counter;  		//重量锁定计数器；当重量稳次数为"Weight_lock_num"时，重量锁定
EXTERN bit Weight_lock_symbol;							// 重量稳定后一定时间后 锁定，直到大于一定的内码突变才解锁；
//-- WEIGHT_LOCK_NUM 必须大于 WEIGHT_LOCK_AVE_NUM
#ifdef ADC_OUT_20Hz	
	#define WEIGHT_LOCK_NUM 	60						// (时间属性)重量稳定后3s 锁定
	#define WEIGHT_LOCK_AVE_NUM 20						// (时间属性)重量锁定中，计算锁定点平均值的ADC数据个数
#endif
#ifdef ADC_OUT_10Hz
	#define WEIGHT_LOCK_NUM 	30						// (时间属性)重量稳定后3s 锁定
	#define WEIGHT_LOCK_AVE_NUM 10						// (时间属性)重量锁定中，计算锁定点平均值前 丢掉的ADC数据个数
#endif

//------------------------------------------------------------------
//--定义 是否启用 "强制归零" ---
//--
EXTERN bit Zero_force_start_en; 	//强制回零 需要开启
#ifdef FORCE_WEIGHT_ZERO			//每个数值代表0.5d
	EXTERN bit Zero_force_symbol; 	//强制回零 标志位
	#define ZERO_FORCE_NUM_1 3		//定义 强制回零 门限 1.5d
	#define ZERO_FORCE_NUM_2 4		//定义 强制回零 门限 2d
	#define ZERO_FORCE_NUM_3 8		//定义 强制回零 门限 4d

	#if INTER_WEIGHT_MODE == 1
		#define ZERO_FORCE_NUM_EN 20000 	//定义 强制回零 启动门限 1/3(60000内部分度)
	#endif
	
	#if INTER_WEIGHT_MODE == 2
		#define ZERO_FORCE_NUM_EN 40000 	//定义 强制回零 启动门限 1/3(120000内部分度)
	#endif
	
	#if INTER_WEIGHT_MODE >= 3
		#define ZERO_FORCE_NUM_EN 80000 	//定义 强制回零 启动门限 1/3(240000内部分度)
	#endif	
#endif




//*************************************************************************//
//----------  预编译选项 ------ 自动计算 部分 ---------
//*************************************************************************//

//-------更改内部重量分度时--下面 不需要 改动-------------
#if INTER_WEIGHT_MODE == 1
	#define CAL_POINT_W 12000	//6万分度的1/5 -> 该点内码12万左右
	#define INTER_WEIGHT_NUM 1  //内部重量 和 外部重量的关系 (2^INTER_WEIGHT_NUM 倍)
	#define MIN_NEG_ZERO_W 8  	//最小的内部负向追零重量
#endif

#if INTER_WEIGHT_MODE == 2
	#define CAL_POINT_W 20000	//12万分度的1/6 -> 该点内码10万左右
	#define INTER_WEIGHT_NUM 2	//内部重量 和 外部重量的关系 (2^INTER_WEIGHT_NUM 倍)
	#define MIN_NEG_ZERO_W 16  	//最小的内部负向追零重量
#endif

#if INTER_WEIGHT_MODE >= 3
	#define CAL_POINT_W 24000	//24万分度的1/10 -> 该点内码6万左右
	#define INTER_WEIGHT_NUM 3	//内部重量 和 外部重量的关系 (2^INTER_WEIGHT_NUM 倍)
	#define MIN_NEG_ZERO_W 24  	//最小的内部负向追零重量
#endif











//*************************************************************************//
//----------  变量定义 ------ (内码的滤波处理) ---------
//*************************************************************************//

//------------------------------------------------------------------
//--- ADC第二级 滤波变量 -- 
EXTERN int idata ADbuf_delta[9];					//用于 ADC 输出的第二级 的滤波缓存;
EXTERN unsigned long idata Downsample_filter;		//用于 ADC 输出的第二级 滤波 中的降采样 缓存;	
EXTERN unsigned char idata Downsample_counter;		//用于 ADC 输出的第二级 滤波 中的降采样 计数;	
//------------------------
//ADbuf_init:高8位作为键盘按键编码使用，低24位用于adc的数据采集
//EXTERN unsigned char KEY_CODE; //按键编码
//EXTERN unsigned long idata	ADbuf_init;			//用于记录 ADC输出 的BUF (用于记录多个ADC输出的公共值);
EXTERN union  ADpattern idata ADbuf_init;			//用于记录 ADC输出 的BUF (用于记录多个ADC输出的公共值);

//--- ADC 处理中的相关门限 -- 
#define  QUICK_CO_A 	2000	//用于 ADC 输出的第二级 滤波	
#define  QUICK_CO_B		200		//用于 ADC 输出的第二级 滤波		
#define  QUICK_CO_C 	15		//用于 ADC 输出的第二级 滤波	
#define  QUICK_CO_D 	80		//用于 快速间歇供电的门限
#define  AVER_CO_A		1600	//处理 多个ADC输出的公共值 

//--- 传感器自动匹配 门限 -- 
//--- 目前传感器(2mV/V)，(24为ADC输出去掉低2位后)，满量程的内码大概60万
//--- 如:30kg量程，1g对应20内码左右
//--- 同理:6kg量程，0.2g对应20内码左右
//--- 对不同的传感器，传感器自动识别按照 1分度 20内码左右识别，这样，无论什么传感器均是 3万分度
//--- (注意，这个3万分度还要经过 "<< INTER_WEIGHT_NUM"才能变为真正的内部重量)
#define  UP_CAL_D_CODE 		27 // 最大 1d内部分度 对应的 AD 内码
#define  LOW_CAL_D_CODE 	14 // 最小 1d内部分度 对应的 AD 内码
#define  IDELA_CAL_D_CODE 	20 // 理想1d内部分度 对应的 AD 内码


//*************************************************************************//
//----------  变量定义 ------ (重量处理) ---------
//*************************************************************************//

//------------------------------------------------------------------
//--- 定义追零判断时间 -- (时间属性) ---
EXTERN bit Zero_buf_symbol;					// 追零标志位
EXTERN unsigned char Follow_zero_counter; 	// 追零控制 计数器
//--
#ifdef ADC_OUT_20Hz
	#define FOLLOW_ZERO_NUM_0 20	// (时间属性)定义追零判断时间，对应 时间为 0.5s；
	#define FOLLOW_ZERO_NUM_1 20	// (时间属性)定义追零判断时间，对应 时间为 1s；
#endif
#ifdef ADC_OUT_10Hz
	#define FOLLOW_ZERO_NUM_0 5		// (时间属性)定义追零判断时间，对应 时间为 0.5s；
	#define FOLLOW_ZERO_NUM_1 10	// (时间属性)定义追零判断时间，对应 时间为 1s；
#endif

//------------------------------------------------------------------
//--- 抗震动相关变量 -- (时间属性) ---
#ifdef GLITCH_FILTER_FUN_EN
	EXTERN unsigned long idata	Glitch_filter;		// 抗震动 平均滤波变量
	EXTERN unsigned char Glitch_filter_counter; 	// 抗震动计数器
	EXTERN bit Glitch_remove_sign;					// 记录 内码偏离 抗震锁定值的方向					
	EXTERN bit Glitch_remove_en;					// = 1 进入 抗震动锁定功能
	//EXTERN bit glitch_remove_symbol; 				//抗震动 功能 打开
#endif
//--
#if INTER_WEIGHT_MODE == 1
	#define GLITCH_W_NUM_0 200 					//	(普通)抗震动 内部重量门限 "1/30"(6万分度)
	#define GLITCH_W_NUM_1 2000 				//	(畜牧)抗震动 内部重量门限 "1/30"(6万分度)
	#define GLITCH_W_NUM_2 300 					//	(畜牧)抗震动 内部重量门限
	#define GLITCH_W_NUM_3 150 					//	(畜牧)抗震动 内部重量门限
	#define GLITCH_W_NUM_4 20 					//	(畜牧)抗震动 内部重量门限
	#define GLITCH_W_EN_NUM 100 				//	启动(畜牧)抗震动 内部重量门限 "1/30"(6万分度)
#endif
#if INTER_WEIGHT_MODE == 2
	#define GLITCH_W_NUM_0 400 					//	(普通)抗震动 内部重量门限 "1/30"(12万分度)
	#define GLITCH_W_NUM_1 4000 				//	(畜牧)抗震动 内部重量门限 "1/30"(12万分度)
	#define GLITCH_W_NUM_2 600 					//	(畜牧)抗震动 内部重量门限
	#define GLITCH_W_NUM_3 300 					//	(畜牧)抗震动 内部重量门限
	#define GLITCH_W_NUM_4 40 					//	(畜牧)抗震动 内部重量门限
	#define GLITCH_W_EN_NUM 200 				//	启动(畜牧)抗震动 内部重量门限 "1/30"(6万分度)
#endif
#if INTER_WEIGHT_MODE >= 3
	#define GLITCH_W_NUM_0 800 					//	(普通)抗震动 内部重量门限 "1/30"(24万分度)
	#define GLITCH_W_NUM_1 8000 				//	(畜牧)抗震动 内部重量门限 "1/30"(24万分度)
	#define GLITCH_W_NUM_2 1200 					//	(畜牧)抗震动 内部重量门限
	#define GLITCH_W_NUM_3 450 					//	(畜牧)抗震动 内部重量门限
	#define GLITCH_W_NUM_4 80 					//	(畜牧)抗震动 内部重量门限
	#define GLITCH_W_EN_NUM 400 				//	启动(畜牧)抗震动 内部重量门限 "1/30"(6万分度)
#endif
//--GLITCH_T_NUM_2 必须大于 GLITCH_R_NUM
#ifdef ADC_OUT_20Hz 
	// -- GLITCH_T_NUM_0 当 GLITCH_W_NUM_0 值比较大时(如400,600等)，才设到10。否则容易晃动
	#define GLITCH_T_NUM_0 6 						//	(普通)抗震动 时间门限
	#define GLITCH_T_NUM_1 20 						//	(畜牧)抗震动 时间门限
	#define GLITCH_T_NUM_2 50 						//	(畜牧)抗震动 时间门限
	#define GLITCH_R_NUM 20 						//	重量变化时丢掉的数据 时间门限
#endif
#ifdef ADC_OUT_10Hz
	#define GLITCH_T_NUM_0 3 						//	(普通)抗震动 时间门限
	#define GLITCH_T_NUM_1 10 						//	(畜牧)抗震动 时间门限
	#define GLITCH_T_NUM_2 25 						//	(畜牧)抗震动 时间门限
	#define GLITCH_R_NUM 10 						//	重量变化时丢掉的数据 时间门限	
#endif

//------------------------------------------------------------------
//--- 传感器稳定性跟踪 相关变量 -- (时间属性) ---
#ifdef GLITCH_DETECT_EN
	EXTERN unsigned char idata AD_stable_counter;  //内码稳定计数器
	EXTERN unsigned long idata AD_stable_buf;  //
	EXTERN unsigned long idata AD_stable_buf_temp;  //
#endif
#ifdef GLITCH_FILTER_FUN_EN
	EXTERN bit AD_stable_symbol;	//内码稳定标志位
#endif
//--
#define AD_STABLE_C_NUM_1 600 						//判断传感器稳定的 内码门限
//--
#ifdef ADC_OUT_20Hz 
	#define AD_STABLE_T_NUM_0 20 					//判断传感器稳定的时间门限
#endif
#ifdef ADC_OUT_10Hz
	#define AD_STABLE_T_NUM_0 10 					//判断传感器稳定的时间门限
#endif

//------------------------------------------------------------------
//--- 滤波 强度 动态处理 ---
EXTERN bit Filter_degree_a; 					//控制滤波级别 = 1精密滤波
EXTERN unsigned char Filter_count_a; 			//精密滤波 启动 计数器
EXTERN unsigned long Filter_buf_a; 				//精密滤波 启动 计数器

//------------------------------------------------------------------
//--- 定义 蠕变跟踪 -- (时间属性) ---
EXTERN unsigned long idata Worm_begin_buf; 		// 开始检测蠕变时的AD转换数据
EXTERN int idata Worm_change_follow_buf;
EXTERN bit Worm_en_symbol; 						//抗蠕变启动 标志位
EXTERN unsigned char idata Worm_change_follow_counter; // 当重量稳定后，每30秒 进行一次蠕变跟踪
//---------   累加解锁  -------
#define  UNLOCK_ACCUM_NUM 	18 // 绝对内码偏差 重量锁定 门限
//--
#ifdef ADC_OUT_20Hz
	#define WORM_CHANGE_FOLLOW_NUM 60				//(时间属性) 对应 20Hz 输出时，即 时间为 3s
#endif
#ifdef ADC_OUT_10Hz
	#define WORM_CHANGE_FOLLOW_NUM 30				//(时间属性) 对应 10Hz 输出时，即 时间为 3s
#endif


//------------------------------------------------------------------
//--- 传感器回程处理 ---
//-- 定义 是否启用 "回程补偿" --
//--
#ifdef W_REDUCE_CAL_EN
	EXTERN bit Sensor_cal; 							// = 1 需要传感器 大重量修正
	EXTERN bit Sensor_cal_2; 						// = 1 需要传感器 大重量修正
	EXTERN unsigned char idata Sensor_cal_counter; 	//定义追零大小
	EXTERN bit Sensor_cal_dis; 						// = 0 启动 = 1 关闭补偿	
#endif

//------------------------------------------------------------------
//--- ADC去极端情况计数器 处理 ---
//EXTERN unsigned char ADfilter_count;
EXTERN unsigned char Filter_up_counter; 	// 滤波增强计数器

//*************************************************************************//
//----------  函数原型 ---------------
//*************************************************************************//

//读取adc转换数据，对其进行适当的平滑、滤波。
void	Filter_ADdata(void);

// 将输入数据1g 转换到 内部数据
unsigned long data_trans_into(void);

// 将内部数据 转换到 顶层输入数据1g 
unsigned long data_trans_out(unsigned long tdata);

//----处理标定数据----
void cal_data_process(void);

//-----------内部重量处理函数*********************
void internal_weight_cal(void);	

#endif