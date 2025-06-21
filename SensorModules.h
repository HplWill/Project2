#ifndef	__SENSORMODULES_H_
#define __SENSORMODULES_H_

#include "stm32f10x.h"                  // Device header


typedef struct
{
	uint8_t humi;
	uint8_t temp;
	uint8_t soilHumi;
	float soilTemp;
	uint16_t lux;	
	uint16_t CO2;	
	
}SensorModules;

typedef struct
{
	uint8_t humiValue;
	uint8_t tempValue;
	uint8_t soilHumiValue;
	uint8_t soilTempValue;
	uint16_t luxValue;	
	uint16_t CO2Value;	
	
}SensorThresholdValue;

extern SensorModules sensorData;			
extern SensorThresholdValue Sensorthreshold;	

#endif
