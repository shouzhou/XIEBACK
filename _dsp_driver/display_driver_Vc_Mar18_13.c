//*************************************************************************//
//* Name				:   display_driver.c
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic ���ӳ� ��ʾ�����ļ�
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

EXTERN code unsigned char 	Lcd2led_table[];	//�˱���Ϊ�ⲿ������������ "display_cal.c"��
//--��������: ��ʾλת�� -- begin --
#ifdef DSP_BIT_CHX_EN
	//EXTERN code unsigned char 	Lcd_buf_ex[];	//�˱���Ϊ�ⲿ������������ "display_cal.c"��
	EXTERN code unsigned char 	Led_buf_ex[];	//�˱���Ϊ�ⲿ������������ "display_cal.c"��
#endif
//--��������: ��ʾλת�� -- end --
//****************************************************
//-------------����ģ��ȫ�ֱ���----------------------
//----ȫ�ֱ��� Lcd2led_table :	//������ "display_cal.c"��
//----IO���� : 					//(ͷ�ļ�)ext_data
//----��ʾ����ѡ��궨�� :		//(ͷ�ļ�)ext_data
//					LCD_DRIVER_1621\LED_DRIVER_6932\LED_DRIVER_1640\LCD_DRIVER_1616
//****************************************************


//��ʱ���� 10us
void delay_10u(void)
{ 	
	unsigned int j;
	//--ע�⣬��Ҫ����ʵ�ʵ�Ƭ�����ٶȵ���ʱ�ӵ�����--
	//--���ʱ�ӳ��� ʹ���ֲ���ٶȣ���Ҫ������ʱ
	//--������ʱ--;
  	for(j= 0;j<1;j++);		
}

//****************************************************
// Һ������ 1621 ����� ��������  ------ begin -------
//****************************************************
#ifdef LCD_DRIVER_1621

void lcd_unit_write(unsigned char d)
{
	unsigned char i;
	unsigned char j;
	//--ע�⣬��Ҫ����ʵ�ʵ�Ƭ�����ٶȵ���ʱ�ӵ�����--
	//--���ʱ�ӳ��� ʹ���ֲ���ٶȣ���Ҫ������ʱ
	for(i=0;i<8;i++)	
	{
		LCD_WR_N = 0;	
		j = d & 0x80;
		LCD_DATA = j;
		//--������ʱ--;
		LCD_WR_N = 1;
		//--������ʱ--;
		d <<= 1;
	}
}

//LCDд�뺯��
//mode:
//   0:	��������
//   1:	������������
//cmd: �������
//----ע�⣺��ʾ�����ж� �����ж� ET0 �Ĳ��� -- (�ڳ�����Ҫ���ж�T0ʱ������ע���ֹ��ʾ������T0�ж����)

void write_lcd(bit mode,unsigned char cmd)
{
	unsigned char i;
	unsigned char addr;
	
	ET0 = 0; //��ֹ�����ж�
	//--��ֹ�źų�ʼ��ƽ����--
	LCD_CS_N = 1;
	LCD_DATA = 1;
	LCD_WR_N = 1;
	delay_10u();

	//--������ʼʱ��
	LCD_CS_N = 0;
	LCD_DATA = 1;
	LCD_WR_N = 0;
	delay_10u();
	LCD_WR_N = 1;
	delay_10u();
	LCD_DATA = 0;
	LCD_WR_N = 0;
	delay_10u();
	LCD_WR_N = 1;
	if(mode==0)
	{
	  	LCD_DATA = 0;
	}
	else 
	{
	  	LCD_DATA = 1;
	}
	LCD_WR_N = 0;
	delay_10u();
	LCD_WR_N = 1;
	delay_10u();

	if(mode == 0)
	{
		lcd_unit_write(cmd);
		delay_10u();
		LCD_WR_N = 0;
		delay_10u();
		LCD_WR_N = 1;
		delay_10u();
	}
	else 
	{
		addr = 0;
		for(i=0;i<6;i++)	// ����д������,�����׵�ַ
		{
			//--ע�⣬��Ҫ����ʵ�ʵ�Ƭ�����ٶȵ���ʱ�ӵ�����--
			//--���ʱ�ӳ��� ʹ���ֲ���ٶȣ���Ҫ������ʱ
			LCD_WR_N = 0;
			LCD_DATA = (addr & 0x80);
			//--������ʱ--;
			LCD_WR_N = 1;
			//--������ʱ--;
			addr <<= 1;
		}
		
		for(i=0;i<16;i++)	// ����д������,��������
		{
		//--��������: ��ʾλת�� -- begin --
		//#ifdef DSP_BIT_CHX_EN
		//	lcd_unit_write(Dsp_buf[Lcd_buf_ex[15 - i]]);
		//#else
			lcd_unit_write(Dsp_buf[i]);
		//#endif
		//--��������: ��ʾλת�� -- end --
		}
	}
	LCD_CS_N = 1;
	ET0 = 1; //�򿪰����ж�
}

#endif
//****************************************************
// Һ������ 1621 ����� ��������  ------ end -------
//****************************************************

//****************************************************
// �������� 6932 ����� ��������  ------ begin -------
//****************************************************
#ifdef LED_DRIVER_6932

// MCU ����8bit���ݵ� LED��������λ��ǰ
void led_unit_write(unsigned char led_data)
{
	unsigned char j;
	for (j=0; j<8; j++)
	{
		//--ע�⣬��Ҫ����ʵ�ʵ�Ƭ�����ٶȵ���ʱ�ӵ�����--
		//--���ʱ�ӳ��� ʹ���ֲ���ٶȣ���Ҫ������ʱ
		LED_SCLK = 0;
		LED_DIN = (bit)(led_data & 0x01);
		led_data >>= 1;
		//--������ʱ--;
		LED_SCLK = 1;	// LED���� �����ز�������;
		//--������ʱ--;
	}
}

//********6932 LED �����ӿں���***********
//----ȫ�ֱ��� Led_bright:  ���ڵ��� 4���Ҷ�
//----ע�⣺��ʾ�����ж� �����ж� ET0 �Ĳ��� -- (�ڳ�����Ҫ���ж�T0ʱ������ע���ֹ��ʾ������T0�ж����)
// mode = 0: LED ��������(�Դ������� Dsp_buf ������)
// mode = 1: LED �ر�
// mode = 2: LED ��
void dsp_updata(unsigned char mode)
{
	unsigned char j;
	ET0 = 0; //��ֹ�����ж�
	//--��ʾ--����--
	j = Led_bright;
	if(mode == 1)
		j += 0x80;
	else
		j += 0x88;
	LED_STB = 1;
	delay_10u();
	LED_STB = 0;
	led_unit_write(j);
	delay_10u();
	LED_STB = 1;
	//--��ʾ����--
	if(mode == 0)
	{		
		//------------------
		delay_10u();
		LED_STB = 0;
		led_unit_write(0xc0);
		//------------------
		for(j=0;j<16;j++)	
		{
			unsigned char num,temp;
			temp = 0;
			for(temp=0,num=0;num<8;num++)
			{
				temp <<= 1;
			//--��������: ��ʾλת�� -- begin --
			#ifdef DSP_BIT_CHX_EN
				if(Dsp_buf[Led_buf_ex[j]] & Lcd2led_table[num])
					temp++;	
			#else
				if(Dsp_buf[j] & Lcd2led_table[num])
					temp++;			
			#endif
			//--��������: ��ʾλת�� -- end --
			}
			led_unit_write(temp);							
			//led_unit_write(Dsp_buf[j]);
		}
		delay_10u();
		LED_STB = 1;
	}
	//�򿪰����ж�	
	ET0 = 1; 
}

#endif
//****************************************************
// Һ������ 6932 ����� ��������  ------ end -------
//****************************************************

//****************************************************
// �������� 1640 ����� ��������  ------ begin -------
//****************************************************
#ifdef LED_DRIVER_1640

// MCU ����8bit���ݵ� LED��������λ��ǰ
void led_unit_write(unsigned char led_data)
{
	unsigned char j;
	for (j=0; j<8; j++)
	{
		//--ע�⣬��Ҫ����ʵ�ʵ�Ƭ�����ٶȵ���ʱ�ӵ�����--
		//--���ʱ�ӳ��� ʹ���ֲ���ٶȣ���Ҫ������ʱ
		LED_DIN = (bit)(led_data & 0x01);
		led_data >>= 1;
		//--������ʱ--;
		LED_SCLK = 1;	// LED���� �����ز�������;
		//--������ʱ--;
		_nop_();
		LED_SCLK = 0;
		//--������ʱ--;
	}
}

//********1640 LED �����ӿں���***********
//----ȫ�ֱ��� Led_bright:  ���ڵ��� 4���Ҷ�
//----ע�⣺��ʾ�����ж� �����ж� ET0 �Ĳ��� -- (�ڳ�����Ҫ���ж�T0ʱ������ע���ֹ��ʾ������T0�ж����)
// mode = 0: LED ��������(�Դ������� Dsp_buf ������)
// mode = 1: LED �ر�
// mode = 2: LED ��
void dsp_updata(unsigned char mode)
{
	unsigned char j;
	ET0 = 0; //��ֹ�����ж�
	//--- �ȸ�����������,��������λ��һ��ȷ��״̬����ֹ�쳣������� ---
	_nop_();
	LED_DIN = 0;
	LED_SCLK = 0;
	_nop_();
	LED_SCLK = 1;
	_nop_();
	LED_DIN = 1;	
	//--- �˴� ���ʵ��Ӧ�ù�����û���⣬����ʡȥ

	if(mode == 0)
	{		
	//---��������---
		LED_DIN = 0;	
		_nop_();
		LED_SCLK = 0;
		_nop_();
		led_unit_write(0x40);
		_nop_();
		LED_DIN = 0;
		_nop_();
		LED_SCLK = 1;
		_nop_();
		LED_DIN = 1;
		_nop_();
	//---���õ�ַ---
		LED_DIN = 0;	
		_nop_();
		LED_SCLK = 0;
		_nop_();
		led_unit_write(0xc0); //�ӵ�ַ0��ʼ
		_nop_();
	//---��������---
		for(j=0;j<8;j++)	
		{				
			unsigned char num,temp;
			temp = 0;
			for(temp=0,num=0;num<8;num++)
			{
				temp <<= 1;
			//--��������: ��ʾλת�� -- begin --
			#ifdef DSP_BIT_CHX_EN
				if(Dsp_buf[Led_buf_ex[num]] & Lcd2led_table[j])
					temp++;	
			#else
				if(Dsp_buf[num] & Lcd2led_table[j])
					temp++;			
			#endif
			//--��������: ��ʾλת�� -- end --
			}
			led_unit_write(temp);								
			//led_unit_write(Dsp_buf[j]);	
		}
		_nop_();
		LED_DIN = 0;
		_nop_();
		LED_SCLK = 1;
		_nop_();
		LED_DIN = 1;
	}
//---��ʾ����---
	j = POWER_MODE;
	if(mode == 1)
		j += 0x80;
	else
		j +=0x88;
	_nop_();
	LED_DIN = 0;	
	_nop_();
	LED_SCLK = 0;
	_nop_();
	led_unit_write(j);
	_nop_();
	LED_DIN = 0;
	_nop_();
	LED_SCLK = 1;
	_nop_();
	LED_DIN = 1;
	//�򿪰����ж�	
	ET0 = 1; 
}

#endif
//****************************************************
// Һ������ 1640 ����� ��������  ------ end -------
//****************************************************


//****************************************************
// �������� 1616 ����� ��������  ------ begin -------
//****************************************************
#ifdef LED_DRIVER_1616

//********1616 LED �����ӿں���***********
//----ȫ�ֱ��� Led_bright:  ���ڵ��� 4���Ҷ�
//----ע�⣺��ʾ�����ж� �����ж� ET0 �Ĳ��� -- (�ڳ�����Ҫ���ж�T0ʱ������ע���ֹ��ʾ������T0�ж����)
// mode = 0: LED ��������(�Դ������� Dsp_buf ������)
// mode = 1: LED �ر�
// mode = 2: LED ��
void dsp_updata(unsigned char mode)
{
	unsigned char i,j;
	unsigned char led_data;

	ET0 = 0; //��ֹ�����ж�
	//--- �ȸ�����������,��������λ��һ��ȷ��״̬����ֹ�쳣������� ---
	LED_SDA = 0;
	LED_SCL = 0;
	_nop_();
	_nop_();	
	LED_SCL = 1;
	_nop_();
	_nop_();
	LED_SDA = 1;	// ֹͣ
	_nop_();
	_nop_();
	//--- �˴� ���ʵ��Ӧ�ù�����û���⣬����ʡȥ
	LED_SDA = 0;	// ����
	_nop_();
	_nop_();
	LED_SCL = 0;	
//------------------
	for(j=0;j<17;j++)	
	{
		if(j==0)
		{
			if(mode) // mode != 0 , �����������;
				led_data = 0x48;
 		    else  // mode == 0 , ������ʾ���ݵ�ַ;
				led_data = 0x60;
		}
		else
		{
			if(mode==0) // mode == 0 , ������ʾ����;
			{									
//				led_data = Dsp_buf[j-1];
				unsigned char num;
				for(led_data=0,num=0;num<8;num++)
				{
					led_data <<= 1;
				//--��������: ��ʾλת�� -- begin --
				#ifdef DSP_BIT_CHX_EN
					if(Dsp_buf[Led_buf_ex[j-1]] & Lcd2led_table[num])
						led_data++;	
				#else
					if(Dsp_buf[j-1] & Lcd2led_table[num])
						led_data++;			
				#endif
				//--��������: ��ʾλת�� -- end --
				}
			}
			else
			{
				if(mode==1) // mode = 1 , ��ʾ�ر�����;
					led_data = 0x00;
				else 		// mode > 1 , ��ʾ������;
					led_data = 0x01;
				j = 16;
			}
		}
		
		for (i=0; i<9; i++)	// ����8λ���ݣ���λ��ǰ;��9λΪȷ��λ;
		{
			//--ע�⣬��Ҫ����ʵ�ʵ�Ƭ�����ٶȵ���ʱ�ӵ�����--
			//--���ʱ�ӳ��� ʹ���ֲ���ٶȣ���Ҫ������ʱ
			if(i==8)
				LED_SDA = 1;
			else
			{
				LED_SDA = (bit)(led_data & 0x80);
				led_data <<= 1;
			}
			LED_SCL = 1;
			//--������ʱ--;
			_nop_();
			_nop_();
			LED_SCL = 0;	// LED���� �����½��ز�������;
			//--������ʱ--;
		}
	}

	LED_SDA = 0;
	_nop_();
	_nop_();	
	LED_SCL = 1;
	_nop_();
	_nop_();
	LED_SDA = 1;	// ֹͣ
	//�򿪰����ж�	
	ET0 = 1; 
}

#endif
//****************************************************
// Һ������ 1616 ����� ��������  ------ end -------
//****************************************************
