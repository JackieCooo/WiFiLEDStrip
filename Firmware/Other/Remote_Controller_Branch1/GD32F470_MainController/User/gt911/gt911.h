#ifndef _GT911_H
#define _GT911_H

#include "gd32f4xx.h"
#include "st7796/st7796.h"

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

//#define GT911_Delay()						delay_us(4)
#define GT911_Min(x, y)						(x > y ? y : x)
#define GT911_TOUCH_POINT_CNT(x)			(x & 0x0F)
#define GT911_IS_BUF_READY(x)				((x & 0x80) == 0x80)
#define GT911_LCD_SCAN_DIR					LCD_SCAN_DIR
#define GT911_LCD_HOR_RES					LCD_HOR_RES
#define GT911_LCD_VER_RES					LCD_VER_RES
#define GT911_WAIT_ACK()					if (GT911_I2CWaitAck() != 0) goto cmd_fail;

#define GT911_OK							0
#define GT911_FAIL							1

#define GT911_WR_ADDR						(0xBA)
#define GT911_RD_ADDR						(0xBB)

#define GT911_REG_CTRL						0x8040  /* Controll */
#define GT911_REG_PID            			0x8140  /* PID */
#define GT911_REG_TPINFO         			0x814E  /* Touch status */
#define GT911_REG_TP1            			0x8150  /* Touch point 1 */
#define GT911_REG_TP2            			0x8158  /* Touch point 2 */
#define GT911_REG_TP3            			0x8160  /* Touch point 3 */
#define GT911_REG_TP4            			0x8168  /* Touch point 4 */
#define GT911_REG_TP5            			0x8170  /* Touch point 5 */

#define GT911_I2C_SCL_PIN                   GPIO_PIN_6
#define GT911_I2C_SCL_GPIO_PORT             GPIOB
#define GT911_I2C_SCL_GPIO_CLK              RCU_GPIOB

#define GT911_I2C_SDA_PIN                   GPIO_PIN_7
#define GT911_I2C_SDA_GPIO_PORT             GPIOB
#define GT911_I2C_SDA_GPIO_CLK              RCU_GPIOB

#define GT911_RST_GPIO_PORT                 GPIOD
#define GT911_RST_GPIO_CLK                  RCU_GPIOD
#define GT911_RST_GPIO_PIN                  GPIO_PIN_6

#define GT911_INT_GPIO_PORT                 GPIOG
#define GT911_INT_GPIO_CLK                  RCU_GPIOG
#define GT911_INT_GPIO_PIN                  GPIO_PIN_9
#define GT911_INT_EXTI_PORTSOURCE           EXTI_PortSourceGPIOG
#define GT911_INT_EXTI_PINSOURCE            EXTI_PinSource9
#define GT911_INT_EXTI_LINE                 EXTI_Line9
#define GT911_INT_EXTI_IRQ                  EXTI9_5_IRQn

#define GT911_SCL_HIGH()					gpio_bit_set(GT911_I2C_SCL_GPIO_PORT, GT911_I2C_SCL_PIN)
#define GT911_SCL_LOW()						gpio_bit_reset(GT911_I2C_SCL_GPIO_PORT, GT911_I2C_SCL_PIN)
#define GT911_SDA_HIGH()					gpio_bit_set(GT911_I2C_SDA_GPIO_PORT, GT911_I2C_SDA_PIN)
#define GT911_SDA_LOW()						gpio_bit_reset(GT911_I2C_SDA_GPIO_PORT, GT911_I2C_SDA_PIN)
#define GT911_SDA_READ()					gpio_input_bit_get(GT911_I2C_SDA_GPIO_PORT, GT911_I2C_SDA_PIN)

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t size;
} gt911_point_t;

void GT911_Init(void);
void GT911_Rst(void);
void GT911_Detect(void);
uint8_t GT911_TouchScan(gt911_point_t* points, uint8_t cnt);

#endif
