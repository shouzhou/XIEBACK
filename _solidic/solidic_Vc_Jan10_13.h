//*************************************************************************//
//*
//* Project				:   All project
//* Function			:   Solidic ͨ�� ͷ�ļ�
//* $Author				:   superc
//* $Name				:   solidic.h
//* Copyright 2012 by solidic
//*
//* Recode:
//*						1: 2012-11-07  First Creat By Superc
//*
//*
//*
//*************************************************************************//
#ifndef _SOLIDIC_H_
#define _SOLIDIC_H_



/*****************������������***************************/
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

/*****************������������***************************/
union	ADpattern				//����������,���ݿ��Բ����ֽں������ַ�ʽ���ʣ�
{
	unsigned long	w ;
	unsigned char   b[4];
};

//---------
union	INTpattern				//����������,���ݿ��Բ����ֽں������ַ�ʽ���ʣ�
{
	unsigned int	i ;
	unsigned char   b[2];
};

//---------
typedef enum _bool
{
    FALSE,TURE
} boolean;
#define TURE ( 1 == 1 )
#define FALSE ( 1 == 0 )




#endif // _SOLIDIC_H_
