//*************************************************************************//
//* Name				:   display_cal.c
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic ���ӳ� �Դ�����ļ�
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
// -- ȫ��ͷ�ļ� ���й��� ---- ���汾�޸��ļ�����䶯 ---  �����޸� ����ļ�
//**************************************************************************
#include "./_solidic/head_file_version.h"

//****************************************************
//----ȫ�ֱ��� Dsp_buf[]:  16�ֽ��Դ�
//----
//****************************************************

//************************************************************************************************************************
// -- ϵͳ -- �û���ʾ����  --begin--
//************************************************************************************************************************

//--- �����ַ������ұ� ---
code unsigned char 	string_pattern[][6]={	//--- Һ�� --- offset == 32
										{LCDC_HH,LCDC_HH,LCDC_H,LCDC_H,LCDC_HH,LCDC_HH},//0:--HH--(0x00)
										{LCDC_HH,LCDC_HH,LCDC_L,LCDC_L,LCDC_HH,LCDC_HH},//1:--LL--(0x01)
										{LCDC_HH,LCDC_HH,LCDC_I,LCDC_N,LCDC_HH,LCDC_HH},//2:--IN--(0x02)
										{LCDC_HH,LCDC_O,LCDC_U,LCDC_7,LCDC_HH,LCDC_HH},//3:-OUT--(0x03)
										{LCDC_HH,LCDC_HH,LCDC_N,LCDC_O,LCDC_HH,LCDC_HH},//4:--NO--(0x04)
										{LCDC_5,LCDC_A,LCDC_P,LCDC_HH,LCDC_HH,LCDC_HH},//5:SAP---(0x05)
										{LCDC_L,LCDC_O,LCDC_A,LCDC_D,LCDC_HH,LCDC_P},//6:LOAD-P(0x06)
										{LCDC_L,LCDC_O,LCDC_A,LCDC_D,LCDC_HH,LCDC_C},//7:LOAD-C(0x07)
										{LCDC_HH,LCDC_S,LCDC_L,LCDC_A,LCDC_C,LCDC_HH},//8:-SLAC-(0x08)
										{LCDC_HH,LCDC_C,LCDC_S,LCDC_L,LCDC_HH,LCDC_HH},//9:-CSL--(0x09)
										{LCDC_HH,LCDC_HH,LCDC_L,LCDC_B,LCDC_HH,LCDC_HH},//10:--LB--(0x0a)
										{LCDC_HH,LCDC_S,LCDC_Y,LCDC_S,LCDC_HH,LCDC_HH},//11:-SYS--(0x0b)
										{LCDC_D,LCDC_HH,LCDC_HH,LCDC_HH,LCDC_HH,LCDC_HH},//12:d-----(0x0c)
										{LCDC_D,LCDC_O,LCDC_7,LCDC_HH,LCDC_HH,LCDC_HH},//13:dot---(0x0d)
										{LCDC_2,LCDC_E,LCDC_R,LCDC_O,LCDC_HH,LCDC_HH},//14:zero--(0x0e)
										{LCDC_HH,LCDC_F,LCDC_U,LCDC_L,LCDC_L,LCDC_HH},//15:-FULL-(0xf)
										{LCDC_HH,LCDC_C,LCDC_A,LCDC_L,LCDC_HH,LCDC_HH},//16:-CAL--(0x10)
										{LCDC_HH,LCDC_O,LCDC_F,LCDC_F,LCDC_HH,LCDC_HH},//17:-OFF--(0x11)
										{LCDC_HH,LCDC_HH,LCDC_O,LCDC_N,LCDC_HH,LCDC_HH},//18:--ON--(0x0x12)
										{LCDC_HH,LCDC_L,LCDC_O,LCDC_A,LCDC_D,LCDC_HH},//19:-LOAD-(0x13)	
										{LCDC_D,LCDC_C,LCDC_BK,LCDC_HH,LCDC_HH,LCDC_HH},//20:DC ---(0x14)
										{LCDC_F,LCDC_I,LCDC_L,LCDC_7,LCDC_HH,LCDC_HH},//21:Filt--(0x15)
										{LCDC_D,LCDC_A,LCDC_HH,LCDC_HH,LCDC_HH,LCDC_HH},//22:Da----(0x16)
										{LCDC_D,LCDC_7,LCDC_HH,LCDC_HH,LCDC_HH,LCDC_HH},//23:Dt----(0x17)
										{LCDC_B,LCDC_HH,LCDC_HH,LCDC_HH,LCDC_O,LCDC_N},//24:B---On(0x18)
										{LCDC_B,LCDC_HH,LCDC_HH,LCDC_O,LCDC_F,LCDC_F},//25:B--OFF(0x19)
										{LCDC_B,LCDC_HH,LCDC_A,LCDC_U,LCDC_7,LCDC_O},//26:B-AUTO(0x1a)
										{LCDC_O,LCDC_N,LCDC_HH,LCDC_HH,LCDC_HH,LCDC_HH},//27:On----(0x1b)								
										{LCDC_E,LCDC_R,LCDC_R,LCDC_HH,LCDC_HH,LCDC_0},//28:ERR--0(0x1c)
										{LCDC_HH,LCDC_HH,LCDC_5,LCDC_E,LCDC_7,LCDC_HH},//29:--SET-(0x1d)
										{LCDC_H,LCDC_L,LCDC_HH,LCDC_HH,LCDC_2,LCDC_L},//30:HL--ZL(0x1e)
										{LCDC_H,LCDC_L,LCDC_HH,LCDC_P,LCDC_C,LCDC_5},//31:HL-PCS(0x1f)
										{LCDC_P,LCDC_HH,LCDC_HH,LCDC_HH,LCDC_HH,LCDC_HH}//32:P-----(0x20)
										//{LCDC_D,LCDC_A,LCDC_HH,LCDC_HH,LCDC_A,LCDC_A},//32:Da--AA(0x20)
										//{LCDC_HH,LCDC_L,LCDC_O,LCDC_A,LCDC_D,LCDC_2},//33:-LOAD2(0x21)	
										//{LCDC_HH,LCDC_L,LCDC_O,LCDC_A,LCDC_D,LCDC_3}//34:-LOAD3(0x22)
										};
//************************************************************************************************************************
// -- ϵͳ -- �û���ʾ���� --end--
//************************************************************************************************************************

//************************************************************************************************************************
// -- ϵͳ -- �Դ涨�� --begin--
//************************************************************************************************************************
//--- ������ʾ���ֲ��ұ� ---
code unsigned char 	pattern_table[]={//----offset_1 = 10----
									LCDC_0,LCDC_1,LCDC_2,LCDC_3,LCDC_4,LCDC_5,LCDC_6,LCDC_7,LCDC_8,LCDC_9,LCDC_A,LCDC_B,LCDC_C,LCDC_D,LCDC_E,LCDC_F};
									
//--- Һ����ʾ���� ת��Ϊ ������ʾ����
//--- Ext:     SEG 8  7   6   5   4   3   2   1
//--- Ext:  LCD    H  C   B   A   D   E   G   F
//--- Ext:  LED    B  G   A   F   E   H   D   C
//--- Ext:   LED[8] = LCD & Lcd2led_table[0];
//----       LED�ĵ�8ΪBΪLCD�ĵ�6λ������Ӧ���� LED[8] = LCD & 0x20; => Lcd2led_table[0] = 0x20; 
//--------------------------------------------------------
//---    SEG 8  7   6   5   4   3   2   1
//--- LCD    H  G   F   E   D   C   B   A
//--- LED    E  D   C   H   B   A   F   G
//--- Ext:   LED[8] = LCD & Lcd2led_table[0];
//code unsigned char 	Lcd2led_table[]={0x10,0x08,0x04,0x80,0x02,0x01,0x20,0x40};
code unsigned char 	Lcd2led_table[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

//-----------------------------------------------------------
//--��������: ��ʾλת�� -- begin --
#ifdef DSP_BIT_CHX_EN

//��ʾλת�� ���ұ�
//��¼��Ŀǰ��Dsp_buf[i] = ��ָ��� ���ӳ� ��ʾλ��
//���ӳ� ��ʾλ��: �ӡ��������λ��- ��������λ��Ϊ[0 - 15]
//-----
//Һ��һ���������˳����ת��
//����պ÷����������� �޸�������������������ת������
//code unsigned char Lcd_buf_ex[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}; 
//-----
//Һ���ܶ�ʱ��˳������
//����պ÷����������� �޸�������������������ת������
//���磺�����ӡ��������λ��- ��������λ��Ϊ[ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
//		ʵ�ʴӡ��������λ��- ��������λ��Ϊ[10,11,12,13,14,15, 0, 1, 2, 3,  4,  5,  6,  7,  8,  9]
// = >  code unsigned char Led_buf_ex[]={6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5};
//      code unsigned char Led_buf_ex[]={6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5};
//ע�⣬ת������������Grid5 ��С���㣬ʵ�ʲ���ʱ����Ϊ��Ӧ������˳��Ϊ11������ֱ�Ӷ�Grid11��ֵС�������
//code unsigned char Led_buf_ex[]={5,4,3,2,1,0,6,7,8,9,10,11,12,13,14,15};
code unsigned char Led_buf_ex[]={6,6,5,4,3,2,1,0,6,6,6,6,6,6,6,6};

#endif
//--��������: ��ʾλת�� -- end --

//************************************************************************************************************************
// -- ϵͳ -- �Դ涨�� --end--
//************************************************************************************************************************



//************************************************************************************************************************
// -- �Դ���� --
//************************************************************************************************************************
//------lcd---led-------����
//-- dsp_mode:
//---------0:	������ʾ
//---------1:	Һ����ʾ
void display_cal(bit dsp_mode)
{
	unsigned char 	dec_num[6];
	unsigned long   num_dsp;

	char num;
	unsigned char set_step;
	unsigned char dsp_point_position;

	bit pure_num_symbol;
	bit zero_num;
	bit dsp_special;
	bit dsp_sleep;
	bit led_no_dsp;		//���� ����ʾ

	set_step = Active_mode & 0x1f;	//�õ� �趨 �궨 �� ����
	dsp_point_position = 0; 		//Ĭ����С����
	pure_num_symbol = 0; 			//Ĭ�ϲ��Ǵ���������ʾ
	dsp_special = 0;
	dsp_sleep = 0;
	led_no_dsp = 0;

	A_input_symbol = 0;
	Sys_txt_warning_a = 0;
	Dsp_state_con = 0;

	//**************************************
	if((Active_mode & 0x20) == 0x20)	//�����궨ģʽ
	{
		switch(set_step)
		{
			case 1:		Dsp_state_con = 0x8c;	//d-----(0x0c)
						num_dsp = Core_data.ee_struct.Cal_d >> INTER_WEIGHT_NUM;
						break;	
			case 2:		Dsp_state_con = 0x8d;	//dot---(0x0d)
						num_dsp = WEIGHT_POINT_NUM;
						break;
			case 3:		Dsp_state_con = 0x8e;	//zero--(0x0e)
						num_dsp = Core_data.ee_struct.Follow_zero_num * 5;
						dsp_point_position = 1;
						break;
			case 4:		Dsp_state_con = 0x4f;	//-FULL-(0x0f)(������ʾ)
						num_dsp = (unsigned long)(Core_data.ee_struct.Cal_max_weight) * 1000;
						dsp_point_position = WEIGHT_POINT_NUM;
						break;	
			case 5:		
		    case 6:		A_input_symbol = 1;
						num_dsp = Price;
						dsp_point_position = WEIGHT_POINT_NUM;
						break;	
			case 7:		Dsp_state_con = 0x53;	//-LOAD-(0x13)(������ʾ)
						num_dsp = Price;
						dsp_point_position = WEIGHT_POINT_NUM;
						break;
/*			case 0x1c:	
			case 8:		Dsp_state_con = 0x21;	//-LOAD2(0x21)
						break;	
			case 0x1b:
			case 9:		Dsp_state_con = 0x22;	//-LOAD3(0x22)
						break;	*/
			case 0x0b:	Dsp_state_con = 0x95;	//21:Filt--(0x15)
						num_dsp = DSP_SPEED_NUM;
						num_dsp ++;
						break;							
			case 0x0c:	if(GLITCH_REMOVE_MODEA)
						{
							Dsp_state_con = 0x20;	//32:Da--AA(0x20)
						}
						else
						{
							Dsp_state_con = 0x96;	//22:Da----(0x16)
							num_dsp = GLITCH_REMOVE_BIT_NUM;
							num_dsp >>= 2;
						}
						break;							
			case 0x0d:	Dsp_state_con = 0x97;	//23:Dt----(0x17)
						//num_dsp = GLITCH_REMOVE_TIME_NUM;
						//num_dsp >>= 5;
						//num_dsp ++;
						break;							
			case 0x0e:	
						if(LCD_LAMP_OFF_MODE)
							Dsp_state_con = 0x19;	//25:B--OFF(0x19)
						else
						if(LCD_LAMP_ON_MODE)
							Dsp_state_con = 0x18;	//24:B---On(0x18)
						else
							Dsp_state_con = 0x1a;	//26:B-AUTO(0x1a)
						break;							
			case 0x0f:	Dsp_state_con = 0x9b;	//27:On----(0x1b)
						if(INIT_ZERO_SET_EN)
							num_dsp = 20;
						else
							num_dsp = 100;							
						break;	
			case 0x15:	Dsp_state_con = 0x40;	//--HH--(0x00)
						num_dsp = Money;	//��ԭ��Weight_HH��Money���������;
						if(WARRING_COUNT_DIS)
							dsp_point_position = WEIGHT_POINT_NUM;
						else
							dsp_point_position = 0;
						break;	
			case 0x16:	A_input_symbol = 1;
						num_dsp = Price;
						if(WARRING_COUNT_DIS)
							dsp_point_position = WEIGHT_POINT_NUM;
						else
							dsp_point_position = 0;
						break;			
			case 0x17:	Dsp_state_con = 0x41;	//--LL--(0x01)
						num_dsp = Money_accum.w;
						if(WARRING_COUNT_DIS)
							dsp_point_position = WEIGHT_POINT_NUM;
						else
							dsp_point_position = 0;
						break;	
			case 0x18:	A_input_symbol = 1;
						num_dsp = Price;
						if(WARRING_COUNT_DIS)
							dsp_point_position = WEIGHT_POINT_NUM;
						else
							dsp_point_position = 0;
						break;		
			case 0x19:	if(WARRING_TYPE_NO) //
							Dsp_state_con = 0x04;	//--NO--(0x04)
						else
						if(WARRING_TYPE_IN) //
							Dsp_state_con = 0x02;	//--IN--(0x02)
						else
						if(WARRING_TYPE_OUT) //
							Dsp_state_con = 0x03;	//-OUT--(0x03)
						break;
			case 0x1d:	if(WARRING_COUNT_DIS)
							Dsp_state_con = 0x1e;	//30:HL--ZL(0x1e)
						else
							Dsp_state_con = 0x1f;	//31:HL-PCS(0x1f)
						break;	
			case 0x1e:	Dsp_state_con = 0x53;	//-LOAD-(0x13)(������ʾ)
						num_dsp = Core_data.ee_struct.Cal_inter_weight;
						dsp_point_position = WEIGHT_POINT_NUM;
						break;
			case 0x1f:	pure_num_symbol = 1;	//(ȡ��λ)����������ʾ
						num_dsp = (unsigned long)(Core_data.ee_struct.Cal_max_weight) * 1000;
						dsp_point_position = WEIGHT_POINT_NUM;
						break;									
			default:	break;	
		}	
	}
	else
	if((Active_mode & 0xc0) == 0x00)	//����ģʽ
	{
		switch(set_step)
		{
			case 0:		pure_num_symbol = 1;	//����������ʾ
						num_dsp = Weight;
						dsp_point_position = WEIGHT_POINT_NUM;
						if((dsp_point_position <= 1) && Imperial_bit)	//0,1λС�� Ĭ�ϵ�λΪg��ת��ΪӢ����Ҫ����3λС��
							dsp_point_position += 3;							
						break;		
			case 0x0a:	pure_num_symbol = 1;//����������ʾ
						//num_dsp = labs(ADcode - Zero_code);
						//num_dsp >>= 2;
						num_dsp = Weight_internal_last;
						dsp_point_position = 0;					
						break;				
			default:	break;	
		}	
	}
	else
	if((Active_mode & 0xc0) == 0x40)	//����ģʽ
	{	
		switch(set_step)
		{
			case 0:		pure_num_symbol = 1;//����������ʾ
						num_dsp = Weight;									
						break;
			case 1:		Dsp_state_con = 0x85;	//SAP---(0x05)
						num_dsp = System_data.sys_struct.Counter_sample_num;
						break;
			case 0x1f:	//ȡ��
			case 2:		Dsp_state_con = 0x07;	//LOAD-C(0x07)
						break;			
			case 3:		Dsp_state_con = 0x09;	//-CSL--(0x09)
						break;	
			default:	break;	
		}	
	}
	else
	if((Active_mode & 0xc0) == 0x80)	//��������ģʽ
	{
//	    if(set_step == 0x08)
		Dsp_state_con = 0x20;
		switch(change_num_a)
		{
		    case 1:  change_num_b = Core_data.ee_struct.Follow_zero_num;
			         break;
			case 2:  if(Core_data.ee_struct.ZERO_NUM == 4)		           
                         change_num_b = 1;
					 else
					 if(Core_data.ee_struct.ZERO_NUM == 10)
		                 change_num_b = 2;
					else
					if(Core_data.ee_struct.ZERO_NUM == 15)		          
                         change_num_b = 3;
					else
					if(Core_data.ee_struct.ZERO_NUM == 20)		            
                         change_num_b = 4;
					else
					if(Core_data.ee_struct.ZERO_NUM == 30)		          
                         change_num_b = 5;
					break;								
			case 3: if(Core_data.ee_struct.TAPE_NUM == 30)
                         change_num_b = 1;
					else
					if(Core_data.ee_struct.TAPE_NUM == 50)		           
                         change_num_b = 2;
					else
					if(Core_data.ee_struct.TAPE_NUM == 60)
		            	 change_num_b = 3;
					else
					if(Core_data.ee_struct.TAPE_NUM == 80)
		            	 change_num_b = 4;
					else
					if(Core_data.ee_struct.TAPE_NUM == 100)
		            	 change_num_b = 5;
					break;
			case 4: if(POWER_MODE_1)
		            	change_num_b = 1;
					else
                    if(POWER_MODE_2)
		            	change_num_b = 2;
					if(POWER_MODE_3)
		            	change_num_b = 3;
					break;
			case 5: if(DISPLAY_MODE_1)		           
					    change_num_b = 1;
					else
					if(DISPLAY_MODE_2)
					    change_num_b = 2;
					break;
            case 6: if(POWER_6V_BIT_EN)		            
					    change_num_b = 2;
					else
					if(POWER_4V_BIT_EN)				
					    change_num_b = 1;
					break;
		
		}
	}
//-- ��������: �ٷֱ� -- begin --
#ifdef PERCENTAGE_FUN_EN
	else
	if((Active_mode & 0xc0) == 0x80)	//�ٷֱ�ģʽ
	{
		switch(set_step)
		{
			case 0:		pure_num_symbol = 1;//����������ʾ
						num_dsp = Weight;
						dsp_point_position = 1;
						break;
			case 0x1f:	//ȡ��
			case 1:		Dsp_state_con = 0x06;	//LOAD-P(0x06)
						break;	
			case 2:		A_input_symbol = 1;
						dsp_point_position = WEIGHT_POINT_NUM;
						num_dsp = Price;
						break;			
			case 3:		Dsp_state_con = 0x09;	//-CSL--(0x09)
						break;		
			default:	break;	
		}	
	}
#endif
//-- ��������: �ٷֱ� -- end --


	if(Vol_state)
	{
		Dsp_state_con = 0x94; 	//DC----(0x14)
		if(!Start_up_end)
			num_dsp = Zero_weight_accum;	//�������ڿ���ʱʹ��
		//����ʱ�������Ҫ��ʾ��ѹ����Ҫ���� Price
		dsp_point_position = 2;	
		pure_num_symbol = 0;
	}
	else
	if(Weight_on_error)	//���� ��λ ����
	{
		if((Active_mode & 0x20) != 0x20)	//�� �����궨ģʽ
		{
			Dsp_state_con = 0x1c;	//28:ERR-0(0x1c)
			pure_num_symbol = 0;
		}
	}
	else
	if(Data_error)	//�궨�������󱨾�
	{
		Dsp_state_con = 0x0b;	//-SYS--(0x0b)
		pure_num_symbol = 0;
	}
	else
	if(Weight_overflow)	//���ر���
	{
		Dsp_state_con = 0x0f;	//-FULL-(0x0f)
		pure_num_symbol = 0;
	}
	else
	if(Weight_ul_error)	//���������趨�������󱨾�
	{
		Dsp_state_con = 0x1d;	//-SET--(0x1d)
		pure_num_symbol = 0;
	}
	else
	if(Lamp_lowv_a)	//��ѹ����
	{
		Dsp_state_con = 0x0a;	//--LB--(0x0a)
		pure_num_symbol = 0;
	}
	else
	if((Lamp_lowv)&&(Code_zero))	//��ѹ����
	{
		Dsp_state_con = 0x0a;	//--LB--(0x0a)
		pure_num_symbol = 0;
	}

//--��������: �����ػ� --begin--
#ifdef KEY_PRESS_PD_FUN_EN
	if(Mcu_stop_pre || Mcu_stop_rec)
	{
		LCD_LAMP_OFF; // �رձ����	
		dsp_special = 1;
	}
	else
#endif
//--��������: �����ػ� --end--	
	if(Sleep_dsp_mode)
	{
		if(dsp_mode == 0)
	  	{
	   		led_no_dsp = 1;
			if((Active_mode & 0x1f) == 0x00)
			{
				dsp_special = 1;
				dsp_sleep = 1;
				Vol_state = 0;
			}
		}
		else
		{
			if(LCD_LAMP_AUTO_MODE)
				LCD_LAMP_OFF; // �رձ����
		}
	}
	else
	if(Price_light_symbol) //������ô򿪱���Ļ�
	{
		LCD_LAMP_ON; // �򿪱����	
	}
	else
	{
		LCD_LAMP_OFF; // �رձ����
	}


//--- ��� ���� ����λ -------
	for(num=6;num<=15;num++)
		Dsp_buf[num] = 0x00;
	if(dsp_mode)
		Dsp_buf[0] = 0x00;
//------- ��ʾ �ַ��� ---------
	if(!dsp_special && dsp_mode)
//	if(!dsp_special)
	{
//		for(num=0;num<=5;num++)
//			Dsp_buf[num] = string_pattern[Dsp_state_con & 0x3f][num];
		for(num=0;num<=5;num++)
		{	
		    if((change_num_a != 0)&&(num==1))
			{
			    Dsp_buf[num] = pattern_table[change_num_a];
			}
			else
			if((change_num_b != 0)&&(num==3))
			{
			    Dsp_buf[num] = pattern_table[change_num_b];
			}
			else
//			if((change_num_a == 0)&&(change_num_b == 0))
			   Dsp_buf[num] = string_pattern[Dsp_state_con & 0x3f][num];
		}	
  	}

	if(!dsp_special)
	{
		if(pure_num_symbol || ((Dsp_state_con & 0x80) == 0x80) || (((Dsp_state_con & 0x40) == 0x40) && Dsp_state_num_state) || A_input_symbol)
		{
			//����ʮ���Ƴ����� "num_dsp" 1-6λ�ϵ���ֵ;
			if(dsp_mode)	//Һ��ʱ�ż����Դ�
			{
				for(num=0;num<=5;num++)
				{	
					dec_num[num] =  num_dsp % 10;		
					num_dsp /= 10;	
				}
			}					
			//--- ȫ������ʾ���� -------
			if(A_input_symbol)
				zero_num = 0;
			else
				zero_num = 1;

			//��ֵ��ʾ
			for(num=5;num>=0;num--)
			{
				unsigned char i;
				i = 5 - num;

				if(dsp_mode)	//Һ��ʱ�ż����Դ�
				{	
					//--������ʾ���ݣ��ӵ�һ���������֣�����С����ǰ�����ֿ�ʼ��ʾ				  						
					if((dec_num[num] == 0) && zero_num)
					{
						if(pure_num_symbol || (((Dsp_state_con & 0x40) == 0x40) && Dsp_state_num_state) )	//������ ���� ������ʾʱ
						{
							Dsp_buf[i] = 0x00;	// Ϊ0 ��ʾ�ո�
						}
					}
					else
					{
						Dsp_buf[i] = pattern_table[dec_num[num]];
						zero_num = 0;
					}
				}
				else	//�������ʱ������������Դ棬ֻ�����¼���С����λ�ã��������Һ����С����λ�ã�
				{
					Dsp_buf[i] &= ~LCDC_DT;
			  	}

				if(num == (dsp_point_position + 1))
					zero_num = 0;	
			}				

			//---- ��ʾС���� ---
			if(dsp_point_position!=0)
			{
				if(dsp_mode == 0)
				{
					Dsp_buf[5-dsp_point_position] |= LCDC_DT;
				}
				else
				{
					Dsp_buf[5-dsp_point_position] |= LCDC_DT;
				}				
			}	

			//---- ��ʾ"����" ---
			if(Sign && (set_step == 0))
			{
				if(dsp_mode == 0)
				{
					Dsp_buf[LED_WEIGHT_6] &= LCDC_1;
					Dsp_buf[LED_WEIGHT_6] |= LCDC_HH; //"-"
				}
				else
				{
					Dsp_buf[LED_WEIGHT_6] |=  LCDC_HH; //"-"
				}		
			}
		}
	}
	else
	{
		for(num=0;num<=5;num++)
			Dsp_buf[num] = 0x00;
	}	
//------ ��ͷ������˸���� -----
	if(A_input_symbol && !A_input_num_symbol)
	{
		Dsp_buf[A_input_setp - 1] = 0x00;
	}
/*	if(!dsp_mode)
	{
		Dsp_buf[LED_LABLE_1] &=  0x00;  //���Һ���ķ�����ʾ
	}*/
//------ ģʽ���� ָʾ�� -----
//--��������: �����ػ� --begin--
#ifdef KEY_PRESS_PD_FUN_EN
if(!Mcu_stop_pre && !Mcu_stop_rec)	//���ʱ����Ƭ���������ģ���ʾҲ�Ǵ򿪵ģ�����Ҫ������һ���֣�
#endif
//--��������: �����ػ� --end--
{
	//--ģʽָʾ��--
	if((Active_mode & 0xc0) == 0x00)	//����ģʽ
	{
		if(dsp_mode == 0)		//--��λָʾ��--
		{
			if(!dsp_special)
			{
				if(Imperial_bit) // Ӣ��
					Dsp_buf[LED_LABLE_1] |=  SCG_A;	//
				else // ����
					Dsp_buf[LED_LABLE_1] |=  SCG_F;	//
		   	}
		}
		else
		{
			if(Imperial_bit) // Ӣ��
				Dsp_buf[LCD_WEIGHT_0] |= 0x02;
			else
			{
				Dsp_buf[LCD_WEIGHT_0] |= 0x0c;
			}
		}
	}
	else
	if((Active_mode & 0xc0) == 0x40)	//����ģʽ
	{
		if(dsp_mode == 0)
		{
			if(!dsp_special)
				Dsp_buf[LED_LABLE_1] |=  SCG_B;	//	
		}
		else
		{
			if(!dsp_special)
				Dsp_buf[LCD_WEIGHT_0] |= 0x01;	//
		}		
	}

}

//--(��������)-----beign-----ע�⣺ Һ��ʱ ����Ҫ����

//--��������: �����ػ� --begin--
#ifdef KEY_PRESS_PD_FUN_EN
if(!Mcu_stop_pre && !Mcu_stop_rec && !Start_up_end)	// ���Ƹ���ָʾ�ƣ������ʱ����Ƭ���������ģ���ʾҲ�Ǵ򿪵ģ�����Ҫ������һ���֣�
#else
if(!Start_up_end)	// ���Ƹ���ָʾ�ƣ������ʱ����Ƭ���������ģ���ʾҲ�Ǵ򿪵ģ�����Ҫ������һ���֣�
#endif
//--��������: �����ػ� --end--
{
	if(A_input_setp == 0x00) //������һ�� ���� ���Բ��� ��ʾȫ8
	{
		for(num=0;num<16;num++)
			Dsp_buf[num] = 0xff;
	}
	else
	if((A_input_setp >= 0x01) && (A_input_setp < 0x0a)) //
	{
		for(num=0;num<6;num++)
		{
			Dsp_buf[num] = pattern_table[10 - A_input_setp];
			Dsp_buf[num] |= LCDC_DT;
		}
	//	for(num=6;num<16;num++)
	//		Dsp_buf[num] = 0x00;
	}
}
//--(��������)-----end-----ע�⣺ Һ��ʱ ����Ҫ����


//--(ָʾ�ƴ���)-----beign-----

//--��������: �����ػ� --begin--
#ifdef KEY_PRESS_PD_FUN_EN
if(!Mcu_stop_pre && !Mcu_stop_rec)	// ���Ƹ���ָʾ�ƣ������ʱ����Ƭ���������ģ���ʾҲ�Ǵ򿪵ģ�����Ҫ������һ���֣�
#endif
//--��������: �����ػ� --end--
{
	if(dsp_mode == 0)
	{
 		if(!dsp_special)
		{
	//		if(Code_zero)							//�������ʱ����zeroָʾ��
	//			Dsp_buf[LED_LABLE_1] |= 0x81; 	
	//		if(Lamp_tape)							//����Lamp_tape ����tapeָʾ��״̬	
	//			Dsp_buf[LED_WEIGHT_6] |=  LCDC_DT; 	
		}
	}
	else
	{
		if(Code_zero)							//�������ʱ����zeroָʾ��
			Dsp_buf[LCD_WEIGHT_0] |= 0x10; 	
		if(Lamp_tape)							//����Lamp_tape ����tapeָʾ��״̬	
			Dsp_buf[LCD_WEIGHT_0] |=  0x80; 	
	}
}
	

//--��������: �����ػ� --begin--
#ifdef KEY_PRESS_PD_FUN_EN
if(!Mcu_stop_pre && !Mcu_stop_rec)	// ���Ƹ���ָʾ�ƣ������ʱ����Ƭ���������ģ���ʾҲ�Ǵ򿪵ģ�����Ҫ������һ���֣�
#endif
//--��������: �����ػ� --end--
{
//--(��������)-----beign-----
	if(dsp_mode == 0)
	{
		if(dsp_sleep)
		{
			Dsp_buf[LCD_WEIGHT_1] = LCDC_HH;	//"-"	
		}
	}
//--(��������)-----end-----
}

}

