//*************************************************************************//
//* Name				:   main.c
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic ���ӳ� ���ļ�
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
#pragma code symbols debug oe
#define EXTERN
#include <REG52.H>
#include <math.h>
//**************************************************************************
// -- ȫ��ͷ�ļ� ���й��� ---- ���汾�޸��ļ�����䶯 ---  �����޸� ����ļ�
//**************************************************************************
#include "./_solidic/head_file_version.h"


main()
{
//************************************************************************************************************************
// -- ��ʼ��ϵͳ --
//************************************************************************************************************************
	P0 = 0xfb;	//P0^2(BUZZER_P) = 0
	P1 = 0xef;	//P1.4(LCD_LAMP) = 0;
	P2 = 0xff;		
//--��ʼ��PWM--
	PWMF_H  = 0x00;
	PWMF_L  = 0xff;
	PWM0  	= LCD_LAMP_B4;
	PWM1  	= 0xff;	
	PWMCON  = 0x04;	//PWM0-P1.4(LCD_LAMP)�������(��PWM0=0xffʱ,����ߵ�ƽ)

//--����IOģʽ 
	//����P0 Ϊ ��ģ������
	P0M0 = 0x00;
	P0M1 = 0xff;
	//����P1 Ϊ ��ģ������
	P1M0 = 0x00;
	P1M1 = 0xff;
//---����Sigma_delta ADC---
	//����P2.0 P2.1 ֻ����
	P2M0 = 0x00;
	P2M1 = 0xff;
//--�ؼ��ܽŵ�IO����
	IO_MODE_RESTORE_A_EN;
	//--����ADC---
	EIE |= 0x04;		//��SG ADC �ж�
	//SGADCON = SGADCON_DEFAULT; 	//20Hz
	//SGADCON2 = 0x30;	//Pga-1/2ն�� Adc-1/2ն��
	//SGADCON2 |= 0x05;	//Pga-1/4ն�� Adc-1/4ն��
	//����P2��Ϊ��ģ�����룬����SGADCON,SGADCON2
	//����ALDOͬ��SG-ADC,�ر�Temp;
	PD_CON = PD_CON_DEFAULT;	
//--ѡ��CPUƵ��
//-- ��������: 2��Ƶʱ��ģʽ -- begin --
#ifdef SDI5219_HALF_SPEED
	CKCON |= 0x01;
#endif
//-- ��������: 2��Ƶʱ��ģʽ -- end --
//--��ʼ����ʱ��
	TMOD = 0x11;	//��ʱ��0��1��������16λ״̬��				
	TH0 = 0x80;                       
	TL0 = 0x00;
	//TH1 = 0x00;                       
	//TL1 = 0x00;
	//TR0  = 1;	//��������0,���ڿ��Ƽ���ɨ��					
	//TR1  = 1;	//��������1,����ʱ�����			
//--�����ж�
	IT0 = 0;		//�趨�ⲿ�ж�0 ���õ͵�ƽ����

	IE   = 0x80;	
	//--�����ж����ȼ�
	IP = 0x05;		//�ⲿ�ж�0,1Ϊ�����ȼ�
	EIP = 0x06;		//��Ƶ���ѣ�24λADC�ж�Ϊ�����ȼ�
//---������ʼ��---
	Led_bright = 0x03;
	System_data.sys_struct.Counter_sample_num = 10;
//-----�ر���ʾ---------------
	LCD_LAMP_OFF; // �رձ����	
	//--IO������--
	IO_MODE_DSP_EN;
	dsp_updata(1); //�ر���ʾ
	//--��ʼ��LCDģʽ
	write_lcd(0,0x01);
	write_lcd(0,0x18);
	write_lcd(0,0x29);
	write_lcd(0,0x02); //�ر�Һ����ʾ
	write_lcd(0,0x40); //������Ƶ��4K
	write_lcd(0,0xe3); //��������ģʽ
	//--IO������--
	IO_MODE_DSP_DIS;
//--��ʱ����֤��ѹ����Լ�EEPROM����׼ȷ
	delay_ms(500);
//--- ��ʼ�����Ź� --------
	//---�򿪿��Ź�
	EA = 0;
	WD_TA = 0x05;
	WD_TA = 0x0a;
	WDCON = 0xbf;	//(SDI5216)���Ź�ʱ�� 4s��,(�ر�)��Ƶ����0.2s
	EA = 1;
	//--�򿪵�Ƶ�����ж�
	EIE |= 0x02;	//(SDI5216)��������Ƶ�����ж�
//----��ʼ���ӵı궨��----
	init_cal_point();
//----��ʼ����ʾ----
	Price_light_symbol = 1; //�������� ���� ���� �Ƿ�������
	LCD_LAMP_ON; // �򿪱����	
	buzzer(0);
	Led_dsp_updata = 1;
	Tape_weight = 0;
//************************************************************************************************************************
// -- ������ѭ�� --
//************************************************************************************************************************
	while(1)
	{
//--SDI5219---
		//--�ؼ��ܽŵ�IO����
		IO_MODE_RESTORE_A_EN;
		//************************************************
		//---ι��--��������ȫ����-begin-
		//************************************************
		EA = 0;
		WD_TA = 0x05;
		WD_TA = 0x0a;
		WDCON = 0xbf;	//(SDI5216)���Ź�ʱ�� 4s��,(�ر�)��Ƶ����0.2s
		EA = 1;

		//************************************************
		//---��������-----------------begin---------------
		//************************************************
		if( Key_press && (!Key_response) )
		{	
			key_scan();
			//Flash ��ȫ��֤�� A
			NRM_securty_a = 0xaa;	//Flash ��ȫ��֤�� A
			if(ADbuf_init.b[0] != NONE)
			{
				Key_counter_en = 1; //��ʼ����ʱ����� 

				buzzer(0);
				//���� ��ʾ����
				if(Sleep_dsp_mode)
				{
					Weight_stable_counter = 0;
					//���ⰴ�� �ر�������ʾ����
					Sleep_dsp_mode = 0;
				}
				Led_dsp_updata = 1; //LED��ʾ��Ҫ����

				//---���� "ZERO" ���ܼ� ------------	
				if(ADbuf_init.b[0] == ZERO)
					Cal_zero_en = 1;
/*				//---���� "UNIT_KEY" ���ܼ� ------------
				if(ADbuf_init.b[0] == UNIT_KEY)
					Cal_unit_en = 1;
				//---���� "MODE_KEY" ���ܼ� ------------
				if(ADbuf_init.b[0] == MODE_KEY)
					Cal_mode_en = 1;*/
				//---���� "SET_KEY" ���ܼ� ------------
				if(ADbuf_init.b[0] == SET_KEY)
					Cal_set_en = 1;
				//---���� "TAPE" ���ܼ� ------------
				if(ADbuf_init.b[0] == TAPE)
					Cal_tape_en = 1;
				//---���� "ARROW_KEY" ���ܼ� ------------
				if(ADbuf_init.b[0] == ARROW_KEY) // ARROW_KEY
				{
					unsigned char set_step;
					set_step = Active_mode & 0x1f;	//�õ� �趨 �궨 �� ����

					if((Active_mode & 0x20) == 0x20)	//�����궨ģʽ
					{
						switch(set_step)
						{
							case 1:		if(Core_data.ee_struct.Cal_d == (1 << INTER_WEIGHT_NUM))
											Core_data.ee_struct.Cal_d = (2 << INTER_WEIGHT_NUM);	//�ֶ�Ϊ 2g
										else
										if(Core_data.ee_struct.Cal_d == (2 << INTER_WEIGHT_NUM))
											Core_data.ee_struct.Cal_d = (5 << INTER_WEIGHT_NUM);	//�ֶ�Ϊ 5g
										else
										if(Core_data.ee_struct.Cal_d == (5 << INTER_WEIGHT_NUM))
											Core_data.ee_struct.Cal_d = (10 << INTER_WEIGHT_NUM);	//�ֶ�Ϊ 10g
										else
										if(Core_data.ee_struct.Cal_d == (10 << INTER_WEIGHT_NUM))
											Core_data.ee_struct.Cal_d = (20 << INTER_WEIGHT_NUM);	//�ֶ�Ϊ 20g
										else
										if(Core_data.ee_struct.Cal_d >= (20 << INTER_WEIGHT_NUM))
											Core_data.ee_struct.Cal_d = (1 << INTER_WEIGHT_NUM);//�ֶ�Ϊ 1g
										follow_zero_cal();	
										break;
							case 2:		if(WEIGHT_POINT_FULL)	//��������С����	
											WEIGHT_POINT_CLR;
										else
											WEIGHT_POINT_NUM_ADD;
										break;
/*							case 3:		if(Core_data.ee_struct.Follow_zero_num >= 10)	//ȷ��׷�㷶Χ
											Core_data.ee_struct.Follow_zero_num = 1;
										else
											Core_data.ee_struct.Follow_zero_num ++;
										follow_zero_cal();
										break;*/
							case 4:		Price = 0;	//-- ��ͷ���� ������������
										A_cycle_num = 0;	//-- ��ͷ���� ״̬ (0-9)����
 										A_input_setp = 1;	//-- ��ͷ���� ����λ�ó�ʼ��
										Active_mode ++;	//-- ���� ��ͷ���� ����
										break;	
							case 5:		a_input_analyse();	//���м��̰������봦��
										break;
							case 6:		a_input_analyse();	//���м��̰������봦��
										break;
							case 7:		Price = 0;	//-- ��ͷ���� ������������
										A_cycle_num = 0;	//-- ��ͷ���� ״̬ (0-9)����
 										A_input_setp = 1;	//-- ��ͷ���� ����λ�ó�ʼ��
										Active_mode --;		//-- ���� ��ͷ���� ����
										break;										
							case 0x0b:	if(!Cal_temp_bit) 	// Filt-- 
											Active_mode ++;	//-- ���� ��ͷ���� ����
										else				//-- ������ʾ �ٶ�
										{
											if(DSP_SPEED_FULL)
												DSP_SPEED_CLR;
											else
												DSP_SPEED_NUM_ADD;
										}
										break;
							case 0x0c:	if(!Cal_temp_bit) 	// Da----
											Active_mode += 2;	//-- ���� ��ͷ���� ����
										else				//-- �������				
										{
											if(GLITCH_REMOVE_BIT_FULL)
												GLITCH_REMOVE_BIT_CLR;
											else
												GLITCH_REMOVE_BIT_ADD;
										}
										break;
							case 0x0d:	
										break;
							case 0x0e:	if(!Cal_temp_bit) 	// B----
											Active_mode ++;	//-- ���� ��ͷ���� ����
										else				//-- �����--��ʽ
										{
											if(LCD_LAMP_MODE_FULL)
												LCD_LAMP_OFF_SET;
											else
												LCD_LAMP_MODE_NUM_ADD;
										}
										break;
							case 0x0f:	if(!Cal_temp_bit) 	// ON----
										{
											Active_mode &= 0xc0;	
											Active_mode |= 0x2b;	//����궨ģʽ �ĵ�11��״̬��Filt--��
										}
										else				//-- ������������				
										{
											if(INIT_ZERO_SET_EN)
												INIT_ZERO_SET_CLR;
											else
												INIT_ZERO_SET_SET;
										}
										break;
							case 0x15:	Price = 0;	//-- ��ͷ���� ������������
										A_cycle_num = 0;	//-- ��ͷ���� ״̬ (0-9)����
 										A_input_setp = 1;	//-- ��ͷ���� ����λ�ó�ʼ��
										Active_mode ++;	//-- ���� ��ͷ���� ����
										break;

							case 0x16:	a_input_analyse();	//���м��̰������봦��
										break;
							case 0x17:	Price = 0;	//-- ��ͷ���� ������������
										A_cycle_num = 0;	//-- ��ͷ���� ״̬ (0-9)����
 										A_input_setp = 1;	//-- ��ͷ���� ����λ�ó�ʼ��
										Active_mode ++;	//-- ���� ��ͷ���� ����
										break;	
							case 0x18:	a_input_analyse();	//���м��̰������봦��
										break;	
							case 0x19:	if(WARRING_TYPE_FULL)	//��ͷ���� �趨 ����ģʽ
											WARRING_TYPE_NO_SET;
										else
											WARRING_TYPE_NUM_ADD;
										break;
							case 0x1d:	WARRING_COUNT_XOR; //�л��������� �� ��������
										break;
							default:	break;
						}//--switch(set_step)
					}//--if((Active_mode & 0x20) == 0x20)	//�����궨ģʽ
					else
					if((Active_mode & 0xe0) == 0x40)	//����ģʽ�µ�ȡ���趨
					{
						switch(set_step)
						{
							case 1:		if(System_data.sys_struct.Counter_sample_num <= 10)	
											System_data.sys_struct.Counter_sample_num = 20;
										else
										if(System_data.sys_struct.Counter_sample_num == 20)	
											System_data.sys_struct.Counter_sample_num = 50;
										else
										if(System_data.sys_struct.Counter_sample_num == 50)	
											System_data.sys_struct.Counter_sample_num = 100;
										else
										if(System_data.sys_struct.Counter_sample_num == 100)	
											System_data.sys_struct.Counter_sample_num = 200;
										else
										if(System_data.sys_struct.Counter_sample_num == 200)	
											System_data.sys_struct.Counter_sample_num = 500;
										else
										if(System_data.sys_struct.Counter_sample_num == 500)	
											System_data.sys_struct.Counter_sample_num = 1000;
										else
										if(System_data.sys_struct.Counter_sample_num >= 1000)	
											System_data.sys_struct.Counter_sample_num = 10;
										break;	
							default:	break;
						}//--switch(set_step)
					}//--if((Active_mode & 0xe0) == 0x40)	//����ģʽ�µ�ȡ���趨
					else
					if((Active_mode & 0xe0) == 0x80)	//ģʽ�µ��趨
					{
						switch(change_num_a)
							{
							    case 1:		if(Core_data.ee_struct.Follow_zero_num >= 5)
												Core_data.ee_struct.Follow_zero_num = 1;
											else
												Core_data.ee_struct.Follow_zero_num ++;
											change_num_b = Core_data.ee_struct.Follow_zero_num;
											follow_zero_cal();
											break;
								case 2:     if(Core_data.ee_struct.ZERO_NUM == 4)
								            {
								               Core_data.ee_struct.ZERO_NUM = 10;
	                                           change_num_b = 2;
											}
											else
											if(Core_data.ee_struct.ZERO_NUM == 10)
								            {
								               Core_data.ee_struct.ZERO_NUM = 15;
	                                           change_num_b = 3;
											}
											else
											if(Core_data.ee_struct.ZERO_NUM == 15)
								            {
								               Core_data.ee_struct.ZERO_NUM = 20;
	                                           change_num_b = 4;
											}
											else
											if(Core_data.ee_struct.ZERO_NUM == 20)
								            {
								               Core_data.ee_struct.ZERO_NUM = 30;
	                                           change_num_b = 5;
											}
											else
											if(Core_data.ee_struct.ZERO_NUM == 30)
								            {
								               Core_data.ee_struct.ZERO_NUM = 4;
	                                           change_num_b = 1;
											}
											break;										
	                            case 3:     if(Core_data.ee_struct.TAPE_NUM == 30)
								            {
								               Core_data.ee_struct.TAPE_NUM = 50;
	                                           change_num_b = 2;
											}
											else
											if(Core_data.ee_struct.TAPE_NUM == 50)
								            {
								               Core_data.ee_struct.TAPE_NUM = 60;
	                                           change_num_b = 3;
											}
											else
											if(Core_data.ee_struct.TAPE_NUM == 60)
								            {
								               Core_data.ee_struct.TAPE_NUM = 80;
	                                           change_num_b = 4;
											}
											else
											if(Core_data.ee_struct.TAPE_NUM == 80)
								            {
								               Core_data.ee_struct.TAPE_NUM = 100;
	                                           change_num_b = 5;
											}
											else
											if(Core_data.ee_struct.TAPE_NUM == 100)
								            {
								               Core_data.ee_struct.TAPE_NUM = 30;
	                                           change_num_b = 1;
											}
											break;
								case 4:     if(POWER_MODE_3)
								            {
											    POWER_MODE_CLK;
	                                            change_num_b = 1;
											}
											else
	                                        if(POWER_MODE_1)
								            {
											    POWER_MODE_ADD;
	                                            change_num_b = 2;
											}
	                                        else
											if(POWER_MODE_2)
								            {
											    POWER_MODE_ADD;
	                                            change_num_b = 3;
											}
											break;
								case 5:     if(DISPLAY_MODE_1)
								            {	
												DISPLAY_MODE_SET;										    
											    change_num_b = 2;
											}										
											else
											if(DISPLAY_MODE_2)
											{
												DISPLAY_MODE_CLK;
											    change_num_b = 1;
											}
											break;
								case 6:     if(POWER_6V_BIT_EN)
								            {
											    POWER_BIT_CLK;
											    change_num_b = 2;
											}
											else
											if(POWER_4V_BIT_EN)
											{
											    if(POWER_BIT_SET);
											    change_num_b = 1;
											}
											break;
	//							case 2:		//--���� �ٷֱ�ȡ�� ����
	//										a_input_analyse();	//���м��̰������봦��
	//										break;									
								default:	break;
							}//--switch(set_step)
					}//--if((Active_mode & 0xe0) == 0x80)	//�ٷֱ�ģʽ�µ�ȡ���趨
				}//--if(KEY_CODE == ARROW_KEY) // ARROW_KEY
				// --- �趨 �����Ѵ��� ��־λ ----
				Key_response = 1;
			}//--if(ADbuf_init.b[0] != NONE)
		}//--if( Key_press && (!Key_response) )

		if(!Key_press && (Cal_zero_en || Cal_unit_en || Cal_mode_en || Cal_set_en || Cal_tape_en))
		{
				Led_dsp_updata = 1; //LED��ʾ��Ҫ����
				//---���� "ZERO" ���ܼ� ------------	
				if(Cal_zero_en)
				{
					if(A_input_setp == 0)	//����׷�� (��ʱ����Counter_sample_num�������ڴ����ģ�ע��ָ�)
					{	
						if(Weight_internal_last < (15000 << INTER_WEIGHT_NUM)) //һ��С���ڲ��� 10% (3000 / 30000)
						{
							if(Sign_code)
								System_data.sys_struct.Counter_sample_num = labs((long)(Zero_weight_accum) - (long)(Weight_internal_last));
							else
								System_data.sys_struct.Counter_sample_num = labs((long)(Zero_weight_accum) + (long)(Weight_internal_last));
					//--- ������Ϊ�ⲿ����ʱ�����ڴ���Core_data.ee_struct.Cal_d�Ŀ��ܷŴ�Ч�����������
					//--- ͬʱ��������6λ����Cal_max_weight ����Ϊ999��ͬ��������Cal_max_weight * (30 << INTER_WEIGHT_NUM))���
					//--- 1000 * 30 * 8 = 24���Ѿ������
					//--- 1000 * 30 * 4 = 12�����
					//--- 1000 * 30 * 2 = 6�������
					//--- ��Ҫ������ INTER_WEIGHT_NUM ת���� 6���������ж�׷��

					//--- ��������: ����ģʽ -- begin ---
					#if INTER_WEIGHT_NUM >= 2
							System_data.sys_struct.Counter_sample_num >>= (INTER_WEIGHT_NUM - 1);
					#endif
					//--- ��������: ����ģʽ -- end ---
							if(Core_data.ee_struct.Intel_cal_d < 0x80)
							{
								System_data.sys_struct.Counter_sample_num /= Core_data.ee_struct.Intel_cal_d;
							}
							else
							{	
								unsigned char j;
								j = Core_data.ee_struct.Intel_cal_d & 0x7f; 
								System_data.sys_struct.Counter_sample_num *= j;
							}
							
							System_data.sys_struct.Counter_sample_num += (Core_data.ee_struct.Cal_d / 2);
							System_data.sys_struct.Counter_sample_num /= Core_data.ee_struct.Cal_d;	
							System_data.sys_struct.Counter_sample_num *= Core_data.ee_struct.Cal_d;					

							if(System_data.sys_struct.Counter_sample_num <= (Core_data.ee_struct.Cal_max_weight * (30 << 1)) )
							{
								Zero_code = ADcode;
								Weight_lock_symbol = 0;
								if(Sign_code)
									Zero_weight_accum = Zero_weight_accum - (int)(Weight_internal_last);
								else
									Zero_weight_accum = Zero_weight_accum + (int)(Weight_internal_last);
								Weight_internal_last = 0;
							
							}	
						 	//--�ָ�Counter_sample_num
							init_cal_point();
						}				
						//if(Weight_zero)
						//	Price_light_symbol = !Price_light_symbol;
					}
					else
//					if((Active_mode & 0x20) == 0x20)	//�����궨ģʽ
					{
						Price = 0;	//-- ��ͷ���� ������������
						A_cycle_num = 0;	//-- ��ͷ���� ״̬ (0-9)����
 						A_input_setp = 1;	//-- ��ͷ���� ����λ�ó�ʼ��
					}
					if((Active_mode & 0x80) == 0x80)
					{
					    Active_mode = 0;
                        change_num_a = 0;
						change_num_b = 0;
						Dsp_state_con = 0;
//						core_memory_write();
						init_cal_point();
					}
					Cal_zero_en = 0;
				}

/*				//---���� "UNIT_KEY" ���ܼ� ------------
				if(Cal_unit_en)
				{
					if((Active_mode & 0x20) != 0x20)
						Imperial_bit = !Imperial_bit;
					Cal_unit_en = 0;
				}

				//---���� "MODE_KEY" ���ܼ� ------------
				if(Cal_mode_en)
				{
					unsigned char i;
					if((Active_mode & 0x80) == 0x80)	//��"�궨"ģʽ��,ֱ���˳��궨ģʽ
					{
						Active_mode = 0x00;
						change_num_a = 0;
						change_num_b = 0;
						Dsp_state_con = 0;
						init_cal_point();			//�˴����� ��ȡ ֮ǰ�� EEPROM ���ڻָ� �趨Ϊ�ɹ��ƻ�������
					}
					else
					if((Active_mode & 0x20) == 0x20)	//��"�궨"ģʽ��,ֱ���˳��궨ģʽ
					{
						if((Active_mode & 0x1f) <= 4) // ��Ҫ����
						{
							//--------- �� �궨 ���� ����EEPROM -------------------------------
							core_memory_write();
							delay_ms(200);
							buzzer(0);
						}
						Active_mode = 0x00;
						init_cal_point();			//�˴����� ��ȡ ֮ǰ�� EEPROM ���ڻָ� �趨Ϊ�ɹ��ƻ�������
					}
					else
					{
						//�Ǳ궨ģʽ�Ļ������� ���� - �ٷֱ� ģʽ ���л�
						Active_mode += 0x40;
					//-- ��������: �ٷֱ� -- begin --
					#ifdef PERCENTAGE_FUN_EN
						if((Active_mode & 0xc0) == 0xc0)
							Active_mode &= 0x3f;
					#else
						if((Active_mode & 0xc0) == 0x80)
							Active_mode &= 0x3f;
				  	#endif
					//-- ��������: �ٷֱ� -- end --	
						if((Active_mode & 0xc0) != 0x40)
						{
							//--��ȡ �û� ����--
							for(i=0;i<SYS_DATA_LENTH;i++)
								System_data.b[i] = nvm_data_read_byte(ADDR_SYS_DATA + i);
						}
					}
//					if((Active_mode & 0xc0) == 0x40)
//                   	Active_mode |= 0x01;
					Active_mode &= 0xc0;
					Imperial_bit = 0; //ÿ�ΰ���ģʽ�� ����� Ӣ��
					Cal_mode_en = 0;
				}
*/
				//---���� "SET_KEY" ���ܼ� ------------
				if(Cal_set_en)
				{
					if((Active_mode & 0x20) == 0x20) 	//��"�궨"ģʽ��,ֱ���˳��궨ģʽ
					{
						if((Active_mode & 0x1f) <= 4) // ��Ҫ����
						{
							//--------- �� �궨 ���� ����EEPROM -------------------------------
							core_memory_write();
							delay_ms(200);
							buzzer(0);
						}
						Active_mode = 0x00;
						init_cal_point();//�˴����� ��ȡ ֮ǰ�� EEPROM ���ڻָ� �趨Ϊ�ɹ��ƻ�������
					}
/*					else
					if(Active_mode== 0x40)
					{
						Active_mode++;
					}*/
					else
					if((Active_mode & 0xc0) == 0x80)
					{
					    if(change_num_a < 6)
						   change_num_a ++;
						else
						   change_num_a = 1;
					}
					else
					{
						if((Active_mode & 0x01) == 0x01)
						    Active_mode = 0;
						else
						{
							//�Ǳ궨ģʽ�Ļ������� ���� - ����ģʽ ���л�
//                       Active_mode &= 0x40;
//							Active_mode += 0x40;
							if((Active_mode & 0xc0) == 0x40)
								Active_mode &= 0x3f;
							else
							if((Active_mode & 0xc0) == 0x00)
								Active_mode += 0x40;
						}	
					}
					if((Active_mode & 0xc0) == 0x40)
                    	Active_mode |= 0x01;
//					Active_mode &= 0xc0;
					Imperial_bit = 0; //ÿ�ΰ���ģʽ�� ����� Ӣ��
					Cal_set_en = 0;
				}

				//---���� "TAPE" ���ܼ� ------------
				if(Cal_tape_en)
				{
					if((Active_mode & 0x1f) == 0x00)
					{
						Tape_en = 1;
					}
					else
					{
						unsigned char set_step;
						set_step = Active_mode & 0x1f;		//�õ� �趨 �궨 �� ����

						if((Active_mode & 0x20) == 0x20)	//�����궨ģʽ
						{
							switch(set_step)	//ע�⣺�궨ģʽ�£�31��(0x1f)����ȡ��㣬30��(0x1e)����ȡ�궨��
							{
								case 1:	 	Active_mode ++;break;// �ֶ�	ȷ��
								case 2:		Active_mode += 2;break;// С���� ȷ��
								//case 3:		// ׷�� ȷ��
								//			Active_mode ++;	//ֱ�ӽ��� ��һ�궨�׶�
								//			break;
								case 4:		Active_mode = 0x3f;		//ֱ�ӽ��� ��� �趨 (0x3f)
											Warring_counter = 0;
											Ave_step_en = 1;
											break;
								case 5:		//--���� ��� ����
											if(A_input_setp == 6)
											{
												A_input_setp = 0;
												Core_data.ee_struct.Cal_max_weight = Price / 1000;//�� ��ͷ������������� ȡ����������������
												Active_mode = 0x3f;		//ֱ�ӽ��� ��� �趨 (0x3f)
												Warring_counter = 0;
												Ave_step_en = 1;
												Price = data_trans_out(Core_data.ee_struct.Cal_inter_weight); //��ʾ�궨����
											}
											else
											{
												A_input_setp ++;	//���� ��ͷ���� ��������;
												A_cycle_num = 0;
											}
											break;
							    case 6:		//--���� �궨�� ����
											if(A_input_setp == 6)
											{
												A_input_setp = 0;
												Core_data.ee_struct.Cal_inter_weight = Price; //�洢�궨���� ���ڻָ���ʾ
												//--�˴�û�� break,��ֱ�ӽ��� case 7 ���� �궨��ȡ��
											}
											else
											{
												A_input_setp ++;	//���� ��ͷ���� ��������;
												A_cycle_num = 0;
												break;
											}
								case 7:		Active_mode = 0x3e;		//ֱ�ӽ��� �궨�� �趨 (0x3e)
											Warring_counter = 0;
											Ave_step_en = 0;
										//	Core_data.ee_struct.Cal_inter_weight = Price;
											break;
/*								case 8:		Active_mode = 0x20 + 0x1c;		//ֱ�ӽ��� �궨��2 �趨 (0x3c)
											Warring_counter = 0;
											Ave_step_en = 0;
											break;
								case 9:		Active_mode = 0x20 + 0x1b;		//ֱ�ӽ��� �궨��3 �趨 (0x3b)
											Warring_counter = 0;
											Ave_step_en = 0;
											break;*/
								case 0x0b:
								case 0x0c:	
								case 0x0d:	
								case 0x0e:	
								case 0x0f:	if(!Cal_temp_bit) // Filt--
												Cal_temp_bit = 1;
											else
											{
												Cal_temp_bit = 0;
												core_memory_write(); //��Ҫ����
												delay_ms(100);
											//	Save_user_data = 1;
												buzzer(0);
											}
											break;	
								case 0x15:	Active_mode += 2;	//ֱ�ӽ�������Ƶ����ʾ set_step == 3
											break;
								case 0x16:	//--���� ��������
											if(A_input_setp == 6)
											{
												Active_mode ++;			//ֱ�ӽ�������Ƶ����ʾ set_step == 3
												A_input_setp = 0;
												Money = Price;	//�趨���� //��ԭ��We	ight_HH��Money���������
											}
											else
											{
												A_input_setp ++;	//���� ��ͷ���� ��������;
												A_cycle_num = 0;
											}
											break;
								case 0x17:	Active_mode += 2;	//ֱ�ӽ��� ������Χ�趨
											break;
								case 0x18:	//--���� ��������
											if(A_input_setp == 6)
											{
												Active_mode ++;	//ֱ�ӽ��� ������Χ�趨
												A_input_setp = 0;
												Money_accum.w = Price;	//�趨���� //��ԭ��Weight_LL��Money_accum.w���������
											}
											else
											{
												A_input_setp ++;	//���� ��ͷ���� ��������;
												A_cycle_num = 0;
											}
											break;
								case 0x19:	//--��� �����趨
											Save_user_data = 1; 		//�洢ϵͳ����
											Active_mode &= 0xc0;
											WARRING_SET_SET;		//�趨�����趨��־λ
											break;
								case 0x1d:	Active_mode &= 0xe0;
											Active_mode += 0x15;
											break;										
								default:	break;
							}//--switch(set_step)
						}//--if((Active_mode & 0x20) == 0x20)	//�����궨ģʽ
						else
						if((Active_mode & 0xe0) == 0x40)	//����ģʽ�µ�ȡ���趨
						{
							switch(set_step)
							{
								case 1:		Active_mode ++;
											break;	
								case 2:		//ȡ��Ʒ����������
											if( Sign || (Weight_internal_last == 0))
											{
												Active_mode ++;
											}
											else
											{
												Active_mode = 0x5f;	
												Warring_counter = 0;
												Ave_step_en = 0;
											}
											break;
								case 3:		Active_mode = 0x00; 	//������������ģʽ
											init_cal_point();		//�˴����� ��ȡ ֮ǰ�� EEPROM ���ڻָ� �趨Ϊ�ɹ��ƻ�������
											break;
								default:	break;
							}//--switch(set_step)
						}//--if((Active_mode & 0xe0) == 0x40)	//����ģʽ�µ�ȡ���趨
						else
						if((Active_mode & 0x80) == 0x80)
						{	
						    core_memory_write();
							delay_ms(200);
							buzzer(0);
						}
					}//---else---if((Active_mode & 0x1f) == 0x00)
					Cal_tape_en = 0;
				}//---if(Cal_tape_en)
		} //---if(!KEY_PRESS && (Cal_zero_en || Cal_unit_en || Cal_mode_en || Cal_set_en || Cal_tape_en))


		//************************************************
		//---��������-----------------end---------------
		//************************************************

		//************************************************
		//---��ʱ�䰴������-----------------begin---------
		//************************************************
		if(Key_counter_en && ((Active_mode & 0x3f) == 0) ) // ��ֹ�� ������С���� ��ԭ���� ��ͻ
		{
			unsigned char j;
			Led_dsp_updata = 1; //LED��ʾ��Ҫ����
			//--��������: �����ػ� --begin--
			#ifdef KEY_PRESS_PD_FUN_EN
				if(Mcu_stop_rec || Mcu_stop_pre)
					j = PRESS_KEY_COUNTER_NUM_2;
				else
			#endif
			//--��������: �����ػ� --end--
					j = PRESS_KEY_COUNTER_NUM;

			if( (ADbuf_init.b[0] == TAPE) && (Stable_light_counter >= j) )
			{
				Key_counter_en = 0;
				Stable_light_counter &= 0x0f;
				//�Ǳ궨ģʽ�Ļ�:
				if((Active_mode== 0x00)||(Active_mode== 0x40))	//����� ����/���� ������ʾ״̬�Ļ��������������趨
				{
					Active_mode += (0x20 + 0x1d); // 0x20 + 0x1d
				}
				buzzer(0);
				Cal_tape_en = 0;
			}

			if( (ADbuf_init.b[0] == ZERO) && (Stable_light_counter >= j) )//�궨
			{
				Key_counter_en = 0;
				Stable_light_counter &= 0x0f;
				if((Active_mode & 0x3f) == 0)	// �� �궨�趨״̬��
				{
					Active_mode |= 0x21;
					Active_mode &= 0xe1;		//����궨ģʽ �ĵ�һ��״̬
					Data_error = 0;
					Imperial_bit = 0; //ÿ�ν���궨�����Ӣ��
				}
				buzzer(0);
				Cal_zero_en = 0;
			}
			if( (ADbuf_init.b[0] == SET_KEY) && (Stable_light_counter >= j) )
			{
				Key_counter_en = 0;
				Stable_light_counter &= 0x0f;
				Cal_tape_en = 0;
				if((Active_mode & 0x3f) == 0)// �Ǳ궨�趨״̬��
				{
					Active_mode = 0x88;
					change_num_a = 1;				
					Cal_temp_bit = 0;
					Data_error = 0;
				}
				buzzer(0);
				Cal_set_en = 0;
			}
		}
		//************************************************
		//---��ʱ�䰴������-----------------end---------
		//************************************************

		//---------------------------------------------------------------
		//-- ģʽ���� --
		if((Active_mode & 0x3f) == 0x00) //���� ���� ���� �ٷֱ� �ж�
		{
			if(((Active_mode & 0xc0) == 0x40) && COUNT_SET_DIS)		//����ģʽ,��û�н��м���ȡ��
				Active_mode |= 0x01;	
		//-- ��������: �ٷֱ� -- begin --
		#ifdef PERCENTAGE_FUN_EN
			if(((Active_mode & 0xc0) == 0x80) && PERCENT_SET_DIS)	//�ٷֱ�ģʽ,��û�н��аٷֱ�ȡ��
				Active_mode |= 0x01;
	  	#endif
		//-- ��������: �ٷֱ� -- end --
		}
		//---------------------------------------------------------------
		//-- �洢�û����� ---
		if(Save_user_data)
		{
			System_data.sys_struct.Weight_HH = Money;					//��ԭ��Weight_HH��Money���������
			System_data.sys_struct.Weight_LL = Money_accum.w;			//��ԭ��Weight_LL��Money_accum.w���������
			uer_memory_write();
			Save_user_data = 0;
			init_cal_point();
			delay_ms(100);
			buzzer(0);
		}

		//************************************************
		//---ADC���ݴ���-----------------begin---------
		//************************************************
	   	if(ADready)
		{
			EA = 0; //���ݴ����ڼ� ���� -- �ر��ж�
			Filter_ADdata();
           	if(Start_up_end)		//�ǿ�������
		   	{
				internal_weight_cal(); //������λ ��ʾ����
				ext_weight_cal();	
			}
			else		//�������� ��"A_input_setp"����ʾ��������
           	if(A_input_setp < 0x0a)
		   	{
				Follow_zero_counter ++;
				if(Follow_zero_counter >= 7)
				{
				    Follow_zero_counter = 0;
				    A_input_setp++;	
					Led_dsp_updata = 1;
					if(A_input_setp == 0x0a)
					{
						Vol_state = 1;	
						Zero_weight_accum = power_detect();
					}
		           	//buzzer(0);
				}
			}						

			//-----------��̬ȡ����------------begin------------
			//Warring_counter = 0;  //��������ȡʱ����ʼ��������
			//ע�⣬�ڶ�̬ȡ���� ʱ Ҫʹ�õ� ��Counter_sample_weight��
			if( (!Start_up_end&&(A_input_setp==0x0a)) || (Active_mode == 0x3f) || (Active_mode == 0x3e))
			{
				unsigned char j;
				bit average_end;
				average_end = 0;
				if((Active_mode == 0x3f) || (Active_mode == 0x3e))
					j = LONG_FILTER_NUM;
				else
					j = LONG_FILTER_NUM_2;

				if(Warring_counter == 0) //�ս����ʱ�򣬽� Price ���
				{	
					Price = 0;
					Vol_auto_det_counter = 0;
					System_data.sys_struct.Counter_sample_weight = ADcode;
				}	
				Warring_counter ++;			
				if(Ave_step_en)		//��ʼִ�в���
				{
					if(labs(ADcode - System_data.sys_struct.Counter_sample_weight) <= 7) //��������ȶ���һ����Χ�ڵĻ�
					{
						Price += ADcode;
						Vol_auto_det_counter ++;
					}
					else  //�䶯�ϴ����������ȡ
					{
						Price = 0;
						Vol_auto_det_counter = 0;
						System_data.sys_struct.Counter_sample_weight = ADcode;
					}
				}
				else	//����ǰ����������---��ֹ����ʱ����--(��ͨ���趨 Sample_step_en ���趨�Ƿ���Ҫȥ����)
				if(	Warring_counter >= AVE_DIS_NUM )	 //-- ע�� -- : SAMPLE_DIS_NUM ����С�� LONG_FILTER_ACCUM_NUM
				{
					Warring_counter = 0;
					Ave_step_en = 1;
				}

				if(Vol_auto_det_counter >= j) //ȡ����ȶ����룬��ƽ��
				{   
					Price /= j;
					average_end = 1; //��Per_cal = 0;��ʾȡ�ȶ��������
				}
				else
				if(Warring_counter >= LONG_FILTER_ACCUM_NUM) //ȡ40���������ܴﵽ�ȶ�������£�ֱ���˳�
				{   
					Price = ADcode;					
					average_end = 1; //��Per_cal = 0;��ʾȡ�ȶ��������
				}

				if(average_end)
				{
					if(!Start_up_end&&(A_input_setp==0x0a))
					{
					    Zero_code = Price;		//ȷ����������
						A_input_setp = 0;
						Vol_state = 0;
						Weight = 0;
						Price = 0;																																																																																																														  
						//--�������---
						//if(PRICE_ZERO_DSP_DIS)
						//{
						//	Price_light_symbol = 0; //�������� ���� ���� �Ƿ�������
						//	LCD_LAMP_OFF; 			// �رձ����	
						//}
						//--���� ���� ׷������Χ
						Startup_zero_symbol = 1;
						Zero_weight_accum = 0;
						Start_up_end = 1;
						Active_mode = 0x00;
						//�ָ� ȡ��λʱ�ƻ�������
						init_cal_point();
						Led_dsp_updata = 1;
						buzzer(0);
						//--������λ���
						if(INIT_ZERO_SET_EN && (Zero_code >= 2220000)) //��λ ���� ���� ������ 2100000����
							Weight_on_error = 1;
					}
					else
					if(Active_mode == 0x3f) //ȡ�����
					{
					    Zero_code = Price; 	// �궨 ��һ��,ȷ���������;
						Active_mode = 0x27;	// = 0x20 + 7
						Zero_weight_accum = 0;
						Price = data_trans_out(Core_data.ee_struct.Cal_inter_weight); //��ʾ�궨����
					}
				//--��������: ���α궨 -- begin --
				#ifdef THREE_CAL_FUN_EN
					else
					if(Active_mode == 0x3e) //ȡ�ñ궨�� 1
					{
						if(Price > Zero_code)
						{
							Core_data.ee_struct.Cal_point_code = Price - Zero_code;  //�õ���һ���궨��	
							Active_mode = 0x28;	// = 0x20 + 8
							buzzer(1);
						}
						else
						{
							buzzer(1);
							buzzer(1);
							init_cal_point();  //�����ط�Ҳ��Ҫע�� ��Cal_point_code���Ĵ�С
							Active_mode &= 0xdf;	//�˳��궨ģʽ
							Active_mode &= 0xc0;		
						}																																																			
					}
					else
					if(Active_mode == 0x3c) //ȡ�ñ궨�� 2 �Ĳ�����
					{
						if(Price > Zero_code)
						{
							Price -= Zero_code;  //�õ��ڶ����궨��	
							Money = Core_data.ee_struct.Cal_point_code << 1;
							if(labs(Price - Money) > (Core_data.ee_struct.Cal_point_code / 4))
							{
								Core_data.ee_struct.Cal_delta_1 = 0;
								Get_cal_data = 1;
							}
							else
							if(labs(Price - Money) > 20000)
							{
								Core_data.ee_struct.Cal_delta_1 = 0;
								Get_cal_data = 1;
							}
							else
							{							
								Core_data.ee_struct.Cal_delta_1 = Price - Money;
								Active_mode = 0x29;	// = 0x20 + 9
								buzzer(1);
							}
						}
						else
						{
							buzzer(1);
							buzzer(1);
							init_cal_point();  //�����ط�Ҳ��Ҫע�� ��Cal_point_code���Ĵ�С
							Active_mode &= 0xdf;	//�˳��궨ģʽ
							Active_mode &= 0xc0;
							Led_dsp_updata = 1;		
						}																																																			
					}
					else
					if(Active_mode == 0x3b) //ȡ�ñ궨�� 3 �Ĳ����� / ��ɱ궨
					{
						if(Price > Zero_code)
						{
							Price -= Zero_code;  //�õ��ڶ����궨��	
							Money = Core_data.ee_struct.Cal_point_code * 3;
							if(labs(Price - Money) > (Core_data.ee_struct.Cal_point_code / 2))
							{
								Core_data.ee_struct.Cal_delta_2 = 0;
							}
							else
							if(labs(Price - Money) > 20000)
							{
								Core_data.ee_struct.Cal_delta_2 = 0;
							}
							else
							{
								Core_data.ee_struct.Cal_delta_2 = Price - Money;
								Core_data.ee_struct.Cal_delta_2 -= 	Core_data.ee_struct.Cal_delta_1; //Ҫ������				
								Core_data.ee_struct.Cal_delta_2 -= 	Core_data.ee_struct.Cal_delta_1;
							}
							Get_cal_data = 1;
						}
						else
						{
							buzzer(1);
							buzzer(1);
							init_cal_point();  //�����ط�Ҳ��Ҫע�� ��Cal_point_code���Ĵ�С
							Active_mode &= 0xdf;	//�˳��궨ģʽ
							Active_mode &= 0xc0;
							Led_dsp_updata = 1;	
						}
					}
				#else
					else
					if(Active_mode == 0x3e) //ȡ�ñ궨�� 1
					{
						if(Price > Zero_code)
						{
							Core_data.ee_struct.Cal_point_code = Price - Zero_code;  //�õ���һ���궨��
							Get_cal_data = 1;
						}
						else
						{
							buzzer(1);
							buzzer(1);
							init_cal_point();  //�����ط�Ҳ��Ҫע�� ��Cal_point_code���Ĵ�С
							Active_mode &= 0xdf;	//�˳��궨ģʽ
							Active_mode &= 0xc0;
							Led_dsp_updata = 1;		
						}																																																			
					}
				#endif
				//--��������: ���α궨 -- end --
					
					if(Get_cal_data)
					{
						Get_cal_data = 0;
						//����� ȡ�궨�� ����
						Price = Core_data.ee_struct.Cal_inter_weight;
						Money = Core_data.ee_struct.Cal_point_code;
						//---------����궨����---------
						//�궨���� ������� MONEY ��
						//�궨���� ����ŵ� Price ��
						//���õ��м���� Money_accum.w
						//������� data_error = 1;
						//�궨��ȷ�Ļ� �õ���ȷ��"Core_data.ee_struct.Cal_point_code"
						cal_data_process();
						//--
						Money = 0;
						Money_accum.w = 0;

						if(Data_error) //�궨���󣬱����˳���
						{
							buzzer(1);
							buzzer(1);
						}
						else	//��ʼ ���� ����������
						{
							if(Active_mode == 0x3b)
								Price *= 3;
							Weight_internal_last = data_trans_into();
							Weight = Price;		
							//SYSTEM_CAL_SYMBOL_CLR;
							//SYSTEM_CAL_SYMBOL_SET;
							follow_zero_cal();
							core_memory_write();
							//Weight_updata_en = 1;
						}
						//����˴�����ʱ�ظ���ȷ�ı궨ֵ����Cal_point_code���Ĵ�С������������ɡ�ѭ����������������
						//�ָ� ��Counter_sample_weight��
						Data_error = 0;
						init_cal_point();  //�����ط�Ҳ��Ҫע�� ��Cal_point_code���Ĵ�С
						Active_mode &= 0xdf;	//�˳��궨ģʽ
						Active_mode &= 0xc0;
						delay_ms(100);
						buzzer(0);					
						Led_dsp_updata = 1;
					}
				}
			}			
			//-----------��̬ȡ����------------end------------	
			//���� �趨��ʽ ʱ ÿ�ζ���Ҫ����
			//����鿴 ��Ҫÿ�θ��� 
			//�ٷֱ� ��������ʱÿ�ζ�����
			if( ((Active_mode & 0x20) == 0x20) || (Active_mode == 0x0a) || (Active_mode == 0x82) )
				Led_dsp_updata = 1;
//--SDI5219---
			if((Active_mode & 0x20) == 0x20)	//�����궨ģʽ
			{
			//-- ��������: �궨״̬�� ˢ����ʾ Ƶ�ʽ��� -- begin --
			#ifdef SDI5219_CAL_DSP_UPDATA_SLOW	
				if (Cal_dsp_slow_counter >= CAL_DSP_SLOW_NUM) // ǰ�μ۸������𣬳���6����ٴ�����۸񰴼�����ǰ��۸񲻹�����
				{
					Cal_dsp_slow_counter = 0;		
					Led_dsp_updata = 1;
				}
				else
				{
					Cal_dsp_slow_counter ++; //�۸���������� ���� ADC���ݸ��� ��ʱ
				}
			#else
				Led_dsp_updata = 1;
			#endif
			//-- ��������: �궨״̬�� ˢ����ʾ Ƶ�ʽ��� -- end --	
			}
			//Dsp_en = 1;
			EA = 1;  //���ݴ����ڼ� ���� -- �ر��ж�
		}
		//************************************************
		//---ADC���ݴ���-----------------end---------
		//************************************************


		//************************************************
		//---(ʱ����ع��ܿ�)-----------------begin-------
		//************************************************
//--SDI5219---
		if(Data_sycle_symbol)
		{
		Data_sycle_symbol = 0;
		//----- �Զ���ص�ѹ��� ------
		//----- ���� ---------
		//�������趨״̬�ż���أ��ű��������� ���� �趨ʱ "Vol_auto_det_counter" "Warring_counter" �ɸ���	
		if(Start_up_end && ((Active_mode & 0x1f) == 0x00))	
		{
			//ע�⣬���趨״̬�£�������ص�ѹ���������� Vol_auto_det_counter �������ڱ��趨ʱ�������ط�
			if(Vol_state)
			{
				Vol_auto_det_counter = 0;	
			}
			else
			if(Vol_auto_det_counter >= VOL_AUTO_DET_NUM) //��ص�ѹ���
			{
				power_detect();
				Vol_auto_det_counter = 0;
				Led_dsp_updata = 1; //��һ���ȶ�ʱ����Ҫ������ʾ
			}
			else
			{
				Vol_auto_det_counter ++;
			}

			//ע�⣬���趨״̬�£����������������� Warring_counter �������ڱ��趨ʱ�������ط�
		//-- ��������: �����ػ� -- begin --
		#ifdef KEY_PRESS_PD_FUN_EN	
			if( (Weight_overflow || Lamp_lowv || Lamp_lowv_a || Weight_warning_h || Weight_warning_l || Weight_warning_in ) && !Mcu_stop_pre && !Mcu_stop_rec )//----- ��������������,�����ػ�������²�����������Ϊ���ʱ����Ƭ��ʵ����ʱ������
		#else
			if( Weight_overflow || Lamp_lowv || Lamp_lowv_a || Weight_warning_h || Weight_warning_l || Weight_warning_in )//----- ��������������,�����ػ�������²�����������Ϊ���ʱ����Ƭ��ʵ����ʱ������
	    #endif
		//-- ��������: �����ػ� -- end --
			{
				Warring_counter++;
				if(Lamp_lowv_a)
				{
					if(Warring_counter >= WARRING_NUM_LOWV_2)
					{
						Warring_counter = 0;
						buzzer(0);
					}
				}
				else
				if(Lamp_lowv)
				{
					if(Warring_counter >= WARRING_NUM_LOWV_1)
					{
						Warring_counter = 0;
						buzzer(0);
					}
				}
				else
				if(Weight_warning_h)
				{
					if(Warring_counter >= WARRING_NUM_HIGH)
					{
						Warring_counter = 0;
						buzzer(0);
					}
				}
				else
				if(Weight_warning_l)
				{
					if(Warring_counter >= WARRING_NUM_LOW)
					{
						Warring_counter = 0;
						buzzer(0);
					}
				}
				else
				{
					if(Warring_counter >= WARRING_NUM_FULL)
					{
						Warring_counter = 0;
						buzzer(0);
					}
				}
			}
			else
			{
				Warring_counter = 0;
			}
		}

		//----- ������������ --- begin ---
		if(Sleep_dsp_mode) //��� ����
		{
			if(Weight_stable_counter >= WEIGHT_SLEEP_NUM)
			{
				Weight_sleep = !Weight_sleep;	//��������������ʾ
				Weight_stable_counter = 0;
				Led_dsp_updata_2 = 1;
			}
			else
			{
				Weight_stable_counter ++;	
			}	
		}
		//----- ������������ --- end ---

		//----- ��ⳤʱ�䰴�� --- begin ---	
		if(Key_counter_en)
		{
			if(Stable_light_counter < PRESS_KEY_COUNTER_NUM)
			{
				if(Double_counter_symbol)
				{
					Stable_light_counter += 0x10;
					Double_counter_symbol = 0;
			   	}
				else
					Double_counter_symbol = 1;
			}
		}
		//----- ��ⳤʱ�䰴�� --- end ---

		//----- ��̬������ --- begin ---
		{
			unsigned char j;
			//--1/4��Ъʱ ���� ��ʱ����

		//--��������: �����ػ� --begin--
		#ifdef KEY_PRESS_PD_FUN_EN
			if(Mcu_stop_rec || Mcu_stop_pre)
				j = CHARGE_COUNTER_NUM / 5;
			else
	   	#endif
		//--��������: �����ػ� --end--
				j = CHARGE_COUNTER_NUM;
			//--�����⵽���--
			if(Charte_bit)	//--�����⵽��� �ҳ��δ���--
			{
				if(Charte_bit_dis)
				{
					Batter_c = 1;		
					Batter_b = 1;		
					Batter_a = 1;
					Led_dsp_updata = 1;
					Dsp_en = 1;
				}
				else
				if (Charge_counter >= j) //��̬���
				{
					Charge_counter = 0;

					if(!Batter_c && !Batter_c_ch)
					{
						Batter_c = 1;		
					}
					else
					if(!Batter_b && !Batter_b_ch)
					{
						Batter_b = 1;		
					}
					else
					if(!Batter_a)
					{
						Batter_a = 1;		
					}
					else
					{
						if(!Batter_c_ch)
							Batter_c = 0;		
						if(!Batter_b_ch)
							Batter_b = 0;				
						Batter_a = 0;
					}
					Led_dsp_updata = 1;
					Dsp_en = 1;
				}
				else
				{
					Charge_counter ++;
				}
			}
			else
			{
				Charge_counter = 0;
			}	
	  	}
		//----- ��̬������ --- end ---
		}
//--SDI5219---
		//************************************************
		//---(�趨 �洢���� 9)-----------------end-------
		//************************************************

		//************************************************
		//--- ��ʾ���� -----------------begin-------
		//************************************************
	#ifdef KEY_PRESS_PD_FUN_EN
		if( Charte_bit || !Mcu_stop_rec)
	#endif
		{	
		//--- ��������: ������ʾ���� -- begin ---
		#ifdef LOW_DSP_UPDATA
			if(Led_dsp_updata && Low_speed_updata)
		#else
			if(Led_dsp_updata)
		#endif
		//--- ��������: ������ʾ���� -- end ---
			{
				//--Һ��--
				display_cal(1); //��������
				//--- ��ֹҺ�� ���� ---
				//--IO������--
				IO_MODE_DSP_EN;
				//��ʼ��LCDģʽ
				write_lcd(0,0x01);
				write_lcd(0,0x03);
				write_lcd(0,0x18);
				write_lcd(0,0x29);
				write_lcd(0,0xe3); //��������ģʽ
				//---				
				write_lcd(4,0);	// ����LED����
				//--IO������--
				IO_MODE_DSP_DIS;
				//����
				display_cal(0); //��������
				//--IO������--
				IO_MODE_DSP_EN;
				dsp_updata(0);// ����LED����
		//--- ��������: ����1616���� -- begin ---
		//--- 1616������,dsp_updata(0)���߱�����ʾ���ܣ�������Ҫ�����һ��
		#ifdef LED_DRIVER_1616
				dsp_updata(2);// ����LED����
		#endif
		//--- ��������: ����1616���� -- end ---
				//--IO������--
				IO_MODE_DSP_DIS;
				Led_dsp_updata = 0; //������ʾ���������
				Led_dsp_updata_2 = 0;
			}
			else
			if(Led_dsp_updata_2)
			{
				if(Sleep_dsp_mode) //��û�м�Ъ�������������µ���������				
				{
					//--IO������--
					IO_MODE_DSP_EN;
					if(!Weight_sleep)
						dsp_updata(2);// ������	
					else
					if(!Charte_bit)
						dsp_updata(1);// �ر�����
					//--IO������--
					IO_MODE_DSP_DIS;
				}
				Led_dsp_updata_2 = 0; //������ʾ���������
			}
		}

		//************************************************
		//--- ��ʾ���� ----------------- end -------
		//************************************************

//------ ��� Flash ��ȫ�� -- beign --
		//ע��:
		//�����̬ȡ����ʱ������NRM_securty_a,��ͱ궨д��ֱ����û�а�������NRM_securty_a���ᵼ���޷�д��
		//�����Ҫ���� �����ж�
		//��:if((Cal_step != 6)&&(!Sample_step_2))��
		if( (Active_mode != 0x3b) && (Active_mode != 0x3c) && (Active_mode != 0x3e) && (Active_mode != 0x5f) && (Active_mode != 0x9f)  && (!Save_user_data) && (!Key_press))
			NRM_securty_a = 0x00;	//Flash ��ȫ��֤�� A
		NRM_securty_b = 0x00;	//Flash ��ȫ��֤�� B
//------ ��� Flash ��ȫ�� -- end --

		//************************************************
		//---  ���ߴ��� ----------------- begin -------
		//************************************************
//--SDI5219---
		//--STOPǰ��IO����
		//CHARGE_SYMBOL = 0;		//(CHARGE_SYMBOL)����Ϊ0,��ֹ���ĵ���
		//--- ֻ����"//���״̬�£�����û�а����ػ��������"�Ž�������
	//--��������: �����ػ� --begin--
	#ifdef KEY_PRESS_PD_FUN_EN
		if((!Mcu_stop_pre && !Mcu_stop_rec) || Charte_bit)	//���״̬�£�����û�а����ػ��������
   	#endif
	//--��������: �����ػ� --end--
		{	
			//--�ر�ʱ��IO����--
			//--��ѹ���,������� Ӧ�ö�Ҫ�趨Ϊ������ģʽ,��ֹ�ڲ����ĵ���	
			//---�رտ��Ź�
			EA = 0;
			WD_TA = 0x05;
			WD_TA = 0x0a;
			WDCON = 0xaf;	//���Ź�ʱ�� 4s��,(�ر�)��Ƶ����0.2s
			EA = 1;	
			_nop_();
			_nop_();
			//************************************************
			//--- ���� STOP ----------------- end -------
			//************************************************
			//--ֻ���ж�MCU��ʱ�ӣ�SG-ADC�����𵴴��ڹ���״̬
			//--��Ƶ���� �� SG-ADC ���ܻ���MCU
			PCON |= 0x02; //STOP1 ----MCU��ͣ�ڴ˴�
			//---�򿪿��Ź�
			EA = 0;
			WD_TA = 0x05;
			WD_TA = 0x0a;
			WDCON = 0xbf;	//(SDI5216)���Ź�ʱ�� 4s��,(�ر�)��Ƶ����0.2s
			EA = 1;	
		}
		//************************************************
		//---  ���ߴ��� ----------------- end -------
		//************************************************
//--SDI5219---
		//************************************************
		//---  �����Լ�IO��� ----------------- begin ----
		//************************************************
		EA = 0;
		//--IO������--
		IO_MODE_KEY_A_EN;
		// ��ֹ������ KEY_PIN1 - KEY_PIN4 д1

		KEY_PIN1 = 1;
		KEY_PIN3 = 1;
		// ����Ƿ��н�����
		KEY_PIN5 = 1;
		KEY_PIN7 = 1;
		//KEY_PIN9 = 0;

		delay_ms(0);

		// ��� KEY_1,KEY_2,KEY_3,KEY_4Ϊ1,��˵��û�м�������
		if(KEY_PIN1 && KEY_PIN3 && KEY_PIN5 && KEY_PIN7)
	//	if(KEY_PIN1 && KEY_PIN4)
		{
			Key_press = 0;
			Key_response = 0;
			Key_counter_en = 0;
			Stable_light_counter &= 0x0f;
		}
		else
	 	{
			Key_press = 1;															
	   	}
		//--IO������--
		IO_MODE_KEY_DIS;
		//--���� �������
		CHARGE_SYMBOL = 1;
		//CHARGE_SYMBOL = 0;	//���ν������
		if(CHARGE_SYMBOL)
		{
			Charte_bit = 1;
			//--������ѹ����
			Lamp_lowv_a = 0;
		}
		else
		{	
			if(Charte_bit) //�˳���̬���ʱ�Զ����һ�µ�ص�ѹ
			{
				if((Active_mode & 0x1f) == 0x00)  
					Vol_auto_det_counter = VOL_AUTO_DET_NUM;
				Power_det_mode = 1;
			}
			Charte_bit = 0;
			Charte_bit_dis = 0;
			Batter_b_ch=0;
			Batter_c_ch=0;
		}
		EA = 1;
		//************************************************
		//---  �����Լ�IO��� ----------------- end ----
		//************************************************																								
	}//---while(1)
}//----main()

// -- ��ͷ���� �������� ������
void a_input_analyse(void)
{
	unsigned long step;

	switch(A_input_setp)
	{
		case 1:	step = 100000;
				break;
		case 2:	step = 10000;
				break;
		case 3:	step = 1000;
				break;
		case 4:	step = 100;
				break;
		case 5:	step = 10;
				break;
		case 6:	step = 1;
				break;
		default: break;
	}

	if(A_cycle_num == 9)
	{
		Price -= (9 * step);
		A_cycle_num = 0;
	}
	else
	{
		Price += step;
		A_cycle_num ++;
	}	
}

//--------------------------------
//----- ��ʱ����,num=1,��ʱ1ms ---
//--SDI5219---
void delay_ms(unsigned int num)
{ 	
	unsigned int i,j;
  	for(i= 0;i<num;i++)	//(SDI5219) ��ʱ1ms
//-- ��������: 2��Ƶʱ��ģʽ -- begin --
#ifdef SDI5219_HALF_SPEED
		for(j=0;j<96;j++)
			;
#else
		for(j=0;j<164;j++)
			;
#endif
//-- ��������: 2��Ƶʱ��ģʽ -- end --
}

//--------------------------------
//----- ���������� ---
//--SDI5219---
void buzzer(unsigned char mode)
{
	unsigned char j;
	unsigned char i=0;
	//--ע�⣬�������е��� Һ������ ���ڴ����� ���ܱ��2�ֽ�
	//write_lcd(0,0x09);	//������
	EA = 0; //��ֹ�����ж�
	//BUZZER_P = 1;
	//BUZZER_N = 1;
//-- ��������: 2��Ƶʱ��ģʽ -- begin --
#ifdef SDI5219_HALF_SPEED
	while(i<200)	//(SDI5219)2KHz ����50ms
	{
		BUZZER_P = !BUZZER_P; //PB2 ȡ��;
		for(j=0;j<57;j++)
		{
			//_nop_();
			//_nop_();
			//_nop_();
			//_nop_();
		}
		i++;
	}
#else
	while(i<200)	//(SDI5219)2KHz ����50ms
	{
		BUZZER_P = !BUZZER_P; //PB2 ȡ��;
		for(j=0;j<111;j++)
		{
			//_nop_();
			//_nop_();
			//_nop_();
			//_nop_();
		}
		i++;
	}	
#endif
//-- ��������: 2��Ƶʱ��ģʽ -- end --
	//------------
	BUZZER_P = 0;
	//BUZZER_N = 0;
	//--ע�⣬�������е��� Һ������ ���ڴ����� ���ܱ��2�ֽ�
	//write_lcd(0,0x08);	//������
	EXIF &= 0xbf;	//���ADC �жϱ�־λ
	EA = 1; //��ֹ�����ж�
}

//------------------------------------------------
//----- ��ȡEEPROM�ı궨�����ݣ���ʼ�궨���ӳ� ---
void init_cal_point(void)
{	
	unsigned char Per_d_code,i;
	bit data_error_a;

	data_error_a = 0;
	Core_data.ee_struct.Sys_seting_1 = nvm_data_read_byte(ADDR_CAL_DATA + 0); // ��0���ֽ����ڴ�� �궨��ֵλ

	//����Ѿ����ù�������ԣ����ȡ�趨ֵ��δ�������ã���д��Ĭ��ֵ
	if( ((Core_data.ee_struct.Sys_seting_1 & 0xf0) == 0x60) ) // ��� ���� ���ο���
	{
		//---��ȡ���ı궨����---
		for(i=0;i<FLASH_DATA_LENTH;i++)
			Core_data.b[i] = nvm_data_read_byte(ADDR_CAL_DATA + i);
	
		//---���ı궨���� �쳣����--
		if(Core_data.ee_struct.Cal_point_code >= 600000)
			data_error_a = 1;
		else
		if(Core_data.ee_struct.Cal_point_code < 10000)
			data_error_a = 1;
		else
		if((Core_data.ee_struct.Intel_cal_d & 0x7f) > 64)
			data_error_a = 1;
		else
		{
			Per_d_code = (Core_data.ee_struct.Cal_point_code << INTER_WEIGHT_NUM) / CAL_POINT_W;//���� 1���ڲ��ֶȶ�Ӧ������
			if((Per_d_code < LOW_CAL_D_CODE)||(Per_d_code > UP_CAL_D_CODE))
			{
				data_error_a = 1;
			}
		}
		//--��ȡ �û� ����--
		for(i=0;i<SYS_DATA_LENTH;i++)
			System_data.b[i] = nvm_data_read_byte(ADDR_SYS_DATA + i);
	}
	else
	{
		data_error_a = 1;
	}
	
	if(data_error_a) // ��� ���ο��� �������ݴ���
	{
		SYS_SETING_1_DEFAULT;
		SYS_SETING_2_DEFAULT;
		SYS_SETING_3_DEFAULT;
		//--- for 0819 ---
		Core_data.ee_struct.Follow_zero_d = 6 << (INTER_WEIGHT_NUM-1); 		//Ĭ��3g׷��
		Core_data.ee_struct.Follow_zero_num = 2; //Ĭ��0.5d׷��
		Core_data.ee_struct.Cal_d = 1 << INTER_WEIGHT_NUM;				//Ĭ�Ϸֶ�1g (1g << INTER_WEIGHT_NUM)
		Core_data.ee_struct.Cal_max_weight = 30; 						//��ʼĬ������Ϊ��30kg��
		//--�� INTER_WEIGHT_NUM = 1(60000�ڲ������ֶ�),1���ڲ��ֶȶ�Ӧ10������
		//--CAL_POINT_W * 10 = 120000
		Core_data.ee_struct.Cal_point_code = (10 * (unsigned long)(CAL_POINT_W)) >> (INTER_WEIGHT_NUM - 1);	
		Core_data.ee_struct.Intel_cal_d = 1;
		Core_data.ee_struct.Cal_inter_weight = (unsigned long)(10000) << INTER_WEIGHT_NUM;		//ûʲô���ã���ȫ�ǵ����ĳ�ʼ��
	//--��������: ���α궨 -- begin --
	#ifdef THREE_CAL_FUN_EN
		Core_data.ee_struct.Cal_delta_1 = 0;
		Core_data.ee_struct.Cal_delta_2 = 0;
	#endif
	//--��������: ���α궨 -- end --
		//--�û�����--
		System_data.sys_struct.User_seting_1 = USER_SETING_1_DEFAULT;
		System_data.sys_struct.Weight_HH = 0;
		System_data.sys_struct.Weight_LL = 0;
		System_data.sys_struct.Counter_sample_num = 10;	
		System_data.sys_struct.Counter_sample_weight = (unsigned long)(10000) << INTER_WEIGHT_NUM;
		System_data.sys_struct.Per_sap_w = (unsigned long)(10000) << INTER_WEIGHT_NUM;	
		Core_data.ee_struct.ZERO_NUM = 30;
		Core_data.ee_struct.TAPE_NUM = 100;
		//core_memory_write();
	}

	if(Core_data.ee_struct.Follow_zero_num < 1)
		Core_data.ee_struct.Follow_zero_num = 1;
	
//---���� ���� --- 
	if(LCD_LAMP_OFF_MODE)
		Price_light_symbol = 0;
	else
		Price_light_symbol = 1;
//---Ϊ�˼���825����,�����±������޷ŵ� Money �� Money_accum.w ��
	Money = System_data.sys_struct.Weight_HH;
	Money_accum.w = System_data.sys_struct.Weight_LL;
	//--
	if((Money < Money_accum.w)&&(!WARRING_TYPE_NO))
	{
		Weight_ul_error = 1;
	}
	else
	{
		Weight_ul_error = 0;
	}

	Weight_stable_counter = 0;

//--��������: ���α궨 -- begin --
#ifdef THREE_CAL_FUN_EN
	//---��ֹ���β�������������
	if((Core_data.ee_struct.Cal_delta_1 > 20000) || (Core_data.ee_struct.Cal_delta_1 < -20000))
	{
		Core_data.ee_struct.Cal_delta_1 = 0;
	}
	if((Core_data.ee_struct.Cal_delta_2 > 20000) || (Core_data.ee_struct.Cal_delta_2 < -20000))
	{
		Core_data.ee_struct.Cal_delta_2 = 0;
	}
#endif
//--��������: ���α궨 -- end --
}

//д���Ĵ洢������
void core_memory_write(void)
{
	unsigned char i,j,temp;
	bit k;

	j = 0; //��¼һ��ѭ���Ĳ�д����
	k = 1;
	//Flash ��ȫ��֤�� B
	NRM_securty_b = 0x55;	//Flash ��ȫ��֤�� B
	//--------------------
	if(!SYSTEM_CAL_SYMBOL) // ��� �� ����
	{
		SYSTEM_CAL_SYMBOL_CLR;
		SYSTEM_CAL_SYMBOL_SET;
		//-- ���û�������
		uer_memory_write();	
	}	
	//--------------------
	while(k)
	{
		FLASH_ENA = 0x05;	//(�ŵ�������)
		e2rom_erase(ADDR_CAL_DATA); //����
		j++;
		k = 0; //׼���˳�
		for(i=0;i<FLASH_DATA_LENTH;i++)
		{
			FLASH_ENA = 0x05;	//nvm_data_write_byte ����������仰
			nvm_data_write_byte(ADDR_CAL_DATA + i,Core_data.b[i]);
			temp = nvm_data_read_byte(ADDR_CAL_DATA + i); //����У��
			if(temp != Core_data.b[i]) //������ֹؼ�����û��д�ԣ��������д������3��
			{
				k = 1;
			}
		}
		if(j>5)
		{
			buzzer(1);
			buzzer(1);
			k=0;
		}
	}	
}

//д �û����� �洢������
void uer_memory_write(void)
{
	unsigned char i,j,temp;
	bit k;
	j = 0; //��¼һ��ѭ���Ĳ�д����
	k = 1;
	//Flash ��ȫ��֤�� B
	NRM_securty_b = 0x55;	//Flash ��ȫ��֤�� B
	while(k)
	{
		FLASH_ENA = 0x05;	//(�ŵ�������)
		e2rom_erase(ADDR_SYS_DATA); //����
		j++;
		k = 0; //׼���˳�
		for(i=0;i<SYS_DATA_LENTH;i++)
		{
			FLASH_ENA = 0x05;	//nvm_data_write_byte ����������仰
			nvm_data_write_byte(ADDR_SYS_DATA + i,System_data.b[i]);
			temp = nvm_data_read_byte(ADDR_SYS_DATA + i); //����У��
			if(temp != System_data.b[i]) //������ֹؼ�����û��д�ԣ��������д������3��
				k = 1;
		}
		if(j>5)
		{
			k=0;
			buzzer(1);
			buzzer(1);
		}
	}
}

//--׷�㷶Χ����
void follow_zero_cal(void)
{
	//-- �趨׷�㷶Χ ----
	Core_data.ee_struct.Follow_zero_d = Core_data.ee_struct.Cal_d * Core_data.ee_struct.Follow_zero_num;
	if(Core_data.ee_struct.Intel_cal_d < 0x80)
	{
		Core_data.ee_struct.Follow_zero_d = Core_data.ee_struct.Follow_zero_d * Core_data.ee_struct.Intel_cal_d / 2;
	}
	else
	{	unsigned char i;
		i = Core_data.ee_struct.Intel_cal_d & 0x7f; 
		Core_data.ee_struct.Follow_zero_d = Core_data.ee_struct.Follow_zero_d / 2 / i;
	}
//	Core_data.ee_struct.Follow_zero_d *= Core_data.ee_struct.Follow_zero_num;
	//if(Core_data.ee_struct.Follow_zero_d < (7 << (INTER_WEIGHT_NUM-1)))
	//	Core_data.ee_struct.Follow_zero_d = (7 << (INTER_WEIGHT_NUM-1)); //Ĭ��ÿ�α궨�󣬶���0.5d < 3.5g�ģ�����3.5g׷��
}


//----------------------------------------------------------
//---   ����ص�ѹ   ----
unsigned int power_detect(void)
{
	unsigned int v;
	EA = 0;
//--- ���ڼ���ѹʱ �ر���ʾ --
//	if(!Power_det_mode)
//		dsp_updata(1); //�ر���ʾ
//	Power_det_mode = 0;
//--- ���IO������ ----
	//-- ��� ��ѹ���ܽźͼ��̹ܽŹ��õĻ�����Ҫ���������м��IO��Ϊ [������ģʽ]
//--- 
	BAT_DET_PIN_HIGH; // ��ص�ѹ���ű��, �����ص�ѹ��⽫������
	//--����ͨ��1�ĵ�ѹ
	SARCON  = 0x0b;  //ͨ��1 �ٶ���� ��Sar-ADC
	delay_ms(1);
	
	if(!(SARCON & 0x04)) // ADC��æ��ʼת��
	{
		SARCON |= 0x04;
		while(SARCON & 0x04) // �ȴ�ת������
		{
		}
	}

//--- ���ڼ���ѹʱ �ر���ʾ --
//-- ��������: �����ػ� -- begin --
//#ifdef KEY_PRESS_PD_FUN_EN
//	if((!Mcu_stop_pre && !Mcu_stop_rec) || Charte_bit)
//	{
//		dsp_updata(2); //����ʾ
//	}
//#else
//	dsp_updata(2); //����ʾ
//#endif
//-- ��������: �����ػ� -- end --

	v = SARDATA;
	v = (unsigned long)v * (unsigned long)V_CO_PRE / 0xff; // �����ص�ѹ

//--- �ָ� ���IO������ ----
	//-- ��� ��ѹ���ܽźͼ��̹ܽŹ��õĻ�����Ҫ���������м��IO��Ϊ [������ģʽ]

	SARCON  &= 0xf7;; //�ر�ADC
	EA = 1;

	if(v >= 20)
	{
		if(!Charte_bit)
		{
			if(POWER_6V_BIT_EN) //6v����
			{
				if(v  >= V_HIG_0) // ��ص��ڹ涨��ѹʱ��Ƿѹ����
					Batter_a=1;	// = 1 ��ʾ �������һ��			
				else
					Batter_a=0;	// = 1 ��ʾ �������һ��	
		
				if(v  >= V_HIG_1)
					Batter_b=1;	// = 1 ��ʾ ����м��һ��
				else
					Batter_b=0;	// = 1 ��ʾ ����м��һ��
		
				if(v  >= V_HIG_2)
					Lamp_lowv = 0;			
				else
					Lamp_lowv = 1;// ������ ��ʾ -- LB -- 			
		
				if(v  >= V_HIG_3)
				{
					Batter_c=1;	// = 1 ��ʾ �����С��һ��
					Lamp_lowv_a = 0;
				}
				else
				{
					Batter_c=0;	// = 1 ��ʾ �����С��һ��
					Lamp_lowv_a = 1;	//������ ��ʾ -- LB -- ������������������
				}				
			}
			else
			{
				if(v  >= V_LOW_0) // ��ص��ڹ涨��ѹʱ��Ƿѹ����
					Batter_a=1;	// = 1 ��ʾ �������һ��			
				else
					Batter_a=0;	// = 1 ��ʾ �������һ��	
		
				if(v  >= V_LOW_1)
					Batter_b=1;	// = 1 ��ʾ ����м��һ��
				else
					Batter_b=0;	// = 1 ��ʾ ����м��һ��
		
				if(v  >= V_LOW_2)
					Lamp_lowv = 0;
				else
					Lamp_lowv = 1;// ������ ��ʾ -- LB -- 
		
				if(v  >= V_LOW_3)
				{
					Batter_c=1;	// = 1 ��ʾ �����С��һ��
					Lamp_lowv_a = 0;
				}
				else
				{
					Batter_c=0;	// = 1 ��ʾ �����С��һ��
					Lamp_lowv_a = 1;	//������ ��ʾ -- LB -- ������������������
				}
			}
		}
		else
		{
			if(POWER_6V_BIT_EN) //6v����
			{
				if(v  >= V_HIG_1_CH)
				{
					Batter_b_ch=1;	// = 1 ��ʾ ����м��һ��
					Batter_b = 1;	// = 1 ��ʾ ����м��һ��
				}
				else
				{
					Batter_b_ch=0;	// = 1 ��ʾ ����м��һ��
				}
		
				if(v  >= V_HIG_2_CH)
				{
					Batter_c_ch=1;	// = 1 ��ʾ �����С��һ��	
					Batter_c =1;	// = 1 ��ʾ �����С��һ��		
				}
				else
				{
					Batter_c_ch=0;	// = 1 ��ʾ �����С��һ��	
				}
			}
			else
			{
				if(v  >= V_LOW_1_CH)
				{
					Batter_b_ch=1;	// = 1 ��ʾ ����м��һ��
					Batter_b = 1;	// = 1 ��ʾ ����м��һ��
				}
				else
				{
					Batter_b_ch=0;	// = 1 ��ʾ ����м��һ��
				}
		
				if(v  >= V_LOW_2_CH)
				{
					Batter_c_ch=1;	// = 1 ��ʾ �����С��һ��	
					Batter_c =1;	// = 1 ��ʾ �����С��һ��		
				}
				else
				{
					Batter_c_ch=0;	// = 1 ��ʾ �����С��һ��	
				}
			}
		}
	}

	//--����Ƿ��س���
	if(POWER_6V_BIT_EN) //6v����
	{
		if(Charte_bit_dis)
		{
			if(v  <= V_HIG_FULL_L) // ��ѹ<=�ڴ�ֵ ��ʾ������
				Charte_bit_dis=0;
		}
		else
		{
			if(v  >= V_HIG_FULL_H) // ��ѹ>=�ڴ�ֵ ��ʾ������
				Charte_bit_dis=1;
		}
	}
	else
	{
		if(Charte_bit_dis)
		{
			if(v  <= V_LOW_FULL_L) // ��ѹ<=�ڴ�ֵ ��ʾ������
				Charte_bit_dis = 0;
		}
		else
		{
			if(v  >= V_LOW_FULL_H) // ��ѹ>=�ڴ�ֵ ��ʾ������
				Charte_bit_dis=1;
		}
	}
	return(v);
}

//��Ƶ�����жϣ�*****************
static void wu_serve(void) interrupt 6
{		
	EA = 0;	 // ��ֹ�ж�
//-- ��������: �����ػ� -- begin --
#ifdef KEY_PRESS_PD_FUN_EN
	if(Mcu_stop_rec)
	{
		ADready = 0;
		Led_dsp_updata = 0;
		//ET0 = 1;	//�򿪼�ʱ���ж�0
		//TR0 = 1;	//���м�ʱ��0
		//---�򿪿��Ź�
		WD_TA = 0x05;
		WD_TA = 0x0a;
		WDCON = 0xbf;	//���Ź�ʱ�� 4s��,��Ƶ����0.2s
	}
	else
	if(Mcu_stop_pre) //ȷ��׼�����밴���ػ�ʱ ADC���ᱻ�жϳ�ϴ����;
	{
	}
	else
#endif
//-- ��������: �����ػ� -- end --
	Data_sycle_symbol = 1;	//�µ���������
	EXIF &= 0xdf; //��� ��Ƶ�����жϱ�־λ
	EA = 1;  // ���ж�
}
//�ڲ� 24bit SG ADC �ж�,��ȡadc���ݣ�*****************
static void sgadc_serve(void) interrupt 7
{		
	EA = 0;	 // ��ֹ�ж�

	data_receive(); // ��ȡ SDI0818 �� ת������
	ADcode_pre &= 0x00ffffff;
	ADcode_pre >>= 2;
		
	ADready = 1;	// SDI0818 ���� ��ȡ Buffer ���� ��־λ �� 1	
	if(ADready)
	{
		//--(comb2)^3--begin--	
		{
		ADbuf[3]=ADbuf[2];
		ADbuf[2]=ADbuf[1];
		ADbuf[1]=ADbuf[0];
    	ADbuf[0] = ADcode_pre;
    	//ADbuf[0] &= 0x00ffffff;	 
    	//ADbuf[0] >>= 2;	 
		ADcode_pre = ADbuf[0] + 3 * ADbuf[1] + 3 * ADbuf[2] + ADbuf[3];
		ADcode_pre >>= 3;
		}
		//--(comb2)^3--end--
	//--- ��������: ������ʾ���� -- begin ---
	#ifdef LOW_DSP_UPDATA
		Low_speed_updata = !Low_speed_updata;
	#endif
	//--- ��������: ������ʾ���� -- end ---
	}	
			
	Data_sycle_symbol = 1;	//�µ���������
	EXIF &= 0xbf;
	EA = 1;  // ���ж�
}



