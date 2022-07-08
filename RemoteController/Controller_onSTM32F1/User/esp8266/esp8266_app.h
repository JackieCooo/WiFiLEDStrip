#ifndef __ESP8266_APP_H_
#define __ESP8266_APP_H_

#include "stm32f10x.h"
#include <stdio.h>
#include <stdbool.h>
#include "./esp8266/esp8266.h"

#define SERVER_IP "192.168.4.1"  // �����IP
#define SERVER_PORT "12345"  // ����˶˿�
#define AP_SSID "ESP_WIFI"  // �ȵ�SSID
#define AP_PASS "12345678"  // �ȵ�����

#define CTL_BUF_LENGTH 4  // LED�����ֶγ���

void ESP8266_AppInit(void);
bool ESP8266_AppTransmit(uint8_t* pBuf, uint16_t length);

#endif
