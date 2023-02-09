#include "ConnectHandler.h"

void ConnectHandler::begin(void) {
    WiFi.onEvent(_wifi_event_cb);
    WiFi.setAutoReconnect(false);

    _matched = false;

    if (strlen(connectivity.ssid) == 0) {  // haven't connected to any wifi before
        sendMessage(MSG_WIFI_SCAN, NULL);
    }
    else {  // have connected to wifi before
        sendMessage(MSG_WIFI_CONNECT, NULL);
    }
}

void ConnectHandler::process(void) {
    msg_request_t request;
    if (xQueueReceive(messageHandler, &request, portMAX_DELAY)) {

        if (request.msg == MSG_WIFI_SCAN) {
            Serial.print("WiFi scaning");
            WiFi.scanNetworks();  // scan in sync mode
        }
        else if (request.msg == MSG_WIFI_CONNECT) {
            if (request.user_data) {
                wifi_connect_t* conn = (wifi_connect_t*) request.user_data;
                WiFi.begin(conn->ssid, conn->password);
                Serial.printf("Connecting to SSID: %s, PSW: %s\n", conn->ssid, conn->password);
                strcpy(connectivity.ssid, conn->ssid);
                strcpy(connectivity.password, conn->password);
                free(conn);
                conn = NULL;
            }
            else {
                WiFi.begin(connectivity.ssid, connectivity.password);
                Serial.printf("Connecting to SSID: %s, PSW: %s\n", connectivity.ssid, connectivity.password);
            }
        }
        else if (request.msg == MSG_MATCH_HOST) {
            msg_reply_t reply;
            reply.msg = MSG_MATCH_HOST;
            reply.user_data = NULL;
            if (_match()) {
                _package.parseFromPackage();
                Serial.printf("Host IP: %d.%d.%d.%d\n", ip4_addr1_val(connectivity.host_ip), ip4_addr2_val(connectivity.host_ip), ip4_addr3_val(connectivity.host_ip), ip4_addr4_val(connectivity.host_ip));
                Serial.println("Matched");
                local_file_t cmd = FILE_CONNECT;
                xQueueSend(saveFileMessage, &cmd, QUEUE_TIMEOUT_MS);
                _matched = true;
                reply.resp = true;
            }
            else {
                Serial.println("Match failed");
                reply.resp = false;
            }
            lv_msg_send(MSG_MATCH_RESULT, &reply);
        }
        else if (request.msg == MSG_ACK_HOST) {
            msg_reply_t reply;
            reply.msg = MSG_ACK_HOST;
            reply.user_data = NULL;
            if (_transmit(PKG_CMD_ACK)) {
                reply.resp = true;
            }
            else {
                reply.resp = false;
            }
            lv_msg_send(MSG_ACK_RESULT, &reply);
        }
        else if (request.msg == MSG_READ_HOST) {
            msg_reply_t reply;
            reply.msg = MSG_READ_HOST;
            reply.user_data = NULL;
            if (_transmit(PKG_CMD_READ_SETTING)) {
                _package.parseFromPackage();
                local_file_t cmd = FILE_CONFIG;
                xQueueSend(saveFileMessage, &cmd, QUEUE_TIMEOUT_MS);
                reply.resp = true;
            }
            else {
                reply.resp = false;
            }
            lv_msg_send(MSG_READ_RESULT, &reply);
        }
        else if (request.msg == MSG_WRITE_HOST) {
            configuration_t* old_data = (configuration_t*) request.user_data;
            msg_reply_t reply;
            reply.msg = MSG_WRITE_HOST;
            if (_transmit(PKG_CMD_WRITE_SETTING)) {
                local_file_t cmd = FILE_CONFIG;
                xQueueSend(saveFileMessage, &cmd, QUEUE_TIMEOUT_MS);
            }
            else {
                memcpy(&configuration, old_data, sizeof(configuration_t));  // if it failed, roll back to the old config
            }
            free(old_data);
            old_data = NULL;
            lv_msg_send(MSG_WRITE_RESULT, &reply);
        }
    }
}

bool ConnectHandler::_match(void) {
    Serial.println("Matching...");
    WiFiUDP receiver, sender;
    bool res = false;
    uint32_t timeout = MATCH_TIMEOUT_MS;

    // broadcast in UDP, in order to get host IP
    receiver.begin(MATCH_PORT);
    sender.beginPacket(IPAddress(255, 255, 255, 255), MATCH_PORT);
    uint8_t tx_buf[PKG_BUF_MAX_LEN];
    _package.pack(tx_buf, PKG_CMD_MATCH);
    sender.write(tx_buf, BUF_SIZE(tx_buf));
    sender.endPacket();

    while (timeout--) {
        if (receiver.parsePacket()) {
            uint8_t rx_buf[PKG_BUF_MAX_LEN];
            memset(rx_buf, 0x00, sizeof(rx_buf));
            receiver.read(rx_buf, sizeof(rx_buf));
            if (_package.parse(rx_buf, sizeof(rx_buf))) {
                res = true;
                break;
            }
        }
        delay(1);
    }

    receiver.stop();

    return res;
}

bool ConnectHandler::_transmit(uint8_t cmd) {
    WiFiClient client;
    bool res = false;
    uint32_t timeout = TIMEOUT_MS;
    if (client.connect(IPAddress(connectivity.host_ip.addr), SERVER_PORT)) {
        uint8_t tx_buf[PKG_BUF_MAX_LEN];
        _package.pack(tx_buf, cmd);
        client.write(tx_buf, BUF_SIZE(tx_buf));
        Serial.println("Sent data to host");
        while (client.connected() && timeout--) {
            if (client.available()) {
                uint8_t rx_buf[PKG_BUF_MAX_LEN];
                memset(rx_buf, 0x00, sizeof(rx_buf));
                client.read(rx_buf, sizeof(rx_buf));
                Serial.println("Received host reply");
                if (_package.parse(rx_buf, sizeof(rx_buf))) {
                    res = true;
                }
            }
            delay(1);
        }
    }
    return res;
}

bool ConnectHandler::get_connect_status(void) {
    return WiFi.isConnected();
}

bool ConnectHandler::get_match_status(void) {
    return _matched;
}

void ConnectHandler::set_match_status(bool status) {
    _matched = status;
}

void ConnectHandler::task(void* args) {
    for (;;) {
        connHandler.process();
        vTaskDelay(10);
    }
}

void ConnectHandler::_wifi_event_cb(arduino_event_id_t event, arduino_event_info_t info) {
    if (event == ARDUINO_EVENT_WIFI_SCAN_DONE) {
        if (info.wifi_scan_done.status == 0) {
            uint8_t num = info.wifi_scan_done.number;

            wifi_list_t* list = list_create();
            for (uint8_t i = 0; i < num; ++i) {
                Serial.printf("SSID: %s, RSSI: %d\n", WiFi.SSID(i).c_str(), WiFi.RSSI(i));
                wifi_info_t* info = (wifi_info_t*) malloc(sizeof(wifi_info_t));
                WiFi.SSID(i).toCharArray(info->ssid, MAX_SSID_LEN);
                info->rssi = WiFi.RSSI(i);
                list_push_back(list, info);
            }
            lv_msg_send(MSG_WIFI_SCAN_DONE, &list);
        }
    }
    else if (event == ARDUINO_EVENT_WIFI_STA_CONNECTED) {
        Serial.println("WiFi connected");
    }
    else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
        uint8_t code = info.wifi_sta_disconnected.reason;

        Serial.printf("WiFi disconnected, error code: %d\n", code);

        WiFi.disconnect();
        if (code == WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT) {
            lv_msg_send(MSG_WIFI_PASSWORD_FAILED, NULL);
        }
        else {
            lv_msg_send(MSG_WIFI_DISCONNECTED, NULL);
        }
    }
    else if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
        Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());

        lv_msg_send(MSG_WIFI_CONNECTED, NULL);

        local_file_t cmd = FILE_CONNECT;
        xQueueSend(saveFileMessage, &cmd, QUEUE_TIMEOUT_MS);  // save connectivity file

        if (connectivity.host_ip.addr == IPADDR_ANY) {  // haven't match before
            sendMessage(MSG_MATCH_HOST, NULL);
            loading_gui_set_text("Matching...");
        }
        else {  // already matched before
            sendMessage(MSG_ACK_HOST, NULL);
            loading_gui_set_text("Acking...");
        }
    }
}

ConnectHandler connHandler;
