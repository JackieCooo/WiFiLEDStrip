#include <Arduino.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID           "HUAWEI-10GJ0E"
#define WIFI_PSW            "abc12345678"
#define SERVER_PORT         20001
#define BUF_MAX_LEN         32
#define TIMEOUT_MS          5000

WiFiClient client;
IPAddress SERVER_IP(192, 168, 1, 3);

static void dump_buf_to_serial(uint8_t* buf, uint16_t size) {
  Serial.print("Data: ");
  Serial.write(buf, size);
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PSW);
  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\nIP: %s\n", WiFi.localIP().toString().c_str());
}

void loop() {
  int cmd_size = 0;
  int resp_size = 0;
  int timeout = TIMEOUT_MS;
  uint8_t cmd_buf[BUF_MAX_LEN];
  uint8_t resp_buf[BUF_MAX_LEN];

  if (cmd_size = Serial.available()) {
    Serial.getRxBufferSize();
    Serial.read(cmd_buf, cmd_size);
    Serial.println("Receive from host");
    dump_buf_to_serial(cmd_buf, cmd_size);
    // if (client.connect(SERVER_IP, SERVER_PORT)) {
    //   while (client.connected()) {
    //     client.write(cmd_buf, cmd_size);
    //     while (timeout--) {
    //       if (resp_size = client.available()) break;
    //       delay(1);
    //     }
    //     client.read(resp_buf, resp_size);
    //     Serial.println("Receive from server");
    //     dump_buf_to_serial(resp_buf, resp_size);
    //     break;
    //   }
    // }
  }
}
