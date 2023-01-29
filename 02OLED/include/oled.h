#ifndef __OLED_H
#define __OLED_H			  	 
#include "base_type.h"

#include "hi_spi.h"
//--------------OLED参数定义---------------------
#define PAGE_SIZE    8
#define XLevelL		0x02 //1.3寸XLevelL为0x02 0.9寸XLevelL为0x00
#define XLevelH		0x10
#define YLevel       0xB0
#define	Brightness	 0xFF 
#define WIDTH 	     128
#define HEIGHT 	     64	

//-------------写命令和数据定义-------------------
#define OLED_CMD     0	//写命令
#define OLED_DATA    1	//写数据 
   						  
//-----------------OLED端口定义----------------
#define OLED_CS   GPIO_Pin_11   //片选信号           
#define OLED_DC   GPIO_Pin_10   //数据/命令控制信号  
#define OLED_RST  GPIO_Pin_12   //复位信号           

//-----------------OLED端口操作定义---------------- 
/*
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,OLED_CS)
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,OLED_CS)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOB,OLED_DC)
#define OLED_DC_Set()  GPIO_SetBits(GPIOB,OLED_DC)
 					   
#define OLED_RST_Clr()  GPIO_ResetBits(GPIOB,OLED_RST)
#define OLED_RST_Set()  GPIO_SetBits(GPIOB,OLED_RST)
*/



#define SPI_WriteByte(SPI2,dat) spi_write_data(0,dat,1);


void oled_gpio_io_init(void);

//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);     							   		    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Reset(void);
void OLED_Init_GPIO(void);	   							   		    
void OLED_Init(void);
void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color);
void OLED_Display(void);
void OLED_Clear(unsigned dat);  
#endif
