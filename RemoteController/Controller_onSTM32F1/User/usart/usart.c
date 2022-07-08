/**
  ******************************************************************************
  * @file    usart.c
  * @author  Jackie
  * @version V1.1
  * @date    2022.07.06
  * @brief   USART�����̼�
  ******************************************************************************
  */ 

#include "./usart/usart.h"

 /**
  * @brief  USART����
  * @param  None
  * @retval None
  */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	USART_InitTypeDef USART_InitStructure = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};

#if USE_UART1
	// �򿪴���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(USART1_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	USART1_APBxClkCmd(USART1_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART1_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART1_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = USART1_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USART1, &USART_InitStructure);
	
	#if USE_UART1_INT
		
	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	/* �������ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	#endif

	// ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);
#endif

#if USE_UART2
	// �򿪴���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(USART2_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	USART2_APBxClkCmd(USART2_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USART2, &USART_InitStructure);
	
	#if USE_UART2_INT
		
	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	/* �������ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	#endif

	// ʹ�ܴ���
	USART_Cmd(USART2, ENABLE);
#endif	

#if USE_UART3
	// �򿪴���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(USART3_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	USART3_APBxClkCmd(USART3_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART3_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART3_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = USART3_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USART3, &USART_InitStructure);
	
	#if USE_UART3_INT
		
	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	/* �������ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);

	#endif

	// ʹ�ܴ���
	USART_Cmd(USART3, ENABLE);
#endif	

#if USE_UART4
	// �򿪴���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(USART4_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	USART4_APBxClkCmd(USART4_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART4_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART4_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART4_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART4_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = USART4_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(UART4, &USART_InitStructure);
	
	#if USE_UART4_INT
		
	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	/* �������ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

	#endif

	// ʹ�ܴ���
	USART_Cmd(UART4, ENABLE);
#endif	

#if USE_UART5
	// �򿪴���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(USART5_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	USART5_APBxClkCmd(USART5_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART5_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART5_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART5_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART5_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = USART5_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(UART5, &USART_InitStructure);
	
	#if USE_UART5_INT
		
	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	/* �������ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

	#endif

	// ʹ�ܴ���
	USART_Cmd(UART5, ENABLE);
#endif	

}

 /**
   * @brief  ����һ���ַ�
   * @param  pUSARTx ���ں�
   * @param  ch �ַ�
   * @retval None
   */
void USART_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx, ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

 /**
   * @brief  ����һ���ַ���
   * @param  pUSARTx ���ں�
   * @param  str �ַ���
   * @retval None
   */
void USART_SendString(USART_TypeDef *pUSARTx, char *str)
{
	uint16_t k = 0;
	do 
	{
		USART_SendByte(pUSARTx, *(str + k));
		k++;
	} while(*(str + k)!='\0');

	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

 /**
   * @brief  �ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
   * @param  ch �ַ�
   * @param  f �ļ�����
   * @retval ���ط��͵��ַ�
   */
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(USART1, (uint8_t)ch);

	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		

	return (ch);
}

 /**
   * @brief  �ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
   * @param  f �ļ�����
   * @retval ���ؽ��յ��ַ�
   */
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
}
