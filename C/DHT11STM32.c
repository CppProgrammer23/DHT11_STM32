#include "DHT11STM32.h"

uint8_t checkParity;

/* 
* I used tim6 to generate microsecond delay for the DHT11
* delay is in microsecond
* the APB1 timer must be set at 50 MHz
* the prescaler to 49
* the ARR to the 0xfffe
* 
* feel free to change the timer instance if you're nit using the tim6
*/
void delay_us(uint32_t delay) 
{
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	while (__HAL_TIM_GET_COUNTER(&htim6) < delay);
}
void setInput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
static void DHT11StartSignal(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) //it's somehow private (only visible in this file)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 0);
	HAL_Delay(18);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 1);
	delay_us(20);
	setInput(GPIOx, GPIO_Pin);
}
static uint8_t DHT11Response(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	uint8_t res = 0;
	/*get response*/
	delay_us(40);
	if (!HAL_GPIO_ReadPin(GPIOx, GPIO_Pin))
	{
		delay_us(80);
		if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin))
			res = 1;
		else
			res = -1;
	}
	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)); //wait to pin goes low
	return res;
}

uint8_t DHT11_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO = GPIOx;
	GPIO_Pin_Num = &GPIO_Pin;
	DHT11StartSignal(GPIOx, GPIO_Pin);
	return DHT11Response(GPIOx, GPIO_Pin);
}
uint8_t readDataDHT11(void)
{
	unsigned j;
	for (unsigned i = 0; i < 8; i++)
	{
		while (!HAL_GPIO_ReadPin(GPIO, GPIO_Pin_Num));
		delay_us(40);
		if (!HAL_GPIO_ReadPin(GPIO, GPIO_Pin_Num))
			j &= ~(1 << (7 - i));
		else
			j |= (1 << (7 - i));
		while (HAL_GPIO_ReadPin(GPIO, GPIO_Pin_Num));
	}
	return j;

}
float DHT11_ReadHumidity(void)
{
	char hum[5];
	uint8_t h1 = readDataDHT11();
	uint8_t h2 = readDataDHT11();
	checkParity += (h1 + h2);
	sprintf(hum, "%i.%i", h1, h2);
	float y = atoff(hum);
	return y;
}
float DHT11_ReadTemperature(void)
{
	char temp[5];
	uint8_t t1 = readDataDHT11();
	uint8_t t2 = readDataDHT11();
	checkParity += (t1 + t2);
	sprintf(temp, "%.2i.%i", t1, t2);
	return atoff(temp);
}
uint8_t checkP(void)
{
	uint8_t p = readDataDHT11();
	if (checkParity != p)
		return -1;
	else
		return 0;
}
float FromCelcuisToFahr(float t)
{
	return t * 1.8 + 32;
}
float FromFahrToCelsuis(float t)
{
	return (t - 32) / 1.8;
}