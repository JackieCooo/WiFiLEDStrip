#include "hal/message_handler.h"

void MSG_SendCmd(void)
{
	esp8266_buf_t pkg;
	
	pack(pkg.buf, PKG_CMD_WRITE_SETTING);
	pkg.size = PKG_BUF_SIZE(pkg.buf) + 4;
	xQueueSend(CommandDataQueue, &pkg, 0);
}
