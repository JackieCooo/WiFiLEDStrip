#ifndef _MESSAGE_HANDLER_H
#define _MESSAGE_HANDLER_H

#include "esp8266/esp8266.h"
#include "package.h"

#include "FreeRTOS.h"
#include "queue.h"

void MSG_SendCmd(void);

#endif
