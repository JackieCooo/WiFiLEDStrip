/**
  ******************************************************************************
  * @file    st7796.c
  * @author  Jackie
  * @version V1.0
  * @date    2023-02-14
  * @brief   3.5inch IPS LCD disply firmware
  ******************************************************************************
  * @attention
  * We use EXMC peripherial to communicate with LCD
  * Pin mapping below:
  * D0 - PD14	D1 - PD15	D2 - PD0	D3 - PD1	D4 - PE7
  * D5 - PE8	D6 - PE9	D7 - PE10	D8 - PE11	D9 - PE12
  * D10 - PE13	D11 - PE14	D12 - PE15	D13 - PD8	D14 - PD9
  * D15 - PD10	DC - PG0	CS - PG12	RD - PD4	WR - PD5
  * RST - PD12	BL - PD13
  */

#include "st7796/st7796.h"

uint16_t LCD_VER_RES;
uint16_t LCD_HOR_RES;
uint8_t LCD_SCAN_DIR;

__inline void ST7796_WriteCmd(uint16_t cmd);
__inline void ST7796_WriteData(uint16_t data);
static void ST7796_GPIOInit(void);
static void ST7796_EXMCInit(void);
static void ST7796_RegInit(void);

/**
  * @brief	Write command
  * @param  cmd: command
  * @retval None
  */
__inline void ST7796_WriteCmd(uint16_t cmd)
{
	*( __IO uint16_t*)(EXMC_Addr_ST7796_CMD) = cmd;
}

/**
  * @brief  Write data
  * @param  data: data
  * @retval None
  */
__inline void ST7796_WriteData(uint16_t data)
{
	*( __IO uint16_t*)(EXMC_Addr_ST7796_DATA) = data;
}

/**
  * @brief  Initialize GPIO
  * @param  None
  * @retval None
  */
static void ST7796_GPIOInit(void)
{
	/* config GPIO clock */
	rcu_periph_clock_enable(ST7796_CS_CLK);
	rcu_periph_clock_enable(ST7796_DC_CLK);
	rcu_periph_clock_enable(ST7796_WR_CLK);
	rcu_periph_clock_enable(ST7796_RD_CLK);
	rcu_periph_clock_enable(ST7796_RST_CLK);
	rcu_periph_clock_enable(ST7796_BL_CLK);
	rcu_periph_clock_enable(ST7796_D0_CLK);
	rcu_periph_clock_enable(ST7796_D1_CLK);
	rcu_periph_clock_enable(ST7796_D2_CLK);
	rcu_periph_clock_enable(ST7796_D3_CLK);
	rcu_periph_clock_enable(ST7796_D4_CLK);
	rcu_periph_clock_enable(ST7796_D5_CLK);
	rcu_periph_clock_enable(ST7796_D6_CLK);
	rcu_periph_clock_enable(ST7796_D7_CLK);
	rcu_periph_clock_enable(ST7796_D8_CLK);
	rcu_periph_clock_enable(ST7796_D9_CLK);
	rcu_periph_clock_enable(ST7796_D10_CLK);
	rcu_periph_clock_enable(ST7796_D11_CLK);
	rcu_periph_clock_enable(ST7796_D12_CLK);
	rcu_periph_clock_enable(ST7796_D13_CLK);
	rcu_periph_clock_enable(ST7796_D14_CLK);
	rcu_periph_clock_enable(ST7796_D15_CLK);
	
	/* EXMC_D[0-15] -> D[0-15] */
	gpio_mode_set(ST7796_D0_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D0_PIN);
	gpio_output_options_set(ST7796_D0_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D0_PIN);
	gpio_af_set(ST7796_D0_PORT, EXMC_AF, ST7796_D0_PIN);
	
	gpio_mode_set(ST7796_D1_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D1_PIN);
	gpio_output_options_set(ST7796_D1_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D1_PIN);
	gpio_af_set(ST7796_D1_PORT, EXMC_AF, ST7796_D1_PIN);
	
	gpio_mode_set(ST7796_D2_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D2_PIN);
	gpio_output_options_set(ST7796_D2_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D2_PIN);
	gpio_af_set(ST7796_D2_PORT, EXMC_AF, ST7796_D2_PIN);
	
	gpio_mode_set(ST7796_D3_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D3_PIN);
	gpio_output_options_set(ST7796_D3_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D3_PIN);
	gpio_af_set(ST7796_D3_PORT, EXMC_AF, ST7796_D3_PIN);
	
	gpio_mode_set(ST7796_D4_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D4_PIN);
	gpio_output_options_set(ST7796_D4_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D4_PIN);
	gpio_af_set(ST7796_D4_PORT, EXMC_AF, ST7796_D4_PIN);
	
	gpio_mode_set(ST7796_D5_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D5_PIN);
	gpio_output_options_set(ST7796_D5_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D5_PIN);
	gpio_af_set(ST7796_D5_PORT, EXMC_AF, ST7796_D5_PIN);
	
	gpio_mode_set(ST7796_D6_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D6_PIN);
	gpio_output_options_set(ST7796_D6_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D6_PIN);
	gpio_af_set(ST7796_D6_PORT, EXMC_AF, ST7796_D6_PIN);
	
	gpio_mode_set(ST7796_D7_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D7_PIN);
	gpio_output_options_set(ST7796_D7_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D7_PIN);
	gpio_af_set(ST7796_D7_PORT, EXMC_AF, ST7796_D7_PIN);
	
	gpio_mode_set(ST7796_D8_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D8_PIN);
	gpio_output_options_set(ST7796_D8_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D8_PIN);
	gpio_af_set(ST7796_D8_PORT, EXMC_AF, ST7796_D8_PIN);
	
	gpio_mode_set(ST7796_D9_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D9_PIN);
	gpio_output_options_set(ST7796_D9_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D9_PIN);
	gpio_af_set(ST7796_D9_PORT, EXMC_AF, ST7796_D9_PIN);
	
	gpio_mode_set(ST7796_D10_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D10_PIN);
	gpio_output_options_set(ST7796_D10_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D10_PIN);
	gpio_af_set(ST7796_D10_PORT, EXMC_AF, ST7796_D10_PIN);
	
	gpio_mode_set(ST7796_D11_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D11_PIN);
	gpio_output_options_set(ST7796_D11_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D11_PIN);
	gpio_af_set(ST7796_D11_PORT, EXMC_AF, ST7796_D11_PIN);
	
	gpio_mode_set(ST7796_D12_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D12_PIN);
	gpio_output_options_set(ST7796_D12_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D12_PIN);
	gpio_af_set(ST7796_D12_PORT, EXMC_AF, ST7796_D12_PIN);
	
	gpio_mode_set(ST7796_D13_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D13_PIN);
	gpio_output_options_set(ST7796_D13_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D13_PIN);
	gpio_af_set(ST7796_D13_PORT, EXMC_AF, ST7796_D13_PIN);
	
	gpio_mode_set(ST7796_D14_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D14_PIN);
	gpio_output_options_set(ST7796_D14_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D14_PIN);
	gpio_af_set(ST7796_D14_PORT, EXMC_AF, ST7796_D14_PIN);

	gpio_mode_set(ST7796_D15_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_D15_PIN);
	gpio_output_options_set(ST7796_D15_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_D15_PIN);
	gpio_af_set(ST7796_D15_PORT, EXMC_AF, ST7796_D15_PIN);

	/* EXMC_NOE -> RD */
	gpio_mode_set(ST7796_RD_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_RD_PIN);
	gpio_output_options_set(ST7796_RD_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_RD_PIN);
	gpio_af_set(ST7796_RD_PORT, EXMC_AF, ST7796_RD_PIN);

	/* EXMC_NWE -> WR */
	gpio_mode_set(ST7796_WR_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_WR_PIN);
	gpio_output_options_set(ST7796_WR_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_WR_PIN);
	gpio_af_set(ST7796_WR_PORT, EXMC_AF, ST7796_WR_PIN);

	/* EXMC_NE3 -> CS */
	gpio_mode_set(ST7796_CS_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_CS_PIN);
	gpio_output_options_set(ST7796_CS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_CS_PIN);
	gpio_af_set(ST7796_CS_PORT, EXMC_AF, ST7796_CS_PIN);

	/* EXMC_A10 -> DC */
	gpio_mode_set(ST7796_DC_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ST7796_DC_PIN);
	gpio_output_options_set(ST7796_DC_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, ST7796_DC_PIN);
	gpio_af_set(ST7796_DC_PORT, EXMC_AF, ST7796_DC_PIN);

	/* RST */
	gpio_mode_set(ST7796_RST_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, ST7796_RST_PIN);
	gpio_output_options_set(ST7796_RST_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ST7796_RST_PIN);

	/* BL */
	gpio_mode_set(ST7796_BL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, ST7796_BL_PIN);
	gpio_output_options_set(ST7796_BL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ST7796_BL_PIN);
}

/**
  * @brief  Initialized EXMC
  * @param  None
  * @retval None
  */
static void ST7796_EXMCInit(void)
{
	exmc_norsram_parameter_struct exmc_init_struct;
	exmc_norsram_timing_parameter_struct exmc_timing_struct;
	
	/* config FMC clock */
	rcu_periph_clock_enable(EXMC_CLK);
	
	exmc_timing_struct.asyn_access_mode = EXMC_ACCESS_MODE_B;
	exmc_timing_struct.asyn_address_setuptime = 8;  // 5 / 240MHz ¡Ö 20ns
	exmc_timing_struct.asyn_data_setuptime = 7;  // (4 + 1) / 240MHz ¡Ö 20ns
	exmc_timing_struct.asyn_address_holdtime = 0;  // unused
	exmc_timing_struct.bus_latency = 0;  // unused
	exmc_timing_struct.syn_data_latency = 0;  // unused
	exmc_timing_struct.syn_clk_division = 0;  // unused

	exmc_init_struct.address_data_mux = DISABLE;
	exmc_init_struct.asyn_wait = DISABLE;
	exmc_init_struct.burst_mode = DISABLE;
	exmc_init_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_16B;
	exmc_init_struct.extended_mode = DISABLE;
	exmc_init_struct.memory_type = EXMC_MEMORY_TYPE_NOR;
	exmc_init_struct.memory_write = ENABLE;
	exmc_init_struct.norsram_region = EXMC_BANK0_REGIONx;
	exmc_init_struct.nwait_config = EXMC_NWAIT_CONFIG_BEFORE;
	exmc_init_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;
	exmc_init_struct.nwait_signal = DISABLE;
	exmc_init_struct.wrap_burst_mode = DISABLE;
	exmc_init_struct.write_mode = EXMC_ASYN_WRITE;
	exmc_init_struct.read_write_timing = &exmc_timing_struct;
	exmc_init_struct.write_timing = &exmc_timing_struct;
	
	exmc_norsram_init(&exmc_init_struct);
	exmc_norsram_enable(EXMC_BANK0_REGIONx);
}

/**
  * @brief  Initialize registers
  * @param  None
  * @retval None
  */
static void ST7796_RegInit(void)
{
	ST7796_WriteCmd(0x11);  // Sleep Out
	ST7796_Delay(100);
	
	ST7796_WriteCmd(0xf0);  // Command Set Control
	ST7796_WriteData(0xc3);  // enable command 2 part I
	
	ST7796_WriteCmd(0xf0);  // Command Set Control
	ST7796_WriteData(0x96);  // enable command 2 part II
	
	ST7796_WriteCmd(0x36);  // Memory Data Access Control
	ST7796_WriteData(0x08);  // BGR color filter panel, LCD vertical refresh Top to Bottom, Left to Right
	
	ST7796_WriteCmd(0x3A);  // Interface Pixel Format
	ST7796_WriteData(0x05);  // 16bit/pixel
	
	ST7796_WriteCmd(0xe8);  // Display Output Ctrl Adjust
	ST7796_WriteData(0x40);
	ST7796_WriteData(0x82);
	ST7796_WriteData(0x07);
	ST7796_WriteData(0x18);
	ST7796_WriteData(0x27);  // Source equalizing period time: 16.5us
	ST7796_WriteData(0x0a);  // the timing "Gate start": (0 + 1) Tclk
	ST7796_WriteData(0xb6);  // Gate driver EQ function ON, the timing "Gate End": (36 + 1) Tclk
	ST7796_WriteData(0x33);
	
	ST7796_WriteCmd(0xc5);  //  VCOM Control
	ST7796_WriteData(0x27);  // VCOM: 0.975
	
	ST7796_WriteCmd(0xc2);  //  Power Control 3
	ST7796_WriteData(0xa7);  // Gamma driving current level high, Source driving current level low
	
	ST7796_WriteCmd(0xe0);  // Positive Gamma Control
	ST7796_WriteData(0xf0);
	ST7796_WriteData(0x01);
	ST7796_WriteData(0x06);
	ST7796_WriteData(0x0f);
	ST7796_WriteData(0x12);
	ST7796_WriteData(0x1d);
	ST7796_WriteData(0x36);
	ST7796_WriteData(0x54);
	ST7796_WriteData(0x44);
	ST7796_WriteData(0x0c);
	ST7796_WriteData(0x18);
	ST7796_WriteData(0x16);
	ST7796_WriteData(0x13);
	ST7796_WriteData(0x15);
	
	ST7796_WriteCmd(0xe1);  // Negative Gamma Control
	ST7796_WriteData(0xf0);
	ST7796_WriteData(0x01);
	ST7796_WriteData(0x05);
	ST7796_WriteData(0x0a);
	ST7796_WriteData(0x0b);
	ST7796_WriteData(0x07);
	ST7796_WriteData(0x32);
	ST7796_WriteData(0x44);
	ST7796_WriteData(0x44);
	ST7796_WriteData(0x0c);
	ST7796_WriteData(0x18);
	ST7796_WriteData(0x17);
	ST7796_WriteData(0x13);
	ST7796_WriteData(0x16);
	
	ST7796_WriteCmd(0xf0);  // Command Set Control
	ST7796_WriteData(0x3c);  // disable command 2 part I
	
	ST7796_WriteCmd(0xf0);  // Command Set Control
	ST7796_WriteData(0x69);  // disable command 2 part II
	ST7796_Delay(120);
	
	ST7796_WriteCmd(0x21);  // Display Inversion On
	ST7796_WriteCmd(0x29);  // Display On
}

/**
  * @brief  Initialize LCD
  * @param  None
  * @retval None
  */
void ST7796_Init(void)
{
	ST7796_GPIOInit();
	ST7796_EXMCInit();
	ST7796_Rst();
	ST7796_RegInit();
	ST7796_DisplyDir(2);
	ST7796_Clear();
	ST7796_BackLedControl(ENABLE);
}

/**
  * @brief  Reset LCD
  * @param  None
  * @retval None
  */
void ST7796_Rst(void)
{
	gpio_bit_reset(ST7796_RST_PORT, ST7796_RST_PIN);
	ST7796_Delay(10);
	gpio_bit_set(ST7796_RST_PORT, ST7796_RST_PIN);
	ST7796_Delay(10);
}

/**
  * @brief  Set LCD backlight
  * @param  state: LCD backlight status
  * @retval None
  */
void ST7796_BackLedControl(ControlStatus state)
{
	if (state)
		 gpio_bit_set(ST7796_BL_PORT, ST7796_BL_PIN);	
	else
		 gpio_bit_reset(ST7796_BL_PORT, ST7796_BL_PIN);
}

/**
  * @brief  Set a pixel filling area
  * @param  x: start x
  * @param  y: start y
  * @param  w: area width
  * @param  h: area height
  * @retval None
  */
void ST7796_OpenWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	ST7796_WriteCmd(0x2a);
	ST7796_WriteData(x >> 8);
	ST7796_WriteData(x & 0xff);
	ST7796_WriteData((x + w - 1) >> 8);
	ST7796_WriteData((x + w - 1) & 0xff);

	ST7796_WriteCmd(0x2b);
	ST7796_WriteData(y >> 8);
	ST7796_WriteData(y & 0xff);
	ST7796_WriteData((y + h - 1) >> 8);
	ST7796_WriteData((y + h - 1) & 0xff);
}

/**
  * @brief  Fill color in an area
  * @param  x: start x
  * @param  y: start y
  * @param  w: area width
  * @param  h: area height
  * @param  c: color
  * @retval None
  */
void ST7796_FillColor(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c)
{
	ST7796_OpenWindow(x, y, w, h);
	
	ST7796_WriteCmd(0x2c);
	for(uint16_t i = 0; i < w; i++)
	{
		for(uint16_t j = 0; j < h; j++)
		{
			ST7796_WriteData(c);
		}
	}
}

/**
  * @brief  Fill colors in an area
  * @param  x: start x
  * @param  y: start y
  * @param  w: area width
  * @param  h: area height
  * @param  cs: color buffer
  * @retval None
  */
void ST7796_FillColors(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t* cs)
{
	ST7796_OpenWindow(x, y, w, h);
	
	ST7796_WriteCmd(0x2c);
	for(uint16_t i = 0; i < w; i++)
	{
		for(uint16_t j = 0; j < h; j++)
		{
			ST7796_WriteData(*cs);
			++cs;
		}
	}
}

/**
  * @brief  Clear LCD
  * @param  None
  * @retval None
  */
void ST7796_Clear(void)
{
	ST7796_OpenWindow(0, 0, LCD_HOR_RES, LCD_VER_RES);
	ST7796_FillColor(0, 0, LCD_HOR_RES, LCD_VER_RES, 0x0000);
}

/**
  * @brief  Set LCD disply direction
  * @param  dir: display direction
  * @retval None
  */
void ST7796_DisplyDir(uint8_t dir)
{
	if (dir > 7) return;
	
	if(dir % 2)
	{
		LCD_HOR_RES = LCD_MORE_RES;
		LCD_VER_RES = LCD_LESS_RES;
	}
	else				
	{
		LCD_HOR_RES = LCD_LESS_RES;
		LCD_VER_RES = LCD_MORE_RES;
	}
	LCD_SCAN_DIR = dir;
	
	ST7796_WriteCmd(0x36);
	ST7796_WriteData(0x08 | (dir << 5));
}
