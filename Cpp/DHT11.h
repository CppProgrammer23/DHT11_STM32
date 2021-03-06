#pragma once
#include <cinttypes>
#include <cstdio>
#include <string>

class DHT11
{
	uint8_t t1, t2, h1, h2, check;
	GPIO_TypeDef* GPIO;
	uint16_t* GPIO_Pin_Num;
	void DHT11StartSignal();
	uint8_t DHT11Response();
	uint8_t DHT11_ReadData();
	uint8_t DHT11_Init();
public:
	DHT11(GPIO_TypeDef*, uint16_t);
	float DHT11_GetHumidity();
	float DHT11_GetTemperature(); //temperature by default is in celsuis
	float DHT11_ConvertToCelsuis(float t);
	float DHT11_ConvertToFahr(float t);
};

