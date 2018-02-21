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
 ALIENTEK ̽����STM32F407��������չʵ��21
 ATK-VL53L0X������ģ�� �ۺϲ���ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);                               //��ʼ����ʱ����
	uart_init(115200);	                           //��ʼ�����ڲ�����Ϊ115200
	usmart_dev.init(84); 	                       //��ʼ��USMART	
	LED_Init();					                   //��ʼ��LED 
 //	LCD_Init();					                   //LCD��ʼ�� 
	KEY_Init(); 				                   //������ʼ��  
	AT24CXX_Init();			                       //IIC��ʼ�� 
 	//POINT_COLOR=RED;                               //��������Ϊ��ɫ 
	//LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	//LCD_ShowString(30,70,200,16,16,"Sensor VL53L0X TEST");	
	//LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	//LCD_ShowString(30,110,200,16,16,"2017/6/25");	    
	//POINT_COLOR=BLUE;//��������Ϊ��ɫ 	
 	while(AT24CXX_Check())//��ⲻ��24c02
	{
		//LCD_ShowString(30,150,200,16,16,"24C02 Check Failed!");
		printf("24C02 Check Failed!\r\n");
		//delay_ms(500);
		//LCD_ShowString(30,150,200,16,16,"Please Check!      ");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}	  
	while(1)
	{
		vl53l0x_test();//vl53l0x����
	} 	    
}
