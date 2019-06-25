//*************************************************************************//
//* Name				:   sdi0819.c
//* Vision				:	VC
//* Project				:   SDI0819系列方案
//* Function			:   SDI0819 接口函数
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
#include <REG52.H>
#include <math.h>
//**************************************************************************
// -- 全部头文件 集中管理 ---- ，版本修改文件名会变动 ---  必须修改 这个文件
//**************************************************************************
#include "./_solidic/head_file_version.h"

//****************************************************
//-------------其他模块全局变量----------------------
//----全局变量 ADCcmd : 		//(头文件)ext_data
//----IO定义 : 					//(头文件)ext_data
//****************************************************

void data_receive(void)
{
	union  ADpattern idata temp;
	//--ADcode_pre--赋值
	temp.b[1] = SGADC3;
	temp.b[2] = SGADC2;
	temp.b[3] = SGADC1;
	temp.b[0] = 0;
	temp.w ^= 0x800000;			// 因为输出为双极性，+0x800000将负端平移上来
	ADcode_pre = temp.w;
	//-- 控制休眠 --
	if(ADCcmd == 1)
	{
		//-- 关闭 SG-ADC --
		//SCLK = 1;
		PD_CON &= 0xe7;		//解锁 SGADCON,SGADCON2
		SGADCON &= 0xbf; 	//SG-ADC 休眠
		PD_CON = 0x7e;		//锁定P2,SGADCON,SGADCON2
		ADCcmd = 0;
	}
}
