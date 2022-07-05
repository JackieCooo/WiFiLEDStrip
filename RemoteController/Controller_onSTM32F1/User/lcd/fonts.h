#ifndef __FONT_H
#define __FONT_H       

#include "stm32f10x.h"

/** @defgroup FONTS_Exported_Types
  * @{
  */ 
typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  
} sFONT;

extern sFONT Font24x32;
extern sFONT Font16x24;
extern sFONT Font8x16;

#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))

//LINEY统一使用汉字字模的高度
#define LINEY(x) ((x) * (16))

#endif /*end of __FONT_H    */
