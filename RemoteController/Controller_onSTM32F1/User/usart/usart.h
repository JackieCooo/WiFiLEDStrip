#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"
#include <stdio.h>

/* ���ڿ���ѡ�� */
#define USE_UART1 1
#define USE_UART2 0
#define USE_UART3 1
#define USE_UART4 0
#define USE_UART5 0

/* �����ж�ѡ�� */
#if USE_UART1
#define USE_UART1_INT 1
#endif
#if USE_UART2
#define USE_UART2_INT 0
#endif
#if USE_UART3
#define USE_UART3_INT 1
#endif
#if USE_UART4
#define USE_UART4_INT 0
#endif
#if USE_UART5
#define USE_UART5_INT 0
#endif

/* ���ڹܽŶ��� */
#if USE_UART1
// ����1-USART1
#define  USART1_CLK                RCC_APB2Periph_USART1
#define  USART1_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  USART1_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  USART1_GPIO_CLK           (RCC_APB2Periph_GPIOA)
    
#define  USART1_TX_GPIO_PORT       GPIOA   
#define  USART1_TX_GPIO_PIN        GPIO_Pin_9
#define  USART1_RX_GPIO_PORT       GPIOA
#define  USART1_RX_GPIO_PIN        GPIO_Pin_10

#endif

#if USE_USRT2

// ����2-USART2
#define  USART2_CLK                RCC_APB1Periph_USART2
#define  USART2_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  USART2_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  USART2_GPIO_CLK           (RCC_APB2Periph_GPIOA)
    
#define  USART2_TX_GPIO_PORT       GPIOA   
#define  USART2_TX_GPIO_PIN        GPIO_Pin_2
#define  USART2_RX_GPIO_PORT       GPIOA
#define  USART2_RX_GPIO_PIN        GPIO_Pin_3

#endif

#if USE_UART3

// ����3-USART3
#define  USART3_CLK                RCC_APB1Periph_USART3
#define  USART3_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  USART3_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  USART3_GPIO_CLK           (RCC_APB2Periph_GPIOB)
    
#define  USART3_TX_GPIO_PORT       GPIOB   
#define  USART3_TX_GPIO_PIN        GPIO_Pin_10
#define  USART3_RX_GPIO_PORT       GPIOB
#define  USART3_RX_GPIO_PIN        GPIO_Pin_11

#endif

#if USE_UART4

// ����4-UART4
#define  USART4_CLK                RCC_APB1Periph_UART4
#define  USART4_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  USART4_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  USART4_GPIO_CLK           (RCC_APB2Periph_GPIOC)
    
#define  USART4_TX_GPIO_PORT       GPIOC  
#define  USART4_TX_GPIO_PIN        GPIO_Pin_10
#define  USART4_RX_GPIO_PORT       GPIOC
#define  USART4_RX_GPIO_PIN        GPIO_Pin_11

#endif

#if USE_UART5

// ����5-UART5
#define  USART5_CLK                RCC_APB1Periph_UART5
#define  USART5_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  USART5_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  USART5_GPIO_CLK           (RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)
    
#define  USART5_TX_GPIO_PORT       GPIOC   
#define  USART5_TX_GPIO_PIN        GPIO_Pin_12
#define  USART5_RX_GPIO_PORT       GPIOD
#define  USART5_RX_GPIO_PIN        GPIO_Pin_2

#endif

void USART_Config(void);
void USART_SendByte(USART_TypeDef * USARTx, uint8_t ch);
void USART_SendString(USART_TypeDef * USARTx, char *str);

#endif /* __USART_H */
