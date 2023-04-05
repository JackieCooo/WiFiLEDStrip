#ifndef _MESSAGE_HANDLER_H
#define _MESSAGE_HANDLER_H

#include "esp8266/esp8266.h"
#include "global.h"

void MSG_SendCmd(uint8_t* buf, uint16_t size);

#endif
