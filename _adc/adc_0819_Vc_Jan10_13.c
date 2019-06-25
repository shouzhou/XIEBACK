//*************************************************************************//
//* Name				:   sdi0819.c
//* Vision				:	VC
//* Project				:   SDI0819ϵ�з���
//* Function			:   SDI0819 �ӿں���
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
// -- ȫ��ͷ�ļ� ���й��� ---- ���汾�޸��ļ�����䶯 ---  �����޸� ����ļ�
//**************************************************************************
#include "./_solidic/head_file_version.h"

//****************************************************
//-------------����ģ��ȫ�ֱ���----------------------
//----ȫ�ֱ��� ADCcmd : 		//(ͷ�ļ�)ext_data
//----IO���� : 					//(ͷ�ļ�)ext_data
//****************************************************

void data_receive(void)
{
	union  ADpattern idata temp;
	//--ADcode_pre--��ֵ
	temp.b[1] = SGADC3;
	temp.b[2] = SGADC2;
	temp.b[3] = SGADC1;
	temp.b[0] = 0;
	temp.w ^= 0x800000;			// ��Ϊ���Ϊ˫���ԣ�+0x800000������ƽ������
	ADcode_pre = temp.w;
	//-- �������� --
	if(ADCcmd == 1)
	{
		//-- �ر� SG-ADC --
		//SCLK = 1;
		PD_CON &= 0xe7;		//���� SGADCON,SGADCON2
		SGADCON &= 0xbf; 	//SG-ADC ����
		PD_CON = 0x7e;		//����P2,SGADCON,SGADCON2
		ADCcmd = 0;
	}
}