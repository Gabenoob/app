#include <stdio.h>
#include "oled.h"
#include "gui.h"
#include "test.h"
//16*16/8
// #define FONT_SIZE 32 //32 = 16*16/8
// unsigned char  font_array[][FONT_SIZE]={


void test_led_screen(void)
{	
  
    printf("before oled clear \r\n");
    OLED_Init();			   //初始化OLED  
    OLED_Clear(0);    
    GUI_ShowString(20,0,"OpenHarmony", 16 ,1  );
    OLED_Display();
    sleep(3);
  
    while(1){    
    

    }
   
}

