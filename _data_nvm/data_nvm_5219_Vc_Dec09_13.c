//*************************************************************************//
//* Name				:   data_nvm_825.c
//* Vision				:	V-Jun20_13
//* Project				:   
//* Function			:   Solidic ���ӳ� ����ʧ������ ����
//* $Author				:   superc
//*
//* Copyright 2012 by solidic
//*
//* Recode:
//*						1: 2013-06-20  First Creat By Superc
//*
//*
//*
//*************************************************************************//

#include <REG52.H>
#include <absacc.h>
#include <math.h>
//***************************************************************************
// -- ȫ��ͷ�ļ� ���й��� ---- ���汾�޸��ļ�����䶯 ---  �����޸� ����ļ�
//***************************************************************************
#include "./_solidic/head_file_version.h"

//****************************************************
//-------------����ģ��ȫ�ֱ���----------------------
//----ȫ�ֱ���  : 		//(ͷ�ļ�)
//****************************************************

//****************************************************
//EEPROM�ӿں���
//****************************************************
/*
//��ȡEEPROM�е�һ���֣�4�ֽڣ���
unsigned long nvm_data_read(unsigned int addr)
{
	unsigned char	i;
	union ADpattern d;	 //����������,���ݿ��Բ����ֽں������ַ�ʽ���ʣ�
	
	for (i=0;i<4;i++)
  	{
		d.b[i] = nvm_data_read_byte(addr);	
		addr ++;	
	}
	return(d.w);
}

//��EEPROM��д��һ���֣�4�ֽڣ���
void nvm_data_write(unsigned int addr, unsigned long in_data)
{
	unsigned char i;
	union ADpattern d;
	d.w = in_data;

	for (i=0;i<4;i++)
  	{
		FLASH_ENA = 0x05;	//nvm_data_write_byte ����������仰
		nvm_data_write_byte(addr,d.b[i]);
		addr ++;		
	}
}
*/

//��ȡEEPROM�е�һ���ֽڣ�
unsigned char nvm_data_read_byte(unsigned int addr)
{
	unsigned char	i;
	i = CBYTE[addr];	
	return(i);
}

//��EEPROM��д��һ���ֽ�
//����ǰ��Ҫ:
//NRM_securty_a,NRM_securty_b
//flash�����ر����жϣ��������Ὺ�����ж�(ע��)
void nvm_data_write_byte(unsigned int addr, unsigned char in_data)
{
	union INTpattern flash_addr;
	bit ea_save;
	flash_addr.i = addr;
   	ea_save  =  EA;            // Save EA
	EA = 0;
	//-- ׼����ַ������ --
	FLASH_ADDRH = flash_addr.b[0]; // point to the address you want to erase 
	FLASH_ADDRL = flash_addr.b[1]; 
	FLASH_DATA = in_data;
	//-- ״̬��� --
	FLASH_ENA = 0x00;
	FLASH_ENB = 0x00;
	FLASH_ENC = 0x00;
	//-- ͨ����ȫ������������ Flash ���� --
	if((NRM_securty_a == 0xaa)&&(NRM_securty_b == 0x55))
		FLASH_CON = 0x03;	//״̬�����ִ��һ��FLASH_CON������ȫ״̬��λ
	_nop_();	//��ʱ
	//-- д��ȫ�뿪��Flash������� --		
	FLASH_ENA = 0x05;
	FLASH_ENB = 0x0a;
	FLASH_ENC = 0x09;
	//-- ͨ����ȫ������������ Flash ���� --
	if((NRM_securty_a == 0xaa)&&(NRM_securty_b == 0x55))
		FLASH_CON = 0x03;
	//-- �����ȫ�� --
	FLASH_ENA = 0x00;
	FLASH_ENB = 0x00;
	FLASH_ENC = 0x00;
	EA = ea_save;
}

//EEPROM BLOCK(1k) ����
//addr = ��0 - 31��* 1024 ,������Ӧ��Block��ַ
//����ǰ��Ҫ:
//NRM_securty_a,NRM_securty_b
//flash�����ر����жϣ��������Ὺ�����ж�(ע��)
void e2rom_erase(unsigned int addr)
{
	union INTpattern flash_addr;
	bit ea_save;
	flash_addr.i = addr;
   	ea_save  =  EA;            // Save EA
	EA = 0;
	FLASH_ADDRH = flash_addr.b[0]; // point to the address you want to erase 
	FLASH_ADDRL = flash_addr.b[1]; 
	//-- ״̬��� --
	FLASH_ENA = 0x00;
	FLASH_ENB = 0x00;
	FLASH_ENC = 0x00;
	//-- ͨ����ȫ������������ Flash ���� --
	if((NRM_securty_a == 0xaa)&&(NRM_securty_b == 0x55))
		FLASH_CON = 0x03;	//״̬�����ִ��һ��FLASH_CON������ȫ״̬��λ
	_nop_();
	//-- д��ȫ�뿪��Flash������� --
	FLASH_ENA = 0x05;
	FLASH_ENB = 0x0a;
	FLASH_ENC = 0x09;
	//-- ͨ����ȫ������������ Flash ���� --
	if((NRM_securty_a == 0xaa)&&(NRM_securty_b == 0x55))
		FLASH_CON = 0x0c;
	//-- �����ȫ�� --
	FLASH_ENA = 0x00;
	FLASH_ENB = 0x00;
	FLASH_ENC = 0x00;
	EA = ea_save;	
}