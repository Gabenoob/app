/***************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
****************************************************************************************************/		
#include "stdlib.h"
#include "stdio.h"
#include "oled.h"
#include "gui.h"
#include "test.h"
#include "bmp.h"

/*****************************************************************************
 * @name       :void Test_Color(void)
 * @date       :2018-08-27 
 * @function   :Color fill test(white,black)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Color(void)
{
	 GUI_Fill(0,0,WIDTH-1,HEIGHT-1,0);
	 GUI_ShowString(10,10,"BLACK",16,1);
	 delay_ms(1000);	
	 GUI_Fill(0,0,WIDTH-1,HEIGHT-1,1);
	 GUI_ShowString(10,10,"WHITE",16,0);
	 delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Rectangular(void))
 * @date       :2018-08-27
 * @function   :Rectangular display and fill test
								Display black,white rectangular boxes in turn,1000 
								milliseconds later,Fill the rectangle in black,white in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Rectangular(void)
{
	GUI_Fill(0,0,WIDTH/2-1,HEIGHT-1,0);
	GUI_Fill(WIDTH/2,0,WIDTH-1,HEIGHT-1,1);
	GUI_DrawRectangle(5, 5, WIDTH/2-1-5, HEIGHT-1-5,1);
	GUI_DrawRectangle(WIDTH/2-1+5, 5, WIDTH-1-5, HEIGHT-1-5,0);
	delay_ms(1000);
	GUI_FillRectangle(5, 5, WIDTH/2-1-5, HEIGHT-1-5,1);
	GUI_FillRectangle(WIDTH/2-1+5, 5, WIDTH-1-5, HEIGHT-1-5,0);
	delay_ms(1500);
}


/*****************************************************************************
 * @name       :void Test_Circle(void)
 * @date       :2018-08-27 
 * @function   :circular display and fill test
								Display black,white circular boxes in turn,1000 
								milliseconds later,Fill the circular in black,white in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Circle(void)
{
	GUI_Fill(0,0,WIDTH/2-1,HEIGHT-1,0);
	GUI_Fill(WIDTH/2,0,WIDTH-1,HEIGHT-1,1);
	GUI_DrawCircle(WIDTH/2/2-1, HEIGHT/2-1, 1, 27);
	GUI_DrawCircle(WIDTH/2+WIDTH/2/2-1, HEIGHT/2-1, 0, 27);
	delay_ms(1000);
	GUI_FillCircle(WIDTH/2/2-1, HEIGHT/2-1, 1, 27);
	GUI_FillCircle(WIDTH/2+WIDTH/2/2-1, HEIGHT/2-1, 0, 27);
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Triangle(void)
 * @date       :2018-08-27 
 * @function   :triangle display and fill test
								Display black,white triangle boxes in turn,1000 
								milliseconds later,Fill the triangle in black,white in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Triangle(void)
{
	GUI_Fill(0,0,WIDTH/2-1,HEIGHT-1,0);
	GUI_Fill(WIDTH/2,0,WIDTH-1,HEIGHT-1,1);
	GUI_DrawTriangel(5,HEIGHT-1-5,WIDTH/2/2-1,5,WIDTH/2-1-5,HEIGHT-1-5,1);
	GUI_DrawTriangel(WIDTH/2-1+5,HEIGHT-1-5,WIDTH/2+WIDTH/2/2-1,5,WIDTH-1-5,HEIGHT-1-5,0);
	delay_ms(1000);
	GUI_FillTriangel(5,HEIGHT-1-5,WIDTH/2/2-1,5,WIDTH/2-1-5,HEIGHT-1-5,1);
	GUI_FillTriangel(WIDTH/2-1+5,HEIGHT-1-5,WIDTH/2+WIDTH/2/2-1,5,WIDTH-1-5,HEIGHT-1-5,0);
	delay_ms(1500);
}


/*****************************************************************************
 * @name       :void TEST_English(void)
 * @date       :2018-08-27 
 * @function   :English display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_English(void)
{
	GUI_ShowString(0,5,"6x8:abcdefghijklmnopqrstuvwxyz",8,1);
	GUI_ShowString(0,25,"8x16:abcdefghijklmnopqrstuvwxyz",16,1);
	delay_ms(1000);
	GUI_ShowString(0,5,"6x8:ABCDEFGHIJKLMNOPQRSTUVWXYZ",8,1);
	GUI_ShowString(0,25,"8x16:ABCDEFGHIJKLMNOPQRSTUVWXYZ",16,1);
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void TEST_Number_Character(void) 
 * @date       :2018-08-27 
 * @function   :number and character display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_Number_Character(void) 
{
	GUI_Fill(0,0,WIDTH-1,HEIGHT/2-1,0);
	GUI_ShowString(0,0,"6x8:!\"#$%&'()*+,-./:;<=>?@[]\\^_`~{}|",8,1);
	GUI_ShowNum(30,16,1234567890,10,8,1);
	GUI_Fill(0,HEIGHT/2,WIDTH-1,HEIGHT-1,1);
	GUI_ShowString(0,HEIGHT/2+2,"6x8:!\"#$%&'()*+,-./:;<=>?@[]\\^_`~{}|",8,0);
	GUI_ShowNum(30,HEIGHT/2+18,1234567890,10,8,0);
	delay_ms(1000);
	OLED_Clear(0); 
  GUI_ShowString(0,0,"8x16:!\"#$%&'()*+,-./:;<=>?@[]\\^_`~{}|",16,1);
	GUI_ShowNum(40,32,1234567890,10,16,1);
	delay_ms(1500);
	OLED_Clear(0);
}

/*****************************************************************************
 * @name       :void TEST_Chinese(void)
 * @date       :2018-08-27
 * @function   :chinese display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_Chinese(void)
{	
	GUI_ShowChinese(32,26,16,"软通动力",1);
	GUI_ShowChinese(8,48,16,"鸿蒙业务发展部",1);
	delay_ms(1000);
	OLED_Clear(0);
}

/*****************************************************************************
 * @name       :void TEST_BMP(void)
 * @date       :2018-08-27 
 * @function   :BMP monochromatic picture display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_BMP(void)
{
    
	GUI_DrawBMP(0,0,128,64, BMP2, 1);
	delay_ms(1000);
	GUI_DrawBMP(0,0,128,64, BMP3, 1);
	delay_ms(1000);
	GUI_DrawBMP(0,0,128,64, BMP4, 1);
	delay_ms(1000);
	GUI_DrawBMP(0,0,128,26, gImage_isoftstone3, 1);
}

void TEST_LINE(void)
{
	GUI_DrawLine(0,34,30,34,1);
	GUI_DrawLine(96,34,WIDTH-1,34,1);
}

/*****************************************************************************
 * @name       :void TEST_SpliteShow(void)
 * @date       :2022-02-27 
 * @function   :Split whole screen into right & left，show  dfferent contents on each side
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_SplitShow(void)
{
	GUI_DrawLine(63,0,63,63,1);
	GUI_ShowChinese(10,10,16,"明",1);
	GUI_ShowChinese(10,40,16,"浩",1);
	GUI_DrawBMP(64,1,58,58, emoji, 1);
	delay_ms(8000);
}