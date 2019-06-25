//*************************************************************************//
//* Name				:   display_cal.c
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
#include <REG52.H>
#include <math.h>
//**************************************************************************
// -- 全部头文件 集中管理 ---- ，版本修改文件名会变动 ---  必须修改 这个文件
//**************************************************************************
#include "./_solidic/head_file_version.h"

//****************************************************
//----全局变量 Dsp_buf[]:  16字节显存
//----
//****************************************************




//----------------------------------------------------
//----- 键码扫描函数 ------------
void key_scan(void)		
{
	EA = 0;

	delay_ms(5);
	// 6ms 延时后再次检测是否有健按下
	// 检测是否有健按下
	//--IO口配置--
	IO_MODE_KEY_A_EN;
	delay_ms(0);
	// 如果 KEY_2,KEY_4为1,则说明没有键被按下
	if(KEY_PIN3 && KEY_PIN1 && KEY_PIN5 && KEY_PIN7)
	{
		ADbuf_init.b[0] = NONE;
	}
	else
	{

//----- 扫描列线 (KEY5) -------------------------------------------
//--- 判断按键 ---
		if(!KEY_PIN1)
		    ADbuf_init.b[0] = SET_KEY;
		else
		if(!KEY_PIN3)
			ADbuf_init.b[0] = ZERO;
		else
		if(!KEY_PIN5)
		    ADbuf_init.b[0] = ARROW_KEY;
		else
		if(!KEY_PIN7)
			ADbuf_init.b[0] = TAPE;
		else
			ADbuf_init.b[0] = NONE;	
	}
	//--IO口配置--
	IO_MODE_KEY_DIS;
	EA = 1;
}
