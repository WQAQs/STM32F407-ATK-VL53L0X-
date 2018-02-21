#include "vl53l0x_gen.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ̽����STM32F407������
//VL53L0X-��ͨ����ģʽ ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/7/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

VL53L0X_RangingMeasurementData_t vl53l0x_data;//�������ṹ��
vu16 Distance_data=0;//����������

//VL53L0X ����ģʽ����
//dev:�豸I2C�����ṹ��
//mode: 0:Ĭ��;1:�߾���;2:������
VL53L0X_Error vl53l0x_set_mode(VL53L0X_Dev_t *dev,u8 mode)
{
	
	 VL53L0X_Error status = VL53L0X_ERROR_NONE;
	 uint8_t VhvSettings;
	 uint8_t PhaseCal;
	 uint32_t refSpadCount;
	 uint8_t isApertureSpads;
	
	 vl53l0x_reset(dev);//��λvl53l0x(Ƶ���л�����ģʽ���׵��²ɼ��������ݲ�׼���������һ����)
	 status = VL53L0X_StaticInit(dev);

     if(AjustOK!=0)//��У׼����,д��У׼ֵ
     {
	    status= VL53L0X_SetReferenceSpads(dev,Vl53l0x_data.refSpadCount,Vl53l0x_data.isApertureSpads);//�趨SpadsУ׼ֵ
        if(status!=VL53L0X_ERROR_NONE) goto error;	
        delay_ms(2);		 
	    status= VL53L0X_SetRefCalibration(dev,Vl53l0x_data.VhvSettings,Vl53l0x_data.PhaseCal);//�趨RefУ׼ֵ
		if(status!=VL53L0X_ERROR_NONE) goto error;
		 delay_ms(2);
	    status=VL53L0X_SetOffsetCalibrationDataMicroMeter(dev,Vl53l0x_data.OffsetMicroMeter);//�趨ƫ��У׼ֵ
		if(status!=VL53L0X_ERROR_NONE) goto error; 
		 delay_ms(2);
		status=VL53L0X_SetXTalkCompensationRateMegaCps(dev,Vl53l0x_data.XTalkCompensationRateMegaCps);//�趨����У׼ֵ
		if(status!=VL53L0X_ERROR_NONE) goto error;
         delay_ms(2);		 
		 
     }
	 else
	 {
		status = VL53L0X_PerformRefCalibration(dev, &VhvSettings, &PhaseCal);//Ref�ο�У׼
		if(status!=VL53L0X_ERROR_NONE) goto error;
		delay_ms(2);
		status = VL53L0X_PerformRefSpadManagement(dev, &refSpadCount, &isApertureSpads);//ִ�вο�SPAD����
		if(status!=VL53L0X_ERROR_NONE) goto error;
        delay_ms(2);		 	 
	 }
	 status = VL53L0X_SetDeviceMode(dev,VL53L0X_DEVICEMODE_SINGLE_RANGING);//ʹ�ܵ��β���ģʽ
	 if(status!=VL53L0X_ERROR_NONE) goto error;
	 delay_ms(2);
	 status = VL53L0X_SetLimitCheckEnable(dev,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,1);//ʹ��SIGMA��Χ���
	 if(status!=VL53L0X_ERROR_NONE) goto error;
	 delay_ms(2);
	 status = VL53L0X_SetLimitCheckEnable(dev,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,1);//ʹ���ź����ʷ�Χ���
	 if(status!=VL53L0X_ERROR_NONE) goto error;
	 delay_ms(2);
	 status = VL53L0X_SetLimitCheckValue(dev,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,Mode_data[mode].sigmaLimit);//�趨SIGMA��Χ
	 if(status!=VL53L0X_ERROR_NONE) goto error;
	 delay_ms(2);
	 status = VL53L0X_SetLimitCheckValue(dev,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,Mode_data[mode].signalLimit);//�趨�ź����ʷ�Χ��Χ
	 if(status!=VL53L0X_ERROR_NONE) goto error;
	 delay_ms(2);
	 status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev,Mode_data[mode].timingBudget);//�趨��������ʱ��
	 if(status!=VL53L0X_ERROR_NONE) goto error;
	 delay_ms(2);
	 status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, Mode_data[mode].preRangeVcselPeriod);//�趨VCSEL��������
	 if(status!=VL53L0X_ERROR_NONE) goto error;
	 delay_ms(2);
	 status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, Mode_data[mode].finalRangeVcselPeriod);//�趨VCSEL�������ڷ�Χ
	 
	 error://������Ϣ
	 if(status!=VL53L0X_ERROR_NONE)
	 {
		print_pal_error(status);
		LCD_Fill(30,140+20,300,300,WHITE);
		return status;
	 }
	 return status;
	
}	

//VL53L0X ���ξ����������
//dev:�豸I2C�����ṹ��
//pdata:����������ݽṹ��
VL53L0X_Error vl53l0x_start_single_test(VL53L0X_Dev_t *dev,VL53L0X_RangingMeasurementData_t *pdata,char *buf)
{
	VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint8_t RangeStatus;
	
	status = VL53L0X_PerformSingleRangingMeasurement(dev, pdata);//ִ�е��β�ಢ��ȡ����������
	if(status !=VL53L0X_ERROR_NONE) return status;
   
	RangeStatus = pdata->RangeStatus;//��ȡ��ǰ����״̬
    memset(buf,0x00,VL53L0X_MAX_STRING_LENGTH);
	VL53L0X_GetRangeStatusString(RangeStatus,buf);//���ݲ���״̬��ȡ״̬�ַ���
	
	Distance_data = pdata->RangeMilliMeter;//�������һ�β���������
	
    return status;
}


//������ͨ����
//dev���豸I2C�����ṹ��
//modeģʽ���� 0:Ĭ��;1:�߾���;2:������
void vl53l0x_general_start(VL53L0X_Dev_t *dev,u8 mode)
{
	static char buf[VL53L0X_MAX_STRING_LENGTH];//����ģʽ�ַ����ַ�������
	VL53L0X_Error Status=VL53L0X_ERROR_NONE;//����״̬
	u8 key=0;
	u8 i=0;
	
	LED0=1;
	mode_string(mode,buf);//��ʾ��ǰ���õ�ģʽ
	while(vl53l0x_set_mode(dev,mode))//���ò���ģʽ
	{
	//	LCD_ShowString(30,140+40,200,16,16,"Mode Set Error!!!");
		printf("Mode Set Error!!!\r\n");

		delay_ms(500);
//		LCD_ShowString(30,140+40,200,16,16,"                 ");
		delay_ms(500);
		i++;
		if(i==2) return;
	
	}
//	LCD_Fill(30,140+20,300,300,WHITE);
//	LCD_ShowString(30,140+30,200,16,16,"KEY_UP: Exit the test             ");		
//	LCD_ShowString(30,140+50,200,16,16,"Mode:        ");
//	LCD_ShowString(80,140+50,200,16,16,(u8*)buf);
//	LCD_ShowString(30,140+70,200,16,16,"State:");//��ʾ����״̬	 
//	LCD_ShowString(30,140+90,200,16,16,"Distance:    0 mm");//��ʾ�������� 
	printf("Mode Set OK\r\n");
	printf("KEY_UP: Exit the test\r\n");

	while(1)
	{
		 key = KEY_Scan(0);
		 if(key == WKUP_PRES)
		 {
			 LED1=1;
			 break;//������һ�˵�
			 
		 }
		 if(Status==VL53L0X_ERROR_NONE)
		 {
				Status = vl53l0x_start_single_test(dev,&vl53l0x_data,buf);//ִ��һ�β���
			//	LCD_ShowString(85,140+70,200,16,16,(u8*)buf);
			//	LCD_ShowxNum(110,140+90,Distance_data,4,16,0);
				printf("State;%i , %s\r\n",vl53l0x_data.RangeStatus,buf);//��ӡ����״̬	
				printf("d: %4imm\r\n",Distance_data);//��ӡ��������
		 }
		 i++; 
		 if(i==5)
		 {
			 i=0;
			 LED1=!LED1;	
		 }
       delay_ms(50);	
		
	}	
	
}

//vl53l0x��ͨ����ģʽUI
void general_ui(void)
{
//	LCD_Fill(30,140+20,300,300,WHITE);
//	POINT_COLOR=RED;        //��������Ϊ��ɫ 
//	LCD_ShowString(30,140+30,300,16,16,"General Mode                  ");
		printf("General Mode\r\n");   
//	LCD_ShowString(30,140+55,300,16,16,"KEY1: Switch working mode    ");
		printf("KEY1: Switch working mode\r\n");   
	//POINT_COLOR=BLUE;       //��������Ϊ��ɫ 
	//LCD_ShowString(30,140+75,300,16,16, "KEY_UP: Return menu    ");
	//LCD_ShowString(30,140+95,300,16,16, "KEY0:   Default        ");
		printf("KEY_UP: Return menu\r\n");   
		printf("KEY0:   general_start\r\n");   
	
}

//vl53l0x��ͨ����ģʽ����
//dev:�豸I2C�����ṹ��
void vl53l0x_general_test(VL53L0X_Dev_t *dev)
{
	u8 key=0;
	u8 i=0;
	u8 mode=0;
	LED1=1;
	general_ui();//��ʾ��ͨ����ģʽUI
	
	while(1)
	{	

		key = KEY_Scan(0);
		
		if(key==WKUP_PRES)	break;//�������˵� 	
		
		else if(key==KEY1_PRES)//ѡ����ģʽ
		{
             mode++;
			 if(mode==4) mode=0;
			 switch(mode)
			 {
				 case Default_Mode:  //LCD_ShowString(95,140+95,300,16,16, "Default        "); 
				                      printf(" Default\r\n");           break;//Ĭ��
				 case HIGH_ACCURACY: //LCD_ShowString(95,140+95,300,16,16, "High Accuracy  ");
				                      printf("High Accuracy\r\n");   break;//�߾���
				 case LONG_RANGE:   // LCD_ShowString(95,140+95,300,16,16, "Long Range     "); 
															printf("Long Range\r\n");   break;//������
				 case HIGH_SPEED:   // LCD_ShowString(95,140+95,300,16,16, "High Speed     ");
															printf("High Speed\r\n");   break;//����
			 }
		}	
		else if(key==KEY0_PRES)//��������
		{
			vl53l0x_general_start(dev,mode);
	   // general_ui();
			mode=0;
		}			
		i++;
		if(i==5)
		{
			i=0;
			LED0=!LED0;
		}
		delay_ms(50);

	}
	
}
