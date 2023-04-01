#include "esp8266/esp8266.h"

void ESP8266_Init(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(ESP8266_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(ESP8266_USART_CLK);
	
	/* Enable DMA clock */
	rcu_periph_clock_enable(ESP8266_DMA_CLK);

    /* configure the USART2 TX pin and USART2 RX pin */
    gpio_af_set(ESP8266_USART_GPIO_PORT, ESP8266_USART_AF, ESP8266_USART_TX_PIN);
    gpio_af_set(ESP8266_USART_GPIO_PORT, ESP8266_USART_AF, ESP8266_USART_RX_PIN);

    /* configure USART2 TX as alternate function push-pull */
    gpio_mode_set(ESP8266_USART_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ESP8266_USART_TX_PIN);
    gpio_output_options_set(ESP8266_USART_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ESP8266_USART_TX_PIN);

    /* configure USART2 RX as alternate function push-pull */
    gpio_mode_set(ESP8266_USART_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ESP8266_USART_RX_PIN);
    gpio_output_options_set(ESP8266_USART_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ESP8266_USART_RX_PIN);

    /* USART configure */
    usart_deinit(ESP8266_USARTx);
	usart_parity_config(ESP8266_USARTx, USART_PM_NONE);
	usart_word_length_set(ESP8266_USARTx, USART_WL_8BIT);
	usart_stop_bit_set(ESP8266_USARTx, USART_STB_1BIT);
    usart_baudrate_set(ESP8266_USARTx, 115200);
//    usart_receive_config(ESP8266_USARTx, USART_RECEIVE_ENABLE);
    usart_transmit_config(ESP8266_USARTx, USART_TRANSMIT_ENABLE);
//	usart_interrupt_enable(ESP8266_USARTx, USART_INT_RBNE);
	usart_dma_transmit_config(ESP8266_USARTx, USART_TRANSMIT_DMA_ENABLE);
    usart_enable(ESP8266_USARTx);
}

void ESP8266_Task(void* args)
{
	uint8_t cmd_buf[BUF_MAX_LEN];
	
	for(;;)
	{
		for (uint8_t i = 0; i < 8; ++i)
		{
			cmd_buf[i] = i+1;
		}
		
		ESP8266_SendData(cmd_buf, 8);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void ESP8266_SendData(uint8_t* buf, uint16_t size)
{
	#if 0
	for (uint16_t i = 0; i < size; ++i)
	{
		usart_data_transmit(ESP8266_USARTx, buf[i]);
		while(RESET == usart_flag_get(ESP8266_USARTx, USART_FLAG_TBE));
	}
	#else
	dma_single_data_parameter_struct dma_init_struct;
	
	dma_deinit(ESP8266_DMAx, ESP8266_DMA_CHy);
	
	dma_init_struct.memory0_addr = (uint32_t)buf;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.periph_addr = (uint32_t)ESP8266_USART_DR;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
	dma_init_struct.number = size;
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
	dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
	dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
	
	dma_single_data_mode_init(ESP8266_DMAx, ESP8266_DMA_CHy, &dma_init_struct);
	dma_channel_subperipheral_select(ESP8266_DMAx, ESP8266_DMA_CHy, DMA_SUBPERI4);
	dma_flag_clear(ESP8266_DMAx, ESP8266_DMA_CHy, DMA_FLAG_FTF);
	dma_channel_enable(ESP8266_DMAx, ESP8266_DMA_CHy);
	#endif
}
