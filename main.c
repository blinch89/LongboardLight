#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32_ub_ws2812.h"
#include "stdio.h"


void Delay(volatile uint32_t nCount);
void init(void);
void INTTIM_Config(void);
void dots_circular(char n_dotlines, WS2812_HSV_t *buf);
void rainbow_spin(WS2812_HSV_t *buf);
void initFrame(WS2812_HSV_t *buf);
void frame_white(WS2812_HSV_t *buf);
void frame_one_color(WS2812_HSV_t *buf, WS2812_HSV_t);




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
        initFrame(frame); //das hier macht viel unnötige laufzeit, das kann man ändern behaupte ich
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
        //GPIOD->ODR ^= GPIO_Pin_15;

        if(updateLEDs)
        {
            for(i = 0;i<WS2812_LED_MAX_ANZ-1;i++)
            {
                UB_WS2812_One_Led_HSV(i,frame[i],0);
            }
            UB_WS2812_One_Led_HSV(WS2812_LED_MAX_ANZ-1,frame[WS2812_LED_MAX_ANZ-1],1); //letzte LED mit refresh
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





// This method initializes the memory area for the WS2812 LEDs.
// All LEDs are set to a rainbow pattern (one full circle in HSV
// color data format). Full saturation is used, but BRIGHTNESS IS
// SET TO ZERO. So no light is emitted after calling this method.
void initFrame(WS2812_HSV_t *buf)
{
    unsigned char i;
    for(i = 0;i<WS2812_LED_MAX_ANZ;i++)
    {
      buf[i].h = (int)( ((float)i/(float)WS2812_LED_MAX_ANZ) * 359.0 );
      buf[i].s = 100;
      buf[i].v = 0;
    }

    
    for(i = 0;i<WS2812_LED_MAX_ANZ-1;i++)
    {
      UB_WS2812_One_Led_HSV(i,buf[i],0);
    }
    //last LED with refresh (50usecs):
    UB_WS2812_One_Led_HSV(WS2812_LED_MAX_ANZ-1,buf[WS2812_LED_MAX_ANZ-1],1);
}





void dots_circular(char n_dotlines, WS2812_HSV_t *buf)
{
    static unsigned char cnt = 0;
    unsigned char i;
    unsigned char offset;    //Abstand der Startdots
    static unsigned char wait = 0;
    wait++;    //bremst die Farbdrehgeschwindigkeit aus, nicht aber die Helligkeitsdrehgeschwindigkeit

    //Farbe drehen
    if(wait>10)
    {
        wait = 0;
        for(i=0;i<WS2812_LED_MAX_ANZ;i++)
        {
            buf[i].h++;
            if(buf[i].h>359)
            buf[i].h=0;
        }
    }

    //Helligkeit drehen

    //Schweif:
    if(buf[(cnt+5)%WS2812_LED_MAX_ANZ].v%5 == 0)
    {
        for(i = 0 ; i < n_dotlines ; i++)
        {
            offset = i*(WS2812_LED_MAX_ANZ/n_dotlines);
            if(buf[(cnt+4+offset)%WS2812_LED_MAX_ANZ].v)
                buf[(cnt+4+offset)%WS2812_LED_MAX_ANZ].v--;
            if(buf[(cnt+3+offset)%WS2812_LED_MAX_ANZ].v)
                buf[(cnt+3+offset)%WS2812_LED_MAX_ANZ].v--;
            if(buf[(cnt+2+offset)%WS2812_LED_MAX_ANZ].v)
                buf[(cnt+2+offset)%WS2812_LED_MAX_ANZ].v--;
            if(buf[(cnt+1+offset)%WS2812_LED_MAX_ANZ].v)
                buf[(cnt+1+offset)%WS2812_LED_MAX_ANZ].v--;
            if(buf[cnt+offset].v)
                buf[cnt+offset].v--;
        }
    }

    //Startdots:
    for(i=0 ; i < n_dotlines ; i++)
    {
        offset = i*(WS2812_LED_MAX_ANZ/n_dotlines);
        buf[(cnt+5+offset)%WS2812_LED_MAX_ANZ].v++;
        if(buf[(cnt+5+offset)%WS2812_LED_MAX_ANZ].v > 100)
        {
            buf[(cnt+offset)%WS2812_LED_MAX_ANZ].v = 0;
            if(!i)
            {
                cnt++;
                cnt = cnt % WS2812_LED_MAX_ANZ;
            }
        }
    }
}

void rainbow_spin(WS2812_HSV_t *buf)
{
    static unsigned char wait = 0;
    wait++;
    int i;

    if(! (wait%10))
    {
        for(i=0;i<WS2812_LED_MAX_ANZ;i++)
        {
            buf[i].h++;
            if(buf[i].h>359)
            buf[i].h=0;
        }
    }

    if(wait > 40)
    {
        wait = 0;
        for(i=0;i<WS2812_LED_MAX_ANZ;i++)
        {
            if(buf[i].v<100)
            buf[i].v++;
        }
    }
}


void frame_white(WS2812_HSV_t *buf)
{
    uint8_t i;
    for(i=0;i<WS2812_LED_MAX_ANZ;i++)
    {
        buf[i].s = 0;
        buf[i].v = 100;
    }
}


void frame_one_color(WS2812_HSV_t *buf, WS2812_HSV_t color)
{
    uint8_t i;
    for(i=0;i<WS2812_LED_MAX_ANZ;i++)
    {
        buf[i].h = color.h;
        buf[i].s = color.s;
        buf[i].v = color.v;
    }
}
