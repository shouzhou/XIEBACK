/****************************************************************/ 
//* 名称  0x				func.h
//* 描述  0x				函数原型：SDI0818_001_main_board 应用系统
//* 作者  0x				cds					
//* 版本号  0x			V1.0
//* 修改记录:
//*						1: Agu01_2008 First created		
//* 版权：           	深圳市芯易德科技有限公司                                                                                                                                                                                                           
/*****************************************************************/

#ifndef	_DisplayC_
#define _DisplayC_
//-----------------------------------------------------------------------
//--- 数码标准字段分布 ---
//		--A--
//		F   B
//		|-G-|
//		E	C
//	    --D-- H	  
//-----------------------------------------------------------------------
	#define LEDC_0 (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F )
	#define LEDC_1 (SEG_B | SEG_C )
	#define LEDC_2 (SEG_A | SEG_B | SEG_D | SEG_E | SEG_G )
	#define LEDC_3 (SEG_A | SEG_B | SEG_C | SEG_D | SEG_G )
	#define LEDC_4 (SEG_B | SEG_C | SEG_F | SEG_G )
	#define LEDC_5 (SEG_A | SEG_C | SEG_D | SEG_F | SEG_G)
	#define LEDC_6 (SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
	#define LEDC_7 (SEG_A | SEG_B | SEG_C)
	#define LEDC_8 (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
	#define LEDC_9 (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)
	#define LEDC_A (SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G)
	#define LEDC_B (SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
	#define LEDC_C (SEG_A | SEG_D | SEG_E | SEG_F)
	#define LEDC_D (SEG_B | SEG_C | SEG_D | SEG_E | SEG_G)
	#define LEDC_E (SEG_A | SEG_D | SEG_E | SEG_F | SEG_G)
	#define LEDC_F (SEG_A | SEG_E | SEG_F | SEG_G)
	#define LEDC_G (SEG_A | SEG_C | SEG_D | SEG_E | SEG_F)
	#define LEDC_H (SEG_B | SEG_C | SEG_E | SEG_F | SEG_G)
	#define LEDC_I (SEG_C )
	#define LEDC_J (SEG_B | SEG_C | SEG_D )
	#define LEDC_K (SEG_B | SEG_D | SEG_E | SEG_F | SEG_G)
	#define LEDC_L (SEG_D | SEG_E | SEG_F)
	#define LEDC_M (SEG_A | SEG_C | SEG_E | SEG_G)
	#define LEDC_N (SEG_C | SEG_E | SEG_G)
	#define LEDC_O (SEG_C | SEG_D | SEG_E | SEG_G)
	#define LEDC_P (SEG_A | SEG_B | SEG_E | SEG_F | SEG_G)
	#define LEDC_Q (SEG_A | SEG_B | SEG_C | SEG_F | SEG_G)
	#define LEDC_R (SEG_E | SEG_G)
	#define LEDC_S (SEG_C | SEG_D | SEG_F | SEG_G)
	#define LEDC_T (SEG_D | SEG_E | SEG_F | SEG_G)
	#define LEDC_U (SEG_C | SEG_D | SEG_E )
	#define LEDC_V (SEG_A | SEG_C | SEG_D | SEG_E)
	#define LEDC_W (SEG_B | SEG_D | SEG_F | SEG_G)
	#define LEDC_X (SEG_B | SEG_C | SEG_E | SEG_F)
	#define LEDC_Y (SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)
	#define LEDC_Z (SEG_A | SEG_D | SEG_G)
	#define LEDC_DT (SEG_H)
	#define LEDC_HH (SEG_G)
	#define LEDC_o (SEG_A | SEG_B | SEG_F | SEG_G)
	#define LEDC_c (SEG_D | SEG_E | SEG_G)
	#define LEDC_BK 0x00

//-----------------------------------------------------------------------
//--- 液晶标准字段分布 ---
//		--A--
//		F   B
//		|-G-|
//		E	C
//	    --D-- H	 
//-----------------------------------------------------------------------
	#define LCDC_0 (SCG_A | SCG_B | SCG_C | SCG_D | SCG_E | SCG_F )
	#define LCDC_1 (SCG_B | SCG_C )
	#define LCDC_2 (SCG_A | SCG_B | SCG_D | SCG_E | SCG_G )
	#define LCDC_3 (SCG_A | SCG_B | SCG_C | SCG_D | SCG_G )
	#define LCDC_4 (SCG_B | SCG_C | SCG_F | SCG_G )
	#define LCDC_5 (SCG_A | SCG_C | SCG_D | SCG_F | SCG_G)
	#define LCDC_6 (SCG_A | SCG_C | SCG_D | SCG_E | SCG_F | SCG_G)
	#define LCDC_7 (SCG_A | SCG_B | SCG_C)
	#define LCDC_8 (SCG_A | SCG_B | SCG_C | SCG_D | SCG_E | SCG_F | SCG_G)
	#define LCDC_9 (SCG_A | SCG_B | SCG_C | SCG_D | SCG_F | SCG_G)
	#define LCDC_A (SCG_A | SCG_B | SCG_C | SCG_E | SCG_F | SCG_G)
	#define LCDC_B (SCG_C | SCG_D | SCG_E | SCG_F | SCG_G)
	#define LCDC_C (SCG_A | SCG_D | SCG_E | SCG_F)
	#define LCDC_D (SCG_B | SCG_C | SCG_D | SCG_E | SCG_G)
	#define LCDC_E (SCG_A | SCG_D | SCG_E | SCG_F | SCG_G)
	#define LCDC_F (SCG_A | SCG_E | SCG_F | SCG_G)
	#define LCDC_G (SCG_A | SCG_C | SCG_D | SCG_E | SCG_F)
	#define LCDC_H (SCG_B | SCG_C | SCG_E | SCG_F | SCG_G)
	#define LCDC_I (SCG_C )
	#define LCDC_J (SCG_B | SCG_C | SCG_D )
	#define LCDC_K (SCG_B | SCG_D | SCG_E | SCG_F | SCG_G)
	#define LCDC_L (SCG_D | SCG_E | SCG_F)
	#define LCDC_M (SCG_A | SCG_C | SCG_E | SCG_G)
	#define LCDC_N (SCG_C | SCG_E | SCG_G)
	#define LCDC_O (SCG_C | SCG_D | SCG_E | SCG_G)
	#define LCDC_P (SCG_A | SCG_B | SCG_E | SCG_F | SCG_G)
	#define LCDC_Q (SCG_A | SCG_B | SCG_C | SCG_F | SCG_G)
	#define LCDC_R (SCG_E | SCG_G)
	#define LCDC_S (SCG_C | SCG_D | SCG_F | SCG_G)
	#define LCDC_T (SCG_D | SCG_E | SCG_F | SCG_G)
	#define LCDC_U (SCG_C | SCG_D | SCG_E )
	#define LCDC_V (SCG_A | SCG_C | SCG_D | SCG_E)
	#define LCDC_W (SCG_B | SCG_D | SCG_F | SCG_G)
	#define LCDC_X (SCG_B | SCG_C | SCG_E | SCG_F)
	#define LCDC_Y (SCG_B | SCG_C | SCG_D | SCG_F | SCG_G)
	#define LCDC_Z (SCG_A | SCG_D | SCG_G)
	#define LCDC_DT (SCG_H)
	#define LCDC_HH (SCG_G)
	#define LCDC_o (SCG_A | SCG_B | SCG_F | SCG_G)
	#define LCDC_c (SCG_D | SCG_E | SCG_G)
	#define LCDC_BK 0x00

#endif