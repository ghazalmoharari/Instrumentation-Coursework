/**
 * @file mk_dht11.h
 *	@brief DHT11 Library (Optimized for STM32F1)
 */

#ifndef MK_DHT11_H_
#define MK_DHT11_H_

#include "stm32f1xx.h"
#include "main.h"

#define OUTPUT 1
#define INPUT 0

struct _dht11_t{
	GPIO_TypeDef* port;
	uint16_t pin;
	TIM_HandleTypeDef *htim;
	uint8_t temperature;
	uint8_t humidty;
};
typedef struct _dht11_t dht11_t;

void init_dht11(dht11_t *dht, TIM_HandleTypeDef *htim, GPIO_TypeDef* port, uint16_t pin);
void set_dht11_gpio_mode(dht11_t *dht, uint8_t pMode);
uint8_t readDHT11(dht11_t *dht);

#endif /* MK_DHT11_H_ */
