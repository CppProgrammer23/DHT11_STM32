#include "DHT11.h"

extern "C" {
	GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	void delay_us(uint32_t delay);
	void setInput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
}
void DHT11::DHT11StartSignal()
{
	HAL_GPIO_WritePin(GPIO, GPIO_Pin_Num, 0);
	HAL_Delay(18);
	HAL_GPIO_WritePin(GPIO, GPIO_Pin_Num, 1);
	delay_us(20);
	setInput(GPIO, GPIO_Pin_Num);
}

uint8_t DHT11::DHT11Response()
{
	uint8_t res = 0;
	/*get response*/
	delay_us(40);
	if (!HAL_GPIO_ReadPin(GPIO, GPIO_Pin_Num));
	{
		delay_us(80);
		if (HAL_GPIO_ReadPin(GPIO, GPIO_Pin_Num))
			res = 1;
		else
			res = -1;
	}
	while (HAL_GPIO_ReadPin(GPIO, GPIO_Pin_Num)); //wait to pin goes low
	return res;
}

uint8_t DHT11::DHT11_ReadData()
{
	unsigned j;
	for (unsigned i = 0; i < 8; i++)
	{
		while (!HAL_GPIO_ReadPin(GPIO, *GPIO_Pin_Num));
		delay_us(40);
		if (!HAL_GPIO_ReadPin(GPIO, *GPIO_Pin_Num))
			j &= ~(1 << (7 - i));
		else
			j |= (1 << (7 - i));
		while (HAL_GPIO_ReadPin(GPIO, *GPIO_Pin_Num));
	}
	return j;
}

uint8_t DHT11::DHT11_Init()
{
	DHT11StartSignal();
	return DHT11Response();
}

DHT11::DHT11(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
	:h1(0),h2(0),t1(0),t2(0),check(0),GPIO(GPIOx),GPIO_Pin_Num(&GPIO_Pin)
{
	DHT11_Init();
	h1 = DHT11_ReadData();
	h2 = DHT11_ReadData();
	t1 = DHT11_ReadData();
	t2 = DHT11_ReadData();
}

float DHT11::DHT11_GetHumidity()
{
	char hum[5];
	sprintf(hum, "%.2i.%.2i", h1, h2);
	return atof(hum);
}

float DHT11::DHT11_GetTemperature()
{
	char temp[5];
	sprintf(temp, "%.2i.%.2i", t1, t2);
	return atof(temp);
}

float DHT11::DHT11_ConvertToCelsuis(float t)
{
	return (t-32) /1.8;
}

float DHT11::DHT11_ConvertToFahr(float t)
{
	return t*1.8 + 32;
}
