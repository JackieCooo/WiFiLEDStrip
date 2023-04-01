#ifndef _ST7796_H
#define _ST7796_H

#include "gd32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"

/* LCD_LESS_RES -> lower resolution side, LCD_MORE_RES -> higher resolution side */
#define LCD_LESS_RES				(320)
#define LCD_MORE_RES				(480)

#define ST7796_Delay(x)				vTaskDelay(x / portTICK_PERIOD_MS)

#define EXMC_Addr_ST7796_CMD		((uint32_t)0x6C000000)
#define EXMC_Addr_ST7796_DATA		((uint32_t)0x6C000800)  // 0x6C000000 | (1 << (1 + Ax))

#define EXMC_BANK0_REGIONx          EXMC_BANK0_NORSRAM_REGION3
#define EXMC_AF                     GPIO_AF_12
#define EXMC_CLK					RCU_EXMC

#define ST7796_CS_CLK               RCU_GPIOG
#define ST7796_CS_PORT              GPIOG
#define ST7796_CS_PIN               GPIO_PIN_12

#define ST7796_DC_CLK               RCU_GPIOG  
#define ST7796_DC_PORT              GPIOG
#define ST7796_DC_PIN               GPIO_PIN_0

#define ST7796_WR_CLK               RCU_GPIOD   
#define ST7796_WR_PORT              GPIOD
#define ST7796_WR_PIN               GPIO_PIN_5

#define ST7796_RD_CLK               RCU_GPIOD   
#define ST7796_RD_PORT              GPIOD
#define ST7796_RD_PIN               GPIO_PIN_4

#define ST7796_RST_CLK              RCU_GPIOD 
#define ST7796_RST_PORT             GPIOD
#define ST7796_RST_PIN              GPIO_PIN_12

#define ST7796_BL_CLK               RCU_GPIOD   
#define ST7796_BL_PORT              GPIOD
#define ST7796_BL_PIN               GPIO_PIN_13

#define ST7796_D0_CLK               RCU_GPIOD   
#define ST7796_D0_PORT              GPIOD
#define ST7796_D0_PIN               GPIO_PIN_14

#define ST7796_D1_CLK               RCU_GPIOD   
#define ST7796_D1_PORT              GPIOD
#define ST7796_D1_PIN               GPIO_PIN_15

#define ST7796_D2_CLK               RCU_GPIOD   
#define ST7796_D2_PORT              GPIOD
#define ST7796_D2_PIN               GPIO_PIN_0

#define ST7796_D3_CLK               RCU_GPIOD  
#define ST7796_D3_PORT              GPIOD
#define ST7796_D3_PIN               GPIO_PIN_1

#define ST7796_D4_CLK               RCU_GPIOE   
#define ST7796_D4_PORT              GPIOE
#define ST7796_D4_PIN               GPIO_PIN_7

#define ST7796_D5_CLK               RCU_GPIOE   
#define ST7796_D5_PORT              GPIOE
#define ST7796_D5_PIN               GPIO_PIN_8

#define ST7796_D6_CLK               RCU_GPIOE   
#define ST7796_D6_PORT              GPIOE
#define ST7796_D6_PIN               GPIO_PIN_9

#define ST7796_D7_CLK               RCU_GPIOE  
#define ST7796_D7_PORT              GPIOE
#define ST7796_D7_PIN               GPIO_PIN_10

#define ST7796_D8_CLK               RCU_GPIOE   
#define ST7796_D8_PORT              GPIOE
#define ST7796_D8_PIN               GPIO_PIN_11

#define ST7796_D9_CLK               RCU_GPIOE   
#define ST7796_D9_PORT              GPIOE
#define ST7796_D9_PIN               GPIO_PIN_12

#define ST7796_D10_CLK              RCU_GPIOE   
#define ST7796_D10_PORT             GPIOE
#define ST7796_D10_PIN              GPIO_PIN_13

#define ST7796_D11_CLK              RCU_GPIOE   
#define ST7796_D11_PORT             GPIOE
#define ST7796_D11_PIN              GPIO_PIN_14

#define ST7796_D12_CLK              RCU_GPIOE   
#define ST7796_D12_PORT             GPIOE
#define ST7796_D12_PIN              GPIO_PIN_15

#define ST7796_D13_CLK              RCU_GPIOD   
#define ST7796_D13_PORT             GPIOD
#define ST7796_D13_PIN              GPIO_PIN_8

#define ST7796_D14_CLK              RCU_GPIOD   
#define ST7796_D14_PORT             GPIOD
#define ST7796_D14_PIN              GPIO_PIN_9

#define ST7796_D15_CLK              RCU_GPIOD
#define ST7796_D15_PORT             GPIOD
#define ST7796_D15_PIN              GPIO_PIN_10

void ST7796_Init(void);
void ST7796_Rst(void);
void ST7796_BackLedControl(ControlStatus state);
void ST7796_OpenWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void ST7796_FillColor(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c);
void ST7796_FillColors(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* cs);
void ST7796_Clear(void);
void ST7796_DisplyDir(uint8_t dir);

extern uint16_t LCD_VER_RES;
extern uint16_t LCD_HOR_RES;
extern uint8_t LCD_SCAN_DIR;

#endif
