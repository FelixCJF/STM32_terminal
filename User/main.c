#include "main.h"
/*******************************  全局变量定义  *******************************/
u16	 		KeyNum;													//键值
u32			KeyCnt,KeyUpCnt;								//按键时间（单位10ms）,按键释放时间（单位10ms）
u8			UICnt,UIRep;										//界面指针，界面指针备份
TimeStruct 			RealTime;								//时钟
CalendarStruct	Calendar,LunarCalendar;	//公历，农历
u8 Cho = 3;
/*****************************  全局变量定义结束  *****************************/
//int flag_TIM2=0;
int main(void)
{
	SystemConfiguration();
	
	ILI9341_Clear( 0 , 0 , 240 , 320 , macWHITE );
	
	RealTime.Year = 2016;
	RealTime.Month = 12;
	RealTime.Date = 11;
	RealTime.Hour = 20;
	RealTime.Min = 52;
	RealTime.Sec = 50;
	//Set_RTCTime( &RealTime );
	Set_RTCTime( &RealTime );
	Calendar.Year = RealTime.Year;
	Calendar.Month = RealTime.Month;
	Calendar.Date = RealTime.Date;

	UICnt = 0x20;	

	while(1)
	{
		switch(UICnt)
		{
			case	0x00:	UI00();	break;
			case	0x01:	UI01();	break;
			case	0x02:	UI02();	break;
			case	0x03:	UI03();	break;
			case	0x04:	UI04();	break;
			case	0x05:	UI05();	break;
			case	0x06:	UI06();	break;
			case	0x10:	UI10();	break;
			case	0x11:	UI11();	break;
			case	0x12:	UI12();	break;
			case	0x20:	UI20();	break;
			default:	break;
		}
	}
}


/*******************************  数据表格  *******************************/
////星期显示字符串(英文)
//const char DayTab[7][4] = {
//	"Sun\0","Mon\0","Tue\0","Wed\0","Thu\0","Fri\0","Sat\0",
//};

//星期显示字符串(中文)
const char DayTab_Han[7][3] = {
	"日\0","一\0","二\0","三\0","四\0","五\0","六\0",
};

////月最大日期表
//const unsigned char DateMaxTab[13] = {
//	0,31,28,31,30,31,30,31,31,30,31,30,31,
//};

////农历日期表
//const unsigned char LunarDateName[][5]=   {
//	"  \0","初一\0","初二\0","初三\0","初四\0","初五\0","初六\0","初七\0","初八\0","初九\0","初十\0",
//	"十一\0","十二\0","十三\0","十四\0","十五\0","十六\0","十七\0","十八\0","十九\0","二十\0",
//	"廿一\0","廿二\0","廿三\0","廿四\0","廿五\0","廿六\0","廿七\0","廿八\0","廿九\0","三十\0"
//};

//农历月份表
const unsigned char LunarMonthName[][5]=   {
	"  \0","正月\0","二月\0","三月\0","四月\0","五月\0","六月\0","七月\0","八月\0","九月\0","十月\0","冬月\0","腊月\0",
};

//星期表格
const char DayTab[7][4] = {
	"Sun\0","Mon\0","Tue\0","Wed\0","Thu\0","Fri\0","Sat\0",
};

//月份大小
const unsigned char DateMaxTab[13] = {
	0,31,28,31,30,31,30,31,31,30,31,30,31,
};

//农历
const unsigned char DayDay[32][8] = {"无语",
"初一","初二","初三","初四","初五","初六","初七",
"初八","初九","初十","十一","十二","十三","十四",
"十五","十六","十七","十八","十九","二十","廿一",
"廿二","廿三","廿四","廿五","廿六","廿七",
"廿八","廿九","三十"};

/*******************************  系统配置函数  *******************************/
void SystemConfiguration( void )
{
	GPIO_Configuration();
	LCD_Init();	
	RTC_Configuration();
	FLASH_SPI_initialize();
	Flash_TouchCalibrate();
	TIM6_Configuration();
	TIM7_Configuration();
	TIM1_Configuration();
	TIM2_Configuration();
	NVIC_Configuration();
}


void GPIO_Configuration()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//必须先开启时钟再配置引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	LEDBLUE_OFF;LEDGREEN_OFF;LEDRED_OFF;
}



void NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;  

	
	//设置TIM2中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	//设置TIM6中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	//设置TIM7中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	//设置RTC中断
	//1级抢占式优先级,0级副优先级,使能引脚作为中断源
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}



/****************************************************************************
* 名    称：void TIM1_Configuration(void)
* 功    能：TIM1初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void TIM1_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// 输出比较通道 GPIO 初始化
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1 , ENABLE );
		
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	
	TIM_TimeBaseInit( TIM1 , &TIM_TimeBaseStructure);

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;// 配置为PWM模式1	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;// 输出使能	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 500;// 设置占空比大小	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;// 有效电平为高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;	//空闲输出低电平
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init( TIM1 , &TIM_OCInitStructure );
	TIM_OC1PreloadConfig( TIM1 , TIM_OCPreload_Enable );//使能TIM1 在 CCR1 上的预装载寄存器
	
	// 使能计数器
//	TIM_Cmd( TIM1 , ENABLE );	
	// 主输出使能，当使用的是通用定时器时，这句不需要
//	TIM_CtrlPWMOutputs( TIM1 , ENABLE );
}

/****************************************************************************
* 名    称：void TIM2_Configuration(void)
* 功    能：TIM2初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：设置定时器T2每10ms中断一次
* 调用方法：无 
****************************************************************************/ 
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_DeInit(TIM2);
	
	TIM_TimeBaseStructure.TIM_Period = 99;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);   
//	TIM_Cmd(TIM2, ENABLE);
}


/****************************************************************************
* 名    称：void TIM6_Configuration(void)
* 功    能：TIM6初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：设置定时器T6每10ms中断一次
* 调用方法：无 
****************************************************************************/ 
void TIM6_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_DeInit(TIM6);
	
	TIM_TimeBaseStructure.TIM_Period = 99; 							//定时10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);   
	TIM_Cmd(TIM6, ENABLE);
}

/****************************************************************************
* 名    称：void TIM7_Configuration(void)
* 功    能：TIM7初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：设置定时器T7每80ms中断一次,用于闹钟定时
* 调用方法：无 
****************************************************************************/ 
void TIM7_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	TIM_DeInit(TIM7);
	
	TIM_TimeBaseStructure.TIM_Period = 799; 							//定时80ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);	

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);   
//	TIM_Cmd(TIM7, ENABLE);
}


 /**
  * @brief  LCD触摸屏校准或从外部flash读取校准系数
  * @param  无
  * @retval 无
  */	
void Flash_TouchCalibrate ( void )
{
	uint8_t ucStoreFlag;                            //LCD触摸屏校准系数存储标志
	
	SPI_FLASH_ReadDeviceID();                       // Get SPI Flash Device ID */
	
	SPI_FLASH_BufferRead ( & ucStoreFlag, 0, 1 );   //从外部flash读出LCD触摸屏校准系数存储标志
	
	if ( ucStoreFlag == 0x52 )                                                                         //已存储过LCD触摸屏校准系数
		SPI_FLASH_BufferRead ( ( void * )  & strXPT2046_TouchPara, 1, sizeof ( strXPT2046_TouchPara ) ); //继续读取LCD触摸屏校准系数
		
	else                                          //尚未存储过LCD触摸屏校准系数
	{
		while( ! XPT2046_Touch_Calibrate () );      //等待触摸屏校正完毕

		/* 存储LCD触摸屏校准系数和标志 */
		ucStoreFlag = 0x52;
		SPI_FLASH_SectorErase ( 0 );
		SPI_FLASH_BufferWrite ( & ucStoreFlag, 0, 1 );     
		SPI_FLASH_BufferWrite ( ( void * )  & strXPT2046_TouchPara, 1, sizeof ( strXPT2046_TouchPara ) );	
	}
	
//	SPI_Flash_PowerDown();                          //进入掉电模式
	
}

/****************************************************************************
* 名    称：void RTC_Configuration(void)
* 功    能：RTC初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：初始化RTC
* 调用方法：无 
****************************************************************************/ 
void RTC_Configuration(void)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	/* Reset Backup Domain */
	BKP_DeInit();
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	PWR_BackupAccessCmd(DISABLE);
}


/**************  功能函数  **************/

void Delay(u32 Cnt)
{
	while(--Cnt);
}


u16 KeyScan()
{
	u16 key;
	
	if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1) && (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==1)) key = 3;	
	else if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0) && (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==1)) key = 2;
	else if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1) && (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0)) key = 1;
	else key = 0;
	return key;
}


/****************************************************************************
* 名    称：void Alarm( u8 cmd )
* 功    能：控制闹钟
* 入口参数：cmd	1:响铃	0:停止响铃
* 出口参数：
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void Alarm( u8 cmd )
{
	if(cmd)
	{
		TIM_Cmd( TIM7 , ENABLE );	
		TIM_Cmd( TIM1 , ENABLE );	
		TIM_CtrlPWMOutputs( TIM1 , ENABLE );
	}
	else{
		TIM_Cmd( TIM7 , DISABLE );	
		TIM_Cmd( TIM1 , DISABLE );	
		TIM_CtrlPWMOutputs( TIM1 , DISABLE );
	}
}

/**************  日历功能函数  **************/


/****************************************************************************
* 名    称：u8 Is_Leap_Year(u16 year)
* 功    能：判断是否是闰年
* 入口参数：year
* 出口参数：1:是闰年；0:非闰年
* 说    明：
* 调用方法：无 
****************************************************************************/ 
u8 Is_LeapYear(u16 year)
{
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除
			else return 0; 
		}
		else return 1; 
	}
	else return 0;
} 

/****************************************************************************
* 名    称：u8 Get_Week(u16 Year,u16 Month,u16 Date)
* 功    能：用蔡勒（Zeller）公式计算星期几	

	w=y+[y/4]+[c/4]-2c+[26（m+1）/10]+d-1

	然后w对7取余。
	公式中的符号含义如下，w：星期；c：世纪；y：年（两位数）；m：月（m大于等于3，小
	于等于14，即在蔡勒公式中，某年的1、2月要看作上一年的13、14月来计算，比如2003年1月1
	日要看作2002年的13月1日来计算）；d：日；[ ]代表取整，即只要整数部分。

* 入口参数：Year:年; Month:月; Date:日。
* 出口参数：星期几 1~6代表星期一~星期六，0代表星期天
* 说    明：
* 调用方法：无
****************************************************************************/ 
u8 Get_Week(u16 Year,u8 Month,u8 Date)
{
	int  W,Y,C,M,D;
	
	C = Year/100;
	Y = Year%100;
	M = Month;
	if(M<3){M+=12;Y--;}
	D = Date;
	W = Y + Y/4 + C/4 - 2*C + 26*(M+1)/10 + D +83;
	if(W<0)W = W+7;
	W = W % 7 ;
	return W;
}

u8 Get_DateMax(u16 Year,u8 Month)
{
	u8 DateMax;
	
	DateMax = DateMaxTab[ Month ];
	if( Month==2 )
	{
		if( Is_LeapYear( Year ) == 1 )	DateMax++;
	}
	
	return DateMax;
}

/****************************************************************************************
//绘制日历表格
//Year:年
//Month：月
//CurrentDate:当前日期
//SelectDate:选定日�
/****************************************************************************************/
void Drawing(void)
{ 
	char tbuf[40];
	char	cStr[ 10 ];						//用来存放数字转换成字符串的字符
	unsigned int Row,Col;				//日历行列号
	unsigned int CalWidth,CalHeight;	//日历每格宽，高
	unsigned int ColOffset,RowOffset;	//日历格中显示字符时横向、纵向偏移值
	unsigned int Cal_YOffset;		//日历表格纵向偏移值
	unsigned int Date;					//日期
	unsigned int DateMax;				//当月最大日期
//	unsigned int Day;						//星期
	uint8_t DayFirst;			//当月1号星期，1~6代表星期一~星期六，0代表星期天
	uint8_t count;
	//设置绘图参数
	CalWidth = 34;
	CalHeight = 42;
	RowOffset = 6;
	
	Cal_YOffset = 80;
	
	//计算月首日星期
	DayFirst = Get_Week(Calendar.Year,Calendar.Month,1);	
	
	//计算月最大日期
	DateMax = DateMaxTab[Calendar.Month];
	if(Calendar.Month==2)
	{
		if(Is_LeapYear(Calendar.Year)==1)		
		DateMax++;
	}
	
	//清屏
	ILI9341_Clear(0,0,240,320,macWHITE);
	
	//显示年月	
	ILI9341_Clear(0,0,240,54,macBLUE2);	
	ILI9341_Clear(0,54,240,18,macYELLOW);	
	sprintf ( cStr, "%d", Calendar.Year );
 	ILI9341_DispString_EN_32(0,Cal_YOffset-58,cStr,macBLUE2,macRED);
	ILI9341_DispString_EN_32(64,Cal_YOffset-58,"-",macBLUE2,macRED);
	sprintf ( cStr, "%d", Calendar.Month );
	ILI9341_DispString_EN_32(80,Cal_YOffset-58,cStr,macBLUE2,macRED);
	ILI9341_DispString_EN_CH(150,Cal_YOffset-45,"<< 今天 >>",macBLUE2,macRED);
//		sprintf((char*)tbuf,"%02d:%02d",RealTime.Hour,RealTime.Min); 
//	ILI9341_DispString_EN( 0,10, (char*)tbuf , macBLUE2,macBLACK);
	
	// 第0行，显示星期
	Row = 0;
	ColOffset = 5;//列
	
	for(Col=0;Col<7;Col++)
	{
		if(Col==0 || Col==6)
		{			
			ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-28,DayTab[Col],macYELLOW,macRED);
		}
		else
		{			
			ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-28,DayTab[Col],macYELLOW,macBLACK);
		}
	}
	
	// 第1-6行，显示日历表	
	Date = 1;
	Row = 0;
  CalWidth = 34;
	CalHeight = 40;
	RowOffset = 2;
	Cal_YOffset = 76;
	ColOffset = 13;
	
	for(Col=DayFirst;Col<7;Col++)
	{		
		sprintf ( cStr, "%d", Date );		
		if(Col==0 || Col==6)
		{			
			ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset,cStr,macWHITE,macRED);
		  ILI9341_DispString_CH(Col*CalWidth+ColOffset-9,Cal_YOffset+Row*CalHeight+RowOffset+16,DayDay[Date], macWHITE, macBLACK );
		}
		else 
		{
			ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset,cStr,macWHITE,macBLACK);
			ILI9341_DispString_CH(Col*CalWidth+ColOffset-9,Cal_YOffset+Row*CalHeight+RowOffset+16,DayDay[Date], macWHITE, macBLACK );
		}
		if(Date==Calendar.Date)
			ILI9341_DrawRectangle( Col*CalWidth+1 , Cal_YOffset+Row*CalHeight-4 , CalWidth, CalHeight+1, macBLUE , 0 );  
		if(Date==Cho)	
		{
			ILI9341_DrawRectangle( Col*CalWidth+1 , Cal_YOffset+Row*CalHeight-4 , CalWidth , CalHeight+1 , macBLUE , 1 );
      ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset,cStr,macBLUE,macWHITE);
			ILI9341_DispString_CH(Col*CalWidth+1,Cal_YOffset+Row*CalHeight+RowOffset+16,DayDay[Date], macBLUE, macBLACK );
		}
		
		Date++;
	}	
		
	if(DayFirst>4)
	{		
		count = 7;
	}
	else 
	{
		count = 6;
	}
	
	for(Row=2;Row<count;Row++)
	{
		for(Col=0;Col<7;Col++)
		{
			sprintf ( cStr, "%d", Date );
			if(Date<10) 
			{
				ColOffset = 13;
			}
			else
			{				
				ColOffset = 9;
			}
			if(Col==0 || Col==6)
			{				
				ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-40,cStr,macWHITE,macRED);
				ILI9341_DispString_CH(Col*CalWidth+1,Cal_YOffset+Row*CalHeight+RowOffset-24,DayDay[Date], macWHITE, macBLACK );
			}
			else 
			{
				ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-40,cStr,macWHITE,macBLACK);
				ILI9341_DispString_CH(Col*CalWidth+1,Cal_YOffset+Row*CalHeight+RowOffset-24,DayDay[Date], macWHITE, macBLACK );
			}
			if(Date==Calendar.Date)	
			{
				ILI9341_DrawRectangle( Col*CalWidth+1 , Cal_YOffset+Row*CalHeight-44 , CalWidth , CalHeight+1, macBLUE , 0 );
			}
			if(Date==Cho)	
			{
				ILI9341_DrawRectangle( Col*CalWidth+1 , Cal_YOffset+Row*CalHeight-44, CalWidth , CalHeight+1, macBLUE , 1 );
				ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-40,cStr,macBLUE,macWHITE);
		 		ILI9341_DispString_CH(Col*CalWidth+1,Cal_YOffset+Row*CalHeight+RowOffset-24,DayDay[Date], macBLUE, macBLACK );
			}
			
			Date++;
			if(Date>DateMax)	break;
		}
	}
	
	//显示最后一周中下月日期，用灰色显示
	Row--;
	if(Col<6)
	{
		Col++;
		Date = 1;
		for(;Col<7;Col++)
		{
			sprintf ( cStr, "%d", Date );
			ColOffset = 13;
			ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-40,cStr,macWHITE,macGREY);
			
			Date++;
		}	
	}
	//显示第一周中上月日期，用灰色显示
	if(DayFirst!=0)
	{
		RealTime.Month--;
		if(RealTime.Month==0)
		{
			RealTime.Year--;
			RealTime.Month = 12;
		}
		
		DateMax = DateMaxTab[RealTime.Month];
		
		if(RealTime.Month==2)
		{
			if(Is_LeapYear(RealTime.Year)==1)
				DateMax++;
		}
		
		Row = 1;
		Date = DateMax + 1 - DayFirst;		
		for(Col=0;Col<DayFirst;Col++)
		{
			sprintf ( cStr, "%d", Date );
			ColOffset = 9;
			ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-40,cStr,macWHITE,macGREY);
			
			Date++;
		}	
	}
  CalHeight = 40;
	Row = 0;
	ILI9341_DrawLine(5,(Row+1)*24+Cal_YOffset-28,235,(Row+1)*24+Cal_YOffset-28,macBLUE);
	// Draw Line

	if(DayFirst>4) count = 7;
	else count = 6;
	
	for(Row=1;Row<count;Row++)
  {	
	  ILI9341_DrawLine(5,(Row+1)*CalHeight+Cal_YOffset-44,235,(Row+1)*CalHeight+Cal_YOffset-44,macBLUE);
  }
}

/****************************************************************************
* 名    称：void Cal_Time(u32 Count,TimeStruct *Time)
* 功    能：根据32位Count计数值计算日期和时间
* 入口参数：Count
* 出口参数：T_STRUCT* Time
* 说    明：
* 调用方法：Cal_Time(Count,&RealTime);
****************************************************************************/ 
void Cal_Time(u32 Count,TimeStruct *Time)
{
	u32 SecTemp=0,DateTemp=0,Temp=0;
	
	DateTemp = Count/86400;					//天数
	if(DateTemp!=0)									//超过1天
	{
		//确定年
		Temp = 1970;
		while(DateTemp>=365)
		{
			if(Is_LeapYear(Temp))	//闰年?
			{
				if(DateTemp>=366) DateTemp -= 366;
				else 
				{
					break;
				}
			}
			else DateTemp -= 365;
			Temp++;
		}
		Time->Year = Temp;
		
		//确定月
		Temp = 1;
		while(DateTemp>=28)
		{
			if(Is_LeapYear(Time->Year)&&Temp==2)
			{
				if(DateTemp>=29)DateTemp -= 29;//闰年的二月天数
        else break; 
			}
			else
			{
				if(DateTemp>=DateMaxTab[Temp]) DateTemp -= DateMaxTab[Temp];
				else break;
			}
			Temp++;
		}
		Time->Month = Temp;
		Time->Date = DateTemp+1;
	}
	else
	{
		Time->Year = 1970;
		Time->Month = 1;
		Time->Date = 1;
	}
	SecTemp = Count%86400;
	Time->Hour = SecTemp/3600;
	Time->Min  = SecTemp%3600/60;
	Time->Sec  = SecTemp%60;
	Time->Day  = Get_Week(Time->Year,Time->Month,Time->Date);
}

/****************************************************************************
* 名    称：u8 Set_RTCTime(TimeStruct *Time)
* 功    能：设置时钟:以1970-01-01 0:0:0为基准,把输入的时钟转换为秒钟,
* 					并写入RTC计数器，1970~2099年为合法年份。
* 入口参数：T_STRUCT* Time
* 出口参数：0:成功;
						1:错误代码，非有效时间(1970~2099)
* 说    明：
* 调用方法：Set_RTCTime(&RealTime);
****************************************************************************/ 
u8 Set_RTCTime( TimeStruct *Time)
{
	u32 Count = 0;
	u16 i;
	if(Time->Year<1970||Time->Year>2099) return 1;
	for(i=1970;i<Time->Year;i++)
	{
		if(Is_LeapYear(i)) Count += 31622400;		//闰年的秒数
		else Count += 31536000;									//平年的秒数
	}
	for(i=1;i<Time->Month;i++)
	{
		Count += (u32)DateMaxTab[i]*86400;
		if(Is_LeapYear(Time->Year) && i==2) Count += 86400;
	}
	Count += (u32)(Time->Date-1)*86400;
	Count += (u32)(Time->Hour)*3600;
	Count += (u32)(Time->Min)*60;
	Count += (u32)(Time->Sec);
	
	//使能PWR和BKP外设时钟,这句一定不能省	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	 
	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
	
	RTC_SetCounter(Count);
	RTC_WaitForLastTask();

	return 0;   
}




/****************************************************************************
* 名    称：void Read_RTCTime(TimeStruct *Time)
* 功    能：从RTC计数器读取时间		
* 入口参数：T_STRUCT* Time
* 出口参数：无
* 说    明：
* 调用方法：Read_RTCTime(&RealTime);
****************************************************************************/ 
void Read_RTCTime( TimeStruct *Time)
{
	u32 Count = 0;
	Count = RTC_GetCounter();
	Cal_Time(Count,Time);
}






/**************  界面设计函数  **************/
void UI00( void )
{
	u16 TouchKey;
	float s_angle;
	int sx,sy;
	int sx1,sy1;
	int sx2,sy2;
	int Ox = 120;
	int Oy = 130;
	char tbuf[40];
	
	
	// 步骤1:调用LCD函数绘制图形界面
	
	
// 	int sx=(int)Radius*cos(s_angle)+Ox;
// 	int sy=(int)Radius*sin(s_angle)+Oy;

	ILI9341_Clear( 0, 0, 240, 320, UI0BackColor);
	ILI9341_DispString_CH( 23, 20, "时钟", macBLACK, macWHITE );
	ILI9341_DispString_CH( 77, 20, "闹钟", macBLACK, macWHITE );
	ILI9341_DispString_CH( 131, 20, "秒表", macBLACK, macWHITE );
	ILI9341_DispString_CH( 185, 20, "计时", macBLACK, macWHITE );
 	ILI9341_DrawCircle( Ox, Oy, 70, macWHITE, 1);
//	ILI9341_DrawLine( 120, 130,(int)60*cos(s_angle1)+120,(int)60*sin(s_angle1)+130, macGREEN);
// 	ILI9341_DrawLine( 120, 130,(int)50*cos(s_angle2)+120,(int)50*sin(s_angle2)+130, macRED);
//	ILI9341_DrawLine( 120, 130, (int)40*cos(s_angle3)+120,(int)40*sin(s_angle3)+130, macBLUE);
//  ILI9341_DispString_EN_CH( 60, 220, "12月10日星期三", macBLACK, macWHITE );
	ILI9341_DrawCircle( 120, 280, 30, macWHITE, 1);
	ILI9341_DispString_CH( 107, 273, "设置",macWHITE, macBLUE);
	// 步骤2:界面循环

	while(1)
	{
	//更新时间
	/*秒针计算*/	
	s_angle=(2*PI/60)*RealTime.Sec-PI/2;
	sx=(int)60*cos(s_angle)+Ox;
  sy=(int)60*sin(s_angle)+Oy;
	/*秒针显示*/
  ILI9341_DrawLine(Ox,Oy,sx,sy,macRED);
		
	/*分针计算*/
	s_angle=(2*PI/60)*RealTime.Min-PI/2;
	sx1=(int)50*cos(s_angle)+Ox;
  sy1=(int)50*sin(s_angle)+Oy;
	/*分针显示*/
	ILI9341_DrawLine(Ox,Oy,sx1,sy1,macBLUE);
			
	/*时针计算*/
	s_angle=(2*PI/12)*RealTime.Hour-PI/2;
	sx2=(int)40*cos(s_angle)+Ox;
  sy2=(int)40*sin(s_angle)+Oy;
	/*时针显示*/
	ILI9341_DrawLine(Ox,Oy,sx2,sy2,macGREEN);
	Delay(1000000);
	
	sprintf((char*)tbuf,"%02d:%02d:%02d",RealTime.Hour,RealTime.Min,RealTime.Sec); 
	ILI9341_DispString_EN( 89,210, (char*)tbuf , macBLACK,macWHITE);
			
	sprintf((char*)tbuf,"%02d-%02d-%02d",RealTime.Year,RealTime.Month,RealTime.Date);		
	ILI9341_DispString_EN( 79,230, (char*)tbuf , macBLACK,macWHITE);

		//判断是否闹钟时间
		//......
		
		//按键处理
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey==121 )
			{
				while( tp_dev.sta );
				UICnt = 0x20;		//修改界面指针
				break;	
			}
			else if( TouchKey==122 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x01;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==123 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x02;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==124 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x03;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==125 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x010;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==126 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x04;		//修改界面指针
				break;					//退出
			}
		}
		/*秒针擦除*/
	ILI9341_DrawLine(Ox,Oy,sx,sy,macWHITE);
		/*分针擦除*/
	ILI9341_DrawLine(Ox,Oy,sx1,sy1,macWHITE);
		/*时针擦除*/
	ILI9341_DrawLine(Ox,Oy,sx2,sy2,macWHITE);
	}
}
//闹钟界面
void UI01( void )
{
	
	u16 TouchKey;
	float s_angle;
	int sx,sy;
	int sx1,sy1;
	int sx2,sy2;
	int Ox = 120;
	int Oy = 100;
	char tbuf[40];	
	int i;

	
	ILI9341_Clear( 0, 0, 240, 320, UI0BackColor);
	ILI9341_DispString_CH( 23, 20, "时钟", macBLACK, macWHITE );
	ILI9341_DispString_CH( 77, 20, "闹钟", macBLACK, macWHITE );
	ILI9341_DispString_CH( 131, 20, "秒表", macBLACK, macWHITE );
	ILI9341_DispString_CH( 185, 20, "计时", macBLACK, macWHITE );
	
	ILI9341_DrawCircle( 120, 100, 50, macWHITE, 1);
	
	ILI9341_DrawRectangle( 0, 155, 240, 50, macWHITE, 1 );
	ILI9341_DispString_EN_32( 10, 155, "00:00", macWHITE, macBLACK );
	ILI9341_DrawLine( 10, 183,151,183, macBLUE);
	ILI9341_DrawRectangle( 185, 165, 40, 20, macBLUE, 1 );
	ILI9341_DrawRectangle( 205, 165, 20, 20, macBLACK, 1 );
	for (i=0; i<7; i++)
	{
		ILI9341_DispString_CH ( 10+i*20, 185, DayTab_Han[i], macWHITE, macBLACK );
	}
	
	ILI9341_DrawRectangle( 0, 210, 240, 50, macWHITE, 1 );
	ILI9341_DispString_EN_32( 10, 210, "00:00", macWHITE, macBLACK );
	ILI9341_DrawLine( 10, 237,151,237, macBLUE);
	ILI9341_DrawRectangle( 185, 220, 40, 20, macBLUE, 1 );
	ILI9341_DrawRectangle( 205, 220, 20, 20, macBLACK, 1 );
	for (i=0; i<7; i++)
	{
		ILI9341_DispString_CH ( 10+i*20, 240, DayTab_Han[i], macWHITE, macBLACK );
	}
	
	ILI9341_DrawRectangle( 0, 265, 240, 50, macWHITE, 1 );
	ILI9341_DispString_EN_32( 10, 265, "00:00", macWHITE, macBLACK );
	ILI9341_DrawLine( 10, 292,151,292, macBLUE);
	ILI9341_DrawRectangle( 185, 275, 40, 20, macBLUE, 1 );
	ILI9341_DrawRectangle( 205, 275, 20, 20, macBLACK, 1 );
	for (i=0; i<7; i++)
	{
		ILI9341_DispString_CH ( 10+i*20, 295, DayTab_Han[i], macWHITE, macBLACK );
	}
		
	
	while(1)
	{
		//更新时间
		/*秒针计算*/	
		s_angle=(2*PI/60)*RealTime.Sec-PI/2;
		sx=(int)45*cos(s_angle)+Ox;
		sy=(int)45*sin(s_angle)+Oy;
		/*秒针显示*/
		ILI9341_DrawLine(Ox,Oy,sx,sy,macRED);
		
		/*分针计算*/
		s_angle=(2*PI/60)*RealTime.Min-PI/2;
		sx1=(int)35*cos(s_angle)+Ox;
		sy1=(int)35*sin(s_angle)+Oy;
		/*分针显示*/
		ILI9341_DrawLine(Ox,Oy,sx1,sy1,macBLUE);
			
		/*时针计算*/
		s_angle=(2*PI/12)*RealTime.Hour-PI/2;
		sx2=(int)25*cos(s_angle)+Ox;
		sy2=(int)25*sin(s_angle)+Oy;
		/*时针显示*/
		ILI9341_DrawLine(Ox,Oy,sx2,sy2,macGREEN);
		Delay(1000000);
		
		//按键处理
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey==121 )
			{
				//按键功能...
				while( tp_dev.sta );
				UICnt = 0x00;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==122 )
			{
				while( tp_dev.sta );
				UICnt = 0x20;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==123 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x02;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==124 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x03;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==138 )
			{
				
			}
			else if( TouchKey==148 )
			{
				
			}
			else if( TouchKey==158 )
			{
				
			}
			else if( TouchKey==137 )
			{
				while( tp_dev.sta );
				UICnt = 0x06;		//修改界面指针
				break;					//退出		
			}
			else if( TouchKey==147 )
			{
				while( tp_dev.sta );
				UICnt = 0x06;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==157 )
			{
				while( tp_dev.sta );
				UICnt = 0x06;		//修改界面指针
				break;					//退出
			}
		}
		
		/*秒针擦除*/
	ILI9341_DrawLine(Ox,Oy,sx,sy,macWHITE);
		/*分针擦除*/
	ILI9341_DrawLine(Ox,Oy,sx1,sy1,macWHITE);
		/*时针擦除*/
	ILI9341_DrawLine(Ox,Oy,sx2,sy2,macWHITE);
	}
}

//秒表界面
int flag_TIM2=0;
void UI02( void )
{
	unsigned int min=0,sec=0,msec=0;//加的
	char tbuf[40];
	u16 TouchKey;
	float s_angle;
	int sx,sy;
	int sx1,sy1;
	int sx2,sy2;
	int Ox = 120;
	int Oy = 100;
	
	ILI9341_Clear( 0, 0, 240, 320, UI0BackColor);
	ILI9341_DispString_CH( 23, 20, "时钟", macBLACK, macWHITE );
	ILI9341_DispString_CH( 77, 20, "闹钟", macBLACK, macWHITE );
	ILI9341_DispString_CH( 131, 20, "秒表", macBLACK, macWHITE );
	ILI9341_DispString_CH( 185, 20, "计时", macBLACK, macWHITE );
	
	ILI9341_DrawCircle( Ox, Oy, 50, macWHITE, 1);
	
//	ILI9341_DispString_CH( 75, 180, "分钟", macBLACK, macWHITE );
//	ILI9341_DispString_EN_48( 60, 200, "00", macBLACK, macWHITE);
//	ILI9341_DispString_CH( 140, 180, "秒", macBLACK, macWHITE );
//	ILI9341_DispString_EN_48( 112, 200, ":", macBLACK, macWHITE);
//	ILI9341_DispString_EN_48( 130, 200, "00", macBLACK, macWHITE);
		
	
	ILI9341_DrawRectangle( 6, 285, 72, 30, macWHITE, 1 );//开始
	ILI9341_DispString_CH( 26, 292,"开始",  macWHITE, macBLUE);
	ILI9341_DrawRectangle( 84, 285, 72, 30, macWHITE, 1 );//暂停
	ILI9341_DispString_CH( 104, 292,"暂停", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 162, 285, 72, 30, macWHITE, 1 );//重置
	ILI9341_DispString_CH( 182, 292,"重置",  macWHITE, macBLUE);
	
	while(1)
	{
		//更新时间
		/*秒针计算*/	
		s_angle=(2*PI/60)*RealTime.Sec-PI/2;
		sx=(int)45*cos(s_angle)+Ox;
		sy=(int)45*sin(s_angle)+Oy;
		/*秒针显示*/
		ILI9341_DrawLine(Ox,Oy,sx,sy,macRED);
		
		/*分针计算*/
		s_angle=(2*PI/60)*RealTime.Min-PI/2;
		sx1=(int)35*cos(s_angle)+Ox;
		sy1=(int)35*sin(s_angle)+Oy;
		/*分针显示*/
		ILI9341_DrawLine(Ox,Oy,sx1,sy1,macBLUE);
		
		
		/*时针计算*/
		s_angle=(2*PI/12)*RealTime.Hour-PI/2;
		sx2=(int)25*cos(s_angle)+Ox;
		sy2=(int)25*sin(s_angle)+Oy;
		/*时针显示*/
		ILI9341_DrawLine(Ox,Oy,sx2,sy2,macGREEN);
		Delay(1000000);
		
		if(flag_TIM2==1)
		{
			flag_TIM2=0;
			msec++;
			if(msec>99)
			{
				msec=0;
				sec++;
				if(sec>59)
				{
					sec=0;
					min++;
					if(min>59)
					{
						min=0;
						sec=0;
						msec=0;
						
						TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); //关中断  
						TIM_Cmd(TIM2, DISABLE);	//关定时器
					}
				}
			}
		}
		
		sprintf(tbuf,"%02d:%02d.%02d",min,sec,msec);
		ILI9341_DispString_EN_48(20,180,tbuf,macBLACK,macWHITE);
		
		//按键处理
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if(tp_dev.x>=6 && tp_dev.x<78 && tp_dev.y>=285&& tp_dev.y<315 ) //开始
			{
					TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //开中断  
					TIM_Cmd(TIM2, ENABLE);	//开定时器
					sprintf(tbuf,"%02d:%02d.%02d",min,sec,msec);
					ILI9341_DispString_EN_48(20,180,tbuf,macBLACK,macWHITE);
			}
			if(tp_dev.x>=84 && tp_dev.x<156 && tp_dev.y>=285&& tp_dev.y<315 ) //暂停
			{
					TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); //关中断  
					TIM_Cmd(TIM2, DISABLE);	//关定时器
					sprintf(tbuf,"%02d:%02d.%02d",min,sec,msec);
				  ILI9341_DispString_EN_48(20,180,tbuf,macBLACK,macWHITE);		
			}
			if(tp_dev.x>=162 && tp_dev.x<234 && tp_dev.y>=285&& tp_dev.y<315 ) //重置
			{
				min=0;
				sec=0;
				msec=0;
				
				TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); //关中断  
				TIM_Cmd(TIM2, DISABLE);	//关定时器
				
				sprintf(tbuf,"%02d:%02d.%02d",min,sec,msec);
				ILI9341_DispString_EN_48(20,180,tbuf,macBLACK,macWHITE);			
			}
			
		}
		
		//按键处理
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
		
			if( TouchKey==121 )
			{
				//按键功能...
				while( tp_dev.sta );
				UICnt = 0x00;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==122 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x01;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==123 )
			{
				while( tp_dev.sta );
				UICnt = 0x20;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==124 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x03;		//修改界面指针
				break;					//退出
			}
		}
		
	/*秒针擦除*/
	ILI9341_DrawLine(Ox,Oy,sx,sy,macWHITE);
	/*分针擦除*/
	ILI9341_DrawLine(Ox,Oy,sx1,sy1,macWHITE);
	/*时针擦除*/
	ILI9341_DrawLine(Ox,Oy,sx2,sy2,macWHITE);
	}
}
//计时界面
int RTC_flag=0;
void UI03( void )
{
	int min=1,sec=00,count = 0;
	char tbuf[40];
	u16 TouchKey;
	float s_angle;
	int sx,sy;
	int sx1,sy1;
	int sx2,sy2;
	int Ox = 120;
	int Oy = 100;
	
	
	ILI9341_Clear( 0, 0, 240, 320, UI0BackColor);
	ILI9341_DispString_CH( 23, 20, "时钟", macBLACK, macWHITE );
	ILI9341_DispString_CH( 77, 20, "闹钟", macBLACK, macWHITE );
	ILI9341_DispString_CH( 131, 20, "秒表", macBLACK, macWHITE );
	ILI9341_DispString_CH( 185, 20, "计时", macBLACK, macWHITE );
	
	ILI9341_DrawCircle( 120, 100, 50, macWHITE, 1);


	ILI9341_DispString_CH( 75, 180, "分钟", macBLACK, macWHITE );
	sprintf(tbuf,"%02d",min);
	ILI9341_DispString_EN_48( 60, 200, tbuf, macBLACK, macWHITE);
	ILI9341_DispString_CH( 140, 180, "秒", macBLACK, macWHITE );
	ILI9341_DispString_EN_48( 112, 200, ":", macBLACK, macWHITE);
	sprintf(tbuf,"%02d",sec);
	ILI9341_DispString_EN_48( 130, 200, tbuf, macBLACK, macWHITE);
	
	ILI9341_DrawRectangle( 6, 285, 72, 30, macWHITE, 1 );//开始
	ILI9341_DispString_CH( 26, 292,"开始",  macWHITE, macBLUE);
	ILI9341_DrawRectangle( 84, 285, 72, 30, macWHITE, 1 );//暂停
	ILI9341_DispString_CH( 104, 292,"暂停", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 162, 285, 72, 30, macWHITE, 1 );//重置
	ILI9341_DispString_CH( 182, 292,"重置",  macWHITE, macBLUE);
	
	while(1)
	{
		//更新时间
		/*秒针计算*/	
		s_angle=(2*PI/60)*RealTime.Sec-PI/2;
		sx=(int)45*cos(s_angle)+Ox;
		sy=(int)45*sin(s_angle)+Oy;
		/*秒针显示*/
		ILI9341_DrawLine(Ox,Oy,sx,sy,macRED);
		
		/*分针计算*/
		s_angle=(2*PI/60)*RealTime.Min-PI/2;
		sx1=(int)35*cos(s_angle)+Ox;
		sy1=(int)35*sin(s_angle)+Oy;
		/*分针显示*/
		ILI9341_DrawLine(Ox,Oy,sx1,sy1,macBLUE);
		
		
		/*时针计算*/
		s_angle=(2*PI/12)*RealTime.Hour-PI/2;
		sx2=(int)25*cos(s_angle)+Ox;
		sy2=(int)25*sin(s_angle)+Oy;
		/*时针显示*/
		ILI9341_DrawLine(Ox,Oy,sx2,sy2,macGREEN);
		Delay(1000000);
		
	if ( tp_dev.sta )
	{
		if(  tp_dev.y>=285&& tp_dev.y<315 && tp_dev.x>=6 && tp_dev.x<78)
			
		{	
			while(1)
			{
				if(  tp_dev.y>=285&& tp_dev.y<315 && tp_dev.x>=64 && tp_dev.x<156)
					break;
				if(RTC_flag==1)
				{
					RTC_flag=0;			
					sec--;			
					if(sec<0)
					{
		//				RTC_flag=0;
						sec=59;
						min--;
						if(min<0)
						{
							RTC_flag=1;
							min=0;
						 sec=0;
						}					
					}
				}

			sprintf(tbuf,"%02d",min);
			ILI9341_DispString_EN_48( 60, 200, tbuf, macBLACK, macWHITE);
			sprintf(tbuf,"%02d",sec);
			ILI9341_DispString_EN_48( 130, 200, tbuf, macBLACK, macWHITE);
			}
		}		
	 }
		
		//按键处理
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey==121 )
			{
				//按键功能...
				while( tp_dev.sta );
				UICnt = 0x00;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==122 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x01;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==123 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x02;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==124 )
			{
				while( tp_dev.sta );
				UICnt = 0x20;		//修改界面指针
				break;					//退出
			}
		}
	/*秒针擦除*/
	ILI9341_DrawLine(Ox,Oy,sx,sy,macWHITE);
	/*分针擦除*/
	ILI9341_DrawLine(Ox,Oy,sx1,sy1,macWHITE);
	/*时针擦除*/
	ILI9341_DrawLine(Ox,Oy,sx2,sy2,macWHITE);
	}
}
//时钟设置界面
void UI04( void )
{
	u16 TouchKey;
	
	ILI9341_Clear( 0, 0, 240, 320, UI0BackColor);
	ILI9341_DispString_EN_CH( 10, 10, "<< 返回", macBLACK, macWHITE );
	ILI9341_DispString_CH( 185, 10,"保存", macBLACK, macWHITE );
	
	ILI9341_DispString_EN( 50, 40,"2016", macBLACK, macWHITE );
	ILI9341_DispString_CH( 90, 40,"年", macBLACK, macWHITE );
	ILI9341_DispString_EN( 110, 40,"12", macBLACK, macWHITE );
	ILI9341_DispString_CH( 130, 40,"月", macBLACK, macWHITE );
	ILI9341_DispString_EN( 150, 40,"10", macBLACK, macWHITE );
	ILI9341_DispString_CH( 170, 40,"日", macBLACK, macWHITE );
	ILI9341_DispString_EN_32( 56, 65,"10:11:12", macBLACK, macWHITE );

	ILI9341_DrawRectangle( 6, 180, 72, 30, macWHITE, 1 );//1
	ILI9341_DispString_EN( 38, 187,"1", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 6, 215, 72, 30, macWHITE, 1 );//4
	ILI9341_DispString_EN( 38, 222,"4", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 6, 250, 72, 30, macWHITE, 1 );//7
	ILI9341_DispString_EN( 38, 257,"7", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 6, 285, 72, 30, macWHITE, 1 );//清除
	ILI9341_DispString_CH( 26, 292,"清除",  macWHITE, macBLUE);
	
	ILI9341_DrawRectangle( 84, 180, 72, 30, macWHITE, 1 );//2
	ILI9341_DispString_EN( 116, 187,"2", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 84, 215, 72, 30, macWHITE, 1 );//5
	ILI9341_DispString_EN( 116, 222,"5", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 84, 250, 72, 30, macWHITE, 1 );//8
	ILI9341_DispString_EN( 116, 257,"8", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 84, 285, 72, 30, macWHITE, 1 );//0
	ILI9341_DispString_EN( 116, 292,"0", macWHITE,  macBLUE);
	
	ILI9341_DrawRectangle( 162, 180, 72, 30, macWHITE, 1 );//3
	ILI9341_DispString_EN( 194, 187,"3", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 162, 215, 72, 30, macWHITE, 1 );//6
	ILI9341_DispString_EN( 194, 222,"6", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 162, 250, 72, 30, macWHITE, 1 );//9
	ILI9341_DispString_EN( 194, 257,"9", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 162, 285, 72, 30, macWHITE, 1 );//确定
	ILI9341_DispString_CH( 182, 292,"确定",  macWHITE, macBLUE);
	
	while(1)
	{
		
		//按键处理
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey==141)
			{
				//按键功能...
				while( tp_dev.sta );
				UICnt = 0x00;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==122 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x01;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==123 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x02;		//修改界面指针
				break;					//退出
			}
			else if( TouchKey==124 )
			{
				//按键功能...
				while( tp_dev.sta );
			}
		}
	}
}
//闹钟工作界面
void UI05( void )
{
}
//闹钟设置页面
void UI06( void )
{
	int i=0;
	u16 TouchKey;
	
	ILI9341_Clear( 0, 0, 240, 320, UI0BackColor);
	ILI9341_DispString_EN_CH( 10, 10, "<< 闹钟", macBLACK, macWHITE );
	ILI9341_DispString_CH( 185, 10,"保存", macBLACK, macWHITE );
	
	
	ILI9341_DispString_EN_48( 60, 40,"06",  macBLACK, macBLUE2);
	ILI9341_DispString_EN_48( 108, 40,":",  macBLACK, macBLUE2);
	ILI9341_DispString_EN_48( 124, 40,"30",  macBLACK, macBLUE2);
	
	for (i=0; i<7; i++)
	{
		ILI9341_DispString_CH ( 40+i*25, 91, DayTab_Han[i], macBLACK, macRED );
	}
	
	
	ILI9341_DrawRectangle( 6, 180, 72, 30, macWHITE, 1 );//1
	ILI9341_DispString_EN( 38, 187,"1", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 6, 215, 72, 30, macWHITE, 1 );//4
	ILI9341_DispString_EN( 38, 222,"4", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 6, 250, 72, 30, macWHITE, 1 );//7
	ILI9341_DispString_EN( 38, 257,"7", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 6, 285, 72, 30, macWHITE, 1 );//清除
	ILI9341_DispString_CH( 26, 292,"清除",  macWHITE, macBLUE);
	
	ILI9341_DrawRectangle( 84, 180, 72, 30, macWHITE, 1 );//2
	ILI9341_DispString_EN( 116, 187,"2", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 84, 215, 72, 30, macWHITE, 1 );//5
	ILI9341_DispString_EN( 116, 222,"5", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 84, 250, 72, 30, macWHITE, 1 );//8
	ILI9341_DispString_EN( 116, 257,"8", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 84, 285, 72, 30, macWHITE, 1 );//0
	ILI9341_DispString_EN( 116, 292,"0", macWHITE,  macBLUE);
	
	ILI9341_DrawRectangle( 162, 180, 72, 30, macWHITE, 1 );//3
	ILI9341_DispString_EN( 194, 187,"3", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 162, 215, 72, 30, macWHITE, 1 );//6
	ILI9341_DispString_EN( 194, 222,"6", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 162, 250, 72, 30, macWHITE, 1 );//9
	ILI9341_DispString_EN( 194, 257,"9", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 162, 285, 72, 30, macWHITE, 1 );//确定
	ILI9341_DispString_CH( 182, 292,"确定",  macWHITE, macBLUE);

	while(1)
	{
		//按键处理
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey==171)
			{
				//按键功能...
				while( tp_dev.sta );
				UICnt = 0x01;		//修改界面指针
				break;					//退出
			}
		}
	}
}
//日历
void UI10( void )
{
	char tbuf[40];
	
	strType_XPT2046_Coordinate strDisplayCoordinate;
	char	cStr[ 10 ];						//用来存放数字转换成字符串的字符
	unsigned int Row,Col;				//日历行列号
	unsigned int CalWidth,CalHeight;	//日历每格宽，高
	unsigned int ColOffset,RowOffset;	//日历格中显示字符时横向、纵向偏移值
	unsigned int Cal_YOffset;		//日历表格纵向偏移值
	unsigned int Date;					//日期
	unsigned int DateMax;				//当月最大日期
//	unsigned int Day;						//星期
	uint8_t DayFirst;			//当月1号星期，1~6代表星期一~星期六，0代表星期天
	uint8_t count;
  uint16_t Last;//参数
	//设置绘图参数
	CalWidth = 34;
	CalHeight = 42;
	RowOffset = 6;
	
	Cal_YOffset = 80;
	
	//计算月首日星期
	DayFirst = Get_Week(Calendar.Year,Calendar.Month,1);	
	
	//计算月最大日期
	DateMax = DateMaxTab[Calendar.Month];
	if(Calendar.Month==2)
	{
		if(Is_LeapYear(Calendar.Year)==1)		
		DateMax++;
	}
	
	//清屏
	ILI9341_Clear(0,0,240,320,macWHITE);
	
	//显示年月	
	ILI9341_Clear(0,0,240,54,macBLUE2);	
	ILI9341_Clear(0,54,240,18,macYELLOW);	
	sprintf ( cStr, "%d", Calendar.Year );
 	ILI9341_DispString_EN_32(0,Cal_YOffset-58,cStr,macBLUE2,macRED);
	ILI9341_DispString_EN_32(64,Cal_YOffset-58,"-",macBLUE2,macRED);
	sprintf ( cStr, "%d", Calendar.Month );
	ILI9341_DispString_EN_32(80,Cal_YOffset-58,cStr,macBLUE2,macRED);
	ILI9341_DispString_EN_CH(150,Cal_YOffset-45,"<< 今天 >>",macBLUE2,macRED);
//	sprintf((char*)tbuf,"%02d:%02d",RealTime.Hour,RealTime.Min); 
//	ILI9341_DispString_EN( 0,10, (char*)tbuf , macBLUE2,macBLACK);
//	
	// 第0行，显示星期
	Row = 0;
	ColOffset = 5;//列
	
	for(Col=0;Col<7;Col++)
	{
		if(Col==0 || Col==6)
		{			
			ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-28,DayTab[Col],macYELLOW,macRED);
		}
		else
		{			
			ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-28,DayTab[Col],macYELLOW,macBLACK);
		}
	}
	
	// 第1-6行，显示日历表	
	Date = 1;
	Row = 0;
  CalWidth = 34;
	CalHeight = 40;
	RowOffset = 2;
	Cal_YOffset = 76;
	ColOffset = 13;
	
	for(Col=DayFirst;Col<7;Col++)
	{		
		sprintf ( cStr, "%d", Date );		
		if(Col==0 || Col==6)
		{			
			ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset,cStr,macWHITE,macRED);
		  ILI9341_DispString_CH(Col*CalWidth+ColOffset-9,Cal_YOffset+Row*CalHeight+RowOffset+16,DayDay[Date], macWHITE, macBLACK );
		}
		else 
		{
			ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset,cStr,macWHITE,macBLACK);
			ILI9341_DispString_CH(Col*CalWidth+ColOffset-9,Cal_YOffset+Row*CalHeight+RowOffset+16,DayDay[Date], macWHITE, macBLACK );
		}
		if(Date==Calendar.Date)
			ILI9341_DrawRectangle( Col*CalWidth+1 , Cal_YOffset+Row*CalHeight-4 , CalWidth, CalHeight+1, macBLUE , 0 );  
		if(Date==Cho)	
		{
			ILI9341_DrawRectangle( Col*CalWidth+1 , Cal_YOffset+Row*CalHeight-4 , CalWidth , CalHeight+1 , macBLUE , 1 );
      ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset,cStr,macBLUE,macWHITE);
			ILI9341_DispString_CH(Col*CalWidth+1,Cal_YOffset+Row*CalHeight+RowOffset+16,DayDay[Date], macBLUE, macBLACK );
		}
		
		Date++;
	}	
		
	if(DayFirst>4)
	{		
		count = 7;
	}
	else 
	{
		count = 6;
	}
	
	for(Row=2;Row<count;Row++)
	{
		for(Col=0;Col<7;Col++)
		{
			sprintf ( cStr, "%d", Date );
			if(Date<10) 
			{
				ColOffset = 13;
			}
			else
			{				
				ColOffset = 9;
			}
			if(Col==0 || Col==6)
			{				
				ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-40,cStr,macWHITE,macRED);
				ILI9341_DispString_CH(Col*CalWidth+1,Cal_YOffset+Row*CalHeight+RowOffset-24,DayDay[Date], macWHITE, macBLACK );
			}
			else 
			{
				ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-40,cStr,macWHITE,macBLACK);
				ILI9341_DispString_CH(Col*CalWidth+1,Cal_YOffset+Row*CalHeight+RowOffset-24,DayDay[Date], macWHITE, macBLACK );
			}
			if(Date==Calendar.Date)	
			{
				ILI9341_DrawRectangle( Col*CalWidth+1 , Cal_YOffset+Row*CalHeight-44 , CalWidth , CalHeight+1, macBLUE , 0 );
			}
			if(Date==Cho)	
			{
				ILI9341_DrawRectangle( Col*CalWidth+1 , Cal_YOffset+Row*CalHeight-44, CalWidth , CalHeight+1, macBLUE , 1 );
				ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-40,cStr,macBLUE,macWHITE);
		 		ILI9341_DispString_CH(Col*CalWidth+1,Cal_YOffset+Row*CalHeight+RowOffset-24,DayDay[Date], macBLUE, macBLACK );
			}
			
			Date++;
			if(Date>DateMax)	break;
		}
	}
	
	//显示最后一周中下月日期，用灰色显示
	Row--;
	if(Col<6)
	{
		Col++;
		Date = 1;
		for(;Col<7;Col++)
		{
			sprintf ( cStr, "%d", Date );
			ColOffset = 13;
			ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-40,cStr,macWHITE,macGREY);
			
			Date++;
		}	
	}
	//显示第一周中上月日期，用灰色显示
	if(DayFirst!=0)
	{
		Calendar.Month--;
		if(Calendar.Month==0)
		{
			Calendar.Year--;
			Calendar.Month = 12;
		}
		
		DateMax = DateMaxTab[Calendar.Month];
		
		if(Calendar.Month==2)
		{
			if(Is_LeapYear(Calendar.Year)==1)
				DateMax++;
		}
		
		Row = 1;
		Date = DateMax + 1 - DayFirst;		
		for(Col=0;Col<DayFirst;Col++)
		{
			sprintf ( cStr, "%d", Date );
			ColOffset = 9;
			ILI9341_DispString_EN(Col*CalWidth+ColOffset,Cal_YOffset+Row*CalHeight+RowOffset-40,cStr,macWHITE,macGREY);
			
			Date++;
		}	
	}
  CalHeight = 40;
	Row = 0;
	ILI9341_DrawLine(5,(Row+1)*24+Cal_YOffset-28,235,(Row+1)*24+Cal_YOffset-28,macBLUE);
	// Draw Line

	if(DayFirst>4) count = 7;
	else count = 6;
	
	for(Row=1;Row<count;Row++)
  {	
	  ILI9341_DrawLine(5,(Row+1)*CalHeight+Cal_YOffset-44,235,(Row+1)*CalHeight+Cal_YOffset-44,macBLUE);
  }

	while(1)
	{
		u16 TouchKey;
		//按键处理
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey == 111)
			{
				//按键功能...
				while( tp_dev.sta );
				UICnt = 0x11;		//修改界面指针
				break;					//退出	
			}
			else if(TouchKey == 1111)
			{
				while( tp_dev.sta );
				UICnt = 0x12;		//修改界面指针
				break;					//退出	
			}
				
		}	
		
		sprintf((char*)tbuf,"%02d:%02d:%2d",RealTime.Hour,RealTime.Min,RealTime.Sec); 
		ILI9341_DispString_EN( 80,10, (char*)tbuf , macBLUE2,macBLACK);
		if (ucXPT2046_TouchFlag = 1)			                                                   //如果触摸了屏幕
    {
			ucXPT2046_TouchFlag = 0;
      if ( XPT2046_Get_TouchedPoint (  &strDisplayCoordinate, & strXPT2046_TouchPara ) )   //获取触摸点的坐标	
			{
				if(150<strDisplayCoordinate.x & strDisplayCoordinate.x<180 & 22<strDisplayCoordinate.y & strDisplayCoordinate.y<54)					
				{					
					  Calendar.Month--;
					  if(Calendar.Month == 0)
					  {
						  Calendar.Year--;
						  Calendar.Month=12;
					  } Drawing();//Set_RTCTime(&RealTime);
				}					
			  if(190<strDisplayCoordinate.x & strDisplayCoordinate.x<240 & 22<strDisplayCoordinate.y & strDisplayCoordinate.y<54)					
			  {
					  Calendar.Month++;
					  if(Calendar.Month == 13)
					  {
						  Calendar.Year++;
							
						  Calendar.Month=1;
					  } Drawing();//DrawCalendar(Year,Month,CurrentDate,SelectDate);		
		    }
				
	      if(Calendar.Month==2)
	      {
		      if(Is_LeapYear(Calendar.Year)==1)
			    {
		        DateMax++;
	        }
				}
				DayFirst = Get_Week(Calendar.Year,Calendar.Month,1);	
	
	      //计算月最大日期
	      DateMax = DateMaxTab[Calendar.Month];
			  for(Col = DayFirst;Col<7;Col++)
			  {
				  if(((1+Col*34)<strDisplayCoordinate.x) & (strDisplayCoordinate.x<(35+Col*34)) & (72<strDisplayCoordinate.y) & (strDisplayCoordinate.y<112))
				  {					  					  
						if(Col == DayFirst)
						{
								Cho=1; Drawing();	//DrawCalendar(Year,Month,CurrentDate,1);
						}
						else if(Col > DayFirst)
						{
								Cho = (Col - DayFirst+1); Drawing(); //DrawCalendar(Year,Month,CurrentDate,(Cc - DayFirst+1));
						}
							
					}
				}
				if(DayFirst>4)
	      {		
		       count = 6;
	      }
	      else 
	      {
		       count = 5;
	      }
				for(Row = 1; Row<(count-1); Row++)
				{
					 for(Col=0;Col<7;Col++)
					 {
						 if(((72+Row*40)<strDisplayCoordinate.y) & (strDisplayCoordinate.y<(112+Row*40)) & ((1+Col*34)<strDisplayCoordinate.x) & (strDisplayCoordinate.x<(35+Col*34)))
					   {							
						     Cho = ((Row-1)*7+(8-DayFirst)+Col);Drawing();//DrawCalendar(Year,Month,CurrentDate,((Row-1)*7+(8-DayFirst)+Cc));								
						 }
					}
				}
				Last = (DateMax - (7-DayFirst) - (count-2)*7);
				for(Col=0;Col<Last;Col++)
				{
					if(((72+(count-1)*40)<strDisplayCoordinate.y) & (strDisplayCoordinate.y<(72+count*40)) & ((1+Col*34)<strDisplayCoordinate.x) & (strDisplayCoordinate.x<(35+Col*34)))
				  {
							Cho = (DateMax-Last+1+Col);Drawing();//DrawCalendar(Year,Month,CurrentDate,(DateMax-Last+1+Cc));							
					}
				}				
		  }
	  }
	

	}
}
//月份选择
void UI12( void )
{
	int i,j;
	char	cStr[ 10 ];
	u16 TouchKey;
	char tbuf[40];
	
	ILI9341_Clear(0,0,240,320,macWHITE);
	ILI9341_Clear(0,0,240,96,macBLUE2);
	
	sprintf ( cStr, "%d", Calendar.Year );
 	ILI9341_DispString_EN_48(0,48,cStr,macBLUE2,macRED);
	ILI9341_DispString_EN_32(140,64,"<<  >>",macBLUE2,macRED);
	
	for (i=0; i<3; i++)
	{
		for (j=0; j<4; j++)
		{
			ILI9341_DrawRectangle( 6*(i+1)+i*72, 100+j*30+6*j, 72, 30, macBLUE2, 0 );
		}
	}
	ILI9341_DispString_EN_CH( 32, 107,"1月", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 32, 143,"4月", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 32, 179,"7月", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 26, 215,"10月", macWHITE,  macBLUE);
	
	ILI9341_DispString_EN_CH( 110, 107,"2月", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 110, 143,"5月", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 110, 179,"8月", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 104, 215,"11月", macWHITE,  macBLUE);
	
	ILI9341_DispString_EN_CH( 188, 107,"3月", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 188, 143,"6月", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 188, 179,"9月", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 182, 215,"12月", macWHITE,  macBLUE);
	
	
	
	
	while(1)
	{
		sprintf((char*)tbuf,"%02d:%02d:%2d",RealTime.Hour,RealTime.Min,RealTime.Sec); 
		ILI9341_DispString_EN_48( 0,0, (char*)tbuf , macBLUE2,macBLACK);
	}
}
//年份选择
void UI11( void )
{
	int i,j;
	char	cStr[ 10 ];
	u16 TouchKey;
	char tbuf[40];
	
	ILI9341_Clear(0,0,240,320,macWHITE);
	ILI9341_Clear(0,0,240,96,macBLUE2);
	
	sprintf ( cStr, "%d", Calendar.Year );
 	ILI9341_DispString_EN_48(0,48,cStr,macBLUE2,macRED);
	ILI9341_DispString_EN_32(140,64,"<<  >>",macBLUE2,macRED);
	
	for (i=0; i<3; i++)
	{
		for (j=0; j<4; j++)
		{
			ILI9341_DrawRectangle( 6*(i+1)+i*72, 100+j*30+6*j, 72, 30, macBLUE2, 0 );
		}
	}
	ILI9341_DispString_EN_CH( 26, 107,"2010", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 26, 143,"2013", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 26, 179,"2016", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 26, 215,"2019", macWHITE,  macBLUE);
	
	ILI9341_DispString_EN_CH( 104, 107,"2011", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 104, 143,"2014", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 104, 179,"2017", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 104, 215,"2020", macWHITE,  macBLUE);
	
	ILI9341_DispString_EN_CH( 182, 107,"2012", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 182, 143,"2015", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 182, 179,"2018", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 182, 215,"2021", macWHITE,  macBLUE);
	
	
	
	
	while(1)
	{
		sprintf((char*)tbuf,"%02d:%02d:%2d",RealTime.Hour,RealTime.Min,RealTime.Sec); 
		ILI9341_DispString_EN_48( 0,0, (char*)tbuf , macBLUE2,macBLACK);
	}
}
//屏保界面
void UI20( void )
{
	u16 TouchKey;
	char tbuf[40];
	
	ILI9341_Clear( 0, 0, 240, 320, UI0BackColor);
	ILI9341_DrawRectangle( 0, 120, 240, 120, macWHITE, 1 );
//	ILI9341_DispString_EN_48( 10, 130, "12:29", macWHITE, macBLACK );
//	ILI9341_DispString_EN_CH( 10, 180, "2016年12月10日,星期四", macWHITE, macBLACK );
	ILI9341_DispString_EN_CH( 10, 210, "电子A1322_05_陈建峰期末设计", macWHITE, macBLACK );
	
	sprintf((char*)tbuf,"%02d:%02d",RealTime.Hour,RealTime.Min); 
	ILI9341_DispString_EN_48( 10,130, (char*)tbuf , macWHITE,macBLACK);
			
	sprintf((char*)tbuf,"%02d-%02d-%02d",RealTime.Year,RealTime.Month,RealTime.Date);		
	ILI9341_DispString_EN_32( 10,175, (char*)tbuf , macWHITE,macBLACK);
	
	while(1)
	{

		//按键处理
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey==110)
			{
				//按键功能...
				while( tp_dev.sta );
				UICnt = 0x00;		//修改界面指针
				break;					//退出	
			}
				
		}	
	}
}



/**************  触屏扫描函数  **************/
u16 Get_TouchKey( void )
{
	u16	KeyTmp;
	
	//时钟界面
	if( UICnt==0x00 )				
	{
		if ( tp_dev.sta )			//如果触摸了屏幕
		{
			//时钟	121
			if( tp_dev.x>=23 && tp_dev.x<55 && tp_dev.y>=16&& tp_dev.y<48 )	KeyTmp = 121;
			//闹钟	122
			else if( tp_dev.x>=23+54 && tp_dev.x<55+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 122;
			//秒表
			else if( tp_dev.x>=77+54 && tp_dev.x<109+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 123;
			//计时
			else if( tp_dev.x>=131+54 && tp_dev.x<163+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 124;
			//数字时钟&日历界面
			else if( tp_dev.x>=75 && tp_dev.x<175 && tp_dev.y>=210 && tp_dev.y<250 )	KeyTmp = 125;
			//时钟设置界面
			else if( tp_dev.x>=90 && tp_dev.x<150 && tp_dev.y>=250 && tp_dev.y<300 )	KeyTmp = 126;
			
			else	KeyTmp = 65535;
		}
	}
	//闹钟界面
	else if( UICnt==0x01 )
	{
		if ( tp_dev.sta )			//如果触摸了屏幕
		{
			//时钟	121
			if( tp_dev.x>=23 && tp_dev.x<55 && tp_dev.y>=16&& tp_dev.y<48 )	KeyTmp = 121;
			//闹钟	122
			else if( tp_dev.x>=23+54 && tp_dev.x<55+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 122;
			//秒表
			else if( tp_dev.x>=77+54 && tp_dev.x<109+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 123;
			//计时
			else if( tp_dev.x>=131+54 && tp_dev.x<163+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 124;
			//开关按钮1
			else if( tp_dev.x>=185 && tp_dev.x<225 && tp_dev.y>=165 && tp_dev.y<185 )	KeyTmp = 138;
			//开关按钮2
			else if( tp_dev.x>=185 && tp_dev.x<225 && tp_dev.y>=220 && tp_dev.y<240 )	KeyTmp = 148;
			//开关按钮3
			else if( tp_dev.x>=185 && tp_dev.x<225 && tp_dev.y>=275 && tp_dev.y<295 )	KeyTmp = 158;
			//闹钟时间1
			else if( tp_dev.x>=10 && tp_dev.x<77 && tp_dev.y>=155 && tp_dev.y<183 )	KeyTmp = 137;
			//闹钟时间2
			else if( tp_dev.x>=10 && tp_dev.x<77 && tp_dev.y>=210 && tp_dev.y<237 )	KeyTmp = 147;
			//闹钟时间3
			else if( tp_dev.x>=10 && tp_dev.x<77 && tp_dev.y>=265 && tp_dev.y<292 )	KeyTmp = 157;
			
			else	KeyTmp = 65535;
		}
	}
	//闹钟设置页面
	else if( UICnt==0x06)
	{
		if ( tp_dev.sta )			//如果触摸了屏幕
		{
			//返回
			if( tp_dev.x>=10 && tp_dev.x<70 && tp_dev.y>=10&& tp_dev.y<30)	KeyTmp = 171;
		}
	}
	//秒表界面
	else if( UICnt==0x02 )
	{
		if ( tp_dev.sta )			//如果触摸了屏幕
		{
			//时钟	121
			if( tp_dev.x>=23 && tp_dev.x<55 && tp_dev.y>=16&& tp_dev.y<48 )	KeyTmp = 121;
			//闹钟	122
			else if( tp_dev.x>=23+54 && tp_dev.x<55+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 122;
			//秒表
			else if( tp_dev.x>=77+54 && tp_dev.x<109+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 123;
			//计时
			else if( tp_dev.x>=131+54 && tp_dev.x<163+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 124;
			
			else	KeyTmp = 65535;
		}		
	}
	//定时界面
	else if( UICnt==0x03 )
	{
		if ( tp_dev.sta )			//如果触摸了屏幕
		{
			//时钟	121
			if( tp_dev.x>=23 && tp_dev.x<55 && tp_dev.y>=16&& tp_dev.y<48 )	KeyTmp = 121;
			//闹钟	122
			else if( tp_dev.x>=23+54 && tp_dev.x<55+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 122;
			//秒表
			else if( tp_dev.x>=77+54 && tp_dev.x<109+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 123;
			//计时
			else if( tp_dev.x>=131+54 && tp_dev.x<163+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 124;

			else	KeyTmp = 65535;
		}		
	}
	//时钟设置界面
	else if( UICnt==0x04)
	{
		if ( tp_dev.sta )			//如果触摸了屏幕
		{
			//返回
			if( tp_dev.x>=10 && tp_dev.x<70 && tp_dev.y>=10&& tp_dev.y<30)	KeyTmp = 141;
		}
	}
	//闹钟工作界面
	else if( UICnt==0x05 )
	{
		if ( tp_dev.sta )			//如果触摸了屏幕
		{
			if( tp_dev.x>=10 && tp_dev.x<70 && tp_dev.y>=10&& tp_dev.y<30)	KeyTmp = 141;
		}
	}
	//屏保
	else if( UICnt==0x20 )
	{
		if ( tp_dev.sta )			//如果触摸了屏幕
		{
			if( tp_dev.x>=0 && tp_dev.x<240 && tp_dev.y>=0&& tp_dev.y<320 )	KeyTmp = 110;
			else	KeyTmp = 65535;
		}
	}
	//日历
	else if( UICnt==0x10 )
	{
		if ( tp_dev.sta )			//如果触摸了屏幕
		{
			if( tp_dev.x>=0 && tp_dev.x<64 && tp_dev.y>=22&& tp_dev.y<54 )	KeyTmp = 111;
			else if ( tp_dev.x>=80 && tp_dev.x<130 && tp_dev.y>=22&& tp_dev.y<54 )	KeyTmp = 1111;
			else	KeyTmp = 65535;
		}
	}
	//......
	
	return KeyTmp;
}


/*********************************************END OF FILE**********************/

