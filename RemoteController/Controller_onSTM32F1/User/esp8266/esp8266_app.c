#include "./esp8266/esp8266_app.h"

void ESP8266_AppInit(void)
{
	printf( "正在配置 ESP8266 ......\r\n" );
	printf( "使能 ESP8266 ......\r\n" );
	macESP8266_CH_ENABLE();
	while( !ESP8266_AT_Test() );

	printf( "正在配置工作模式 STA ......\r\n" );
	while( !ESP8266_Net_Mode_Choose( STA ) );

	printf( "正在连接 WiFi ......\r\n" );
	while( !ESP8266_JoinAP( AP_SSID, AP_PASS ) );

	printf( "禁止多连接 ......\r\n" );
	while( !ESP8266_Enable_MultipleId( DISABLE ) );

}

bool ESP8266_AppTransmit(uint8_t* pBuf, uint16_t length)
{
	printf( "正在连接 Server ......\r\n" );
	if(!ESP8266_Link_Server( enumTCP, SERVER_IP, SERVER_PORT, Single_ID_0 ))
	{
		printf("连接失败\r\n");
		return false;
	}
	
	if (ESP8266_SendData(DISABLE, pBuf, length, Single_ID_0, 0, 0, 500))
	{
		printf( "成功发送\r\n" );
		return true;
	}
	else
	{
		printf( "发送失败\r\n" );
		return false;
	}

}
