//*************************************************************************//
//* Name				:   data_nvm_825.h
//* Vision				:	V-Nov07_12
//* Project				:   ���� 825 ϵ�е�Ƭ��
//* Function			:   Solidic ���ӳ� ����ʧ������ ����
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

#ifndef _DATA_NRM_SDI5219_H_
#define _DATA_NRM_SDI5219_H_


EXTERN unsigned char xdata NRM_securty_a;						// EEPROM ��ȫ��A
EXTERN unsigned char xdata NRM_securty_b;						// EEPROM ��ȫ��B
//-- NRM_securty_temp1 �����������ܵĻ��������� --
//EXTERN unsigned char idata NRM_securty_temp1  _at_ 0xc4;		// ����0xc4��RAM����ֹ��FLASH_ENA��ͻ
//-- NRM_securty_temp2/3 �����������û��ϵ
//EXTERN unsigned char idata NRM_securty_temp2  _at_ 0xc5;		// ����0xc5��RAM����ֹ��FLASH_ENB��ͻ
//EXTERN unsigned char idata NRM_securty_temp3  _at_ 0xc6;		// ����0xc6��RAM����ֹ��FLASH_ENC��ͻ

//**************************
//EEPROM�ӿں���
//**************************
//��ȡEEPROM�е�һ���֣�4�ֽڣ���
unsigned long nvm_data_read(unsigned int addr);
//��EEPROM��д��һ���֣�4�ֽڣ���
void nvm_data_write(unsigned int addr, unsigned long in_data);
//��ȡEEPROM�е�һ���ֽ�
unsigned char nvm_data_read_byte(unsigned int addr);
//��EEPROM��д��һ���ֽ�
void nvm_data_write_byte(unsigned int addr, unsigned char in_data);
//EEPROM����������
void e2rom_erase(unsigned int addr);


#endif