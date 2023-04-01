#include <Arduino.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID           "HUAWEI-10GJ0E"
#define WIFI_PSW            "abc12345678"
#define SERVER_PORT         20001
#define BUF_MAX_LEN         32
#define TIMEOUT_MS          2000

static int cmd_size = 0;
static int resp_size = 0;
static uint8_t cmd_buf[BUF_MAX_LEN];
static uint8_t resp_buf[BUF_MAX_LEN];

WiFiClient client;
IPAddress SERVER_IP(192, 168, 3, 66);

static void dump_buf_to_serial(uint8_t* buf, uint16_t size) {
  Serial.print("Data: ");
  for (uint16_t i = 0; i < size; i++) {
    Serial.printf("%02X ", buf[i]);
  }
  Serial.println();
}

static void receive_package(void) {
  memset(cmd_buf, 0x00, sizeof(cmd_buf));
  Serial.read(cmd_buf, sizeof(cmd_buf));
  for (cmd_size = sizeof(cmd_buf); !cmd_buf[cmd_size-1]; cmd_size--) {}
  Serial.println("Receive from host");
  dump_buf_to_serial(cmd_buf, cmd_size);
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
  if (Serial.available()) {
    delay(10);
    receive_package();
    if (client.connect(SERVER_IP, SERVER_PORT)) {
      Serial.println("Connected to server");
      while (client.connected()) {
        client.write(cmd_buf, cmd_size);
        
        uint16_t timeout = TIMEOUT_MS;
        while (timeout--) {
          if (resp_size = client.available()) {
            memset(resp_buf, 0x00, sizeof(resp_buf));
            client.read(resp_buf, resp_size);
            Serial.println("Receive from server");
            dump_buf_to_serial(resp_buf, resp_size);
            break;
          }
          delay(1);
        }
        break;
      }
    }
    else {
      Serial.println("Connected to server failed");
    }
    
    while(Serial.read() > 0);
  }
}
