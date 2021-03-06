#ifndef __DHT11STM32_H
#define __DHT11STM32_H
//#include <cinttypes>

/*
* 
* How to use this driver:
* 1/call init ti inialize the sensor
* 2/to properly read the values:
*	start calling the DHT11_ReadHumidity() function
*	then the DHT11_ReadTemperature() function
*	and finally call the checkP() to check if the values are correct if the function returns 0 then it's correct otherwise it's not
* 
*/

static GPIO_TypeDef* GPIO;
static uint16_t* GPIO_Pin_Num;


void delay_us(uint32_t delay);
static uint8_t DHT11_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
static void setInput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
float DHT11_ReadHumidity(void);
float DHT11_ReadTemperature(void);
float FromCelcuisToFahr(float);
uint8_t checkP(void);

#endif
