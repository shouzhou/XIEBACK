//*************************************************************************//
//* Name				:   display_cal.h
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic ���ӳ� �˲�����������������
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
//----------  Ԥ����ѡ�� ------ �ֶ����� ���� ---------
//*************************************************************************//

//------------------------------------------------------------------
//-- ���� �ڲ����� �� �ֶ�
// 1:	�ڲ����� 6��ֶ�
// 2:	�ڲ����� 12��ֶ�
// 3:	�ڲ����� 24��ֶ�
#define INTER_WEIGHT_MODE 1
//-- ���� �ڲ�������������������(ͨ��ȡֵ����)
// 1:	�ڲ����� 6��ֶ�
// 2:	�ڲ����� 12��ֶ�
// 4:	�ڲ����� 24��ֶ�
#define INTER_WEIGHT_LOCK_TH 1	
	
//------------------------------------------------------------------
//-- ���� ���ݴ����������� --
#define OUT_ZERO_TH 14		//�����ڲ�������0�����������С����	
#define INTO_ZERO_TH 7		//�����ڲ�������0�����������С����	
#define DE_JETTER_TH 4		//ȥ��������
//-- �����������߲��õĴ������������ԼӴ��������
//#define OUT_ZERO_TH 14		//�����ڲ�������0�����������С����	
//#define INTO_ZERO_TH 7		//�����ڲ�������0�����������С����	
//#define DE_JETTER_TH 4		//ȥ��������

//------------------------------------------------------------------
//--���� �Ƿ����� "����������" --
//--- Ŀǰ������(2mV/V)��(24ΪADC���ȥ����2λ��)�������̵�������60��
//--- ��:30kg���̣�1g��Ӧ20��������
//--- ͬ��:6kg���̣�0.2g��Ӧ20��������
#ifdef W_LINE_CAL_EN			//һ�� 30kg �����̲��� 1.5g������
	#define  BEGIN_CAL_CODE 	300000 // ��ʼ������ 15kg
	#define  SLOPE_CAL_CODE 	10000 // (600000 - 300000 ) / 30 ; ( ��ʼ������ 15kg - ���������� 30kg) / // �������� 1.5g
	//#define  SLOPE_CAL_CODE 	2500 // (600000 - 300000 ) / 120 ; ( ��ʼ������ 15kg - ���������� 30kg) / // �������� 6g	
#endif

//------------------------------------------------------------------
//--���� �Ƿ����� "��������"  -- (ʱ������) ---
//-- һ�����ʱ�Ľ�������Ҫ��ͨ��"���ٽ���"��"�ۼӽ���"
//-- "����ƫ�ƽ���"����Ư�ƴ��ʱ�򣬴�ʱ"���ٽ���"��"�ۼӽ���"�����趨��ͨ����������Ư���ж��Ƿ����
//-- "����ƫ�ƽ���"Ŀǰʹ�õĺ��٣���������˲�ǿ���й�ϵ��ʹ��ʱҪ�������

//---------   ���ٽ���  -------
//-- ʵ����,�����ض�����ʱ��,�󲿷ֶ���ͨ�����ַ�ʽ����
//-- ���ԣ����趨Ϊ8ʱ������0.5g�Ķ��������϶��ܽ���
#ifdef INTER_WEIGHT_LOCK
	#define QUICK_UNLOCK_W 	10 							// ���ٽ���������� ��ADC��������

//---------   ����ƫ�ƽ���  -------	
//-- ʵ����,�����ض�����ʱ��,����������һ����������7��,
//-- ����һ����4�ν������Ļ�����Ҫ��������24�������У��˲���ֵ��ֵ����������һ����������7��
//-- ���仹��Ҫ��30�������ڣ�"�����ԣ�����趨Ϊ(7 -- 30),��ʹ����7g����ʱ��Ҳ�����޷�������"
//-- �趨Ϊ " UNLOCK_NUM(7) - UNLOCK_TIME_NUM(30)" �Ļ���һ��ûʲô�ã�������ȥ������
//--
//-- 0819���������ص�ʱ�������ֵ�����3���ң������趨Ϊ5��ʱ��һ�����1g�ܺܺõ�ͨ�����ַ�ʽ����
//-- ���ԣ��������ӵ����ʱ��Ӧ���趨Ϊ " UNLOCK_NUM(5) - UNLOCK_TIME_NUM(40)"
//-- ���Ϊ�˱�֤�����õ㣬Ҳ���ǲ�ҪƵ���Ƴ������Ļ����趨Ϊ " UNLOCK_NUM(7) - UNLOCK_TIME_NUM(30)"
//--
//-- Filter-R�˲��趨Ϊ " UNLOCK_NUM(7) - UNLOCK_TIME_NUM(30)" �Ļ���һ��ûʲô�ã���Ҫ��ͨ��"���ٽ���"��"�ۼӽ���"
	
	#ifdef ADC_OUT_20Hz	
		#define UNLOCK_NUM 			7 					// (ʱ������)����������� ��ADC��������
		#define UNLOCK_TIME_NUM 	30					// (ʱ������)���������жϵ�ʱ�� 1.5s  (20Hz ADC���)
	#endif
	#ifdef ADC_OUT_10Hz
		#define UNLOCK_NUM 			4 					// (ʱ������)����������� ��ADC��������
		#define UNLOCK_TIME_NUM 	15					// (ʱ������)���������жϵ�ʱ�� 1.5s  (10Hz ADC���)
	#endif
	//------------------------
	EXTERN char idata	ADcode_unlock_buf[2]; 			//�����ж����������� �������
#endif

EXTERN unsigned char idata Weight_lock_counter;  		//�����������������������ȴ���Ϊ"Weight_lock_num"ʱ����������
EXTERN bit Weight_lock_symbol;							// �����ȶ���һ��ʱ��� ������ֱ������һ��������ͻ��Ž�����
//-- WEIGHT_LOCK_NUM ������� WEIGHT_LOCK_AVE_NUM
#ifdef ADC_OUT_20Hz	
	#define WEIGHT_LOCK_NUM 	60						// (ʱ������)�����ȶ���3s ����
	#define WEIGHT_LOCK_AVE_NUM 20						// (ʱ������)���������У�����������ƽ��ֵ��ADC���ݸ���
#endif
#ifdef ADC_OUT_10Hz
	#define WEIGHT_LOCK_NUM 	30						// (ʱ������)�����ȶ���3s ����
	#define WEIGHT_LOCK_AVE_NUM 10						// (ʱ������)���������У�����������ƽ��ֵǰ ������ADC���ݸ���
#endif

//------------------------------------------------------------------
//--���� �Ƿ����� "ǿ�ƹ���" ---
//--
EXTERN bit Zero_force_start_en; 	//ǿ�ƻ��� ��Ҫ����
#ifdef FORCE_WEIGHT_ZERO			//ÿ����ֵ����0.5d
	EXTERN bit Zero_force_symbol; 	//ǿ�ƻ��� ��־λ
	#define ZERO_FORCE_NUM_1 3		//���� ǿ�ƻ��� ���� 1.5d
	#define ZERO_FORCE_NUM_2 4		//���� ǿ�ƻ��� ���� 2d
	#define ZERO_FORCE_NUM_3 8		//���� ǿ�ƻ��� ���� 4d

	#if INTER_WEIGHT_MODE == 1
		#define ZERO_FORCE_NUM_EN 20000 	//���� ǿ�ƻ��� �������� 1/3(60000�ڲ��ֶ�)
	#endif
	
	#if INTER_WEIGHT_MODE == 2
		#define ZERO_FORCE_NUM_EN 40000 	//���� ǿ�ƻ��� �������� 1/3(120000�ڲ��ֶ�)
	#endif
	
	#if INTER_WEIGHT_MODE >= 3
		#define ZERO_FORCE_NUM_EN 80000 	//���� ǿ�ƻ��� �������� 1/3(240000�ڲ��ֶ�)
	#endif	
#endif




//*************************************************************************//
//----------  Ԥ����ѡ�� ------ �Զ����� ���� ---------
//*************************************************************************//

//-------�����ڲ������ֶ�ʱ--���� ����Ҫ �Ķ�-------------
#if INTER_WEIGHT_MODE == 1
	#define CAL_POINT_W 12000	//6��ֶȵ�1/5 -> �õ�����12������
	#define INTER_WEIGHT_NUM 1  //�ڲ����� �� �ⲿ�����Ĺ�ϵ (2^INTER_WEIGHT_NUM ��)
	#define MIN_NEG_ZERO_W 8  	//��С���ڲ�����׷������
#endif

#if INTER_WEIGHT_MODE == 2
	#define CAL_POINT_W 20000	//12��ֶȵ�1/6 -> �õ�����10������
	#define INTER_WEIGHT_NUM 2	//�ڲ����� �� �ⲿ�����Ĺ�ϵ (2^INTER_WEIGHT_NUM ��)
	#define MIN_NEG_ZERO_W 16  	//��С���ڲ�����׷������
#endif

#if INTER_WEIGHT_MODE >= 3
	#define CAL_POINT_W 24000	//24��ֶȵ�1/10 -> �õ�����6������
	#define INTER_WEIGHT_NUM 3	//�ڲ����� �� �ⲿ�����Ĺ�ϵ (2^INTER_WEIGHT_NUM ��)
	#define MIN_NEG_ZERO_W 24  	//��С���ڲ�����׷������
#endif











//*************************************************************************//
//----------  �������� ------ (������˲�����) ---------
//*************************************************************************//

//------------------------------------------------------------------
//--- ADC�ڶ��� �˲����� -- 
EXTERN int idata ADbuf_delta[9];					//���� ADC ����ĵڶ��� ���˲�����;
EXTERN unsigned long idata Downsample_filter;		//���� ADC ����ĵڶ��� �˲� �еĽ����� ����;	
EXTERN unsigned char idata Downsample_counter;		//���� ADC ����ĵڶ��� �˲� �еĽ����� ����;	
//------------------------
//ADbuf_init:��8λ��Ϊ���̰�������ʹ�ã���24λ����adc�����ݲɼ�
//EXTERN unsigned char KEY_CODE; //��������
//EXTERN unsigned long idata	ADbuf_init;			//���ڼ�¼ ADC��� ��BUF (���ڼ�¼���ADC����Ĺ���ֵ);
EXTERN union  ADpattern idata ADbuf_init;			//���ڼ�¼ ADC��� ��BUF (���ڼ�¼���ADC����Ĺ���ֵ);

//--- ADC �����е�������� -- 
#define  QUICK_CO_A 	2000	//���� ADC ����ĵڶ��� �˲�	
#define  QUICK_CO_B		200		//���� ADC ����ĵڶ��� �˲�		
#define  QUICK_CO_C 	15		//���� ADC ����ĵڶ��� �˲�	
#define  QUICK_CO_D 	80		//���� ���ټ�Ъ���������
#define  AVER_CO_A		1600	//���� ���ADC����Ĺ���ֵ 

//--- �������Զ�ƥ�� ���� -- 
//--- Ŀǰ������(2mV/V)��(24ΪADC���ȥ����2λ��)�������̵�������60��
//--- ��:30kg���̣�1g��Ӧ20��������
//--- ͬ��:6kg���̣�0.2g��Ӧ20��������
//--- �Բ�ͬ�Ĵ��������������Զ�ʶ���� 1�ֶ� 20��������ʶ������������ʲô���������� 3��ֶ�
//--- (ע�⣬���3��ֶȻ�Ҫ���� "<< INTER_WEIGHT_NUM"���ܱ�Ϊ�������ڲ�����)
#define  UP_CAL_D_CODE 		27 // ��� 1d�ڲ��ֶ� ��Ӧ�� AD ����
#define  LOW_CAL_D_CODE 	14 // ��С 1d�ڲ��ֶ� ��Ӧ�� AD ����
#define  IDELA_CAL_D_CODE 	20 // ����1d�ڲ��ֶ� ��Ӧ�� AD ����


//*************************************************************************//
//----------  �������� ------ (��������) ---------
//*************************************************************************//

//------------------------------------------------------------------
//--- ����׷���ж�ʱ�� -- (ʱ������) ---
EXTERN bit Zero_buf_symbol;					// ׷���־λ
EXTERN unsigned char Follow_zero_counter; 	// ׷����� ������
//--
#ifdef ADC_OUT_20Hz
	#define FOLLOW_ZERO_NUM_0 20	// (ʱ������)����׷���ж�ʱ�䣬��Ӧ ʱ��Ϊ 0.5s��
	#define FOLLOW_ZERO_NUM_1 20	// (ʱ������)����׷���ж�ʱ�䣬��Ӧ ʱ��Ϊ 1s��
#endif
#ifdef ADC_OUT_10Hz
	#define FOLLOW_ZERO_NUM_0 5		// (ʱ������)����׷���ж�ʱ�䣬��Ӧ ʱ��Ϊ 0.5s��
	#define FOLLOW_ZERO_NUM_1 10	// (ʱ������)����׷���ж�ʱ�䣬��Ӧ ʱ��Ϊ 1s��
#endif

//------------------------------------------------------------------
//--- ������ر��� -- (ʱ������) ---
#ifdef GLITCH_FILTER_FUN_EN
	EXTERN unsigned long idata	Glitch_filter;		// ���� ƽ���˲�����
	EXTERN unsigned char Glitch_filter_counter; 	// ���𶯼�����
	EXTERN bit Glitch_remove_sign;					// ��¼ ����ƫ�� ��������ֵ�ķ���					
	EXTERN bit Glitch_remove_en;					// = 1 ���� ������������
	//EXTERN bit glitch_remove_symbol; 				//���� ���� ��
#endif
//--
#if INTER_WEIGHT_MODE == 1
	#define GLITCH_W_NUM_0 200 					//	(��ͨ)���� �ڲ��������� "1/30"(6��ֶ�)
	#define GLITCH_W_NUM_1 2000 				//	(����)���� �ڲ��������� "1/30"(6��ֶ�)
	#define GLITCH_W_NUM_2 300 					//	(����)���� �ڲ���������
	#define GLITCH_W_NUM_3 150 					//	(����)���� �ڲ���������
	#define GLITCH_W_NUM_4 20 					//	(����)���� �ڲ���������
	#define GLITCH_W_EN_NUM 100 				//	����(����)���� �ڲ��������� "1/30"(6��ֶ�)
#endif
#if INTER_WEIGHT_MODE == 2
	#define GLITCH_W_NUM_0 400 					//	(��ͨ)���� �ڲ��������� "1/30"(12��ֶ�)
	#define GLITCH_W_NUM_1 4000 				//	(����)���� �ڲ��������� "1/30"(12��ֶ�)
	#define GLITCH_W_NUM_2 600 					//	(����)���� �ڲ���������
	#define GLITCH_W_NUM_3 300 					//	(����)���� �ڲ���������
	#define GLITCH_W_NUM_4 40 					//	(����)���� �ڲ���������
	#define GLITCH_W_EN_NUM 200 				//	����(����)���� �ڲ��������� "1/30"(6��ֶ�)
#endif
#if INTER_WEIGHT_MODE >= 3
	#define GLITCH_W_NUM_0 800 					//	(��ͨ)���� �ڲ��������� "1/30"(24��ֶ�)
	#define GLITCH_W_NUM_1 8000 				//	(����)���� �ڲ��������� "1/30"(24��ֶ�)
	#define GLITCH_W_NUM_2 1200 					//	(����)���� �ڲ���������
	#define GLITCH_W_NUM_3 450 					//	(����)���� �ڲ���������
	#define GLITCH_W_NUM_4 80 					//	(����)���� �ڲ���������
	#define GLITCH_W_EN_NUM 400 				//	����(����)���� �ڲ��������� "1/30"(6��ֶ�)
#endif
//--GLITCH_T_NUM_2 ������� GLITCH_R_NUM
#ifdef ADC_OUT_20Hz 
	// -- GLITCH_T_NUM_0 �� GLITCH_W_NUM_0 ֵ�Ƚϴ�ʱ(��400,600��)�����赽10���������׻ζ�
	#define GLITCH_T_NUM_0 6 						//	(��ͨ)���� ʱ������
	#define GLITCH_T_NUM_1 20 						//	(����)���� ʱ������
	#define GLITCH_T_NUM_2 50 						//	(����)���� ʱ������
	#define GLITCH_R_NUM 20 						//	�����仯ʱ���������� ʱ������
#endif
#ifdef ADC_OUT_10Hz
	#define GLITCH_T_NUM_0 3 						//	(��ͨ)���� ʱ������
	#define GLITCH_T_NUM_1 10 						//	(����)���� ʱ������
	#define GLITCH_T_NUM_2 25 						//	(����)���� ʱ������
	#define GLITCH_R_NUM 10 						//	�����仯ʱ���������� ʱ������	
#endif

//------------------------------------------------------------------
//--- �������ȶ��Ը��� ��ر��� -- (ʱ������) ---
#ifdef GLITCH_DETECT_EN
	EXTERN unsigned char idata AD_stable_counter;  //�����ȶ�������
	EXTERN unsigned long idata AD_stable_buf;  //
	EXTERN unsigned long idata AD_stable_buf_temp;  //
#endif
#ifdef GLITCH_FILTER_FUN_EN
	EXTERN bit AD_stable_symbol;	//�����ȶ���־λ
#endif
//--
#define AD_STABLE_C_NUM_1 600 						//�жϴ������ȶ��� ��������
//--
#ifdef ADC_OUT_20Hz 
	#define AD_STABLE_T_NUM_0 20 					//�жϴ������ȶ���ʱ������
#endif
#ifdef ADC_OUT_10Hz
	#define AD_STABLE_T_NUM_0 10 					//�жϴ������ȶ���ʱ������
#endif

//------------------------------------------------------------------
//--- �˲� ǿ�� ��̬���� ---
EXTERN bit Filter_degree_a; 					//�����˲����� = 1�����˲�
EXTERN unsigned char Filter_count_a; 			//�����˲� ���� ������
EXTERN unsigned long Filter_buf_a; 				//�����˲� ���� ������

//------------------------------------------------------------------
//--- ���� ������ -- (ʱ������) ---
EXTERN unsigned long idata Worm_begin_buf; 		// ��ʼ������ʱ��ADת������
EXTERN int idata Worm_change_follow_buf;
EXTERN bit Worm_en_symbol; 						//��������� ��־λ
EXTERN unsigned char idata Worm_change_follow_counter; // �������ȶ���ÿ30�� ����һ��������
//---------   �ۼӽ���  -------
#define  UNLOCK_ACCUM_NUM 	18 // ��������ƫ�� �������� ����
//--
#ifdef ADC_OUT_20Hz
	#define WORM_CHANGE_FOLLOW_NUM 60				//(ʱ������) ��Ӧ 20Hz ���ʱ���� ʱ��Ϊ 3s
#endif
#ifdef ADC_OUT_10Hz
	#define WORM_CHANGE_FOLLOW_NUM 30				//(ʱ������) ��Ӧ 10Hz ���ʱ���� ʱ��Ϊ 3s
#endif


//------------------------------------------------------------------
//--- �������س̴��� ---
//-- ���� �Ƿ����� "�س̲���" --
//--
#ifdef W_REDUCE_CAL_EN
	EXTERN bit Sensor_cal; 							// = 1 ��Ҫ������ ����������
	EXTERN bit Sensor_cal_2; 						// = 1 ��Ҫ������ ����������
	EXTERN unsigned char idata Sensor_cal_counter; 	//����׷���С
	EXTERN bit Sensor_cal_dis; 						// = 0 ���� = 1 �رղ���	
#endif

//------------------------------------------------------------------
//--- ADCȥ������������� ���� ---
//EXTERN unsigned char ADfilter_count;
EXTERN unsigned char Filter_up_counter; 	// �˲���ǿ������

//*************************************************************************//
//----------  ����ԭ�� ---------------
//*************************************************************************//

//��ȡadcת�����ݣ���������ʵ���ƽ�����˲���
void	Filter_ADdata(void);

// ����������1g ת���� �ڲ�����
unsigned long data_trans_into(void);

// ���ڲ����� ת���� ������������1g 
unsigned long data_trans_out(unsigned long tdata);

//----����궨����----
void cal_data_process(void);

//-----------�ڲ�����������*********************
void internal_weight_cal(void);	

#endif