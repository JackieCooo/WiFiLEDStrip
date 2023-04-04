#ifndef _ESP8266_H
#define _ESP8266_H

#include "gd32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdio.h>

#define BUF_MAX_LEN						32

#define ESP8266_GPIO_CLK				RCU_GPIOB
#define ESP8266_USART_GPIO_PORT			GPIOB
#define ESP8266_USART_TX_PIN			GPIO_PIN_10
#define ESP8266_USART_RX_PIN			GPIO_PIN_11
#define ESP8266_USARTx					USART2
#define ESP8266_USART_CLK				RCU_USART2
#define ESP8266_USART_AF				GPIO_AF_7
#define ESP8266_USART_DR				0x40004804

#define ESP8266_DMAx					DMA0
#define ESP8266_DMA_CHy					DMA_CH3
#define ESP8266_DMA_CLK					RCU_DMA0

typedef struct {
	uint8_t buf[BUF_MAX_LEN];
	uint16_t size;
} esp8266_buf_t;

extern xQueueHandle CommandDataQueue;

void ESP8266_Init(void);
void ESP8266_Task(void* args);
void ESP8266_SendData(uint8_t* buf, uint16_t size);

#endif
