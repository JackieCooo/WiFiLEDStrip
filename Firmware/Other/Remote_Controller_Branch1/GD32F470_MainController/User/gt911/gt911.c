/**
  ******************************************************************************
  * @file    gt911.c 
  * @author  Jackie
  * @version V1.0
  * @date    2023-2-13
  * @brief   GT911 touch IC firmware
  ******************************************************************************
  */

#include "gt911/gt911.h"

const static uint16_t GT911_REG_TP[5] = {
	GT911_REG_TP1,
	GT911_REG_TP2,
	GT911_REG_TP3,
	GT911_REG_TP4,
	GT911_REG_TP5
};

static __inline void GT911_Delay(void);
static void GT911_GPIOInit(void);
static void GT911_I2CStart(void);
static void GT911_I2CStop(void);
static uint8_t GT911_I2CWaitAck(void);
static void GT911_I2CAck(void);
static void GT911_I2CNAck(void);
static void GT911_I2CWriteByte(uint8_t byte);
static uint8_t GT911_I2CReadByte(void);
static uint8_t GT911_WriteReg(uint16_t reg, uint8_t* buf, uint16_t num);
static uint8_t GT911_ReadReg(uint16_t reg, uint8_t* buf, uint16_t num);
static void GT911_TranslateTouchPoint(gt911_point_t* point, uint8_t* buf);

static __inline void GT911_Delay(void)
{
	for(uint8_t i = 0; i < 120; ++i);
}

/**
  * @brief  Init GPIO
  * @param  None
  * @retval None
  */
static void GT911_GPIOInit(void)
{
	/* config GPIO clock */
	rcu_periph_clock_enable(GT911_I2C_SCL_GPIO_CLK);
	rcu_periph_clock_enable(GT911_I2C_SDA_GPIO_CLK);
	rcu_periph_clock_enable(GT911_RST_GPIO_CLK);
	rcu_periph_clock_enable(GT911_INT_GPIO_CLK);

	/* SCL */
	gpio_mode_set(GT911_I2C_SCL_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GT911_I2C_SCL_PIN);
	gpio_output_options_set(GT911_I2C_SCL_GPIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GT911_I2C_SCL_PIN);

	/* SDA */
	gpio_mode_set(GT911_I2C_SDA_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GT911_I2C_SDA_PIN);
	gpio_output_options_set(GT911_I2C_SDA_GPIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GT911_I2C_SDA_PIN);

	/* RST */
	gpio_mode_set(GT911_RST_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GT911_RST_GPIO_PIN);
	gpio_output_options_set(GT911_RST_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GT911_RST_GPIO_PIN);

	/* INT */
	gpio_mode_set(GT911_INT_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GT911_INT_GPIO_PIN);
	gpio_output_options_set(GT911_INT_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GT911_INT_GPIO_PIN);
}

/**
  * @brief  Read from register
  * @param  reg: register address
  * @param  buf: receive buffer
  * @param  num: data length
  * @retval 0: succeeded, 1: failed
  */
static uint8_t GT911_ReadReg(uint16_t reg, uint8_t* buf, uint16_t num)
{
    /* 1. Send START signal */
    GT911_I2CStart();
    
    /* 2. Send address */
    GT911_I2CWriteByte(GT911_WR_ADDR);  // Write cmd
    
    /* 3. Wait for ACK */
    GT911_WAIT_ACK();
	
	/* 4. Write register address */
	GT911_I2CWriteByte((uint8_t)(reg >> 8) & 0xFF);
	GT911_WAIT_ACK();
	GT911_I2CWriteByte((uint8_t)reg & 0xFF);
	GT911_WAIT_ACK();
	
	/* 5. Send STOP signal */
	GT911_I2CStop();
	
	/* 6. Send START signal */
	GT911_I2CStart();
	
	/* 7. Send address */
	GT911_I2CWriteByte(GT911_RD_ADDR);  // Read cmd
	
    /* 8. Wait for ACK */
	GT911_WAIT_ACK();

	/* 9. Read data */
	for(uint16_t i = 0; i < num; ++i)
	{
		buf[i] = GT911_I2CReadByte();
		if (i == num - 1) GT911_I2CNAck();
		else GT911_I2CAck();
	}
	
	/* 10. Send STOP signal */
	GT911_I2CStop();
	
	return GT911_OK;

cmd_fail:
	GT911_I2CStop();
	return GT911_FAIL;
}

/**
  * @brief  Write to register
  * @param  reg: register address
  * @param  buf: send buffer
  * @param  num: data length
  * @retval 0: succeeded, 1: failed
  */
static uint8_t GT911_WriteReg(uint16_t reg, uint8_t* buf, uint16_t num)
{
    /* 1. Send START signal */
    GT911_I2CStart();
    
    /* 2. Send address */
    GT911_I2CWriteByte(GT911_WR_ADDR);  // Write cmd
    
    /* 3. Wait for ACK */
	GT911_WAIT_ACK();
	
	/* 4. Write register address */
	GT911_I2CWriteByte((uint8_t)(reg >> 8) & 0xFF);
	GT911_WAIT_ACK();
	GT911_I2CWriteByte((uint8_t)reg & 0xFF);
	GT911_WAIT_ACK();

	/* 5. Write data */
	for(uint16_t i = 0; i < num; ++i)
	{
		GT911_I2CWriteByte(buf[i]);
		GT911_WAIT_ACK();
	}
	
	/* 6. Send STOP signal */
	GT911_I2CStop();
	
	return GT911_OK;
	
cmd_fail:
	GT911_I2CStop();
	return GT911_FAIL;
}

/**
  * @brief  I2C START signal
  * @param  None
  * @retval None
  */
static void GT911_I2CStart(void)
{
	GT911_SDA_HIGH();
	GT911_SCL_HIGH();
	GT911_Delay();
	GT911_SDA_LOW();
	GT911_Delay();
	GT911_SCL_LOW();
	GT911_Delay();
}

/**
  * @brief  I2C STOP signal
  * @param  None
  * @retval None
  */
static void GT911_I2CStop(void)
{
	GT911_SDA_LOW();
	GT911_SCL_HIGH();
	GT911_Delay();
	GT911_SDA_HIGH();
}

/**
  * @brief  I2C wait for ACK signal
  * @param  None
  * @retval 0: acked, 1: no respond
  */
static uint8_t GT911_I2CWaitAck(void)
{
	uint8_t re;

	GT911_SDA_HIGH();
	GT911_Delay();
	GT911_SCL_HIGH();
	GT911_Delay();
	if (GT911_SDA_READ())
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	GT911_SCL_LOW();
	GT911_Delay();
	return re;
}

/**
  * @brief  I2C ACK signal
  * @param  None
  * @retval None
  */
static void GT911_I2CAck(void)
{
	GT911_SDA_LOW();
	GT911_Delay();
	GT911_SCL_HIGH();
	GT911_Delay();
	GT911_SCL_LOW();
	GT911_Delay();
	GT911_SDA_HIGH();
}

/**
  * @brief  I2C NACK signal
  * @param  None
  * @retval None
  */
static void GT911_I2CNAck(void)
{
	GT911_SDA_HIGH();
	GT911_Delay();
	GT911_SCL_HIGH();
	GT911_Delay();
	GT911_SCL_LOW();
	GT911_Delay();	
}

/**
  * @brief  I2C write a byte
  * @param  byte: data byte
  * @retval None
  */
static void GT911_I2CWriteByte(uint8_t byte)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{		
		if (byte & 0x80)
		{
			GT911_SDA_HIGH();
		}
		else
		{
			GT911_SDA_LOW();
		}
		GT911_Delay();
		GT911_SCL_HIGH();
		GT911_Delay();	
		GT911_SCL_LOW();
		if (i == 7)
		{
			 GT911_SDA_HIGH();
		}
		byte <<= 1;
		GT911_Delay();
	}
}

/**
  * @brief  I2C read a byte
  * @param  None
  * @retval return received data byte
  */
static uint8_t GT911_I2CReadByte(void)
{
	uint8_t i;
	uint8_t value;

	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		GT911_SCL_HIGH();
		GT911_Delay();
		if (GT911_SDA_READ())
		{
			value++;
		}
		GT911_SCL_LOW();
		GT911_Delay();
	}
	return value;
}

/**
  * @brief  Convert raw data to touch point data
  * @param  point: touch point data struct
  * @param  buf: raw data buffer
  * @retval None
  */
static void GT911_TranslateTouchPoint(gt911_point_t* point, uint8_t* buf)
{
	point->size = ((uint16_t)buf[5] << 8) | buf[4];
	point->x = ((uint16_t)buf[1] << 8) | buf[0];
	point->y = ((uint16_t)buf[3] << 8) | buf[2];
}

/**
  * @brief  Init GT911
  * @param  None
  * @retval None
  */
void GT911_Init(void)
{
	GT911_GPIOInit();
	GT911_Rst();
	GT911_Detect();
}

/**
  * @brief  Reset GT911
  * @param  None
  * @retval None
  */
void GT911_Rst(void)
{
	/* Set INT pin to low when initializing in order to get the correct address -- 0xBA */
	gpio_mode_set(GT911_INT_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GT911_INT_GPIO_PIN);
	gpio_output_options_set(GT911_INT_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GT911_INT_GPIO_PIN);

	/* Reset */
	gpio_bit_reset(GT911_RST_GPIO_PORT, GT911_RST_GPIO_PIN);
	vTaskDelay(10 / portTICK_PERIOD_MS);
	gpio_bit_set(GT911_RST_GPIO_PORT, GT911_RST_GPIO_PIN);
	vTaskDelay(10 / portTICK_PERIOD_MS);

	/* Set INT pin floating in order to receive interrupt signal */
	gpio_mode_set(GT911_INT_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GT911_INT_GPIO_PIN);
}

/**
  * @brief  Detect GT911
  * @param  None
  * @retval None
  */
void GT911_Detect(void)
{
	uint8_t buf[4];
	uint8_t ret;
	
	do
	{
		ret = GT911_ReadReg(GT911_REG_PID, buf, sizeof(buf));
		if (ret == GT911_FAIL)
		{
			printf("Touch IC detect failed\n");
			vTaskDelay(500 / portTICK_PERIOD_MS);
		}
	} while (ret == GT911_FAIL);
	printf("PID: %c%c%c%c\n", buf[0], buf[1], buf[2], buf[3]);
}

/**
  * @brief  Scan for touch points
  * @param  points: touch point data struct buffer
  * @param  cnt: touch point count
  * @retval return how many touch points GT911 detected
  */
uint8_t GT911_TouchScan(gt911_point_t* points, uint8_t cnt)
{
	if (cnt == 0 || cnt > 5) return 0;
	
	uint8_t status;
	
	GT911_ReadReg(GT911_REG_TPINFO, &status, sizeof(status));
	if (GT911_IS_BUF_READY(status))  // Data is ready
	{
		uint8_t point_cnt = GT911_TOUCH_POINT_CNT(status);
		
		for(uint8_t i = 0; i < GT911_Min(cnt, point_cnt); ++i)
		{
			uint8_t buf[6];  // x: 2byte, y: 2byte, size: 2byte
			GT911_ReadReg(GT911_REG_TP[i], buf, sizeof(buf));
			GT911_TranslateTouchPoint(&points[i], buf);
		}
		
		status = 0;
		GT911_WriteReg(GT911_REG_TPINFO, &status, sizeof(status));
		
		return point_cnt;
	}
	else
	{
		return 0;
	}
}
