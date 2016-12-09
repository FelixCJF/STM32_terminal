#include "main.h"
/*******************************  È«¾Ö±äÁ¿¶¨Òå  *******************************/
u16	 		KeyNum;													//¼üÖµ
u32			KeyCnt,KeyUpCnt;								//°´¼üÊ±¼ä£¨µ¥Î»10ms£©,°´¼üÊÍ·ÅÊ±¼ä£¨µ¥Î»10ms£©
u8			UICnt,UIRep;										//½çÃæÖ¸Õë£¬½çÃæÖ¸Õë±¸·İ
TimeStruct 			RealTime;								//Ê±ÖÓ
CalendarStruct	Calendar,LunarCalendar;	//¹«Àú£¬Å©Àú
u8 Cho = 3;
/*****************************  È«¾Ö±äÁ¿¶¨Òå½áÊø  *****************************/
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


/*******************************  Êı¾İ±í¸ñ  *******************************/
////ĞÇÆÚÏÔÊ¾×Ö·û´®(Ó¢ÎÄ)
//const char DayTab[7][4] = {
//	"Sun\0","Mon\0","Tue\0","Wed\0","Thu\0","Fri\0","Sat\0",
//};

//ĞÇÆÚÏÔÊ¾×Ö·û´®(ÖĞÎÄ)
const char DayTab_Han[7][3] = {
	"ÈÕ\0","Ò»\0","¶ş\0","Èı\0","ËÄ\0","Îå\0","Áù\0",
};

////ÔÂ×î´óÈÕÆÚ±í
//const unsigned char DateMaxTab[13] = {
//	0,31,28,31,30,31,30,31,31,30,31,30,31,
//};

////Å©ÀúÈÕÆÚ±í
//const unsigned char LunarDateName[][5]=   {
//	"  \0","³õÒ»\0","³õ¶ş\0","³õÈı\0","³õËÄ\0","³õÎå\0","³õÁù\0","³õÆß\0","³õ°Ë\0","³õ¾Å\0","³õÊ®\0",
//	"Ê®Ò»\0","Ê®¶ş\0","Ê®Èı\0","Ê®ËÄ\0","Ê®Îå\0","Ê®Áù\0","Ê®Æß\0","Ê®°Ë\0","Ê®¾Å\0","¶şÊ®\0",
//	"Ø¥Ò»\0","Ø¥¶ş\0","Ø¥Èı\0","Ø¥ËÄ\0","Ø¥Îå\0","Ø¥Áù\0","Ø¥Æß\0","Ø¥°Ë\0","Ø¥¾Å\0","ÈıÊ®\0"
//};

//Å©ÀúÔÂ·İ±í
const unsigned char LunarMonthName[][5]=   {
	"  \0","ÕıÔÂ\0","¶şÔÂ\0","ÈıÔÂ\0","ËÄÔÂ\0","ÎåÔÂ\0","ÁùÔÂ\0","ÆßÔÂ\0","°ËÔÂ\0","¾ÅÔÂ\0","Ê®ÔÂ\0","¶¬ÔÂ\0","À°ÔÂ\0",
};

//ĞÇÆÚ±í¸ñ
const char DayTab[7][4] = {
	"Sun\0","Mon\0","Tue\0","Wed\0","Thu\0","Fri\0","Sat\0",
};

//ÔÂ·İ´óĞ¡
const unsigned char DateMaxTab[13] = {
	0,31,28,31,30,31,30,31,31,30,31,30,31,
};

//Å©Àú
const unsigned char DayDay[32][8] = {"ÎŞÓï",
"³õÒ»","³õ¶ş","³õÈı","³õËÄ","³õÎå","³õÁù","³õÆß",
"³õ°Ë","³õ¾Å","³õÊ®","Ê®Ò»","Ê®¶ş","Ê®Èı","Ê®ËÄ",
"Ê®Îå","Ê®Áù","Ê®Æß","Ê®°Ë","Ê®¾Å","¶şÊ®","Ø¥Ò»",
"Ø¥¶ş","Ø¥Èı","Ø¥ËÄ","Ø¥Îå","Ø¥Áù","Ø¥Æß",
"Ø¥°Ë","Ø¥¾Å","ÈıÊ®"};

/*******************************  ÏµÍ³ÅäÖÃº¯Êı  *******************************/
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
	
	//±ØĞëÏÈ¿ªÆôÊ±ÖÓÔÙÅäÖÃÒı½Å
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

	
	//ÉèÖÃTIM2ÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	//ÉèÖÃTIM6ÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	//ÉèÖÃTIM7ÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	//ÉèÖÃRTCÖĞ¶Ï
	//1¼¶ÇÀÕ¼Ê½ÓÅÏÈ¼¶,0¼¶¸±ÓÅÏÈ¼¶,Ê¹ÄÜÒı½Å×÷ÎªÖĞ¶ÏÔ´
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}



/****************************************************************************
* Ãû    ³Æ£ºvoid TIM1_Configuration(void)
* ¹¦    ÄÜ£ºTIM1³õÊ¼»¯º¯Êı
* Èë¿Ú²ÎÊı£ºÎŞ
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎŞ 
****************************************************************************/ 
void TIM1_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// Êä³ö±È½ÏÍ¨µÀ GPIO ³õÊ¼»¯
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1 , ENABLE );
		
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// Ê±ÖÓ·ÖÆµÒò×Ó £¬ÅäÖÃËÀÇøÊ±¼äÊ±ĞèÒªÓÃµ½
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		// ¼ÆÊıÆ÷¼ÆÊıÄ£Ê½£¬ÉèÖÃÎªÏòÉÏ¼ÆÊı
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	
	TIM_TimeBaseInit( TIM1 , &TIM_TimeBaseStructure);

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;// ÅäÖÃÎªPWMÄ£Ê½1	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;// Êä³öÊ¹ÄÜ	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 500;// ÉèÖÃÕ¼¿Õ±È´óĞ¡	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;// ÓĞĞ§µçÆ½Îª¸ß
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;	//¿ÕÏĞÊä³öµÍµçÆ½
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init( TIM1 , &TIM_OCInitStructure );
	TIM_OC1PreloadConfig( TIM1 , TIM_OCPreload_Enable );//Ê¹ÄÜTIM1 ÔÚ CCR1 ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
	
	// Ê¹ÄÜ¼ÆÊıÆ÷
//	TIM_Cmd( TIM1 , ENABLE );	
	// Ö÷Êä³öÊ¹ÄÜ£¬µ±Ê¹ÓÃµÄÊÇÍ¨ÓÃ¶¨Ê±Æ÷Ê±£¬Õâ¾ä²»ĞèÒª
//	TIM_CtrlPWMOutputs( TIM1 , ENABLE );
}

/****************************************************************************
* Ãû    ³Æ£ºvoid TIM2_Configuration(void)
* ¹¦    ÄÜ£ºTIM2³õÊ¼»¯º¯Êı
* Èë¿Ú²ÎÊı£ºÎŞ
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£ºÉèÖÃ¶¨Ê±Æ÷T2Ã¿10msÖĞ¶ÏÒ»´Î
* µ÷ÓÃ·½·¨£ºÎŞ 
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
* Ãû    ³Æ£ºvoid TIM6_Configuration(void)
* ¹¦    ÄÜ£ºTIM6³õÊ¼»¯º¯Êı
* Èë¿Ú²ÎÊı£ºÎŞ
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£ºÉèÖÃ¶¨Ê±Æ÷T6Ã¿10msÖĞ¶ÏÒ»´Î
* µ÷ÓÃ·½·¨£ºÎŞ 
****************************************************************************/ 
void TIM6_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_DeInit(TIM6);
	
	TIM_TimeBaseStructure.TIM_Period = 99; 							//¶¨Ê±10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);   
	TIM_Cmd(TIM6, ENABLE);
}

/****************************************************************************
* Ãû    ³Æ£ºvoid TIM7_Configuration(void)
* ¹¦    ÄÜ£ºTIM7³õÊ¼»¯º¯Êı
* Èë¿Ú²ÎÊı£ºÎŞ
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£ºÉèÖÃ¶¨Ê±Æ÷T7Ã¿80msÖĞ¶ÏÒ»´Î,ÓÃÓÚÄÖÖÓ¶¨Ê±
* µ÷ÓÃ·½·¨£ºÎŞ 
****************************************************************************/ 
void TIM7_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	TIM_DeInit(TIM7);
	
	TIM_TimeBaseStructure.TIM_Period = 799; 							//¶¨Ê±80ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);	

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);   
//	TIM_Cmd(TIM7, ENABLE);
}


 /**
  * @brief  LCD´¥ÃşÆÁĞ£×¼»ò´ÓÍâ²¿flash¶ÁÈ¡Ğ£×¼ÏµÊı
  * @param  ÎŞ
  * @retval ÎŞ
  */	
void Flash_TouchCalibrate ( void )
{
	uint8_t ucStoreFlag;                            //LCD´¥ÃşÆÁĞ£×¼ÏµÊı´æ´¢±êÖ¾
	
	SPI_FLASH_ReadDeviceID();                       // Get SPI Flash Device ID */
	
	SPI_FLASH_BufferRead ( & ucStoreFlag, 0, 1 );   //´ÓÍâ²¿flash¶Á³öLCD´¥ÃşÆÁĞ£×¼ÏµÊı´æ´¢±êÖ¾
	
	if ( ucStoreFlag == 0x52 )                                                                         //ÒÑ´æ´¢¹ıLCD´¥ÃşÆÁĞ£×¼ÏµÊı
		SPI_FLASH_BufferRead ( ( void * )  & strXPT2046_TouchPara, 1, sizeof ( strXPT2046_TouchPara ) ); //¼ÌĞø¶ÁÈ¡LCD´¥ÃşÆÁĞ£×¼ÏµÊı
		
	else                                          //ÉĞÎ´´æ´¢¹ıLCD´¥ÃşÆÁĞ£×¼ÏµÊı
	{
		while( ! XPT2046_Touch_Calibrate () );      //µÈ´ı´¥ÃşÆÁĞ£ÕıÍê±Ï

		/* ´æ´¢LCD´¥ÃşÆÁĞ£×¼ÏµÊıºÍ±êÖ¾ */
		ucStoreFlag = 0x52;
		SPI_FLASH_SectorErase ( 0 );
		SPI_FLASH_BufferWrite ( & ucStoreFlag, 0, 1 );     
		SPI_FLASH_BufferWrite ( ( void * )  & strXPT2046_TouchPara, 1, sizeof ( strXPT2046_TouchPara ) );	
	}
	
//	SPI_Flash_PowerDown();                          //½øÈëµôµçÄ£Ê½
	
}

/****************************************************************************
* Ãû    ³Æ£ºvoid RTC_Configuration(void)
* ¹¦    ÄÜ£ºRTC³õÊ¼»¯º¯Êı
* Èë¿Ú²ÎÊı£ºÎŞ
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£º³õÊ¼»¯RTC
* µ÷ÓÃ·½·¨£ºÎŞ 
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


/**************  ¹¦ÄÜº¯Êı  **************/

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
* Ãû    ³Æ£ºvoid Alarm( u8 cmd )
* ¹¦    ÄÜ£º¿ØÖÆÄÖÖÓ
* Èë¿Ú²ÎÊı£ºcmd	1:ÏìÁå	0:Í£Ö¹ÏìÁå
* ³ö¿Ú²ÎÊı£º
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎŞ 
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

/**************  ÈÕÀú¹¦ÄÜº¯Êı  **************/


/****************************************************************************
* Ãû    ³Æ£ºu8 Is_Leap_Year(u16 year)
* ¹¦    ÄÜ£ºÅĞ¶ÏÊÇ·ñÊÇÈòÄê
* Èë¿Ú²ÎÊı£ºyear
* ³ö¿Ú²ÎÊı£º1:ÊÇÈòÄê£»0:·ÇÈòÄê
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎŞ 
****************************************************************************/ 
u8 Is_LeapYear(u16 year)
{
	if(year%4==0) //±ØĞëÄÜ±»4Õû³ı
	{ 
		if(year%100==0) 
		{
			if(year%400==0)return 1;//Èç¹ûÒÔ00½áÎ²,»¹ÒªÄÜ±»400Õû³ı
			else return 0; 
		}
		else return 1; 
	}
	else return 0;
} 

/****************************************************************************
* Ãû    ³Æ£ºu8 Get_Week(u16 Year,u16 Month,u16 Date)
* ¹¦    ÄÜ£ºÓÃ²ÌÀÕ£¨Zeller£©¹«Ê½¼ÆËãĞÇÆÚ¼¸	

	w=y+[y/4]+[c/4]-2c+[26£¨m+1£©/10]+d-1

	È»ºów¶Ô7È¡Óà¡£
	¹«Ê½ÖĞµÄ·ûºÅº¬ÒåÈçÏÂ£¬w£ºĞÇÆÚ£»c£ºÊÀ¼Í£»y£ºÄê£¨Á½Î»Êı£©£»m£ºÔÂ£¨m´óÓÚµÈÓÚ3£¬Ğ¡
	ÓÚµÈÓÚ14£¬¼´ÔÚ²ÌÀÕ¹«Ê½ÖĞ£¬Ä³ÄêµÄ1¡¢2ÔÂÒª¿´×÷ÉÏÒ»ÄêµÄ13¡¢14ÔÂÀ´¼ÆËã£¬±ÈÈç2003Äê1ÔÂ1
	ÈÕÒª¿´×÷2002ÄêµÄ13ÔÂ1ÈÕÀ´¼ÆËã£©£»d£ºÈÕ£»[ ]´ú±íÈ¡Õû£¬¼´Ö»ÒªÕûÊı²¿·Ö¡£

* Èë¿Ú²ÎÊı£ºYear:Äê; Month:ÔÂ; Date:ÈÕ¡£
* ³ö¿Ú²ÎÊı£ºĞÇÆÚ¼¸ 1~6´ú±íĞÇÆÚÒ»~ĞÇÆÚÁù£¬0´ú±íĞÇÆÚÌì
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎŞ
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
//»æÖÆÈÕÀú±í¸ñ
//Year:Äê
//Month£ºÔÂ
//CurrentDate:µ±Ç°ÈÕÆÚ
//SelectDate:Ñ¡¶¨ÈÕÆ
/****************************************************************************************/
void Drawing(void)
{ 
	char tbuf[40];
	char	cStr[ 10 ];						//ÓÃÀ´´æ·ÅÊı×Ö×ª»»³É×Ö·û´®µÄ×Ö·û
	unsigned int Row,Col;				//ÈÕÀúĞĞÁĞºÅ
	unsigned int CalWidth,CalHeight;	//ÈÕÀúÃ¿¸ñ¿í£¬¸ß
	unsigned int ColOffset,RowOffset;	//ÈÕÀú¸ñÖĞÏÔÊ¾×Ö·ûÊ±ºáÏò¡¢×İÏòÆ«ÒÆÖµ
	unsigned int Cal_YOffset;		//ÈÕÀú±í¸ñ×İÏòÆ«ÒÆÖµ
	unsigned int Date;					//ÈÕÆÚ
	unsigned int DateMax;				//µ±ÔÂ×î´óÈÕÆÚ
//	unsigned int Day;						//ĞÇÆÚ
	uint8_t DayFirst;			//µ±ÔÂ1ºÅĞÇÆÚ£¬1~6´ú±íĞÇÆÚÒ»~ĞÇÆÚÁù£¬0´ú±íĞÇÆÚÌì
	uint8_t count;
	//ÉèÖÃ»æÍ¼²ÎÊı
	CalWidth = 34;
	CalHeight = 42;
	RowOffset = 6;
	
	Cal_YOffset = 80;
	
	//¼ÆËãÔÂÊ×ÈÕĞÇÆÚ
	DayFirst = Get_Week(Calendar.Year,Calendar.Month,1);	
	
	//¼ÆËãÔÂ×î´óÈÕÆÚ
	DateMax = DateMaxTab[Calendar.Month];
	if(Calendar.Month==2)
	{
		if(Is_LeapYear(Calendar.Year)==1)		
		DateMax++;
	}
	
	//ÇåÆÁ
	ILI9341_Clear(0,0,240,320,macWHITE);
	
	//ÏÔÊ¾ÄêÔÂ	
	ILI9341_Clear(0,0,240,54,macBLUE2);	
	ILI9341_Clear(0,54,240,18,macYELLOW);	
	sprintf ( cStr, "%d", Calendar.Year );
 	ILI9341_DispString_EN_32(0,Cal_YOffset-58,cStr,macBLUE2,macRED);
	ILI9341_DispString_EN_32(64,Cal_YOffset-58,"-",macBLUE2,macRED);
	sprintf ( cStr, "%d", Calendar.Month );
	ILI9341_DispString_EN_32(80,Cal_YOffset-58,cStr,macBLUE2,macRED);
	ILI9341_DispString_EN_CH(150,Cal_YOffset-45,"<< ½ñÌì >>",macBLUE2,macRED);
//		sprintf((char*)tbuf,"%02d:%02d",RealTime.Hour,RealTime.Min); 
//	ILI9341_DispString_EN( 0,10, (char*)tbuf , macBLUE2,macBLACK);
	
	// µÚ0ĞĞ£¬ÏÔÊ¾ĞÇÆÚ
	Row = 0;
	ColOffset = 5;//ÁĞ
	
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
	
	// µÚ1-6ĞĞ£¬ÏÔÊ¾ÈÕÀú±í	
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
	
	//ÏÔÊ¾×îºóÒ»ÖÜÖĞÏÂÔÂÈÕÆÚ£¬ÓÃ»ÒÉ«ÏÔÊ¾
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
	//ÏÔÊ¾µÚÒ»ÖÜÖĞÉÏÔÂÈÕÆÚ£¬ÓÃ»ÒÉ«ÏÔÊ¾
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
* Ãû    ³Æ£ºvoid Cal_Time(u32 Count,TimeStruct *Time)
* ¹¦    ÄÜ£º¸ù¾İ32Î»Count¼ÆÊıÖµ¼ÆËãÈÕÆÚºÍÊ±¼ä
* Èë¿Ú²ÎÊı£ºCount
* ³ö¿Ú²ÎÊı£ºT_STRUCT* Time
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºCal_Time(Count,&RealTime);
****************************************************************************/ 
void Cal_Time(u32 Count,TimeStruct *Time)
{
	u32 SecTemp=0,DateTemp=0,Temp=0;
	
	DateTemp = Count/86400;					//ÌìÊı
	if(DateTemp!=0)									//³¬¹ı1Ìì
	{
		//È·¶¨Äê
		Temp = 1970;
		while(DateTemp>=365)
		{
			if(Is_LeapYear(Temp))	//ÈòÄê?
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
		
		//È·¶¨ÔÂ
		Temp = 1;
		while(DateTemp>=28)
		{
			if(Is_LeapYear(Time->Year)&&Temp==2)
			{
				if(DateTemp>=29)DateTemp -= 29;//ÈòÄêµÄ¶şÔÂÌìÊı
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
* Ãû    ³Æ£ºu8 Set_RTCTime(TimeStruct *Time)
* ¹¦    ÄÜ£ºÉèÖÃÊ±ÖÓ:ÒÔ1970-01-01 0:0:0Îª»ù×¼,°ÑÊäÈëµÄÊ±ÖÓ×ª»»ÎªÃëÖÓ,
* 					²¢Ğ´ÈëRTC¼ÆÊıÆ÷£¬1970~2099ÄêÎªºÏ·¨Äê·İ¡£
* Èë¿Ú²ÎÊı£ºT_STRUCT* Time
* ³ö¿Ú²ÎÊı£º0:³É¹¦;
						1:´íÎó´úÂë£¬·ÇÓĞĞ§Ê±¼ä(1970~2099)
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºSet_RTCTime(&RealTime);
****************************************************************************/ 
u8 Set_RTCTime( TimeStruct *Time)
{
	u32 Count = 0;
	u16 i;
	if(Time->Year<1970||Time->Year>2099) return 1;
	for(i=1970;i<Time->Year;i++)
	{
		if(Is_LeapYear(i)) Count += 31622400;		//ÈòÄêµÄÃëÊı
		else Count += 31536000;									//Æ½ÄêµÄÃëÊı
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
	
	//Ê¹ÄÜPWRºÍBKPÍâÉèÊ±ÖÓ,Õâ¾äÒ»¶¨²»ÄÜÊ¡	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	 
	PWR_BackupAccessCmd(ENABLE);	//Ê¹ÄÜRTCºÍºó±¸¼Ä´æÆ÷·ÃÎÊ 
	
	RTC_SetCounter(Count);
	RTC_WaitForLastTask();

	return 0;   
}




/****************************************************************************
* Ãû    ³Æ£ºvoid Read_RTCTime(TimeStruct *Time)
* ¹¦    ÄÜ£º´ÓRTC¼ÆÊıÆ÷¶ÁÈ¡Ê±¼ä		
* Èë¿Ú²ÎÊı£ºT_STRUCT* Time
* ³ö¿Ú²ÎÊı£ºÎŞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºRead_RTCTime(&RealTime);
****************************************************************************/ 
void Read_RTCTime( TimeStruct *Time)
{
	u32 Count = 0;
	Count = RTC_GetCounter();
	Cal_Time(Count,Time);
}






/**************  ½çÃæÉè¼Æº¯Êı  **************/
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
	
	
	// ²½Öè1:µ÷ÓÃLCDº¯Êı»æÖÆÍ¼ĞÎ½çÃæ
	
	
// 	int sx=(int)Radius*cos(s_angle)+Ox;
// 	int sy=(int)Radius*sin(s_angle)+Oy;

	ILI9341_Clear( 0, 0, 240, 320, UI0BackColor);
	ILI9341_DispString_CH( 23, 20, "Ê±ÖÓ", macBLACK, macWHITE );
	ILI9341_DispString_CH( 77, 20, "ÄÖÖÓ", macBLACK, macWHITE );
	ILI9341_DispString_CH( 131, 20, "Ãë±í", macBLACK, macWHITE );
	ILI9341_DispString_CH( 185, 20, "¼ÆÊ±", macBLACK, macWHITE );
 	ILI9341_DrawCircle( Ox, Oy, 70, macWHITE, 1);
//	ILI9341_DrawLine( 120, 130,(int)60*cos(s_angle1)+120,(int)60*sin(s_angle1)+130, macGREEN);
// 	ILI9341_DrawLine( 120, 130,(int)50*cos(s_angle2)+120,(int)50*sin(s_angle2)+130, macRED);
//	ILI9341_DrawLine( 120, 130, (int)40*cos(s_angle3)+120,(int)40*sin(s_angle3)+130, macBLUE);
//  ILI9341_DispString_EN_CH( 60, 220, "12ÔÂ10ÈÕĞÇÆÚÈı", macBLACK, macWHITE );
	ILI9341_DrawCircle( 120, 280, 30, macWHITE, 1);
	ILI9341_DispString_CH( 107, 273, "ÉèÖÃ",macWHITE, macBLUE);
	// ²½Öè2:½çÃæÑ­»·

	while(1)
	{
	//¸üĞÂÊ±¼ä
	/*ÃëÕë¼ÆËã*/	
	s_angle=(2*PI/60)*RealTime.Sec-PI/2;
	sx=(int)60*cos(s_angle)+Ox;
  sy=(int)60*sin(s_angle)+Oy;
	/*ÃëÕëÏÔÊ¾*/
  ILI9341_DrawLine(Ox,Oy,sx,sy,macRED);
		
	/*·ÖÕë¼ÆËã*/
	s_angle=(2*PI/60)*RealTime.Min-PI/2;
	sx1=(int)50*cos(s_angle)+Ox;
  sy1=(int)50*sin(s_angle)+Oy;
	/*·ÖÕëÏÔÊ¾*/
	ILI9341_DrawLine(Ox,Oy,sx1,sy1,macBLUE);
			
	/*Ê±Õë¼ÆËã*/
	s_angle=(2*PI/12)*RealTime.Hour-PI/2;
	sx2=(int)40*cos(s_angle)+Ox;
  sy2=(int)40*sin(s_angle)+Oy;
	/*Ê±ÕëÏÔÊ¾*/
	ILI9341_DrawLine(Ox,Oy,sx2,sy2,macGREEN);
	Delay(1000000);
	
	sprintf((char*)tbuf,"%02d:%02d:%02d",RealTime.Hour,RealTime.Min,RealTime.Sec); 
	ILI9341_DispString_EN( 89,210, (char*)tbuf , macBLACK,macWHITE);
			
	sprintf((char*)tbuf,"%02d-%02d-%02d",RealTime.Year,RealTime.Month,RealTime.Date);		
	ILI9341_DispString_EN( 79,230, (char*)tbuf , macBLACK,macWHITE);

		//ÅĞ¶ÏÊÇ·ñÄÖÖÓÊ±¼ä
		//......
		
		//°´¼ü´¦Àí
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey==121 )
			{
				while( tp_dev.sta );
				UICnt = 0x20;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;	
			}
			else if( TouchKey==122 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x01;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==123 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x02;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==124 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x03;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==125 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x010;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==126 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x04;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
		}
		/*ÃëÕë²Á³ı*/
	ILI9341_DrawLine(Ox,Oy,sx,sy,macWHITE);
		/*·ÖÕë²Á³ı*/
	ILI9341_DrawLine(Ox,Oy,sx1,sy1,macWHITE);
		/*Ê±Õë²Á³ı*/
	ILI9341_DrawLine(Ox,Oy,sx2,sy2,macWHITE);
	}
}
//ÄÖÖÓ½çÃæ
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
	ILI9341_DispString_CH( 23, 20, "Ê±ÖÓ", macBLACK, macWHITE );
	ILI9341_DispString_CH( 77, 20, "ÄÖÖÓ", macBLACK, macWHITE );
	ILI9341_DispString_CH( 131, 20, "Ãë±í", macBLACK, macWHITE );
	ILI9341_DispString_CH( 185, 20, "¼ÆÊ±", macBLACK, macWHITE );
	
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
		//¸üĞÂÊ±¼ä
		/*ÃëÕë¼ÆËã*/	
		s_angle=(2*PI/60)*RealTime.Sec-PI/2;
		sx=(int)45*cos(s_angle)+Ox;
		sy=(int)45*sin(s_angle)+Oy;
		/*ÃëÕëÏÔÊ¾*/
		ILI9341_DrawLine(Ox,Oy,sx,sy,macRED);
		
		/*·ÖÕë¼ÆËã*/
		s_angle=(2*PI/60)*RealTime.Min-PI/2;
		sx1=(int)35*cos(s_angle)+Ox;
		sy1=(int)35*sin(s_angle)+Oy;
		/*·ÖÕëÏÔÊ¾*/
		ILI9341_DrawLine(Ox,Oy,sx1,sy1,macBLUE);
			
		/*Ê±Õë¼ÆËã*/
		s_angle=(2*PI/12)*RealTime.Hour-PI/2;
		sx2=(int)25*cos(s_angle)+Ox;
		sy2=(int)25*sin(s_angle)+Oy;
		/*Ê±ÕëÏÔÊ¾*/
		ILI9341_DrawLine(Ox,Oy,sx2,sy2,macGREEN);
		Delay(1000000);
		
		//°´¼ü´¦Àí
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey==121 )
			{
				//°´¼ü¹¦ÄÜ...
				while( tp_dev.sta );
				UICnt = 0x00;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==122 )
			{
				while( tp_dev.sta );
				UICnt = 0x20;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==123 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x02;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==124 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x03;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
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
				UICnt = 0x06;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö		
			}
			else if( TouchKey==147 )
			{
				while( tp_dev.sta );
				UICnt = 0x06;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==157 )
			{
				while( tp_dev.sta );
				UICnt = 0x06;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
		}
		
		/*ÃëÕë²Á³ı*/
	ILI9341_DrawLine(Ox,Oy,sx,sy,macWHITE);
		/*·ÖÕë²Á³ı*/
	ILI9341_DrawLine(Ox,Oy,sx1,sy1,macWHITE);
		/*Ê±Õë²Á³ı*/
	ILI9341_DrawLine(Ox,Oy,sx2,sy2,macWHITE);
	}
}

//Ãë±í½çÃæ
int flag_TIM2=0;
void UI02( void )
{
	unsigned int min=0,sec=0,msec=0;//¼ÓµÄ
	char tbuf[40];
	u16 TouchKey;
	float s_angle;
	int sx,sy;
	int sx1,sy1;
	int sx2,sy2;
	int Ox = 120;
	int Oy = 100;
	
	ILI9341_Clear( 0, 0, 240, 320, UI0BackColor);
	ILI9341_DispString_CH( 23, 20, "Ê±ÖÓ", macBLACK, macWHITE );
	ILI9341_DispString_CH( 77, 20, "ÄÖÖÓ", macBLACK, macWHITE );
	ILI9341_DispString_CH( 131, 20, "Ãë±í", macBLACK, macWHITE );
	ILI9341_DispString_CH( 185, 20, "¼ÆÊ±", macBLACK, macWHITE );
	
	ILI9341_DrawCircle( Ox, Oy, 50, macWHITE, 1);
	
//	ILI9341_DispString_CH( 75, 180, "·ÖÖÓ", macBLACK, macWHITE );
//	ILI9341_DispString_EN_48( 60, 200, "00", macBLACK, macWHITE);
//	ILI9341_DispString_CH( 140, 180, "Ãë", macBLACK, macWHITE );
//	ILI9341_DispString_EN_48( 112, 200, ":", macBLACK, macWHITE);
//	ILI9341_DispString_EN_48( 130, 200, "00", macBLACK, macWHITE);
		
	
	ILI9341_DrawRectangle( 6, 285, 72, 30, macWHITE, 1 );//¿ªÊ¼
	ILI9341_DispString_CH( 26, 292,"¿ªÊ¼",  macWHITE, macBLUE);
	ILI9341_DrawRectangle( 84, 285, 72, 30, macWHITE, 1 );//ÔİÍ£
	ILI9341_DispString_CH( 104, 292,"ÔİÍ£", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 162, 285, 72, 30, macWHITE, 1 );//ÖØÖÃ
	ILI9341_DispString_CH( 182, 292,"ÖØÖÃ",  macWHITE, macBLUE);
	
	while(1)
	{
		//¸üĞÂÊ±¼ä
		/*ÃëÕë¼ÆËã*/	
		s_angle=(2*PI/60)*RealTime.Sec-PI/2;
		sx=(int)45*cos(s_angle)+Ox;
		sy=(int)45*sin(s_angle)+Oy;
		/*ÃëÕëÏÔÊ¾*/
		ILI9341_DrawLine(Ox,Oy,sx,sy,macRED);
		
		/*·ÖÕë¼ÆËã*/
		s_angle=(2*PI/60)*RealTime.Min-PI/2;
		sx1=(int)35*cos(s_angle)+Ox;
		sy1=(int)35*sin(s_angle)+Oy;
		/*·ÖÕëÏÔÊ¾*/
		ILI9341_DrawLine(Ox,Oy,sx1,sy1,macBLUE);
		
		
		/*Ê±Õë¼ÆËã*/
		s_angle=(2*PI/12)*RealTime.Hour-PI/2;
		sx2=(int)25*cos(s_angle)+Ox;
		sy2=(int)25*sin(s_angle)+Oy;
		/*Ê±ÕëÏÔÊ¾*/
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
						
						TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); //¹ØÖĞ¶Ï  
						TIM_Cmd(TIM2, DISABLE);	//¹Ø¶¨Ê±Æ÷
					}
				}
			}
		}
		
		sprintf(tbuf,"%02d:%02d.%02d",min,sec,msec);
		ILI9341_DispString_EN_48(20,180,tbuf,macBLACK,macWHITE);
		
		//°´¼ü´¦Àí
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if(tp_dev.x>=6 && tp_dev.x<78 && tp_dev.y>=285&& tp_dev.y<315 ) //¿ªÊ¼
			{
					TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //¿ªÖĞ¶Ï  
					TIM_Cmd(TIM2, ENABLE);	//¿ª¶¨Ê±Æ÷
					sprintf(tbuf,"%02d:%02d.%02d",min,sec,msec);
					ILI9341_DispString_EN_48(20,180,tbuf,macBLACK,macWHITE);
			}
			if(tp_dev.x>=84 && tp_dev.x<156 && tp_dev.y>=285&& tp_dev.y<315 ) //ÔİÍ£
			{
					TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); //¹ØÖĞ¶Ï  
					TIM_Cmd(TIM2, DISABLE);	//¹Ø¶¨Ê±Æ÷
					sprintf(tbuf,"%02d:%02d.%02d",min,sec,msec);
				  ILI9341_DispString_EN_48(20,180,tbuf,macBLACK,macWHITE);		
			}
			if(tp_dev.x>=162 && tp_dev.x<234 && tp_dev.y>=285&& tp_dev.y<315 ) //ÖØÖÃ
			{
				min=0;
				sec=0;
				msec=0;
				
				TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); //¹ØÖĞ¶Ï  
				TIM_Cmd(TIM2, DISABLE);	//¹Ø¶¨Ê±Æ÷
				
				sprintf(tbuf,"%02d:%02d.%02d",min,sec,msec);
				ILI9341_DispString_EN_48(20,180,tbuf,macBLACK,macWHITE);			
			}
			
		}
		
		//°´¼ü´¦Àí
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
		
			if( TouchKey==121 )
			{
				//°´¼ü¹¦ÄÜ...
				while( tp_dev.sta );
				UICnt = 0x00;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==122 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x01;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==123 )
			{
				while( tp_dev.sta );
				UICnt = 0x20;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==124 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x03;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
		}
		
	/*ÃëÕë²Á³ı*/
	ILI9341_DrawLine(Ox,Oy,sx,sy,macWHITE);
	/*·ÖÕë²Á³ı*/
	ILI9341_DrawLine(Ox,Oy,sx1,sy1,macWHITE);
	/*Ê±Õë²Á³ı*/
	ILI9341_DrawLine(Ox,Oy,sx2,sy2,macWHITE);
	}
}
//¼ÆÊ±½çÃæ
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
	ILI9341_DispString_CH( 23, 20, "Ê±ÖÓ", macBLACK, macWHITE );
	ILI9341_DispString_CH( 77, 20, "ÄÖÖÓ", macBLACK, macWHITE );
	ILI9341_DispString_CH( 131, 20, "Ãë±í", macBLACK, macWHITE );
	ILI9341_DispString_CH( 185, 20, "¼ÆÊ±", macBLACK, macWHITE );
	
	ILI9341_DrawCircle( 120, 100, 50, macWHITE, 1);


	ILI9341_DispString_CH( 75, 180, "·ÖÖÓ", macBLACK, macWHITE );
	sprintf(tbuf,"%02d",min);
	ILI9341_DispString_EN_48( 60, 200, tbuf, macBLACK, macWHITE);
	ILI9341_DispString_CH( 140, 180, "Ãë", macBLACK, macWHITE );
	ILI9341_DispString_EN_48( 112, 200, ":", macBLACK, macWHITE);
	sprintf(tbuf,"%02d",sec);
	ILI9341_DispString_EN_48( 130, 200, tbuf, macBLACK, macWHITE);
	
	ILI9341_DrawRectangle( 6, 285, 72, 30, macWHITE, 1 );//¿ªÊ¼
	ILI9341_DispString_CH( 26, 292,"¿ªÊ¼",  macWHITE, macBLUE);
	ILI9341_DrawRectangle( 84, 285, 72, 30, macWHITE, 1 );//ÔİÍ£
	ILI9341_DispString_CH( 104, 292,"ÔİÍ£", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 162, 285, 72, 30, macWHITE, 1 );//ÖØÖÃ
	ILI9341_DispString_CH( 182, 292,"ÖØÖÃ",  macWHITE, macBLUE);
	
	while(1)
	{
		//¸üĞÂÊ±¼ä
		/*ÃëÕë¼ÆËã*/	
		s_angle=(2*PI/60)*RealTime.Sec-PI/2;
		sx=(int)45*cos(s_angle)+Ox;
		sy=(int)45*sin(s_angle)+Oy;
		/*ÃëÕëÏÔÊ¾*/
		ILI9341_DrawLine(Ox,Oy,sx,sy,macRED);
		
		/*·ÖÕë¼ÆËã*/
		s_angle=(2*PI/60)*RealTime.Min-PI/2;
		sx1=(int)35*cos(s_angle)+Ox;
		sy1=(int)35*sin(s_angle)+Oy;
		/*·ÖÕëÏÔÊ¾*/
		ILI9341_DrawLine(Ox,Oy,sx1,sy1,macBLUE);
		
		
		/*Ê±Õë¼ÆËã*/
		s_angle=(2*PI/12)*RealTime.Hour-PI/2;
		sx2=(int)25*cos(s_angle)+Ox;
		sy2=(int)25*sin(s_angle)+Oy;
		/*Ê±ÕëÏÔÊ¾*/
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
		
		//°´¼ü´¦Àí
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey==121 )
			{
				//°´¼ü¹¦ÄÜ...
				while( tp_dev.sta );
				UICnt = 0x00;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==122 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x01;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==123 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x02;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==124 )
			{
				while( tp_dev.sta );
				UICnt = 0x20;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
		}
	/*ÃëÕë²Á³ı*/
	ILI9341_DrawLine(Ox,Oy,sx,sy,macWHITE);
	/*·ÖÕë²Á³ı*/
	ILI9341_DrawLine(Ox,Oy,sx1,sy1,macWHITE);
	/*Ê±Õë²Á³ı*/
	ILI9341_DrawLine(Ox,Oy,sx2,sy2,macWHITE);
	}
}
//Ê±ÖÓÉèÖÃ½çÃæ
void UI04( void )
{
	u16 TouchKey;
	
	ILI9341_Clear( 0, 0, 240, 320, UI0BackColor);
	ILI9341_DispString_EN_CH( 10, 10, "<< ·µ»Ø", macBLACK, macWHITE );
	ILI9341_DispString_CH( 185, 10,"±£´æ", macBLACK, macWHITE );
	
	ILI9341_DispString_EN( 50, 40,"2016", macBLACK, macWHITE );
	ILI9341_DispString_CH( 90, 40,"Äê", macBLACK, macWHITE );
	ILI9341_DispString_EN( 110, 40,"12", macBLACK, macWHITE );
	ILI9341_DispString_CH( 130, 40,"ÔÂ", macBLACK, macWHITE );
	ILI9341_DispString_EN( 150, 40,"10", macBLACK, macWHITE );
	ILI9341_DispString_CH( 170, 40,"ÈÕ", macBLACK, macWHITE );
	ILI9341_DispString_EN_32( 56, 65,"10:11:12", macBLACK, macWHITE );

	ILI9341_DrawRectangle( 6, 180, 72, 30, macWHITE, 1 );//1
	ILI9341_DispString_EN( 38, 187,"1", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 6, 215, 72, 30, macWHITE, 1 );//4
	ILI9341_DispString_EN( 38, 222,"4", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 6, 250, 72, 30, macWHITE, 1 );//7
	ILI9341_DispString_EN( 38, 257,"7", macWHITE,  macBLUE);
	ILI9341_DrawRectangle( 6, 285, 72, 30, macWHITE, 1 );//Çå³ı
	ILI9341_DispString_CH( 26, 292,"Çå³ı",  macWHITE, macBLUE);
	
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
	ILI9341_DrawRectangle( 162, 285, 72, 30, macWHITE, 1 );//È·¶¨
	ILI9341_DispString_CH( 182, 292,"È·¶¨",  macWHITE, macBLUE);
	
	while(1)
	{
		
		//°´¼ü´¦Àí
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey==141)
			{
				//°´¼ü¹¦ÄÜ...
				while( tp_dev.sta );
				UICnt = 0x00;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==122 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x01;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==123 )
			{
				//...
				while( tp_dev.sta );
				UICnt = 0x02;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
			else if( TouchKey==124 )
			{
				//°´¼ü¹¦ÄÜ...
				while( tp_dev.sta );
			}
		}
	}
}
//ÄÖÖÓ¹¤×÷½çÃæ
void UI05( void )
{
}
//ÄÖÖÓÉèÖÃÒ³Ãæ
void UI06( void )
{
	int i=0;
	u16 TouchKey;
	
	ILI9341_Clear( 0, 0, 240, 320, UI0BackColor);
	ILI9341_DispString_EN_CH( 10, 10, "<< ÄÖÖÓ", macBLACK, macWHITE );
	ILI9341_DispString_CH( 185, 10,"±£´æ", macBLACK, macWHITE );
	
	
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
	ILI9341_DrawRectangle( 6, 285, 72, 30, macWHITE, 1 );//Çå³ı
	ILI9341_DispString_CH( 26, 292,"Çå³ı",  macWHITE, macBLUE);
	
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
	ILI9341_DrawRectangle( 162, 285, 72, 30, macWHITE, 1 );//È·¶¨
	ILI9341_DispString_CH( 182, 292,"È·¶¨",  macWHITE, macBLUE);

	while(1)
	{
		//°´¼ü´¦Àí
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey==171)
			{
				//°´¼ü¹¦ÄÜ...
				while( tp_dev.sta );
				UICnt = 0x01;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö
			}
		}
	}
}
//ÈÕÀú
void UI10( void )
{
	char tbuf[40];
	
	strType_XPT2046_Coordinate strDisplayCoordinate;
	char	cStr[ 10 ];						//ÓÃÀ´´æ·ÅÊı×Ö×ª»»³É×Ö·û´®µÄ×Ö·û
	unsigned int Row,Col;				//ÈÕÀúĞĞÁĞºÅ
	unsigned int CalWidth,CalHeight;	//ÈÕÀúÃ¿¸ñ¿í£¬¸ß
	unsigned int ColOffset,RowOffset;	//ÈÕÀú¸ñÖĞÏÔÊ¾×Ö·ûÊ±ºáÏò¡¢×İÏòÆ«ÒÆÖµ
	unsigned int Cal_YOffset;		//ÈÕÀú±í¸ñ×İÏòÆ«ÒÆÖµ
	unsigned int Date;					//ÈÕÆÚ
	unsigned int DateMax;				//µ±ÔÂ×î´óÈÕÆÚ
//	unsigned int Day;						//ĞÇÆÚ
	uint8_t DayFirst;			//µ±ÔÂ1ºÅĞÇÆÚ£¬1~6´ú±íĞÇÆÚÒ»~ĞÇÆÚÁù£¬0´ú±íĞÇÆÚÌì
	uint8_t count;
  uint16_t Last;//²ÎÊı
	//ÉèÖÃ»æÍ¼²ÎÊı
	CalWidth = 34;
	CalHeight = 42;
	RowOffset = 6;
	
	Cal_YOffset = 80;
	
	//¼ÆËãÔÂÊ×ÈÕĞÇÆÚ
	DayFirst = Get_Week(Calendar.Year,Calendar.Month,1);	
	
	//¼ÆËãÔÂ×î´óÈÕÆÚ
	DateMax = DateMaxTab[Calendar.Month];
	if(Calendar.Month==2)
	{
		if(Is_LeapYear(Calendar.Year)==1)		
		DateMax++;
	}
	
	//ÇåÆÁ
	ILI9341_Clear(0,0,240,320,macWHITE);
	
	//ÏÔÊ¾ÄêÔÂ	
	ILI9341_Clear(0,0,240,54,macBLUE2);	
	ILI9341_Clear(0,54,240,18,macYELLOW);	
	sprintf ( cStr, "%d", Calendar.Year );
 	ILI9341_DispString_EN_32(0,Cal_YOffset-58,cStr,macBLUE2,macRED);
	ILI9341_DispString_EN_32(64,Cal_YOffset-58,"-",macBLUE2,macRED);
	sprintf ( cStr, "%d", Calendar.Month );
	ILI9341_DispString_EN_32(80,Cal_YOffset-58,cStr,macBLUE2,macRED);
	ILI9341_DispString_EN_CH(150,Cal_YOffset-45,"<< ½ñÌì >>",macBLUE2,macRED);
//	sprintf((char*)tbuf,"%02d:%02d",RealTime.Hour,RealTime.Min); 
//	ILI9341_DispString_EN( 0,10, (char*)tbuf , macBLUE2,macBLACK);
//	
	// µÚ0ĞĞ£¬ÏÔÊ¾ĞÇÆÚ
	Row = 0;
	ColOffset = 5;//ÁĞ
	
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
	
	// µÚ1-6ĞĞ£¬ÏÔÊ¾ÈÕÀú±í	
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
	
	//ÏÔÊ¾×îºóÒ»ÖÜÖĞÏÂÔÂÈÕÆÚ£¬ÓÃ»ÒÉ«ÏÔÊ¾
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
	//ÏÔÊ¾µÚÒ»ÖÜÖĞÉÏÔÂÈÕÆÚ£¬ÓÃ»ÒÉ«ÏÔÊ¾
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
		//°´¼ü´¦Àí
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey == 111)
			{
				//°´¼ü¹¦ÄÜ...
				while( tp_dev.sta );
				UICnt = 0x11;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö	
			}
			else if(TouchKey == 1111)
			{
				while( tp_dev.sta );
				UICnt = 0x12;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö	
			}
				
		}	
		
		sprintf((char*)tbuf,"%02d:%02d:%2d",RealTime.Hour,RealTime.Min,RealTime.Sec); 
		ILI9341_DispString_EN( 80,10, (char*)tbuf , macBLUE2,macBLACK);
		if (ucXPT2046_TouchFlag = 1)			                                                   //Èç¹û´¥ÃşÁËÆÁÄ»
    {
			ucXPT2046_TouchFlag = 0;
      if ( XPT2046_Get_TouchedPoint (  &strDisplayCoordinate, & strXPT2046_TouchPara ) )   //»ñÈ¡´¥ÃşµãµÄ×ø±ê	
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
	
	      //¼ÆËãÔÂ×î´óÈÕÆÚ
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
//ÔÂ·İÑ¡Ôñ
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
	ILI9341_DispString_EN_CH( 32, 107,"1ÔÂ", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 32, 143,"4ÔÂ", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 32, 179,"7ÔÂ", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 26, 215,"10ÔÂ", macWHITE,  macBLUE);
	
	ILI9341_DispString_EN_CH( 110, 107,"2ÔÂ", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 110, 143,"5ÔÂ", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 110, 179,"8ÔÂ", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 104, 215,"11ÔÂ", macWHITE,  macBLUE);
	
	ILI9341_DispString_EN_CH( 188, 107,"3ÔÂ", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 188, 143,"6ÔÂ", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 188, 179,"9ÔÂ", macWHITE,  macBLUE);
	ILI9341_DispString_EN_CH( 182, 215,"12ÔÂ", macWHITE,  macBLUE);
	
	
	
	
	while(1)
	{
		sprintf((char*)tbuf,"%02d:%02d:%2d",RealTime.Hour,RealTime.Min,RealTime.Sec); 
		ILI9341_DispString_EN_48( 0,0, (char*)tbuf , macBLUE2,macBLACK);
	}
}
//Äê·İÑ¡Ôñ
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
//ÆÁ±£½çÃæ
void UI20( void )
{
	u16 TouchKey;
	char tbuf[40];
	
	ILI9341_Clear( 0, 0, 240, 320, UI0BackColor);
	ILI9341_DrawRectangle( 0, 120, 240, 120, macWHITE, 1 );
//	ILI9341_DispString_EN_48( 10, 130, "12:29", macWHITE, macBLACK );
//	ILI9341_DispString_EN_CH( 10, 180, "2016Äê12ÔÂ10ÈÕ,ĞÇÆÚËÄ", macWHITE, macBLACK );
	ILI9341_DispString_EN_CH( 10, 210, "µç×ÓA1322_05_³Â½¨·åÆÚÄ©Éè¼Æ", macWHITE, macBLACK );
	
	sprintf((char*)tbuf,"%02d:%02d",RealTime.Hour,RealTime.Min); 
	ILI9341_DispString_EN_48( 10,130, (char*)tbuf , macWHITE,macBLACK);
			
	sprintf((char*)tbuf,"%02d-%02d-%02d",RealTime.Year,RealTime.Month,RealTime.Date);		
	ILI9341_DispString_EN_32( 10,175, (char*)tbuf , macWHITE,macBLACK);
	
	while(1)
	{

		//°´¼ü´¦Àí
		if ( tp_dev.sta )
		{
			TouchKey = Get_TouchKey();
			
			if( TouchKey==110)
			{
				//°´¼ü¹¦ÄÜ...
				while( tp_dev.sta );
				UICnt = 0x00;		//ĞŞ¸Ä½çÃæÖ¸Õë
				break;					//ÍË³ö	
			}
				
		}	
	}
}



/**************  ´¥ÆÁÉ¨Ãèº¯Êı  **************/
u16 Get_TouchKey( void )
{
	u16	KeyTmp;
	
	//Ê±ÖÓ½çÃæ
	if( UICnt==0x00 )				
	{
		if ( tp_dev.sta )			//Èç¹û´¥ÃşÁËÆÁÄ»
		{
			//Ê±ÖÓ	121
			if( tp_dev.x>=23 && tp_dev.x<55 && tp_dev.y>=16&& tp_dev.y<48 )	KeyTmp = 121;
			//ÄÖÖÓ	122
			else if( tp_dev.x>=23+54 && tp_dev.x<55+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 122;
			//Ãë±í
			else if( tp_dev.x>=77+54 && tp_dev.x<109+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 123;
			//¼ÆÊ±
			else if( tp_dev.x>=131+54 && tp_dev.x<163+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 124;
			//Êı×ÖÊ±ÖÓ&ÈÕÀú½çÃæ
			else if( tp_dev.x>=75 && tp_dev.x<175 && tp_dev.y>=210 && tp_dev.y<250 )	KeyTmp = 125;
			//Ê±ÖÓÉèÖÃ½çÃæ
			else if( tp_dev.x>=90 && tp_dev.x<150 && tp_dev.y>=250 && tp_dev.y<300 )	KeyTmp = 126;
			
			else	KeyTmp = 65535;
		}
	}
	//ÄÖÖÓ½çÃæ
	else if( UICnt==0x01 )
	{
		if ( tp_dev.sta )			//Èç¹û´¥ÃşÁËÆÁÄ»
		{
			//Ê±ÖÓ	121
			if( tp_dev.x>=23 && tp_dev.x<55 && tp_dev.y>=16&& tp_dev.y<48 )	KeyTmp = 121;
			//ÄÖÖÓ	122
			else if( tp_dev.x>=23+54 && tp_dev.x<55+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 122;
			//Ãë±í
			else if( tp_dev.x>=77+54 && tp_dev.x<109+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 123;
			//¼ÆÊ±
			else if( tp_dev.x>=131+54 && tp_dev.x<163+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 124;
			//¿ª¹Ø°´Å¥1
			else if( tp_dev.x>=185 && tp_dev.x<225 && tp_dev.y>=165 && tp_dev.y<185 )	KeyTmp = 138;
			//¿ª¹Ø°´Å¥2
			else if( tp_dev.x>=185 && tp_dev.x<225 && tp_dev.y>=220 && tp_dev.y<240 )	KeyTmp = 148;
			//¿ª¹Ø°´Å¥3
			else if( tp_dev.x>=185 && tp_dev.x<225 && tp_dev.y>=275 && tp_dev.y<295 )	KeyTmp = 158;
			//ÄÖÖÓÊ±¼ä1
			else if( tp_dev.x>=10 && tp_dev.x<77 && tp_dev.y>=155 && tp_dev.y<183 )	KeyTmp = 137;
			//ÄÖÖÓÊ±¼ä2
			else if( tp_dev.x>=10 && tp_dev.x<77 && tp_dev.y>=210 && tp_dev.y<237 )	KeyTmp = 147;
			//ÄÖÖÓÊ±¼ä3
			else if( tp_dev.x>=10 && tp_dev.x<77 && tp_dev.y>=265 && tp_dev.y<292 )	KeyTmp = 157;
			
			else	KeyTmp = 65535;
		}
	}
	//ÄÖÖÓÉèÖÃÒ³Ãæ
	else if( UICnt==0x06)
	{
		if ( tp_dev.sta )			//Èç¹û´¥ÃşÁËÆÁÄ»
		{
			//·µ»Ø
			if( tp_dev.x>=10 && tp_dev.x<70 && tp_dev.y>=10&& tp_dev.y<30)	KeyTmp = 171;
		}
	}
	//Ãë±í½çÃæ
	else if( UICnt==0x02 )
	{
		if ( tp_dev.sta )			//Èç¹û´¥ÃşÁËÆÁÄ»
		{
			//Ê±ÖÓ	121
			if( tp_dev.x>=23 && tp_dev.x<55 && tp_dev.y>=16&& tp_dev.y<48 )	KeyTmp = 121;
			//ÄÖÖÓ	122
			else if( tp_dev.x>=23+54 && tp_dev.x<55+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 122;
			//Ãë±í
			else if( tp_dev.x>=77+54 && tp_dev.x<109+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 123;
			//¼ÆÊ±
			else if( tp_dev.x>=131+54 && tp_dev.x<163+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 124;
			
			else	KeyTmp = 65535;
		}		
	}
	//¶¨Ê±½çÃæ
	else if( UICnt==0x03 )
	{
		if ( tp_dev.sta )			//Èç¹û´¥ÃşÁËÆÁÄ»
		{
			//Ê±ÖÓ	121
			if( tp_dev.x>=23 && tp_dev.x<55 && tp_dev.y>=16&& tp_dev.y<48 )	KeyTmp = 121;
			//ÄÖÖÓ	122
			else if( tp_dev.x>=23+54 && tp_dev.x<55+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 122;
			//Ãë±í
			else if( tp_dev.x>=77+54 && tp_dev.x<109+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 123;
			//¼ÆÊ±
			else if( tp_dev.x>=131+54 && tp_dev.x<163+54 && tp_dev.y>=16 && tp_dev.y<38 )	KeyTmp = 124;

			else	KeyTmp = 65535;
		}		
	}
	//Ê±ÖÓÉèÖÃ½çÃæ
	else if( UICnt==0x04)
	{
		if ( tp_dev.sta )			//Èç¹û´¥ÃşÁËÆÁÄ»
		{
			//·µ»Ø
			if( tp_dev.x>=10 && tp_dev.x<70 && tp_dev.y>=10&& tp_dev.y<30)	KeyTmp = 141;
		}
	}
	//ÄÖÖÓ¹¤×÷½çÃæ
	else if( UICnt==0x05 )
	{
		if ( tp_dev.sta )			//Èç¹û´¥ÃşÁËÆÁÄ»
		{
			if( tp_dev.x>=10 && tp_dev.x<70 && tp_dev.y>=10&& tp_dev.y<30)	KeyTmp = 141;
		}
	}
	//ÆÁ±£
	else if( UICnt==0x20 )
	{
		if ( tp_dev.sta )			//Èç¹û´¥ÃşÁËÆÁÄ»
		{
			if( tp_dev.x>=0 && tp_dev.x<240 && tp_dev.y>=0&& tp_dev.y<320 )	KeyTmp = 110;
			else	KeyTmp = 65535;
		}
	}
	//ÈÕÀú
	else if( UICnt==0x10 )
	{
		if ( tp_dev.sta )			//Èç¹û´¥ÃşÁËÆÁÄ»
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

