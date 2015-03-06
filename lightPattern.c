//--------------------------------------------------------------
// File       : lightPattern.c
// Date       : 20th Feb 2015
// Version    : 1.0
// Author     : Ulrich ter Horst
// EMail      : ulrich.terhorst@gmail.com

// Description: This is a library of methods to create light
//              animations with ws2812 LEDs. It uses the
//              stm32_ub_ws2812 module from
//              www.mikrocontroller-4u.de
//              (E-mail: mc-4u(@)t-online.de) to control the
//              LEDs.

//              Update:
//              http://mikrocontroller.bplaced.net/wordpress/?page_id=3665

//--------------------------------------------------------------




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
    unsigned char offset;    //distance between dots (number of LEDs)
    static unsigned char wait = 0;
    wait++;    //slows down the color turning speed, but not the brightness turning speed
    
    //turning the color:
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


    //turning the brightness:
    //tail:
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

    //start dots:
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