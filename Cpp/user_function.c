/* 
* I used tim6 to generate microsecond delay for the DHT11
* delay is in microsecond
* the APB1 timer must be set at 50 MHz
* the prescaler to 49
* the ARR to the 0xfffe
*/

void delay_us(uint32_t delay) 
{
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	while (__HAL_TIM_GET_COUNTER(&htim6) < delay);
}
