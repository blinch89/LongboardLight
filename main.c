#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32_ub_ws2812.h"
#include "stdio.h"
#include "lightPattern.h"


void Delay(volatile uint32_t nCount);
void init(void);
void INTTIM_Config(void);


int main(void)
{
  uint8_t control = '7';
  uint8_t changeFlag = 1;
  uint8_t updateLEDs = 1;
  int i = 0;
  init();
  WS2812_HSV_t frame[WS2812_LED_MAX_ANZ];
  //initFrame(frame);
  while(1)
  {
    if(changeFlag)
    {
        initFrame(frame); //this line could may be moved to somewhere else, TODO, refactor! think!
    }
    if (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) != RESET)
    {
        TIM_ClearFlag(TIM2, TIM_IT_Update);
        switch(control)
        {
        case '0':
            rainbow_spin(frame);
            break;
        case '1':
        case '2':
        case '3':
            dots_circular(control-48, frame);
            break;
        if(changeFlag)
        {
            case '6':
                frame_white(frame);
                break;
            case '7':
                frame_one_color(frame, WS2812_HSV_COL_OFF);
                break;
            case '8':
                frame_one_color(frame, WS2812_HSV_COL_BLUE);
                break;
            case '9':
                frame_one_color(frame, WS2812_HSV_COL_CYAN);
                break;
            case 'A':
                frame_one_color(frame, WS2812_HSV_COL_GREEN);
                break;
            case 'B':
                frame_one_color(frame, WS2812_HSV_COL_MAGENTA);
                break;
            case 'C':
                frame_one_color(frame, WS2812_HSV_COL_RED);
                break;
            case 'D':
                frame_one_color(frame,WS2812_HSV_COL_YELLOW);
                break;
        }
        default:
            break;
        }
        changeFlag = 0;

        if(updateLEDs)
        {
            for(i = 0;i<WS2812_LED_MAX_ANZ-1;i++)
            {
                UB_WS2812_One_Led_HSV(i,frame[i],0);
            }
            //last LED with refresh:
            UB_WS2812_One_Led_HSV(WS2812_LED_MAX_ANZ-1,frame[WS2812_LED_MAX_ANZ-1],1); 
        }
        else if ( !(control >= '0' && control <= '3') )
            updateLEDs = 0;
  }

  //Userbutten start *******
  if((GPIOA->IDR & 0b1)==1)
  {
      Delay(10000000);
      updateLEDs = 1;
      changeFlag = 1;
      switch(control)
      {
      case '0':
      case '1':
      case '2':
      case '6':
      case '7':
      case '8':
          control++;
          break;
      case '3':
          control = '6';
          break;
      case '9':
          control = 'A';
          break;
      case 'A':
      case 'B':
      case 'C':
          control++;
          break;
      case 'D':
          control = '0';
          break;
      default:
          break;
      }
  }
  //Userbutten end   *******
 }
}





void Delay(volatile uint32_t nCount)
{
  while(nCount--)
  {
  }
}





void init(void)
{
    SystemInit();
    UB_WS2812_Init();
    INTTIM_Config();
    
    /*
    init_USART1(9600);
    USART_puts(USART1, "STM_Start\n");
    */
}





void INTTIM_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 600;       // 1MHz / this value = half frequency
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; // Down to 1 MHz (adjust per your clock)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM2, ENABLE);
}






