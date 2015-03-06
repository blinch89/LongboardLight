//--------------------------------------------------------------
// File     : stm32_ub_ws2812.c
// Date     : 25.04.2014
// Version  : 1.2
// Authr    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : GPIO, TIM, DMA, MISC
// Descript.: driver-library for WS2812-LEDs, up to 4 chains

//            Update:
//            http://mikrocontroller.bplaced.net/wordpress/?page_id=3665

// comment translation to english and minor changes by
// Ulrich ter Horst, 20th Feb 2015

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_ws2812.h"


//--------------------------------------------------------------
// Global internal variables
//--------------------------------------------------------------
uint32_t ws2812_dma_status;
uint16_t WS2812_TIMER_BUF[WS2812_TIMER_BUF_LEN];
uint8_t  ws2812_channel;
WS2812_RGB_t *ws2812_ptr;
uint32_t ws2812_maxanz;


//--------------------------------------------------------------
// internal methods
//--------------------------------------------------------------
void p_WS2812_clearAll(void);
void p_WS2812_calcTimerBuf(void);
void p_WS2812_InitIO(void);
void p_WS2812_InitTIM(void);
void p_WS2812_InitDMA(void);
void p_WS2812_InitNVIC(void);
void p_WS2812_DMA_Start(void);



//--------------------------------------------------------------
// init all WS2812-chains
// all LEDs off
//--------------------------------------------------------------
void UB_WS2812_Init(void)
{
  uint32_t n;
  ws2812_dma_status=0;
  ws2812_channel=0;
  ws2812_maxanz=0;

  // init of Timer Array
  for(n=0;n<WS2812_TIMER_BUF_LEN;n++) {
    WS2812_TIMER_BUF[n]=0; // 0 => for waiting time
  }

  // init of all LED arrays of all chains
  #if WS2812_LED_CH4_ANZ>0
    for(n=0;n<WS2812_LED_CH4_ANZ;n++) {
      WS2812_LED_BUF_CH4[n]=WS2812_RGB_COL_OFF;
    }
    ws2812_channel=4;
  #endif
  #if WS2812_LED_CH3_ANZ>0
    for(n=0;n<WS2812_LED_CH3_ANZ;n++) {
      WS2812_LED_BUF_CH3[n]=WS2812_RGB_COL_OFF;
    }
    ws2812_channel=3;
  #endif
  #if WS2812_LED_CH2_ANZ>0
    for(n=0;n<WS2812_LED_CH2_ANZ;n++) {
      WS2812_LED_BUF_CH2[n]=WS2812_RGB_COL_OFF;
    }
    ws2812_channel=2;
  #endif
  #if WS2812_LED_CH1_ANZ>0
    for(n=0;n<WS2812_LED_CH1_ANZ;n++) {
      WS2812_LED_BUF_CH1[n]=WS2812_RGB_COL_OFF;
    }
    ws2812_channel=1;
  #endif

  if(ws2812_channel==0) return;

  UB_WS2812_SetChannel(ws2812_channel);
  p_WS2812_InitIO();   // init GPIO
  p_WS2812_InitTIM();  // init Timer
  p_WS2812_InitNVIC(); // init NVIC
  p_WS2812_InitDMA();  // init DMA
  p_WS2812_clearAll();
}


//--------------------------------------------------------------
// selection of active LED-chain
// ch = [1...4]
//--------------------------------------------------------------
void UB_WS2812_SetChannel(uint8_t ch)
{
  #if WS2812_LED_CH1_ANZ>0
    if(ch==1) {
      ws2812_channel=1;
      ws2812_ptr=&WS2812_LED_BUF_CH1[0];
      ws2812_maxanz=WS2812_LED_CH1_ANZ;
    }
  #endif
  #if WS2812_LED_CH2_ANZ>0
    if(ch==2) {
      ws2812_channel=2;
      ws2812_ptr=&WS2812_LED_BUF_CH2[0];
      ws2812_maxanz=WS2812_LED_CH2_ANZ;
    }
  #endif
  #if WS2812_LED_CH3_ANZ>0
    if(ch==3) {
      ws2812_channel=3;
      ws2812_ptr=&WS2812_LED_BUF_CH3[0];
      ws2812_maxanz=WS2812_LED_CH3_ANZ;
    }
  #endif
  #if WS2812_LED_CH4_ANZ>0
    if(ch==4) {
      ws2812_channel=4;
      ws2812_ptr=&WS2812_LED_BUF_CH4[0];
      ws2812_maxanz=WS2812_LED_CH4_ANZ;
    }
  #endif
}






//--------------------------------------------------------------
// Refresh of active LED-chain
// update aller LEDs
// RGB-data must be in array "WS2812_LED_BUF[n]"
// n = [0...WS2812_LED_ANZ-1]
//--------------------------------------------------------------
void UB_WS2812_Refresh(void)
{
  if(ws2812_channel==0) return;
  while(ws2812_dma_status!=0); //wait until DMA transfer is done
  p_WS2812_calcTimerBuf();     //calculate timer value
  p_WS2812_DMA_Start();
}







//--------------------------------------------------------------
// converts a HSV - color value into a RGB value
// (source is from UlrichRadig.de)
//--------------------------------------------------------------
void UB_WS2812_RGB_2_HSV(WS2812_HSV_t hsv_col, WS2812_RGB_t *rgb_col)
{
  uint8_t diff;

  // limit values
  if(hsv_col.h>359) hsv_col.h=359;
  if(hsv_col.s>100) hsv_col.s=100;
  if(hsv_col.v>100) hsv_col.v=100;

  if(hsv_col.h < 61) {
    rgb_col->red = 255;
    rgb_col->green = (425 * hsv_col.h) / 100;
    rgb_col->blue = 0;
  }else if(hsv_col.h < 121){
    rgb_col->red = 255 - ((425 * (hsv_col.h-60))/100);
    rgb_col->green = 255;
    rgb_col->blue = 0;
  }else if(hsv_col.h < 181){
    rgb_col->red = 0;
    rgb_col->green = 255;
    rgb_col->blue = (425 * (hsv_col.h-120))/100;
  }else if(hsv_col.h < 241){
    rgb_col->red = 0;
    rgb_col->green = 255 - ((425 * (hsv_col.h-180))/100);
    rgb_col->blue = 255;
  }else if(hsv_col.h < 301){
    rgb_col->red = (425 * (hsv_col.h-240))/100;
    rgb_col->green = 0;
    rgb_col->blue = 255;
  }else {
    rgb_col->red = 255;
    rgb_col->green = 0;
    rgb_col->blue = 255 - ((425 * (hsv_col.h-300))/100);
  }

  hsv_col.s = 100 - hsv_col.s;
  diff = ((255 - rgb_col->red) * hsv_col.s)/100;
  rgb_col->red = rgb_col->red + diff;
  diff = ((255 - rgb_col->green) * hsv_col.s)/100;
  rgb_col->green = rgb_col->green + diff;
  diff = ((255 - rgb_col->blue) * hsv_col.s)/100;
  rgb_col->blue = rgb_col->blue + diff;

  rgb_col->red = (rgb_col->red * hsv_col.v)/100;
  rgb_col->green = (rgb_col->green * hsv_col.v)/100;
  rgb_col->blue = (rgb_col->blue * hsv_col.v)/100;
}







//--------------------------------------------------------------
// set color to "rgb_col" for an LED specified by "nr" of an
// active LED-chain
// nr      : [0...WS2812_LED_ANZ-1]
// rgb_col : color value (in RGB)
// refresh : 0=without refresh, 1=with refresh
//--------------------------------------------------------------
void UB_WS2812_One_Led_RGB(uint32_t nr, WS2812_RGB_t rgb_col, uint8_t refresh)
{
  if(ws2812_channel==0) return;

  if(nr<ws2812_maxanz) {
    ws2812_ptr[nr]=rgb_col;
        
    if(refresh==1) UB_WS2812_Refresh();
  }
}






//--------------------------------------------------------------
// set color to "rgb_col" for all LEDs of an active LED-chain
// rgb_col : color value (in RGB)
// refresh : 0=without refresh, 1=with refresh
//--------------------------------------------------------------
void UB_WS2812_All_Led_RGB(WS2812_RGB_t rgb_col, uint8_t refresh)
{
    uint32_t n;
    if(ws2812_channel==0) return;
    for(n=0;n<ws2812_maxanz;n++)
        ws2812_ptr[n]=rgb_col;
    if(refresh==1) UB_WS2812_Refresh();
}







//--------------------------------------------------------------
// set an LED "nr" of active LED-chain to "hsv_col"
// nr      : [0...WS2812_LED_ANZ-1]
// hsv_col : color value (in HSV)
// refresh : 0=without refresh, 1=with refresh
//--------------------------------------------------------------
void UB_WS2812_One_Led_HSV(uint32_t nr, WS2812_HSV_t hsv_col, uint8_t refresh)
{
    WS2812_RGB_t rgb_col;
    if(ws2812_channel==0) return;
    if(nr<ws2812_maxanz)
    {
        UB_WS2812_RGB_2_HSV(hsv_col, &rgb_col);
        ws2812_ptr[nr]=rgb_col;
        if(refresh==1) UB_WS2812_Refresh();
    }
}







//--------------------------------------------------------------
// set all LEDs of active LED-chain to "hsv_col"
// hsv_col : color value (in HSV)
// refresh : 0=without refresh, 1=with refresh
//--------------------------------------------------------------
void UB_WS2812_All_Led_HSV(WS2812_HSV_t hsv_col, uint8_t refresh)
{
    uint32_t n;
    WS2812_RGB_t rgb_col;
    if(ws2812_channel==0) return;
    UB_WS2812_RGB_2_HSV(hsv_col, &rgb_col);
    for(n=0;n<ws2812_maxanz;n++)
        ws2812_ptr[n]=rgb_col;
    if(refresh==1) UB_WS2812_Refresh();
}







//--------------------------------------------------------------
// left shift all LEDs of active chain (shift one position)
// set last LED to "off" (linear shift)
//--------------------------------------------------------------
void UB_WS2812_Shift_Left(void)
{
    uint32_t n;
    if(ws2812_channel==0) return;
    if(ws2812_maxanz>1)
    {
        for(n=1;n<ws2812_maxanz;n++)
            ws2812_ptr[n-1]=ws2812_ptr[n];
        ws2812_ptr[n-1]=WS2812_RGB_COL_OFF;
        UB_WS2812_Refresh();
    }
}







//--------------------------------------------------------------
// right shift all LEDs of active chain (shift one position)
// set first LED to "off" (linear shift)
//--------------------------------------------------------------
void UB_WS2812_Shift_Right(void)
{
    uint32_t n;
    if(ws2812_channel==0) return;
    if(ws2812_maxanz>1) 
    {
        for(n=ws2812_maxanz-1;n>0;n--)
            ws2812_ptr[n]=ws2812_ptr[n-1];
        ws2812_ptr[n]=WS2812_RGB_COL_OFF;
        UB_WS2812_Refresh();
    }
}







//--------------------------------------------------------------
// left shift all LEDs of active chain (shift one position)
// set last LED to color of first LED (circular shift)
//--------------------------------------------------------------
void UB_WS2812_Rotate_Left(void)
{
    uint32_t n;
    WS2812_RGB_t d;
    if(ws2812_channel==0) return;
    if(ws2812_maxanz>1) 
    {
        d=ws2812_ptr[0];
        for(n=1;n<ws2812_maxanz;n++)
            ws2812_ptr[n-1]=ws2812_ptr[n];
        ws2812_ptr[n-1]=d;
        UB_WS2812_Refresh();
    }
}







//--------------------------------------------------------------
// left shift all LEDs of active chain (shift one position)
// set first LED to color of last LED (circular shift)
//--------------------------------------------------------------
void UB_WS2812_Rotate_Right(void)
{
    uint32_t n;
    WS2812_RGB_t d;
    if(ws2812_channel==0) return;
    if(ws2812_maxanz>1) 
    {
        d=ws2812_ptr[ws2812_maxanz-1];
        for(n=ws2812_maxanz-1;n>0;n--) 
            ws2812_ptr[n]=ws2812_ptr[n-1];
        ws2812_ptr[n]=d;
        UB_WS2812_Refresh();
    }
}








//--------------------------------------------------------------
// deletes all chains (for internal use only)
//--------------------------------------------------------------
void p_WS2812_clearAll(void)
{
    //-------------------------
    // 1) check if channel is used (longer than 0)
    // 2) wait until DMA transfer is finished
    // 3) start DMA
    //-------------------------
    if(WS2812_LED_CH4_ANZ>0) 
    {
        UB_WS2812_SetChannel(4);
        while(ws2812_dma_status!=0);
        p_WS2812_DMA_Start();
        UB_WS2812_All_Led_RGB(WS2812_RGB_COL_OFF,1);
    }
    if(WS2812_LED_CH3_ANZ>0) 
    {
        UB_WS2812_SetChannel(3);
        while(ws2812_dma_status!=0);
        p_WS2812_DMA_Start();
        UB_WS2812_All_Led_RGB(WS2812_RGB_COL_OFF,1);
    }
    if(WS2812_LED_CH2_ANZ>0)
    {
        UB_WS2812_SetChannel(2);
        while(ws2812_dma_status!=0);
        p_WS2812_DMA_Start();
        UB_WS2812_All_Led_RGB(WS2812_RGB_COL_OFF,1);
    }
    if(WS2812_LED_CH1_ANZ>0)
    {
        UB_WS2812_SetChannel(1);
        while(ws2812_dma_status!=0);
        p_WS2812_DMA_Start();
        UB_WS2812_All_Led_RGB(WS2812_RGB_COL_OFF,1);
    }
}








//--------------------------------------------------------------
// calculates PWM-values from RGB-data for active LED-chain
// (for internal use only)
//--------------------------------------------------------------
void p_WS2812_calcTimerBuf(void)
{
  uint32_t n;
  uint32_t pos;
  WS2812_RGB_t led;
  if(ws2812_channel==0) return;
  pos=0;
  for(n=0;n<ws2812_maxanz;n++) // set timings for all LEDs
  {
      led=ws2812_ptr[n];  
      // Col:Green , Bit:7..0
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.green&0x80)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.green&0x40)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.green&0x20)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.green&0x10)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.green&0x08)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.green&0x04)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.green&0x02)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.green&0x01)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;

      // Col:Red , Bit:7..0
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.red&0x80)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.red&0x40)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.red&0x20)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.red&0x10)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.red&0x08)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.red&0x04)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.red&0x02)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.red&0x01)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;

      // Col:Blue , Bit:7..0
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.blue&0x80)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.blue&0x40)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.blue&0x20)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.blue&0x10)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.blue&0x08)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.blue&0x04)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.blue&0x02)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
      WS2812_TIMER_BUF[pos]=WS2812_LO_TIME;
      if((led.blue&0x01)!=0) WS2812_TIMER_BUF[pos++]=WS2812_HI_TIME;
  } 
    // append a waiting delay after color timing values for refresh (2*30ms)
    for(n=0;n<WS2812_PAUSE_ANZ*WS2812_BIT_PER_LED;n++) 
        WS2812_TIMER_BUF[pos++]=0;  // 0 => for waiting delay
}








//--------------------------------------------------------------
// start DMA and Timer
// (is stopped via Transfer-Complete-Interrupt)
//--------------------------------------------------------------
void p_WS2812_DMA_Start(void)
{
  if(ws2812_channel==0) return;
  ws2812_dma_status=1; // set "busy" state
  p_WS2812_InitDMA();
  if(ws2812_channel==1)
  {
    // enable Transfer-Complete Interrupt & DMA
    DMA_ITConfig(WS2812_DMA_CH1_STREAM, DMA_IT_TC, ENABLE);
    DMA_Cmd(WS2812_DMA_CH1_STREAM, ENABLE);
  }
  else if(ws2812_channel==2) 
  {
    // enable Transfer-Complete Interrupt & DMA
    DMA_ITConfig(WS2812_DMA_CH2_STREAM, DMA_IT_TC, ENABLE);
    DMA_Cmd(WS2812_DMA_CH2_STREAM, ENABLE);
  }
  else if(ws2812_channel==3) 
  {
    // enable Transfer-Complete Interrupt & DMA
    DMA_ITConfig(WS2812_DMA_CH3_STREAM, DMA_IT_TC, ENABLE);
    DMA_Cmd(WS2812_DMA_CH3_STREAM, ENABLE);
  }
  else if(ws2812_channel==4) 
  {
    // enable Transfer-Complete Interrupt & DMA
    DMA_ITConfig(WS2812_DMA_CH4_STREAM, DMA_IT_TC, ENABLE);
    DMA_Cmd(WS2812_DMA_CH4_STREAM, ENABLE);
  }
  TIM_Cmd(WS2812_TIM, ENABLE); //timer enable
}






//--------------------------------------------------------------
// init GPIOs
//--------------------------------------------------------------
void p_WS2812_InitIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  #if WS2812_LED_CH1_ANZ>0 
    RCC_AHB1PeriphClockCmd(WS2812_CH1_CLOCK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = WS2812_CH1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(WS2812_CH1_PORT, &GPIO_InitStructure);
    WS2812_CH1_PORT->BSRRH = WS2812_CH1_PIN; // set to 'low' output
    GPIO_PinAFConfig(WS2812_CH1_PORT, WS2812_CH1_SOURCE, WS2812_TIM_AF); //alternative funct
  #endif

  #if WS2812_LED_CH2_ANZ>0 
    RCC_AHB1PeriphClockCmd(WS2812_CH2_CLOCK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = WS2812_CH2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(WS2812_CH2_PORT, &GPIO_InitStructure);
    WS2812_CH2_PORT->BSRRH = WS2812_CH2_PIN; // set to 'low' output
    GPIO_PinAFConfig(WS2812_CH2_PORT, WS2812_CH2_SOURCE, WS2812_TIM_AF); //alternative funct
  #endif

  #if WS2812_LED_CH3_ANZ>0 
    RCC_AHB1PeriphClockCmd(WS2812_CH3_CLOCK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = WS2812_CH3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(WS2812_CH3_PORT, &GPIO_InitStructure);
    WS2812_CH3_PORT->BSRRH = WS2812_CH3_PIN; // set to 'low' output
    GPIO_PinAFConfig(WS2812_CH3_PORT, WS2812_CH3_SOURCE, WS2812_TIM_AF); //alternativ funct
  #endif

  #if WS2812_LED_CH4_ANZ>0 
    RCC_AHB1PeriphClockCmd(WS2812_CH4_CLOCK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = WS2812_CH4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(WS2812_CH4_PORT, &GPIO_InitStructure);
    WS2812_CH4_PORT->BSRRH = WS2812_CH4_PIN; //set to 'low' output
    GPIO_PinAFConfig(WS2812_CH4_PORT, WS2812_CH4_SOURCE, WS2812_TIM_AF); //alternativ funct
  #endif
}






//--------------------------------------------------------------
// init Timer
//--------------------------------------------------------------
void p_WS2812_InitTIM(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  RCC_APB1PeriphClockCmd(WS2812_TIM_CLOCK, ENABLE); 
  RCC_AHB1PeriphClockCmd(WS2812_DMA_CLOCK, ENABLE);
  TIM_TimeBaseStructure.TIM_Period = WS2812_TIM_PERIODE;
  TIM_TimeBaseStructure.TIM_Prescaler = WS2812_TIM_PRESCALE;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(WS2812_TIM, &TIM_TimeBaseStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  #if WS2812_LED_CH1_ANZ>0
    TIM_OC1Init(WS2812_TIM, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(WS2812_TIM, TIM_OCPreload_Enable);
  #endif
  #if WS2812_LED_CH2_ANZ>0
    TIM_OC2Init(WS2812_TIM, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(WS2812_TIM, TIM_OCPreload_Enable);
  #endif
  #if WS2812_LED_CH3_ANZ>0
    TIM_OC3Init(WS2812_TIM, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(WS2812_TIM, TIM_OCPreload_Enable);
  #endif
  #if WS2812_LED_CH4_ANZ>0
    TIM_OC4Init(WS2812_TIM, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(WS2812_TIM, TIM_OCPreload_Enable);
  #endif
  TIM_ARRPreloadConfig(WS2812_TIM, ENABLE); //enable timer
}






//--------------------------------------------------------------
// init DMA
//--------------------------------------------------------------
void p_WS2812_InitDMA(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  if(ws2812_channel==0) return;
  if(ws2812_channel==1)
  {
    DMA_Cmd(WS2812_DMA_CH1_STREAM, DISABLE);
    DMA_DeInit(WS2812_DMA_CH1_STREAM);
    DMA_InitStructure.DMA_Channel = WS2812_DMA_CH1_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &WS2812_TIM_CCR_REG1;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)WS2812_TIMER_BUF;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = WS2812_TIMER_BUF_LEN1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 16bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(WS2812_DMA_CH1_STREAM, &DMA_InitStructure);
  }
  else if(ws2812_channel==2)
  {
    DMA_Cmd(WS2812_DMA_CH2_STREAM, DISABLE);
    DMA_DeInit(WS2812_DMA_CH2_STREAM);
    DMA_InitStructure.DMA_Channel = WS2812_DMA_CH2_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &WS2812_TIM_CCR_REG2;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)WS2812_TIMER_BUF;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = WS2812_TIMER_BUF_LEN2;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 16bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(WS2812_DMA_CH2_STREAM, &DMA_InitStructure);
  }
  else if(ws2812_channel==3) 
  {
    DMA_Cmd(WS2812_DMA_CH3_STREAM, DISABLE);
    DMA_DeInit(WS2812_DMA_CH3_STREAM);
    DMA_InitStructure.DMA_Channel = WS2812_DMA_CH3_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &WS2812_TIM_CCR_REG3;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)WS2812_TIMER_BUF;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = WS2812_TIMER_BUF_LEN3;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 16bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(WS2812_DMA_CH3_STREAM, &DMA_InitStructure);
  }
  else if(ws2812_channel==4) 
  {
    DMA_Cmd(WS2812_DMA_CH4_STREAM, DISABLE);
    DMA_DeInit(WS2812_DMA_CH4_STREAM);
    DMA_InitStructure.DMA_Channel = WS2812_DMA_CH4_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &WS2812_TIM_CCR_REG4;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)WS2812_TIMER_BUF;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = WS2812_TIMER_BUF_LEN4;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 16bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(WS2812_DMA_CH4_STREAM, &DMA_InitStructure);
  }
}






//--------------------------------------------------------------
// init NVIC
//--------------------------------------------------------------
void p_WS2812_InitNVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  #if WS2812_LED_CH1_ANZ>0
    TIM_DMACmd(WS2812_TIM, WS2812_TIM_DMA_TRG1, ENABLE);
  #endif
  #if WS2812_LED_CH2_ANZ>0
    TIM_DMACmd(WS2812_TIM, WS2812_TIM_DMA_TRG2, ENABLE);
  #endif
  #if WS2812_LED_CH3_ANZ>0
    TIM_DMACmd(WS2812_TIM, WS2812_TIM_DMA_TRG3, ENABLE);
  #endif
  #if WS2812_LED_CH4_ANZ>0
    TIM_DMACmd(WS2812_TIM, WS2812_TIM_DMA_TRG4, ENABLE);
  #endif

  #if WS2812_LED_CH1_ANZ>0
    NVIC_InitStructure.NVIC_IRQChannel = WS2812_DMA_CH1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  #endif
  #if WS2812_LED_CH2_ANZ>0
    NVIC_InitStructure.NVIC_IRQChannel = WS2812_DMA_CH2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  #endif
  #if WS2812_LED_CH3_ANZ>0
    NVIC_InitStructure.NVIC_IRQChannel = WS2812_DMA_CH3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  #endif
  #if WS2812_LED_CH4_ANZ>0
    NVIC_InitStructure.NVIC_IRQChannel = WS2812_DMA_CH4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  #endif
}





//--------------------------------------------------------------
// ISR of DMA (CH1)
// is called if all data has been transmitted
//--------------------------------------------------------------
void WS2812_DMA_CH1_ISR(void)
{
  // check Transfer-Complete Interrupt Flag
  if (DMA_GetITStatus(WS2812_DMA_CH1_STREAM, WS2812_DMA_CH1_IRQ_FLAG))
  {
    // flag reset
    DMA_ClearITPendingBit(WS2812_DMA_CH1_STREAM, WS2812_DMA_CH1_IRQ_FLAG);

    // Timer disable
    TIM_Cmd(WS2812_TIM, DISABLE);
    // DMA disable
    DMA_Cmd(WS2812_DMA_CH1_STREAM, DISABLE);
    
    // set state to "ready"
    ws2812_dma_status=0;
  }
}


//--------------------------------------------------------------
// ISR of DMA (CH2)
// is called if all data has been transmitted
//--------------------------------------------------------------
void WS2812_DMA_CH2_ISR(void)
{
  // check Transfer-Complete Interrupt Flag
  if (DMA_GetITStatus(WS2812_DMA_CH2_STREAM, WS2812_DMA_CH2_IRQ_FLAG))
  {
    // flag reset
    DMA_ClearITPendingBit(WS2812_DMA_CH2_STREAM, WS2812_DMA_CH2_IRQ_FLAG);

    // Timer disable
    TIM_Cmd(WS2812_TIM, DISABLE);
    // DMA disable
    DMA_Cmd(WS2812_DMA_CH2_STREAM, DISABLE);

    // set state to "ready"
    ws2812_dma_status=0;
  }
}


//--------------------------------------------------------------
// ISR of DMA (CH3)
// is called if all data has been transmitted
//--------------------------------------------------------------
void WS2812_DMA_CH3_ISR(void)
{
  // check Transfer-Complete Interrupt Flag
  if (DMA_GetITStatus(WS2812_DMA_CH3_STREAM, WS2812_DMA_CH3_IRQ_FLAG))
  {
    // flag reset
    DMA_ClearITPendingBit(WS2812_DMA_CH3_STREAM, WS2812_DMA_CH3_IRQ_FLAG);

    // Timer disable
    TIM_Cmd(WS2812_TIM, DISABLE);
    // DMA disable
    DMA_Cmd(WS2812_DMA_CH3_STREAM, DISABLE);

    // set state to "ready"
    ws2812_dma_status=0;
  }
}


//--------------------------------------------------------------
// ISR of DMA (CH4)
// is called, if all data has been transmitted
//--------------------------------------------------------------
void WS2812_DMA_CH4_ISR(void)
{
  // check Transfer-Complete Interrupt Flag
  if (DMA_GetITStatus(WS2812_DMA_CH4_STREAM, WS2812_DMA_CH4_IRQ_FLAG))
  {
    // flag reset
    DMA_ClearITPendingBit(WS2812_DMA_CH4_STREAM, WS2812_DMA_CH4_IRQ_FLAG);

    // Timer disable
    TIM_Cmd(WS2812_TIM, DISABLE);
    // DMA disable
    DMA_Cmd(WS2812_DMA_CH4_STREAM, DISABLE);

    // set state to"ready"
    ws2812_dma_status=0;
  }
}
