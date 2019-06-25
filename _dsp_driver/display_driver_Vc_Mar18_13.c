//*************************************************************************//
//* Name				:   display_driver.c
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic 电子称 显示驱动文件
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
// -- 全部头文件 集中管理 ---- ，版本修改文件名会变动 ---  必须修改 这个文件
//**************************************************************************
#include "./_solidic/head_file_version.h"

EXTERN code unsigned char 	Lcd2led_table[];	//此变量为外部变量，定义在 "display_cal.c"中
//--条件编译: 显示位转换 -- begin --
#ifdef DSP_BIT_CHX_EN
	//EXTERN code unsigned char 	Lcd_buf_ex[];	//此变量为外部变量，定义在 "display_cal.c"中
	EXTERN code unsigned char 	Led_buf_ex[];	//此变量为外部变量，定义在 "display_cal.c"中
#endif
//--条件编译: 显示位转换 -- end --
//****************************************************
//-------------其他模块全局变量----------------------
//----全局变量 Lcd2led_table :	//定义在 "display_cal.c"中
//----IO定义 : 					//(头文件)ext_data
//----显示驱动选择宏定义 :		//(头文件)ext_data
//					LCD_DRIVER_1621\LED_DRIVER_6932\LED_DRIVER_1640\LCD_DRIVER_1616
//****************************************************


//延时函数 10us
void delay_10u(void)
{ 	
	unsigned int j;
	//--注意，需要根据实际单片机的速度调节时钟的脉宽--
	//--如果时钟超出 使用手册的速度，需要插入延时
	//--插入延时--;
  	for(j= 0;j<1;j++);		
}

//****************************************************
// 液晶驱动 1621 程序段 条件编译  ------ begin -------
//****************************************************
#ifdef LCD_DRIVER_1621

void lcd_unit_write(unsigned char d)
{
	unsigned char i;
	unsigned char j;
	//--注意，需要根据实际单片机的速度调节时钟的脉宽--
	//--如果时钟超出 使用手册的速度，需要插入延时
	for(i=0;i<8;i++)	
	{
		LCD_WR_N = 0;	
		j = d & 0x80;
		LCD_DATA = j;
		//--插入延时--;
		LCD_WR_N = 1;
		//--插入延时--;
		d <<= 1;
	}
}

//LCD写入函数
//mode:
//   0:	命令输入
//   1:	更新所有区域
//cmd: 命令编码
//----注意：显示函数中对 按键中断 ET0 的操作 -- (在程序不需要开中断T0时，尤其注意防止显示函数将T0中断误打开)

void write_lcd(bit mode,unsigned char cmd)
{
	unsigned char i;
	unsigned char addr;
	
	ET0 = 0; //禁止按键中断
	//--防止信号初始电平不对--
	LCD_CS_N = 1;
	LCD_DATA = 1;
	LCD_WR_N = 1;
	delay_10u();

	//--正常开始时序
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
		for(i=0;i<6;i++)	// 数据写入命令,发送首地址
		{
			//--注意，需要根据实际单片机的速度调节时钟的脉宽--
			//--如果时钟超出 使用手册的速度，需要插入延时
			LCD_WR_N = 0;
			LCD_DATA = (addr & 0x80);
			//--插入延时--;
			LCD_WR_N = 1;
			//--插入延时--;
			addr <<= 1;
		}
		
		for(i=0;i<16;i++)	// 数据写入命令,发送数据
		{
		//--条件编译: 显示位转换 -- begin --
		//#ifdef DSP_BIT_CHX_EN
		//	lcd_unit_write(Dsp_buf[Lcd_buf_ex[15 - i]]);
		//#else
			lcd_unit_write(Dsp_buf[i]);
		//#endif
		//--条件编译: 显示位转换 -- end --
		}
	}
	LCD_CS_N = 1;
	ET0 = 1; //打开按键中断
}

#endif
//****************************************************
// 液晶驱动 1621 程序段 条件编译  ------ end -------
//****************************************************

//****************************************************
// 数码驱动 6932 程序段 条件编译  ------ begin -------
//****************************************************
#ifdef LED_DRIVER_6932

// MCU 发送8bit数据到 LED驱动，低位在前
void led_unit_write(unsigned char led_data)
{
	unsigned char j;
	for (j=0; j<8; j++)
	{
		//--注意，需要根据实际单片机的速度调节时钟的脉宽--
		//--如果时钟超出 使用手册的速度，需要插入延时
		LED_SCLK = 0;
		LED_DIN = (bit)(led_data & 0x01);
		led_data >>= 1;
		//--插入延时--;
		LED_SCLK = 1;	// LED驱动 上升沿采样数据;
		//--插入延时--;
	}
}

//********6932 LED 驱动接口函数***********
//----全局变量 Led_bright:  用于调节 4级灰度
//----注意：显示函数中对 按键中断 ET0 的操作 -- (在程序不需要开中断T0时，尤其注意防止显示函数将T0中断误打开)
// mode = 0: LED 输入数据(显存数据在 Dsp_buf 数组中)
// mode = 1: LED 关闭
// mode = 2: LED 打开
void dsp_updata(unsigned char mode)
{
	unsigned char j;
	ET0 = 0; //禁止按键中断
	//--显示--控制--
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
	//--显示数据--
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
			//--条件编译: 显示位转换 -- begin --
			#ifdef DSP_BIT_CHX_EN
				if(Dsp_buf[Led_buf_ex[j]] & Lcd2led_table[num])
					temp++;	
			#else
				if(Dsp_buf[j] & Lcd2led_table[num])
					temp++;			
			#endif
			//--条件编译: 显示位转换 -- end --
			}
			led_unit_write(temp);							
			//led_unit_write(Dsp_buf[j]);
		}
		delay_10u();
		LED_STB = 1;
	}
	//打开按键中断	
	ET0 = 1; 
}

#endif
//****************************************************
// 液晶驱动 6932 程序段 条件编译  ------ end -------
//****************************************************

//****************************************************
// 数码驱动 1640 程序段 条件编译  ------ begin -------
//****************************************************
#ifdef LED_DRIVER_1640

// MCU 发送8bit数据到 LED驱动，低位在前
void led_unit_write(unsigned char led_data)
{
	unsigned char j;
	for (j=0; j<8; j++)
	{
		//--注意，需要根据实际单片机的速度调节时钟的脉宽--
		//--如果时钟超出 使用手册的速度，需要插入延时
		LED_DIN = (bit)(led_data & 0x01);
		led_data >>= 1;
		//--插入延时--;
		LED_SCLK = 1;	// LED驱动 上升沿采样数据;
		//--插入延时--;
		_nop_();
		LED_SCLK = 0;
		//--插入延时--;
	}
}

//********1640 LED 驱动接口函数***********
//----全局变量 Led_bright:  用于调节 4级灰度
//----注意：显示函数中对 按键中断 ET0 的操作 -- (在程序不需要开中断T0时，尤其注意防止显示函数将T0中断误打开)
// mode = 0: LED 输入数据(显存数据在 Dsp_buf 数组中)
// mode = 1: LED 关闭
// mode = 2: LED 打开
void dsp_updata(unsigned char mode)
{
	unsigned char j;
	ET0 = 0; //禁止按键中断
	//--- 先给出结束条件,将驱动置位到一个确定状态，防止异常情况发生 ---
	_nop_();
	LED_DIN = 0;
	LED_SCLK = 0;
	_nop_();
	LED_SCLK = 1;
	_nop_();
	LED_DIN = 1;	
	//--- 此处 如果实际应用过程中没问题，可以省去

	if(mode == 0)
	{		
	//---设置数据---
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
	//---设置地址---
		LED_DIN = 0;	
		_nop_();
		LED_SCLK = 0;
		_nop_();
		led_unit_write(0xc0); //从地址0开始
		_nop_();
	//---传送数据---
		for(j=0;j<8;j++)	
		{				
			unsigned char num,temp;
			temp = 0;
			for(temp=0,num=0;num<8;num++)
			{
				temp <<= 1;
			//--条件编译: 显示位转换 -- begin --
			#ifdef DSP_BIT_CHX_EN
				if(Dsp_buf[Led_buf_ex[num]] & Lcd2led_table[j])
					temp++;	
			#else
				if(Dsp_buf[num] & Lcd2led_table[j])
					temp++;			
			#endif
			//--条件编译: 显示位转换 -- end --
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
//---显示控制---
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
	//打开按键中断	
	ET0 = 1; 
}

#endif
//****************************************************
// 液晶驱动 1640 程序段 条件编译  ------ end -------
//****************************************************


//****************************************************
// 数码驱动 1616 程序段 条件编译  ------ begin -------
//****************************************************
#ifdef LED_DRIVER_1616

//********1616 LED 驱动接口函数***********
//----全局变量 Led_bright:  用于调节 4级灰度
//----注意：显示函数中对 按键中断 ET0 的操作 -- (在程序不需要开中断T0时，尤其注意防止显示函数将T0中断误打开)
// mode = 0: LED 输入数据(显存数据在 Dsp_buf 数组中)
// mode = 1: LED 关闭
// mode = 2: LED 打开
void dsp_updata(unsigned char mode)
{
	unsigned char i,j;
	unsigned char led_data;

	ET0 = 0; //禁止按键中断
	//--- 先给出结束条件,将驱动置位到一个确定状态，防止异常情况发生 ---
	LED_SDA = 0;
	LED_SCL = 0;
	_nop_();
	_nop_();	
	LED_SCL = 1;
	_nop_();
	_nop_();
	LED_SDA = 1;	// 停止
	_nop_();
	_nop_();
	//--- 此处 如果实际应用过程中没问题，可以省去
	LED_SDA = 0;	// 启动
	_nop_();
	_nop_();
	LED_SCL = 0;	
//------------------
	for(j=0;j<17;j++)	
	{
		if(j==0)
		{
			if(mode) // mode != 0 , 输入控制命令;
				led_data = 0x48;
 		    else  // mode == 0 , 输入显示数据地址;
				led_data = 0x60;
		}
		else
		{
			if(mode==0) // mode == 0 , 输入显示数据;
			{									
//				led_data = Dsp_buf[j-1];
				unsigned char num;
				for(led_data=0,num=0;num<8;num++)
				{
					led_data <<= 1;
				//--条件编译: 显示位转换 -- begin --
				#ifdef DSP_BIT_CHX_EN
					if(Dsp_buf[Led_buf_ex[j-1]] & Lcd2led_table[num])
						led_data++;	
				#else
					if(Dsp_buf[j-1] & Lcd2led_table[num])
						led_data++;			
				#endif
				//--条件编译: 显示位转换 -- end --
				}
			}
			else
			{
				if(mode==1) // mode = 1 , 显示关闭命令;
					led_data = 0x00;
				else 		// mode > 1 , 显示打开命令;
					led_data = 0x01;
				j = 16;
			}
		}
		
		for (i=0; i<9; i++)	// 发送8位数据，高位在前;第9位为确认位;
		{
			//--注意，需要根据实际单片机的速度调节时钟的脉宽--
			//--如果时钟超出 使用手册的速度，需要插入延时
			if(i==8)
				LED_SDA = 1;
			else
			{
				LED_SDA = (bit)(led_data & 0x80);
				led_data <<= 1;
			}
			LED_SCL = 1;
			//--插入延时--;
			_nop_();
			_nop_();
			LED_SCL = 0;	// LED驱动 上升下降沿采样数据;
			//--插入延时--;
		}
	}

	LED_SDA = 0;
	_nop_();
	_nop_();	
	LED_SCL = 1;
	_nop_();
	_nop_();
	LED_SDA = 1;	// 停止
	//打开按键中断	
	ET0 = 1; 
}

#endif
//****************************************************
// 液晶驱动 1616 程序段 条件编译  ------ end -------
//****************************************************
