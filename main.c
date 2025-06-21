#include "stm32f10x.h"                  // Device header
#include "iwdg.h"
#include "adcx.h"
#include "ldr.h"
#include "oled.h"
#include "dht11.h"
#include "led.h"
#include "key.h"
#include "tim2.h"   
#include "tim3.h"   
#include "usart3.h"   
#include "usart.h"
#include "yl_69.h"
#include "motor.h"
#include "sensormodules.h"
#include "gizwits_product.h"
#include "flash.h"


#define KEY_1	1
#define KEY_2	2
#define KEY_3	3
#define KEY_4	4

#define FLASH_START_ADDR	0x0801f000	

SensorModules sensorData;	
SensorThresholdValue Sensorthreshold;	

uint8_t menu = 1;	
uint8_t OLED_Clear_Flag;	
uint8_t mode = 0;	

enum 
{
	display_page1 = 1,
	display_page2,
	settingsPage
	
}MenuPages;


void OLED_Menu1(void)
{
	//显示系统名
	OLED_ShowChinese(1, 1, 0);
	OLED_ShowChinese(1, 2, 1);
	OLED_ShowChinese(1, 3, 2);
	OLED_ShowChinese(1, 4, 3);
	OLED_ShowChinese(1, 5, 4);
	OLED_ShowChinese(1, 6, 5);
	OLED_ShowChinese(1, 7, 6);
	OLED_ShowChinese(1, 8, 7);
	
	//显示“系统模式：”
	OLED_ShowChinese(2, 1, 6);
	OLED_ShowChinese(2, 2, 7);
	OLED_ShowChinese(2, 3, 19);
	OLED_ShowChinese(2, 4, 20);	
	OLED_ShowChar(2, 9, ':');
	
	//显示”光照强度：  Lux“
	OLED_ShowChinese(3, 1, 15);
	OLED_ShowChinese(3, 2, 16);	
	OLED_ShowChinese(3, 3, 28);
	OLED_ShowChinese(3, 4, 29);	
	OLED_ShowChar(3, 9, ':');
	OLED_ShowString(3, 14, "Lux");
	
	//显示“二氧化碳： ”
	OLED_ShowChinese(4, 1, 24);
	OLED_ShowChinese(4, 2, 25);
	OLED_ShowChinese(4, 3, 26);
	OLED_ShowChinese(4, 4, 27);
	OLED_ShowChar(4, 9, ':');
	OLED_ShowString(4, 14, "ppm");
}

/**
  * @brief  显示菜单2的固定内容
  * @param  无
  * @retval 无
  */
void OLED_Menu2(void)
{
	//显示“环境温度：  C”
	OLED_ShowChinese(1, 1, 13);
	OLED_ShowChinese(1, 2, 14);
	OLED_ShowChinese(1, 3, 8);
	OLED_ShowChinese(1, 4, 9);	
	OLED_ShowChar(1, 9, ':');
	OLED_ShowChar(1, 12, 'C');
	
	//显示“环境湿度：  %”
	OLED_ShowChinese(2, 1, 13);
	OLED_ShowChinese(2, 2, 14);
	OLED_ShowChinese(2, 3, 10);
	OLED_ShowChinese(2, 4, 9);	
	OLED_ShowChar(2, 9, ':');
	OLED_ShowChar(2, 12, '%');		
	
	//显示“土壤温度：  C”
	OLED_ShowChinese(3, 1, 11);
	OLED_ShowChinese(3, 2, 12);
	OLED_ShowChinese(3, 3, 8);
	OLED_ShowChinese(3, 4, 9);	
	OLED_ShowChar(3, 9, ':');
	OLED_ShowChar(3, 12, 'C');		
	
	//显示“土壤湿度：  %”
	OLED_ShowChinese(4, 1, 11);
	OLED_ShowChinese(4, 2, 12);
	OLED_ShowChinese(4, 3, 10);
	OLED_ShowChinese(4, 4, 9);	
	OLED_ShowChar(4, 9, ':');
	OLED_ShowChar(4, 12, '%');	
}

/**
  * @brief  显示菜单1的传感器数据
  * @param  无
  * @retval 无
  */
void SensorDataDisplay1(void)
{
	//显示系统状态数据
	if (!mode)
	{
		OLED_ShowChinese(2, 6, 21);
		OLED_ShowChinese(2, 7, 22);		
	}
	else
	{
		OLED_ShowChinese(2, 6, 23);
		OLED_ShowChinese(2, 7, 22);			
	}
	
	//显示光照强度数据
	OLED_ShowNum(3, 10, sensorData.lux, 4);
	
	//显示CO2浓度数据
	OLED_ShowNum(4, 10, sensorData.CO2, 4);
}

/**
  * @brief  显示菜单2的传感器数据
  * @param  无
  * @retval 无
  */
void SensorDataDisplay2(void)
{
	//显示环境温度数据
	OLED_ShowNum(1, 10, sensorData.temp, 2);
	
	//显示环境湿度数据
	OLED_ShowNum(2, 10, sensorData.humi, 2);
	
	//显示土壤温度数据
	OLED_ShowNum(3, 10, sensorData.soilTemp, 2);
	
	//显示土壤湿度数据
	OLED_ShowNum(4, 10, sensorData.soilHumi, 2);
}

/**
  * @brief  显示阈值设置界面1的固定内容
  * @param  无
  * @retval 无
  */
void OLED_settingsPage1(void)
{
	//显示“环境温度：”
	OLED_ShowChinese(1, 2, 13);
	OLED_ShowChinese(1, 3, 14);
	OLED_ShowChinese(1, 4, 8);
	OLED_ShowChinese(1, 5, 9);	
	OLED_ShowChar(1, 11, ':');

	//显示“环境湿度：”
	OLED_ShowChinese(2, 2, 13);
	OLED_ShowChinese(2, 3, 14);
	OLED_ShowChinese(2, 4, 10);
	OLED_ShowChinese(2, 5, 9);	
	OLED_ShowChar(2, 11, ':');
	
	
	//显示“土壤温度：”
	OLED_ShowChinese(3, 2, 11);
	OLED_ShowChinese(3, 3, 12);
	OLED_ShowChinese(3, 4, 8);
	OLED_ShowChinese(3, 5, 9);	
	OLED_ShowChar(3, 11, ':');
	
	
	//显示“土壤湿度：”
	OLED_ShowChinese(4, 2, 11);
	OLED_ShowChinese(4, 3, 12);
	OLED_ShowChinese(4, 4, 10);
	OLED_ShowChinese(4, 5, 9);	
	OLED_ShowChar(4, 11, ':');
}

/**
  * @brief  显示阈值设置界面2的固定内容
  * @param  无
  * @retval 无
  */
void OLED_settingsPage2(void)
{
	//显示”光照强度：“
	OLED_ShowChinese(1, 2, 15);
	OLED_ShowChinese(1, 3, 16);	
	OLED_ShowChinese(1, 4, 28);
	OLED_ShowChinese(1, 5, 29);	
	OLED_ShowChar(1, 11, ':');
	
	//显示“二氧化碳：”
	OLED_ShowChinese(2, 2, 24);
	OLED_ShowChinese(2, 3, 25);
	OLED_ShowChinese(2, 4, 26);
	OLED_ShowChinese(2, 5, 27);
	OLED_ShowChar(2, 11, ':');	
}

/**
  * @brief  显示阈值界面1的传感器数据
  * @param  无
  * @retval 无
  */
void settingsThresholdDisplay1(void)
{
	//显示环境温度阈值数据
	OLED_ShowNum(1, 13, Sensorthreshold.tempValue, 2);
	
	//显示环境湿度阈值数据
	OLED_ShowNum(2, 13, Sensorthreshold.humiValue, 2);
	
	//显示土壤温度阈值数据
	OLED_ShowNum(3, 13, Sensorthreshold.soilTempValue, 2);
	
	//显示土壤湿度阈值数据
	OLED_ShowNum(4, 13, Sensorthreshold.soilHumiValue, 2);
}

/**
  * @brief  显示阈值界面2的传感器数据
  * @param  无
  * @retval 无
  */
void settingsThresholdDisplay2(void)
{
	//显示光照强度阈值数据
	OLED_ShowNum(1, 13, Sensorthreshold.luxValue, 4);
	
	//显示CO2浓度阈值数据
	OLED_ShowNum(2, 13, Sensorthreshold.CO2Value, 4);
}

/**
  * @brief  显示阈值界面的选择符号
  * @param  num 为显示的位置
  * @retval 无
  */
void OLED_Option(uint8_t num)
{
	switch(num)
	{
		case 1:	
			OLED_ShowChar(1,1,'>');
			OLED_ShowChar(2,1,' ');
			OLED_ShowChar(3,1,' ');
			OLED_ShowChar(4,1,' ');
			break;
		case 2:	
			OLED_ShowChar(1,1,' ');
			OLED_ShowChar(2,1,'>');
			OLED_ShowChar(3,1,' ');
			OLED_ShowChar(4,1,' ');
			break;
		case 3:	
			OLED_ShowChar(1,1,' ');
			OLED_ShowChar(2,1,' ');
			OLED_ShowChar(3,1,'>');
			OLED_ShowChar(4,1,' ');
			break;
		case 4:	
			OLED_ShowChar(1,1,' ');
			OLED_ShowChar(2,1,' ');
			OLED_ShowChar(3,1,' ');
			OLED_ShowChar(4,1,'>');
			break;
		case 5:	
			OLED_ShowChar(1,1,'>');
			OLED_ShowChar(2,1,' ');
			OLED_ShowChar(3,1,' ');
			OLED_ShowChar(4,1,' ');
			break;
		case 6:	
			OLED_ShowChar(1,1,' ');
			OLED_ShowChar(2,1,'>');
			OLED_ShowChar(3,1,' ');
			OLED_ShowChar(4,1,' ');
			break;
		default: break;
	}
}

/**
  * @brief  根据标志位控制步进电机的运行
  * @param  无
  * @retval 无
  */
void MotorOperation(void)
{
	if (motorFlag == 1)
	{
		MOTOR_Direction_Angle(1, 0, 90, 1);
		MOTOR_STOP();
		motorFlag = 0;
	}
	else if (motorFlag == 2)
	{
		MOTOR_Direction_Angle(0, 0, 90, 1);
		MOTOR_STOP();
		motorFlag = 0;		
	}

}

/**
  * @brief  记录阈值界面下按KEY1的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetSelection(void)
{
	static uint8_t count = 1;
	if(KeyNum == KEY_1)
	{
		KeyNum = 0;
		count++;
		if (count == 5)
		{
			OLED_Clear();
		}
		else if (count > 6)
		{
			OLED_Clear();
			count = 1;
		}
	}
	return count;
}


void ThresholdModification(uint8_t num)
{
	switch (num)
	{
		case 1:
			if (KeyNum == 3)
			{
				KeyNum = 0;
				Sensorthreshold.tempValue++;
				if (Sensorthreshold.tempValue > 99)
				{
					Sensorthreshold.tempValue = 1;
				}
			}
			else if (KeyNum == 4)
			{
				KeyNum = 0;
				Sensorthreshold.tempValue--;
				if (Sensorthreshold.tempValue < 1)
				{
					Sensorthreshold.tempValue = 99;
				}				
			}
			break;
		case 2:
			if (KeyNum == 3)
			{
				KeyNum = 0;
				Sensorthreshold.humiValue++;
				if (Sensorthreshold.humiValue > 99)
				{
					Sensorthreshold.humiValue = 1;
				}
			}
			else if (KeyNum == 4)
			{
				KeyNum = 0;
				Sensorthreshold.humiValue--;
				if (Sensorthreshold.humiValue < 1)
				{
					Sensorthreshold.humiValue = 99;
				}				
			}			
			break;
		case 3:
			if (KeyNum == 3)
			{
				KeyNum = 0;
				Sensorthreshold.soilTempValue++;
				if (Sensorthreshold.soilTempValue > 99)
				{
					Sensorthreshold.soilTempValue = 1;
				}
			}
			else if (KeyNum == 4)
			{
				KeyNum = 0;
				Sensorthreshold.soilTempValue--;
				if (Sensorthreshold.soilTempValue < 1)
				{
					Sensorthreshold.soilTempValue = 99;
				}				
			}
			break;
		case 4:
			if (KeyNum == 3)
			{
				KeyNum = 0;
				Sensorthreshold.soilHumiValue++;
				if (Sensorthreshold.soilHumiValue > 99)
				{
					Sensorthreshold.soilHumiValue = 1;
				}
			}
			else if (KeyNum == 4)
			{
				KeyNum = 0;
				Sensorthreshold.soilHumiValue--;
				if (Sensorthreshold.soilHumiValue < 1)
				{
					Sensorthreshold.soilHumiValue = 99;
				}				
			}
			break;
		case 5:
			if (KeyNum == 3)
			{
				KeyNum = 0;
				Sensorthreshold.luxValue += 10;
				if (Sensorthreshold.luxValue > 2000)
				{
					Sensorthreshold.luxValue = 0;
				}
			}
			else if (KeyNum == 4)
			{
				KeyNum = 0;
				Sensorthreshold.luxValue -= 10;
				if (Sensorthreshold.luxValue > 2000)
				{
					Sensorthreshold.luxValue = 2000;
				}				
			}			
			break;
		case 6:
			if (KeyNum == 3)
			{
				KeyNum = 0;
				Sensorthreshold.CO2Value += 10;
				if (Sensorthreshold.CO2Value > 2000)
				{
					Sensorthreshold.CO2Value = 0;
				}
			}
			else if (KeyNum == 4)
			{
				KeyNum = 0;
				Sensorthreshold.CO2Value -= 10;
				if (Sensorthreshold.CO2Value > 2000)
				{
					Sensorthreshold.CO2Value = 2000;
				}				
			}
			break;	
		default: break;		
	}
}

/**
  * @brief  获取二氧化碳数据
  * @param  *data 为数据传参
  * @retval 无
  */
void CO2GetData(uint16_t *data)
{
	if (Usart3_RxFlag == 1)
	{
		Usart3_RxFlag = 0;
		*data = Usart3_RxPacket[1] * 256 + Usart3_RxPacket[2];
	}
}

/**
  * @brief  传感器数据扫描
  * @param  无
  * @retval 无
  */
void SensorScan(void)
{
	DHT11_Read_Data(&sensorData.humi, &sensorData.temp);
	YL69_PercentageData(&sensorData.soilHumi);
	LDR_LuxData(&sensorData.lux);
	CO2GetData(&sensorData.CO2);
	DS18B20_Read_Temp(&sensorData.soilTemp);
}



int main(void)
{
	ADCX_Init();
	Timer2_Init(9,14398);
	Uart2_Init(9600);
	Uart1_Init(115200);
	Uart3_Init();
	IWDG_Init();	//初始化看门狗
	
	LDR_Init();
	YL69_Init();
	OLED_Init();
	DHT11_Init();
	LED_Init();
	Buzzer_Init();
	Relay_Init();
	MOTOR_Init();
	Key_Init();
	
	Sensorthreshold.CO2Value = FLASH_R(FLASH_START_ADDR);	//从指定页的地址读FLASH
	Sensorthreshold.luxValue = FLASH_R(FLASH_START_ADDR+2);	//从指定页的地址读FLASH
	Sensorthreshold.tempValue = FLASH_R(FLASH_START_ADDR+4);	//从指定页的地址读FLASH
	Sensorthreshold.humiValue = FLASH_R(FLASH_START_ADDR+6);	//从指定页的地址读FLASH
	Sensorthreshold.soilTempValue = FLASH_R(FLASH_START_ADDR+8);	//从指定页的地址读FLASH
	Sensorthreshold.soilHumiValue = FLASH_R(FLASH_START_ADDR+10);	//从指定页的地址读FLASH

	GENERAL_TIM_Init();
	userInit();		
	gizwitsInit();	
	
	while (1)
	{
		IWDG_ReloadCounter(); 
		SensorScan();	
		switch (menu)
		{
			case display_page1:
				SensorDataDisplay1();	//显示传感器1数据
				OLED_Menu1();	//显示主页面1固定信息
			
				if (KeyNum == KEY_2)	//是否按下按键2
				{
					KeyNum = 0;
					OLED_Clear();	//清屏
					menu = display_page2;	//menu = 主页面2
				}
				MotorOperation();
				break;
				
			case display_page2:		
				SensorDataDisplay2();	//显示传感器2数据		
				OLED_Menu2();	//显示主页面2固定信息
				if (KeyNum == KEY_2)	//是否按下按键2
				{
					KeyNum = 0;
					OLED_Clear();	//清屏
					menu = display_page1;	//menu = 主页面1
				}
				MotorOperation();
				break;
				
			case settingsPage:
				//从主页面跳转至设置页面时进行一次清屏
				if (OLED_Clear_Flag)
				{
					OLED_Clear_Flag = 0;	//清除清屏标志位
					OLED_Clear();	//清屏
				}
				ThresholdModification(SetSelection());	//调节传感器阈值
				
				OLED_Option(SetSelection());	//获取按键次数，从而判断“>”显示位置
			
				//按键次数小于等于4时，显示设置页面1
				if (SetSelection() <= 4)		
				{
					settingsThresholdDisplay1();	//显示传感器阈值1数据	
					OLED_settingsPage1();	//显示阈值设置界面1固定信息

				}
				else	//否则显示设置页面2
				{
					settingsThresholdDisplay2();	//显示传感器阈值2数据	
					OLED_settingsPage2();	//显示阈值设置界面2固定信息
				}
	
				if (KeyNum == KEY_2)	//判断用户是否按下退出按键
				{
					KeyNum = 0;
					OLED_Clear();	//清屏
					menu = display_page1;	
					

				
					FLASH_W(FLASH_START_ADDR, Sensorthreshold.CO2Value, Sensorthreshold.luxValue,
							Sensorthreshold.tempValue, Sensorthreshold.humiValue,
							Sensorthreshold.soilTempValue, Sensorthreshold.soilHumiValue);
				}
				break;
			default: break;
		}
		userHandle();	
		gizwitsHandle((dataPoint_t *)&currentDataPoint);					
	}
}
