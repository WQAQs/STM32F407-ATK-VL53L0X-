#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "24cxx.h"
#include "usmart.h"
#include "vl53l0x.h"

/************************************************
 ALIENTEK 探索者STM32F407开发板扩展实验21
 ATK-VL53L0X激光测距模块 综合测试实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);                               //初始化延时函数
	uart_init(115200);	                           //初始化串口波特率为115200
	usmart_dev.init(84); 	                       //初始化USMART	
	LED_Init();					                   //初始化LED 
 //	LCD_Init();					                   //LCD初始化 
	KEY_Init(); 				                   //按键初始化  
	AT24CXX_Init();			                       //IIC初始化 
 	//POINT_COLOR=RED;                               //设置字体为红色 
	//LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	//LCD_ShowString(30,70,200,16,16,"Sensor VL53L0X TEST");	
	//LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	//LCD_ShowString(30,110,200,16,16,"2017/6/25");	    
	//POINT_COLOR=BLUE;//设置字体为蓝色 	
 	while(AT24CXX_Check())//检测不到24c02
	{
		//LCD_ShowString(30,150,200,16,16,"24C02 Check Failed!");
		printf("24C02 Check Failed!\r\n");
		//delay_ms(500);
		//LCD_ShowString(30,150,200,16,16,"Please Check!      ");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}	  
	while(1)
	{
		vl53l0x_test();//vl53l0x测试
	} 	    
}
