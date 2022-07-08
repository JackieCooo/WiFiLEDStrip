#include "./esp8266/esp8266_app.h"

void ESP8266_AppInit(void)
{
	printf( "�������� ESP8266 ......\r\n" );
	printf( "ʹ�� ESP8266 ......\r\n" );
	macESP8266_CH_ENABLE();
	while( !ESP8266_AT_Test() );

	printf( "�������ù���ģʽ STA ......\r\n" );
	while( !ESP8266_Net_Mode_Choose( STA ) );

	printf( "�������� WiFi ......\r\n" );
	while( !ESP8266_JoinAP( AP_SSID, AP_PASS ) );

	printf( "��ֹ������ ......\r\n" );
	while( !ESP8266_Enable_MultipleId( DISABLE ) );

}

bool ESP8266_AppTransmit(uint8_t* pBuf, uint16_t length)
{
	printf( "�������� Server ......\r\n" );
	if(!ESP8266_Link_Server( enumTCP, SERVER_IP, SERVER_PORT, Single_ID_0 ))
	{
		printf("����ʧ��\r\n");
		return false;
	}
	
	if (ESP8266_SendData(DISABLE, pBuf, length, Single_ID_0, 0, 0, 500))
	{
		printf( "�ɹ�����\r\n" );
		return true;
	}
	else
	{
		printf( "����ʧ��\r\n" );
		return false;
	}

}
