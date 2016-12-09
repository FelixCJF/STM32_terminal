#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include "bsp_lcd.h"
#include <stdio.h>
#include <math.h>

#define LEDBLUE_ON		GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define LEDBLUE_OFF		GPIO_SetBits( GPIOB, GPIO_Pin_1)
#define LEDBLUE_CPL		GPIO_WriteBit( GPIOB , GPIO_Pin_1 , !GPIO_ReadOutputDataBit( GPIOB, GPIO_Pin_1 ) )
#define LEDGREEN_ON		GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define LEDGREEN_OFF	GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define LEDGREEN_CPL	GPIO_WriteBit( GPIOB , GPIO_Pin_0 , !GPIO_ReadOutputDataBit( GPIOB, GPIO_Pin_0 ) )
#define LEDRED_ON			GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define LEDRED_OFF		GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define LEDRED_CPL		GPIO_WriteBit( GPIOB , GPIO_Pin_5 , !GPIO_ReadOutputDataBit( GPIOB, GPIO_Pin_5 ) )
#define UI0BackColor  macBLACK
#define PI            3.14

//时钟结构体
typedef struct{
	u16 Year;
	u8 Day;
	u8 Month;
	u8 Date;
	u8 Hour;
	u8 Min;
	u8 Sec;
}TimeStruct;

//日历结构体
typedef struct{
	u16 Year;
	u8	Month;
	u8	Date;
}CalendarStruct;

extern int flag_TIM2;
extern int RTC_flag;
/*******************************  全局变量定义  *******************************/
extern	u16	 		KeyNum;													//键值
extern	u32			KeyCnt,KeyUpCnt;								//按键时间（单位10ms）,按键释放时间（单位10ms）
extern	u8			UICnt,UIRep;										//界面指针，界面指针备份
extern	TimeStruct 		RealTime;									//时钟
extern	CalendarStruct	Calendar,LunarCalendar;	//公历，农历
/*****************************  全局变量定义结束  *****************************/


/************  系统配置函数  ************/
void SystemConfiguration( void );
void GPIO_Configuration( void );
void NVIC_Configuration( void );
void TIM6_Configuration( void );
void TIM7_Configuration( void );
void TIM1_Configuration( void );
void TIM2_Configuration( void );
void Flash_TouchCalibrate ( void );
void RTC_Configuration( void );

/**************  功能函数  **************/
void Delay(u32 Cnt);
u16	 KeyScan( void );
void Alarm( u8 cmd );

/**************  日历功能函数  **************/
u8 Is_LeapYear( u16 year );
u8 Get_Week( u16 Year , u8 Month , u8 Date );
u8 Get_DateMax(u16 Year,u8 Month);
u8 Set_RTCTime( TimeStruct *Time );
void Read_RTCTime( TimeStruct *Time );

/**************  界面设计函数  **************/
void UI00( void );
void UI01( void );
void UI02( void );
void UI03( void );
void UI04( void );
void UI05( void );
void UI06( void );
void UI10( void );
void UI11( void );
void UI12( void );
void UI20( void );


/**************  触屏扫描函数  **************/
u16 Get_TouchKey( void );




#endif
