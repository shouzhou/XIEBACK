//*************************************************************************//
//* Name				:   ext_data.h
//* Vision				:	V-Nov07_12
//* Project				:   ���ӼƼ۳� ϵ�з���
//* Function			:   ȫ�ֱ��� ͷ�ļ�
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
// -- ���� Ԥ���� ѡ�� ���� --
//************************************************************************************************************************

//------------------------------------------------------------------
//--- ѡ�� ����ģ�� --
//------------------------------------------------------------------
//#define PERCENTAGE_FUN_EN			//���øĺ꣬��ʾ���� "�ٷֱȹ���"

//#define THREE_CAL_FUN_EN			//���øĺ꣬��ʾ���� "���α궨����"

//--- ȥ���� ---
#define GLITCH_FILTER_FUN_EN		//���øĺ꣬��ʾ���� "������������"
#define GLITCH_DETECT_EN			//(Ҫ����9���ֽ��ڴ�)���øĺ꣬��ʾ���� "�������ȶ��Ը���"
//#define ADC_DE_PULSE_FUN_EN		//���øĺ꣬��ʾ���� "ADC���뿹����"
//#define ADC_DE_GLITCH_FUN_EN		//���øĺ꣬��ʾ���� "ADC���뿹����"

//--- �����ػ� ---
//#define KEY_PRESS_PD_FUN_EN			//���øú꣬��ʾ���� "�����ػ�����"
#define SDI5219_HALF_SPEED			//SDI5219����1/2��Ƶ����
#define SDI5219_CAL_DSP_UPDATA_SLOW			//SDI5219 �궨״̬�� ˢ����ʾ Ƶ�ʽ��� "������߱궨�ͷǱ궨ʱ��һ�£���������"

//�¶ȼ��
//�¶Ȳ���
//ʱ����ʾ

//--- �ڲ��������� ---(ͨ������Ҫ�䶯)---
#define INTER_WEIGHT_LOCK			//���øĺ꣬��ʾ���� "�ڲ���������"
//-ǿ�ƹ���-
#define FORCE_WEIGHT_ZERO			//���øĺ꣬��ʾ���� "ǿ�ƹ���"
//#define FORCE_UP_ZERO				//ע�⣺�ĺ���Ҫ��ǿ�ƹ���һ����������Ч�����øĺ꣬��ʾ���� "ǿ�ƹ���ʱ��ǿһ��ʱ���׷��"

//--��������: ���α궨 -- begin --
	//("3�α궨�������ر����Բ���")
#ifndef THREE_CAL_FUN_EN
	#define W_LINE_CAL_EN				//���øĺ꣬��ʾ���� "����������"	
#endif
//--��������: ���α궨 -- end --

//------------------------------------------------------------------
//--- ѡ����ʾ���� --
//------------------------------------------------------------------

//����-"./_dsp_driver/display_driver.c" ��ʹ�õ�LCD �� LED����
//#define LCD_DRIVER_1621		
#define LED_DRIVER_1640
//#define LED_DRIVER_6932
//#define LED_DRIVER_1616

//�����Ƿ���� ��ʾλת�� ���ұ� (һ�㲻��)
#define  DSP_BIT_CHX_EN				//���øĺ꣬��ʾ���� "��ʾλת��"	

//------------------------------------------------------------------
//--- ���� �߾���ADC�����Ƶ�� --
// -- ��Ϊ�������кܶ�ʱ�䶨����ͨ�� SDI0819��������ݸ������еģ���������Ҫ�趨SDI0819�����Ƶ�ʣ�20Hz ���� 10Hz��
// -- ���� Ƶ����Ҫ ���� ��� �޸� ���о���   "(ʱ������)"   �ı���
//------------------------------------------------------------------
#define ADC_OUT_20Hz	//ʹ�� �ú궨�� ������ SDI0819 �����Ƶ�ʣ�20Hz
//#define ADC_OUT_10Hz	//ʹ�� �ú궨�� ������ SDI0819 �����Ƶ�ʣ�10Hz

//------------------------------------------------------------------
//--- ��ʾ�����ٶ� ---(ͨ������Ҫ�䶯)---
#ifdef ADC_OUT_20Hz  //ֻ��20Hz��ʱ����Ч
	//#define LOW_DSP_UPDATA				//���ڽ��͸��¹��ģ���ʾ ��������ADC���ݲŸ���һ����ʾ
#endif

//------------------------------------------------------------------
//--- ���� --
//------------------------------------------------------------------



//************************************************************************************************************************
// -- MCU IO �ӿڶ��嶨�� ���� --
//************************************************************************************************************************
//------------ ����adc�Ľӿ� ------------
//sbit DRDY_SDO = P0^0;
//sbit SCLK = P0^2;
//------------ ���÷������Ľӿ� ------------
sbit BUZZER_P = P1^5;
//sbit BUZZER_N = P1^7;
//------------ ����LED�Ľӿ� ------------
//--1640--
//sbit LED_DIN = P0^4;
//sbit LED_SCLK = P0^5;
sbit LED_DIN = P1^3;
sbit LED_SCLK = P1^4;
//--6932--
//sbit LED_DIN = P0^3;
//sbit LED_SCLK = P0^5;
//sbit LED_STB = P1^1;
//------------ ����LCD�Ľӿ� ------------
//sbit LCD_DATA = P0^3;
//sbit LCD_WR_N = P0^4;
//sbit LCD_CS_N = P0^5;
sbit LCD_DATA = P0^3;
sbit LCD_WR_N = P1^3;
sbit LCD_CS_N = P1^4;
//------------ ����LCD-����Ľӿ� ------------
sbit LCD_LAMP = P1^5;
EXTERN bit Lcd_lamp_symbol;		//���� ȷ��Һ��������Ƿ���
EXTERN unsigned char Lcd_bright;	//��ʾ���ȱ���
#define  LCD_LAMP_B4		0xff	//�������(����Ϊ��)
#define  LCD_LAMP_B3		0x60	//3����	
#define  LCD_LAMP_B2		0x30	//2����
#define  LCD_LAMP_B1		0x10	//1����
//#define  LCD_LAMP_ON		{LCD_LAMP = 1;Lcd_lamp_symbol = 1; PWMCON  |= 0x04;}		// (ͨ��PWM���Ʊ����)����ƴ�  (�������ܺͲ���������,�߼������Ƿ���,����귽���޸�)
//#define  LCD_LAMP_OFF		{LCD_LAMP = 0;Lcd_lamp_symbol = 0; PWMCON  &= 0xfb;}		// (ͨ��PWM���Ʊ����)����ƹر�
#define  LCD_LAMP_ON		{LCD_LAMP = 1;Lcd_lamp_symbol = 1;}		// (ͨ��PWM���Ʊ����)����ƴ�  (�������ܺͲ���������,�߼������Ƿ���,����귽���޸�)
#define  LCD_LAMP_OFF		{LCD_LAMP = 0;Lcd_lamp_symbol = 0;}		// (ͨ��PWM���Ʊ����)����ƹر�
//------------ ���� ���������Ľӿ� ------------
sbit CHARGE_SYMBOL = P1^5;
//------------ ���� ��ص�ѹ���Ľӿ� ------------
sbit BAT_DET_PIN = P1^6;
#define  BAT_DET_PIN_HIGH		(BAT_DET_PIN = 1)		// ��ѹ��� ���� ���
#define  BAT_DET_PIN_LOW		(BAT_DET_PIN = 0)		// ��ѹ��� ���� ���
//------------ ���� ���̵Ľӿ� ------------
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

//------------ IO�ڲ���(�궨��) ------------
//�ؼ��ܽŵ�IO����
//-- P1^5(CHARGE_SYMBOL)		���缫��· 
//-- P1^6(BAT_DET_PIN)			������
//-- P1^4(LCD_LAMP) 			ǿ����
//-- P1^3(BUZZER_P) 			ǿ����
//#define  IO_MODE_RESTORE_A_EN {P1M0 |= 0x38;P1M0 &= 0xbf;P1M1 &= 0xe7;P1M1 |= 0x60;};
#define  IO_MODE_RESTORE_A_EN {P1M0 &= 0xbf;P1M1 |= 0x40;};

//������ʾIO���� //P0.3(LCD_DATA-KEY_PIN9) P0.4(LCD_WR_N) P0.5(LCD_CS_N) ǿ����;
//#define  IO_MODE_DSP_EN	{P0M0 |= 0x38;P0M1 &= 0xc7;}
#define  IO_MODE_DSP_EN	{P0M0 |= 0x18;P0M1 &= 0xe7;}	
//�ر���ʾIO,�趨��ʾIO //
#define  IO_MODE_DSP_DIS {LCD_CS_N = 1;LCD_WR_N = 1;LCD_DATA = 1;}
//#define  IO_MODE_DSP_DIS {LED_DIN = 1;LED_SCLK = 1;}	

//���� ����IO����(������ⲿ��) // P1^1(KEY_PIN1),P0^1(KEY_PIN2),P1^2(KEY_PIN3),P0^0(KEY_PIN4) ��ͨ����;	
#define  IO_MODE_KEY_A_EN	 {P0M0 &= 0xfc;P0M1 &= 0xfc;P1M0 &= 0xf9;P1M1 &= 0xf9;}	
// KEY_PIN5-KEY_PIN10 ǿ����	
#define  IO_MODE_KEY_B_EN	 {P0M0 |= 0x38;P0M1 &= 0xc7;P1M0 |= 0x28;P1M1 &= 0xd7;}	
//�ر� ����IO // KEY_PIN1-KEY_PIN10����Ϊ��ģ������,���1; 
//����: (CHARGE_SYMBOL���缫��·) (BUZZER_P��������Ϊ1��ģ������)  (����ʾ���õ��߲�������Ϊģ������)
#define  IO_MODE_KEY_DIS {P0M0 &= 0xfc;P0M1 |= 0x03;P1M0 &= 0xf9;P1M1 |= 0x06;KEY_PIN1 = 1;KEY_PIN3 = 1;KEY_PIN5 = 1;KEY_PIN7 = 1;}	
//---����SG-ADC�Ĺ���ģʽ--
//--5219--
//#define SGADCON_DEFAULT	0xc9	//���� "SGADCON" ��Ĭ��ֵ
#define SGADCON_DEFAULT	0xc9	//���� "SGADCON" ��Ĭ��ֵ
#define PD_CON_DEFAULT  0x7e	//���� "PD_CON" ��Ĭ��ֵ (����P2 ����sgadcon��sgadcon2)
//--5216--
//#define SGADCON_DEFAULT	0x34	//(�ر�SOC-SGADC,�¶�ͨ��,160Hz)���� "SGADCON" ��Ĭ��ֵ
//#define PD_CON_DEFAULT  0x18	//(����sgadcon��sgadcon2)(�ر�ALDO,�ر��¶�)���� "PD_CON" ��Ĭ��ֵ 
//************************************************************************************************************************
// -- ϵͳ���ñ��� --
// -- EEPROM �ռ�  --
//************************************************************************************************************************
//--- EEPROM �鶨�� --
#define ADDR_CAL_DATA 0x6800		// (26��)���ڴ�� ���ı궨����
#define ADDR_CAL_BACKUP 0x6c00		// (27��) �������� ���ı궨���� ���� 
#define ADDR_SYS_DATA 0x7000		// (28��) �������ڴ��   ϵͳ���� ������Ϣ ��
#define ADDR_PRICE_DATA_A 0x7400	// (29��) ���ڴ�ż۸����� PRICE_M1 - PRICE_M8

//------------------------------------------------------------------
//--- EEPROM �ռ� -- 
//--��������: ���α궨 -- end --
#define  FLASH_DATA_LENTH_INIT 21				// flash ���ݵ� �ֽ���

//--��������: ���α궨 -- begin --
#ifdef THREE_CAL_FUN_EN
	#define  FLASH_DATA_LENTH (FLASH_DATA_LENTH_INIT + 4)
#else
	#define  FLASH_DATA_LENTH FLASH_DATA_LENTH_INIT
#endif

//--- 
//�ڲ�24���ʱ����Follow_zero_d�� ��Cal_d�� ������ó� ��int��
//--
union	eedata_pattern_a	//����������,���ݿ��Բ����ֽں������ַ�ʽ���ʣ�
{
	struct
	{  
		unsigned char Sys_seting_1;									//	����1 +��ϵͳ��־
		unsigned long Cal_point_code;								//	�궨����ֵ = (�궨��ADC���� - ���ADC����)
	//--��������: ���α궨 -- begin --
	#ifdef THREE_CAL_FUN_EN
		int	Cal_delta_1;		//���ԶȲ���1
		int Cal_delta_2;		//���ԶȲ���2
	#endif
	//--��������: ���α궨 -- end --
		unsigned int Follow_zero_d;//(�ڲ�24���ʱ��������int) 	// ��������׷���С
		unsigned char Follow_zero_num;								// ����׷��ķ�Χ�� 0.5d�Ķ��ٱ���
		unsigned int Cal_d; //(�ڲ�24���ʱ��������int)			//��ʾ�ֶ� d(�ڲ�����)
		unsigned int Cal_max_weight; 								//����������
		unsigned char Intel_cal_d; 									//����1���ֶȶ�Ӧ�������С
		unsigned long Cal_inter_weight;								//---�궨���� (�ڲ�)����--------
		unsigned char Sys_seting_2;									//	ϵͳ��־
		unsigned char TAPE_NUM;  									//ȥƤ�ٷֱ�
		unsigned char ZERO_NUM;  									//����ٷֱ�
		unsigned char Sys_seting_3;									//	ϵͳ��־
	} ee_struct;
	unsigned char b[FLASH_DATA_LENTH];
};
EXTERN union eedata_pattern_a idata Core_data;

//------------------------------------------------------------------
//--- ϵͳ���ñ��� -- (˵��) --
//Sys_seting_1; 
		///��0,1,2λ:	(WEIGHT_POINT: = 000 ��ʾ��������ʾ0����; = 001 ��ʾ1����; = 010 ��ʾ2����; = 011 ��ʾ3����; = 100 ��ʾ4����) 
		//��3λ: 		(DISPLAY_MODE: = 0 ��ʾ��ʾһ��"0";   = 1 ��ʾ��ʾһ��"o";)
 		//��4��5��6��7λ Ϊ: 0110 ��ʾ ϵͳ ���趨�Ѿ�д�� ���ֵ
//Sys_seting_2; 	
		//��0,1λ 	(DSP_SPEED ���� ��ʾ 4�� ��ʾ �ٶ�)
		//��2,3,4λ (GLITCH_REMOVE_BIT ������ʾ �����˲� ģʽ)
		//��5,6λ   ()
		//��7λ     (INIT_ZERO_SET = 1 ������ʾ ������������)
//Sys_seting_3; 
		//��0λ     (POWER_6V_BIT ������ʾ6V���� POWER_4V_BIT������ʾ4V����)
        //��2,3λ   (POWER_MODE ����ʾ��������)

#define  SYS_SETING_1_DEFAULT	(Core_data.ee_struct.Sys_seting_1 = 0x03);
#define  SYS_SETING_2_DEFAULT	(Core_data.ee_struct.Sys_seting_2 = 0x01);
#define  SYS_SETING_3_DEFAULT	(Core_data.ee_struct.Sys_seting_3 = 0x04);
//-- ����С���� ��־λ --- SYSTEM ---(����λ�������ֽڵ����λ������ֵ�����)	
#define  WEIGHT_POINT_LOAD		(Core_data.ee_struct.Sys_seting_1)						// ��ֵ
#define  WEIGHT_POINT_NUM		(Core_data.ee_struct.Sys_seting_1 & 0x07)				// ��ֵ
#define  WEIGHT_POINT_NUM_ADD	(Core_data.ee_struct.Sys_seting_1++)					// ++����
#define  WEIGHT_POINT_NUM_DEC	(Core_data.ee_struct.Sys_seting_1--)					// --����
#define  WEIGHT_POINT_FULL		(WEIGHT_POINT_NUM >= 4)									// �ﵽ���ֵ
#define  WEIGHT_POINT_ZERO		(WEIGHT_POINT_NUM == 0)									// �ﵽ���ֵ
#define  WEIGHT_POINT_CLR		(Core_data.ee_struct.Sys_seting_1 &= 0xf8)				// �趨Ϊ 0λ
#define  WEIGHT_POINT_1_SET		(Core_data.ee_struct.Sys_seting_1 |= 0x01)				// �趨Ϊ 1λ
#define  WEIGHT_POINT_2_SET		(Core_data.ee_struct.Sys_seting_1 |= 0x02)				// �趨Ϊ 2λ
#define  WEIGHT_POINT_3_SET		(Core_data.ee_struct.Sys_seting_1 |= 0x03)				// �趨Ϊ 3λ
#define  WEIGHT_POINT_4_SET		(Core_data.ee_struct.Sys_seting_1 |= 0x04)				// �趨Ϊ 4λ

//--����������ʾ��˸��ʶ--------
#define  DISPLAY_MODE_1         ((Core_data.ee_struct.Sys_seting_1 & 0x08) == 0x00)     // ��ʾ"0"
#define  DISPLAY_MODE_2         ((Core_data.ee_struct.Sys_seting_1 & 0x08) == 0x08)     // ��ʾ"o"
#define  DISPLAY_MODE_CLK		(Core_data.ee_struct.Sys_seting_1 &= 0xf7)
#define  DISPLAY_MODE_SET		(Core_data.ee_struct.Sys_seting_1 |= 0x08)

#define  SYSTEM_CAL_SYMBOL		((Core_data.ee_struct.Sys_seting_1 & 0xf0) == 0x60)		//ϵͳ�趨��
#define  SYSTEM_CAL_SYMBOL_CLR	(Core_data.ee_struct.Sys_seting_1 &= 0x0f)				//ϵͳ�趨
#define  SYSTEM_CAL_SYMBOL_SET	(Core_data.ee_struct.Sys_seting_1 |= 0x60)				//ϵͳ�趨

//-- ��ʾ�ٶ� ��־λ --- USER ---	
#define  DSP_SPEED_LOAD			(Core_data.ee_struct.Sys_seting_2)						// ��ֵ
#define  DSP_SPEED_NUM			(Core_data.ee_struct.Sys_seting_2 & 0x03)				// ��ֵ
#define  DSP_SPEED_NUM_ADD		(Core_data.ee_struct.Sys_seting_2 ++)				// ++����
#define  DSP_SPEED_NUM_DEC		(Core_data.ee_struct.Sys_seting_2 --)				// --����
#define  DSP_SPEED_FULL			(DSP_SPEED_NUM >= 0x02)									// �ﵽ���ֵ
#define  DSP_SPEED_ZERO			(DSP_SPEED_NUM == 0x00)									// �ﵽ��Сֵ
#define  DSP_SPEED_CLR			(Core_data.ee_struct.Sys_seting_2 &= 0xfc)				// �趨Ϊ 0λ
#define  DSP_SPEED_0_SET		(Core_data.ee_struct.Sys_seting_2 &= 0xfc)				// �趨Ϊ 1λ
#define  DSP_SPEED_1_SET		(Core_data.ee_struct.Sys_seting_2 |= 0x01)				// �趨Ϊ 2λ
#define  DSP_SPEED_2_SET		(Core_data.ee_struct.Sys_seting_2 |= 0x02)				// �趨Ϊ 3λ
#define  DSP_SPEED_0			((Core_data.ee_struct.Sys_seting_2 & 0x03) == 0x00)		
#define  DSP_SPEED_1			((Core_data.ee_struct.Sys_seting_2 & 0x03) == 0x01)	
#define  DSP_SPEED_2			((Core_data.ee_struct.Sys_seting_2 & 0x03) == 0x02)	

//-- ��������־λ ��Ҫ���� --
#define  GLITCH_REMOVE_BIT_EN	((Core_data.ee_struct.Sys_seting_2 & 0x1C) != 0x00)		// ��־�ж� (= 1��ʾ ����)
#define  GLITCH_REMOVE_BIT_DIS	((Core_data.ee_struct.Sys_seting_2 & 0x1C) == 0x00)		// ��־�ж� (= 1��ʾ �ر�)
#define  GLITCH_REMOVE_MODE0	((Core_data.ee_struct.Sys_seting_2 & 0x1C) == 0x04)		// ��־�ж� ()
#define  GLITCH_REMOVE_MODE1	((Core_data.ee_struct.Sys_seting_2 & 0x1C) == 0x08)		// ��־�ж� ()
#define  GLITCH_REMOVE_MODE2	((Core_data.ee_struct.Sys_seting_2 & 0x1C) == 0x0c)		// ��־�ж� ()
#define  GLITCH_REMOVE_MODEA	((Core_data.ee_struct.Sys_seting_2 & 0x1C) == 0x10)		// ��־�ж� ()
#define  GLITCH_REMOVE_BIT_CLR	(Core_data.ee_struct.Sys_seting_2 &= 0xe3)				// �ر�
#define  GLITCH_REMOVE_MODE0_SET 	{Core_data.ee_struct.Sys_seting_2 &= 0xe3;Core_data.ee_struct.Sys_seting_2 |= 0x04;}				// ��
#define  GLITCH_REMOVE_MODE1_SET	{Core_data.ee_struct.Sys_seting_2 &= 0xe3;Core_data.ee_struct.Sys_seting_2 |= 0x08;}				// ��־λȡ��
#define  GLITCH_REMOVE_MODE2_SET	{Core_data.ee_struct.Sys_seting_2 &= 0xe3;Core_data.ee_struct.Sys_seting_2 |= 0x0c;}
#define  GLITCH_REMOVE_MODEA_SET	{Core_data.ee_struct.Sys_seting_2 &= 0xe3;Core_data.ee_struct.Sys_seting_2 |= 0x10;}
#define  GLITCH_REMOVE_BIT_ADD	(Core_data.ee_struct.Sys_seting_2 += 0x04)
#define  GLITCH_REMOVE_BIT_NUM	(Core_data.ee_struct.Sys_seting_2 & 0x1C)				// ��־�ж� ()
#define  GLITCH_REMOVE_BIT_FULL	(GLITCH_REMOVE_BIT_NUM >= 0x10)							// ��־�ж� ()

//-- ������������ ��־λ --- USER ---
#define  INIT_ZERO_SET_EN		((Core_data.ee_struct.Sys_seting_2 & 0x80) == 0x80)		// ������������
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
//---������� ��Ҫ ���� �䶯�� �û����� --- 

//EXTERN unsigned char User_seting_1;
//--- Xdata ---
//--- SYS DATA ��ض��� ---
#define  SYS_DATA_LENTH 21		// SYS ���ݵ� �ֽ���
union	SYS_pattern	//����������,���ݿ��Բ����ֽں������ַ�ʽ���ʣ�
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
EXTERN union SYS_pattern xdata System_data;									//	����1
//---������� ��Ҫ ���� �䶯�� ���� --- 
//User_seting_1; 	
				//��0,1λ:	(WARRING_TYPE: = 00 ������; = 01 ��Χ�ڱ���; = 10 ��Χ�ⱨ��; = 11 ��Χ�ⱨ��)
				//��2λ 	(WARRING_COUNT: = 0 �������� = 1��������)
				//��3λ		(WARRING_SET: = 1 �趨������ֵ)	
 				//��4λ		(COUNT_SET: = 1 �趨�� ����)
				//��5λ		(PERCENT_SET: = 1 �趨�� �ٷֱ�)
				//��6,7λ	(LCD_LAMP_MODE: = 00 ����ƹر� 01����ƴ� 02������Զ�)

#define  USER_SETING_1_DEFAULT	(0x80)

//-- ��������--��ʽ ��־λ --- USER ---(����λ�������ֽڵ����λ������ֵ�����)	
#define  WARRING_TYPE_LOAD		(System_data.sys_struct.User_seting_1)						// ��ֵ
#define  WARRING_TYPE_NUM		(System_data.sys_struct.User_seting_1 & 0x03)				// ��ֵ
#define  WARRING_TYPE_NUM_ADD	(System_data.sys_struct.User_seting_1++)					// ++����
#define  WARRING_TYPE_NUM_DEC	(System_data.sys_struct.User_seting_1--)					// --����
#define  WARRING_TYPE_NO		(WARRING_TYPE_NUM == 0x00)			// 
#define  WARRING_TYPE_IN		(WARRING_TYPE_NUM == 0x01)			// 
#define  WARRING_TYPE_OUT		(WARRING_TYPE_NUM == 0x02)			// 
#define  WARRING_TYPE_FULL		(WARRING_TYPE_NUM >= 2)				// �ﵽ���ֵ
#define  WARRING_TYPE_ZERO		(WARRING_TYPE_NUM == 0)				// �ﵽ��Сֵ
#define  WARRING_TYPE_NO_SET	(System_data.sys_struct.User_seting_1 &= 0xfc)				// �趨Ϊ
#define  WARRING_TYPE_IN_SET	(System_data.sys_struct.User_seting_1 |= 0x01)				// �趨Ϊ
#define  WARRING_TYPE_OUT_SET	(System_data.sys_struct.User_seting_1 |= 0x02)				// �趨Ϊ 

//-- �������� ��־λ --- USER ---
#define  WARRING_COUNT_EN		((System_data.sys_struct.User_seting_1 & 0x04) == 0x04)		// ��������
#define  WARRING_COUNT_DIS		((System_data.sys_struct.User_seting_1 & 0x04) == 0x00)		// ��������
#define  WARRING_COUNT_CLR		(System_data.sys_struct.User_seting_1 &= 0xfb)				// �趨Ϊ 0
#define  WARRING_COUNT_SET		(System_data.sys_struct.User_seting_1 |= 0x04)				// �趨Ϊ 1
#define  WARRING_COUNT_XOR		(System_data.sys_struct.User_seting_1 ^= 0x04)				// ��־λȡ��

//-- �����趨�� ��־λ --- USER ---
#define  WARRING_SET_EN			((System_data.sys_struct.User_seting_1 & 0x08) == 0x08)	// �趨������
#define  WARRING_SET_DIS		((System_data.sys_struct.User_seting_1 & 0x08) == 0x00)	// û��
#define  WARRING_SET_CLR		(System_data.sys_struct.User_seting_1 &= 0xf7)				// 
#define  WARRING_SET_SET		(System_data.sys_struct.User_seting_1 |= 0x08)				// 
#define  WARRING_SET_XOR		(System_data.sys_struct.User_seting_1 ^= 0x08)				// 

//-- �����趨�� ��־λ --- USER ---
#define  COUNT_SET_EN			((System_data.sys_struct.User_seting_1 & 0x10) == 0x10)		// �趨������
#define  COUNT_SET_DIS			((System_data.sys_struct.User_seting_1 & 0x10) == 0x00)		// û�� �趨������
#define  COUNT_SET_CLR			(System_data.sys_struct.User_seting_1 &= 0xef)				// 
#define  COUNT_SET_SET			(System_data.sys_struct.User_seting_1 |= 0x10)				// 
#define  COUNT_SET_XOR			(System_data.sys_struct.User_seting_1 ^= 0x10)				// 

//-- �ٷֱ��趨�� ��־λ --- USER ---
#define  PERCENT_SET_EN			((System_data.sys_struct.User_seting_1 & 0x20) == 0x20)		// �趨���ٷֱ�
#define  PERCENT_SET_DIS		((System_data.sys_struct.User_seting_1 & 0x20) == 0x00)		// û�� �趨���ٷֱ�
#define  PERCENT_SET_CLR		(System_data.sys_struct.User_seting_1 &= 0xdf)				// 
#define  PERCENT_SET_SET		(System_data.sys_struct.User_seting_1 |= 0x20)				// 
#define  PERCENT_SET_XOR		(System_data.sys_struct.User_seting_1 ^= 0x20)				// 

//-- �����--��ʽ ��־λ --- USER ---(����λ�������ֽڵ����λ������ֵ�����)	
#define  LCD_LAMP_MODE_LOAD		(System_data.sys_struct.User_seting_1)						// ��ֵ
#define  LCD_LAMP_MODE_NUM		(System_data.sys_struct.User_seting_1 & 0xc0)				// ��ֵ
#define  LCD_LAMP_MODE_NUM_ADD	(System_data.sys_struct.User_seting_1 += 0x40)				// ++����
#define  LCD_LAMP_MODE_NUM_DEC	(System_data.sys_struct.User_seting_1 -= 0x40)				// --����
#define  LCD_LAMP_OFF_MODE		(LCD_LAMP_MODE_NUM == 0x00)		// �趨Ϊ 
#define  LCD_LAMP_ON_MODE		(LCD_LAMP_MODE_NUM == 0x40)			// �趨Ϊ 
#define  LCD_LAMP_AUTO_MODE		(LCD_LAMP_MODE_NUM == 0x80)			// �趨Ϊ 
#define  LCD_LAMP_MODE_FULL		(LCD_LAMP_MODE_NUM >= 0x80)			// �ﵽ���ֵ
#define  LCD_LAMP_MODE_ZERO		(LCD_LAMP_MODE_NUM == 0x00)			// �ﵽ��Сֵ
#define  LCD_LAMP_OFF_SET		(System_data.sys_struct.User_seting_1 &= 0x3f)				// �趨Ϊ 
#define  LCD_LAMP_ON_SET		(System_data.sys_struct.User_seting_1 |= 0x40)				// �趨Ϊ 
#define  LCD_LAMP_AUTO_SET		(System_data.sys_struct.User_seting_1 |= 0x80)				// �趨Ϊ 


//*************************************************************************//
//----------  ʱ����� (�궨��) (����)-- (ʱ������) ------ 
//*************************************************************************//

//------------------------------------------------------------------
//--- (ȡ��λ-�궨��)��ʱ���ȡADC���������  -- (ʱ������) ---
#ifdef ADC_OUT_20Hz
	#define LONG_FILTER_NUM 10			// (ʱ������)��ȡADC���ȶ�����
	#define LONG_FILTER_NUM_2 20		// (ʱ������)��ȡADC���ȶ�����
	#define LONG_FILTER_ACCUM_NUM 40	// (ʱ������)�ۼƶ�ȡADC�ĸ���
#endif
#ifdef ADC_OUT_10Hz
	#define LONG_FILTER_NUM 5			// (ʱ������)��ȡADC���ȶ�����
	#define LONG_FILTER_NUM_2 10		// (ʱ������)��ȡADC���ȶ�����
	#define LONG_FILTER_ACCUM_NUM 20	// (ʱ������)�ۼƶ�ȡADC�ĸ���
#endif

//------------------------------------------------------------------
//-- "�����ȶ�������" ʱ�����  		-- (ʱ������) ---
//-- "��ѹ��ʾ" ʱ�����  				-- (ʱ������) ---
//-- "���ȵ���ʾ�ַ�����ʾ" ʱ�����  	-- (ʱ������) ---
EXTERN unsigned char Weight_stable_counter;  //�����ȶ����������������ȴ���Ϊ"Weight_stable_num"ʱ��LED���"������"��"�����"������ʾ��
EXTERN bit Weight_sleep; 					// ==1 ����������ģʽ�� ��ʾ "-"; == 0 ����ʾ
EXTERN bit Sleep_dsp_mode; 					// == 1 LED ���������ʾ
EXTERN bit Vol_state; 						// ==1 ��ʾ��Ҫ��ʾ��ص�ѹ
//EXTERN bit Lp_dsp_symbol; 				// = 1 ������ʾ������ʾ

//-- ���� ������� ���� �����ȶ�ʱ��LED��� ������ʾʱ��
#ifdef ADC_OUT_20Hz
	#define WEIGHT_ZERO_NUM 50			// (ʱ������)"��� ������ʾʱ��"��20Hz(2.5s)
	#define WEIGHT_STABLE_NUM 200		// (ʱ������)"����� ������ʾʱ��"��20Hz(10s)
	#define WEIGHT_SLEEP_NUM 8			// (ʱ������)"����ʱ ����������ʱ��"��20Hz(0.4s)
	#define VOL_DSP_NUM 40				// (ʱ������)"��ѹ��ʾ��ʱ��"��20Hz(2s)
	#define LP_DSP_NUM 10				// (ʱ������)"��ѹ��ʾ��ʱ��"��20Hz(0.5s)
#endif
#ifdef ADC_OUT_10Hz
	#define WEIGHT_ZERO_NUM 25			// (ʱ������)"��� ������ʾʱ��"��10Hz(2.5s)
	#define WEIGHT_STABLE_NUM 100		// (ʱ������)"����� ������ʾʱ��"��10Hz(10s)
	#define WEIGHT_SLEEP_NUM 4			// (ʱ������)"����ʱ ����������ʱ��"��10Hz(0.4s)
	#define VOL_DSP_NUM 20				// (ʱ������)"��ѹ��ʾ��ʱ��"��10Hz(2s)
	#define LP_DSP_NUM 5				// (ʱ������)"��ѹ��ʾ��ʱ��"��10Hz(0.5s)
#endif

//-----------------------------------------
//-- ���������� ʱ�����  -- (ʱ������) ---
EXTERN unsigned char Warring_counter;  // ���������� ������
EXTERN bit Lamp_lowv_a; // ��ѹ���ͣ����Ʒ���������
EXTERN bit Lamp_lowv; // ��ѹ���ͣ���ʾ LB ����Ϊ0 �ҷ�����ʱ����
//--
#ifdef ADC_OUT_20Hz	
	#define WARRING_NUM_LOWV_1 250		//(ʱ������)
	#define WARRING_NUM_LOWV_2 20		//(ʱ������)
	#define WARRING_NUM_FULL 4			//(ʱ������)
	//---
	#define WARRING_NUM_HIGH 2			//(ʱ������)
	#define WARRING_NUM_LOW 6			//(ʱ������)
	#define WARRING_NUM_FULL 4			//(ʱ������)
#endif
#ifdef ADC_OUT_10Hz
	#define WARRING_NUM_LOWV_1 125		//(ʱ������)
	#define WARRING_NUM_LOWV_2 10		//(ʱ������)
	#define WARRING_NUM_FULL 2			//(ʱ������)
	//---
	#define WARRING_NUM_HIGH 1			//(ʱ������)
	#define WARRING_NUM_LOW 3			//(ʱ������)
	#define WARRING_NUM_FULL 2			//(ʱ������)
#endif

//-----------------------------------------
//-- ��ʾ�����ȶ� ʱ�����  -- (ʱ������) ---
EXTERN unsigned char Stable_light_counter;
EXTERN bit Stable_light_symbol; 			// =1 ��ʾ��ʾ�����ȶ�
//--
#ifdef ADC_OUT_20Hz	
	#define STABLE_LIGHT_NUM 15				//(ʱ������)
#endif
#ifdef ADC_OUT_10Hz
	#define STABLE_LIGHT_NUM 7				//(ʱ������)
#endif

//-----------------------------------------
//-- ��ص�ѹ��� ʱ�����  -- (ʱ������) ---
EXTERN unsigned char Vol_auto_det_counter;
EXTERN bit Power_det_mode; 					//== 1 ��ʾ ����ѹʱ�����ر���ʾ
//--
#ifdef ADC_OUT_20Hz	
	#define VOL_AUTO_DET_NUM 240			//(ʱ������)
#endif
#ifdef ADC_OUT_10Hz
	#define VOL_AUTO_DET_NUM 120			//(ʱ������)
#endif

//-----------------------------------------
//-- ��̬��� ���� ʱ�����  -- (ʱ������) ---
EXTERN bit Charte_bit; // = 1 ��ʾ ���ӽ�������
EXTERN bit Charte_bit_dis; // = 1 ��ʾ ���ӽ������������
EXTERN unsigned char idata Charge_counter; // ��̬��������
//--
#ifdef ADC_OUT_20Hz	
	#define CHARGE_COUNTER_NUM 10			//(ʱ������)
#endif
#ifdef ADC_OUT_10Hz
	#define CHARGE_COUNTER_NUM 5			//(ʱ������)
#endif

//-----------------------------------------
//-- ��ʱ�䰴������ ʱ�����  -- (ʱ������) ---
//-- ͨ����Ƭ����ʱ�����е�ʱ����� 
//#define Press_key_counter_num 15
#define PRESS_KEY_COUNTER_NUM 0xe0
#define PRESS_KEY_COUNTER_NUM_2 0x30
//------------------------
//--�� stable_light_counter �� Press_key_counter �ϲ�
//--Press_key_counter ʹ�� stable_light_counter �� ��4λ
//EXTERN unsigned char Press_key_counter; 
EXTERN bit Key_counter_en; 					//== 1 ��ʾ��Ҫ��ⳤʱ�䰴��

//*************************************************************************//
//----------  �������ر��� ------ 
//*************************************************************************//

//------------------------------------------------------------------
//--- ADC ���� ��� --
EXTERN bit ADready;									// ==1 ��ʾ ADC �����ݸ���
EXTERN unsigned long idata	ADbuf[4];				//���� ADC ����ĵ�һ�� COMB^3(2)���˲�����;
EXTERN unsigned long idata	ADcode;					//�����˲��������������������� ADC����
EXTERN unsigned long idata	Zero_code;				//��������
EXTERN bit Sign_code; 								//����ķ���(���ڻ���С����λ����)
EXTERN bit Code_zero; 								// ���봦�����  CODE_ZERO �� WEIGHT_ZERO ������ ���ں���Ҫ����TAPE(ȥƤ)
EXTERN bit Cal_zero_en; 							// == 1 ��ʾ��Ҫ�����������
EXTERN bit ADCcmd;

//------------------------------------------------------------------
//--- ADC ���� ƽ��ȡ������ --
EXTERN bit Ave_step_en; 	// = 1 ��ʾ��Ҫȡ����һ������ʼȡ��
													// == 0 ��ʾ��Ҫ����һ������ʱ���ܽ��� ƽ��ȡ������
//--ƽ��ȡ�� ǰ�涪����ADC����
//-- ע�� -- : AVE_DIS_NUM ����С�� LONG_FILTER_ACCUM_NUM
#ifdef ADC_OUT_20Hz	
	#define AVE_DIS_NUM	10
#endif
#ifdef ADC_OUT_10Hz 
	#define AVE_DIS_NUM	5
#endif


//--- ADC ���� ������ --
#ifdef ADC_DE_PULSE_FUN_EN
	EXTERN unsigned char Ad_depulse_count_1;
#endif
//--- ADC ���� ȥ���� --
#ifdef ADC_DE_GLITCH_FUN_EN
	EXTERN unsigned char Ad_depulse_count_2;
	EXTERN bit Filter_en;
#endif
//------------------------------------------------------------------
//--- �ڲ����� --
EXTERN unsigned long idata	Weight_internal_last;			//�ڲ�����(û�н����¶ȵȲ���ǰ���ڲ�����)
EXTERN int Zero_weight_accum;								//��¼�ۼ� ����׷��
EXTERN bit Startup_zero_symbol;								//����ǿ������׷�㷶Χ
//------------------------------------------------------------------
//--- ���ر��� --
EXTERN unsigned long idata	Weight;				//��ʾ����
EXTERN unsigned long xdata	Tape_weight;		//Ƥ�ص�����
EXTERN bit Weight_zero;							//��ʾ����������־λ,����LED���"������"������ʾ��
EXTERN bit Sign;								//���������ķ���λ
EXTERN bit Weight_overflow; 					// �������� ������ �� 1Hz ��Ƶ�ʱ���
EXTERN bit Tape_en; 							// ==1 ��ʾ��Ҫ����ȥƤ����

//------------------------------------------------------------------
// ���ر������ޱ���
//Ϊ�˼��ݼƼ۳ӵ�"filter_cal_Vc"�ļ���
//��ԭ��Weight_HH��Money���������,��ԭ��Weight_LL��Money_accum.w�����������
EXTERN unsigned long idata Money; 			//Weight_HH;	//���ر�������
EXTERN union  ADpattern idata Money_accum; 	//Weight_LL;	//���ر�������

//------------------------------------------------------------------
//---�������� --
//EXTERN unsigned long idata Counter_sample_weight;
//EXTERN unsigned int idata Counter_sample_num;

//--ƽ��ȡ�� ǰ�涪���� �ڲ������ĸ���
//-- ע�� -- : AVE_IW_DIS_NUM ����С�� AVE_IW_ACCUM_NUM
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
//---�ٷֱȹ��� --
//#ifdef PERCENTAGE_FUN_EN
//	EXTERN unsigned long idata Per_sap_w;	//�ٷֱ�ȡ������;
//#endif


//*************************************************************************//
//----------  ���� ���� ���� ------ 
//*************************************************************************//

//------------------------------------------------------------------
//--- У�����Ʊ��� --
EXTERN bit Data_error; 								// ���ı궨���ݴ����־λ: == 1 ��ʾ �������ݴ���
EXTERN bit Weight_updata_en; 						//����궨������ʱ = 1,Ҫ����������,������������
EXTERN bit Start_up_end;						//==0 ��ʾ���ڿ������裬==1��ʾ��������
EXTERN bit Save_user_data; 						//== 1 ��ʾ��Ҫ�洢 �û�����
EXTERN bit Get_cal_data; 						//== 1 ��ʾ��Ҫ ִ�б궨��������
//------------------------------------------------------------------
//-- ���ؼ��� ���� --
EXTERN unsigned char idata Active_mode;

//Active_mode ������־ �� �Ĳ�ͬ״̬ : ֱ�ӹ�ϵ �Ƶ���ʾ �� ������Ӧ
// bit7 - bit6: 
// 00:	����ģʽ
// 01:	����ģʽ
// 10:  �ٷֱ�ģʽ
// bit5:
//	 	== 1: �궨
// bit4 - bit0: ��32��״̬,�ֱ����ڸ���ģʽ����Ĳ���

//����ģʽ = 0 ���� = 1 ���� = 2 �ٷֱ�
// 10 - 29 ���� �궨 ģʽ
// 30 - 49 ���� �����趨
// 50 - 69 ���� �����趨
// 40 - 89 ���� �ٷֱ��趨


//------------------------------------------------------------------
// ��ͷ���� �������
//A_input_num,Ϊ�˼��ݼƼ۳ӵ�"filter_cal_Vc"�ļ�����ԭ��A_input_num��Price���������
EXTERN unsigned long idata Price;	//A_input_num
EXTERN unsigned char idata A_cycle_num;
EXTERN unsigned char idata A_input_setp;
EXTERN bit A_input_symbol;	//==1��ʾ���� ���� ��ͷ��������
EXTERN bit A_input_num_symbol;	//==1��ʾ���� ���� ������ʾ

EXTERN bit Cal_temp_bit; //== 1 ��ʾ ����ѭ��ѡ��

//------------------------------------------------------------------
// ��ʾ״̬���Ʊ���
EXTERN unsigned char idata Dsp_state_con;
//bit7: = 0 ��ʾ ����Ҫ��ʾ����
//		= 1 ��ʾ ��Ҫ��ʵ����(���ֲ�һ�����ַ�����)
//bit6: = 1 ��ʾ ��Ҫ������ʾ ���� �� �ַ�(��ʱ������ʱ������)
//bit0-bit5(64��״̬):
//������ʾ��ͬ���ַ�(dsp_pattern)
EXTERN bit Dsp_state_num_state; //		 = 0 ��ʾ ������ʾ���ַ���
								//		= 1 ��ʾ ������ʾ�����֣�

//------------------------------------------------------------------
//-- ����������־ --
EXTERN bit Cal_zero_en; 	//==1 ��ʾ��Ҫ �������� ����
EXTERN bit Cal_set_en; 		//==1 
EXTERN bit Cal_mode_en; 	//==1 
EXTERN bit Cal_unit_en; 	//==1 
EXTERN bit Cal_tape_en; 	//==1 

//------------------------------------------------------------------
//-- ϵͳ�����־ --
EXTERN bit Weight_ul_error;		//==1��ʾ����������������������
EXTERN bit Weight_warning_h; 	// = 1 ��������
EXTERN bit Weight_warning_l; 	// = 1 ��������
EXTERN bit Weight_warning_in; 	// = 1 ��������
EXTERN bit Sys_txt_warning_a; 	// = 1 ��ʾ��Ҫ ǿ����ʾ ϵͳ�� ����
EXTERN bit Sys_txt_warning_b; 	// = 1 ��ʾ��Ҫ ǿ����ʾ ϵͳ�� ���� �� һ���˳��� Press_key_num_a��
EXTERN bit Weight_on_error; 	// = 1 ��ʾ���� ��λ ����





//*************************************************************************//
//----------  ����ģ�� ���� ------ 
//*************************************************************************//

//-----------------------------------------
//-- ��ص�ѹ��ⶨ��  ---

//-- 4.3v������� ---
//#define V_LOW_FULL_H 505 	//��������-�ߣ���ѹ>=�ڴ�ֵ ��ʾ��������4.3v���磩
//#define V_LOW_FULL_L 495 	//��������-�ͣ���ѹ>=�ڴ�ֵ ��ʾ��������4.3v���磩
#define V_LOW_FULL_H 490 	//��������-�ߣ���ѹ>=�ڴ�ֵ ��ʾ��������4.3v���磩
#define V_LOW_FULL_L 480 	//��������-�ͣ���ѹ>=�ڴ�ֵ ��ʾ��������4.3v���磩
#define V_LOW_1_CH 470		
#define V_LOW_2_CH 450

#define V_LOW_0 420 		//������һ�����ޣ���ѹ>=�ڴ�ֵ ��ʾ���4��4.3v���磩
#define V_LOW_1 390 		//�����ڶ������ޣ���ѹ>=�ڴ�ֵ ��ʾ���3��4.3v���磩
#define V_LOW_2 370 		//�������������ޣ���ѹ>=�ڴ�ֵ ��ʾ���2��4.3v���磩
#define V_LOW_3 360 		//�������ĸ����ޣ���ѹ>=�ڴ�ֵ ��ʾ���1��4.3v���磩

#define V_CO_PRE  984 	//��ѹϵ�� = VDD * 3 �� ��ѹ����� 3.3,�� V_CO = 990 
						// 3 Ϊ VCCͨ�� ��ѹ����ϵ��.//��:150k / 50k =>  = 3
						// 
//#define V_CO_PRE  670 	

//#define V_HIG_FULL_H 705 //7.05V   //��ѹ>=�ڴ�ֵ ��ʾ��������6.3v���磩
//#define V_HIG_FULL_L 695 //6.95V   //��ѹ>=�ڴ�ֵ ��ʾ��������6.3v���磩
#define V_HIG_FULL_H 690 //690V   //��ѹ>=�ڴ�ֵ ��ʾ��������6.3v���磩
#define V_HIG_FULL_L 680 //6.80V   //��ѹ>=�ڴ�ֵ ��ʾ��������6.3v���磩
#define V_HIG_1_CH 670
#define V_HIG_2_CH 650

#define V_HIG_0 620 //6.20V   ////��ѹ>=�ڴ�ֵ ��ʾ���ȫ��6v���磩
#define V_HIG_1 590 //5.90V  //��ѹ>=�ڴ�ֵ ��ʾ��ص�һ��6v���磩
#define V_HIG_2 570 //5.70V  //��ѹ���ڴ�ֵ Ƿѹ���� ��������  ��ѹ>=�ڴ�ֵ ��ʾ��ؿ�6v���磩
#define V_HIG_3 560 //5.70V  //��ѹ���ڴ�ֵ Ƿѹ���� ��������  ��ѹ>=�ڴ�ֵ ��ʾ��ؿ�6v���磩

EXTERN bit	Batter_a;	// = 1 ��ʾ �������һ��
EXTERN bit	Batter_b;	// = 1 ��ʾ ����м��һ��
EXTERN bit	Batter_c;	// = 1 ��ʾ �����С��һ��


EXTERN bit	Batter_b_ch;	// = 1 ��ʾ ����м��һ��
EXTERN bit	Batter_c_ch;	// = 1 ��ʾ �����С��һ��

//-----------------------------------------
//-- �����ػ�  ---
#ifdef KEY_PRESS_PD_FUN_EN
	EXTERN bit Mcu_stop_pre; //== 1 ��ʾ׼������ػ�״̬
	EXTERN bit Mcu_stop_rec; //== 1 ��ʾ���ڹػ�״̬
#endif
//-----------------------------------------
//-- ������λ���  ---
//EXTERN bit Init_load_over; //== 1 ��ʾ ���� ��λ���ߣ�>�����̵� 15%���ң�

//-----------------------------------------
//SDI5219 �궨״̬�� ˢ����ʾ Ƶ�ʽ��� "������߱궨�ͷǱ궨ʱ��һ�£���������"
//-- �궨״̬�� ˢ����ʾ Ƶ�ʽ��� ʱ�����  -- (ʱ������) ---
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
//----------  �������⹦�� ���� ------ 
//*************************************************************************//
//EXTERN unsigned char xdata Imperial_num; //==0,���� ==1Ӣ��  ==2 VISS

EXTERN bit Imperial_bit; //==0,���� ==1Ӣ��

EXTERN bit Warning_low;

EXTERN bit Warning_high;

EXTERN bit Warning_in;

//------------------�������ñ�־λ----------------
EXTERN unsigned char idata  change_num_a;
EXTERN unsigned char idata  change_num_b;
//*************************************************************************//
//----------  ���� ���� ------ 
//*************************************************************************//

EXTERN bit Data_sycle_symbol;		//���� �����Ƿ�����������, = 1��ʾ �µ���������

//-----------------------------------------
//-- (ǿ�� ����׷�� ��־λ) ---
#ifdef FORCE_UP_ZERO
	EXTERN bit Force_up_zero_symbol;			// = 1 ǿ�� ����׷�㷶Χ
	EXTERN unsigned char Force_up_zero_counter;	// ǿ������׷�������
	#ifdef ADC_OUT_20Hz	
		#define FORCE_UP_ZERO_NUM 140			//	
	#endif
	#ifdef ADC_OUT_10Hz
		#define FORCE_UP_ZERO_NUM 70			//
	#endif
#endif

EXTERN bit Double_counter_symbol; //���ӳ�������ʱ��1��  ==1 �ſ�ʼ����

#endif

