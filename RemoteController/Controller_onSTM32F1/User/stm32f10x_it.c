/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.6.0
  * @date    20-September-2021
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2011 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "./systick/systick.h"
#include "./esp8266/esp8266.h"
#include <stdio.h>
#include "lvgl.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
	lv_tick_inc(1);
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @brief  ����3�жϴ���
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	uint8_t ucCh;
	
	if ( USART_GetITStatus ( macESP8266_USARTx, USART_IT_RXNE ) != RESET )
	{
		ucCh = USART_ReceiveData( macESP8266_USARTx );
		
		if ( strEsp8266_Fram_Record.InfBit.FramLength < ( RX_BUF_MAX_LEN - 1 ) )  // Ԥ��1���ֽ�д������
			strEsp8266_Fram_Record.Data_RX_BUF[ strEsp8266_Fram_Record.InfBit.FramLength++ ] = ucCh;
	}
	 	 
	if ( USART_GetITStatus( macESP8266_USARTx, USART_IT_IDLE ) == SET )  // ����֡�������
	{
		strEsp8266_Fram_Record.InfBit.FramFinishFlag = 1;
		ucCh = USART_ReceiveData( macESP8266_USARTx );  // �������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)
		ucTcpClosedFlag = strstr( strEsp8266_Fram_Record.Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;  // ��ȡ����״̬
	}
}

/**
  * @brief  ����1�жϴ���
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	uint8_t ucCh;
	if ( USART_GetITStatus ( USART1, USART_IT_RXNE ) != RESET )
	{
		ucCh  = USART_ReceiveData( USART1 );
		
		if ( strUSART_Fram_Record.InfBit.FramLength < ( RX_BUF_MAX_LEN - 1 ) )  // Ԥ��1���ֽ�д������
		{
			strUSART_Fram_Record.Data_RX_BUF[ strUSART_Fram_Record.InfBit.FramLength++ ]  = ucCh;
		}
	}
	if ( USART_GetITStatus( USART1, USART_IT_IDLE ) == SET )  // ����֡�������
	{
		strUSART_Fram_Record.InfBit.FramFinishFlag = 1;		
		ucCh = USART_ReceiveData( USART1 );  // �������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)	
	}	
}

/**
  * @}
  */ 


