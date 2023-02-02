#include "ConnectHandler.h"

void ConnectHandler::begin(void) {
    WiFi.onEvent(_wifi_event_cb);

    connectivity.connected = false;
    connectivity.matched = false;

    if (strlen(connectivity.ssid) == 0) {  // haven't connected to any wifi before
        // show_connect_gui();
        msg_request_t request;
        request.msg = MSG_WIFI_SCAN;
        request.user_data = NULL;
        xQueueSend(messageHandler, &request, QUEUE_TIMEOUT_MS);
    }
    else {  // have connected to wifi before
        msg_request_t request;
        request.msg = MSG_WIFI_CONNECT;
        request.user_data = NULL;
        xQueueSend(messageHandler, &request, QUEUE_TIMEOUT_MS);
    }
}

void ConnectHandler::process(void) {
    msg_request_t request;
    if (xQueueReceive(messageHandler, &request, QUEUE_TIMEOUT_MS)) {
        _construct_transaction_data(request);

        msg_reply_t reply;
        if (request.msg == MSG_WIFI_SCAN) {
            Serial.print("WiFi scaning");
            WiFi.scanNetworks();  // scan in sync mode
        }
        else if (request.msg == MSG_WIFI_CONNECT) {
            if (request.user_data) {
                wifi_connect_t* conn = (wifi_connect_t*) request.user_data;
                WiFi.begin(conn->ssid, conn->password);
                Serial.printf("Connecting to SSID: %s, PSW: %s\n", conn->ssid, conn->password);
                free(conn);
                conn = NULL;
            }
            else {
                WiFi.begin(connectivity.ssid, connectivity.password);
                Serial.printf("Connecting to SSID: %s, PSW: %s\n", connectivity.ssid, connectivity.password);
            }
            while(WiFi.status() != WL_CONNECTED) {
                Serial.print(".");
                delay(200);
            }
            Serial.printf("\nIP: %s\n", WiFi.localIP().toString().c_str());
        }
        else if (request.msg == MSG_MATCH) {
            if (_match()) reply.resp = true;
            else reply.resp = false;
        }
        else {
            if (_transmit()) reply.resp = true;
            else reply.resp = false;
        }
        reply.msg = request.msg;
        reply.user_data = request.user_data;
        _handle_reply(reply);
    }
}

bool ConnectHandler::_match(void) {
    Serial.println("Matching...");
    WiFiUDP receiver, sender;
    bool res = false;
    uint32_t timeout = MATCH_TIMEOUT_MS;
    package_t& p = _package.getPackage();

    receiver.begin(MATCH_PORT);
    sender.beginPacket(IPAddress(255, 255, 255, 255), MATCH_PORT);
    uint8_t tx_buf[PKG_BUF_MAX_LEN];
    _package.pack(tx_buf, sizeof(tx_buf));
    sender.write(tx_buf, BUF_SIZE(tx_buf));
    sender.endPacket();

    while (!connectivity.connected && timeout--) {
        if (receiver.parsePacket()) {
            uint8_t rx_buf[PKG_BUF_MAX_LEN];
            memset(rx_buf, 0x00, sizeof(rx_buf));
            receiver.read(rx_buf, sizeof(rx_buf));
            if (_package.parse(rx_buf, sizeof(rx_buf))) {
                res = true;
            }
        }
        delay(1);
    }

    receiver.stop();

    return res;
}

bool ConnectHandler::_transmit(void) {
    WiFiClient client;
    bool res = false;
    uint32_t timeout = TIMEOUT_MS;
    if (client.connect(IPAddress(connectivity.host_ip.addr), SERVER_PORT)) {
        uint8_t tx_buf[PKG_BUF_MAX_LEN];
        _package.pack(tx_buf, sizeof(tx_buf));
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
                break;
            }
            delay(1);
        }
        client.stop();
    }
    return res;
}

void ConnectHandler::_construct_transaction_data(msg_request_t& msg) {
    package_t& p = _package.getPackage();

    if (msg.msg == MSG_WRITE_CONFIG) {
        Serial.println("Write config message");
        p.cmd = PKG_CMD_WRITE_SETTING;
        p.data.strip.power = configuration.power;
        if (configuration.mode == MODE_NORMAL) {
            p.data.strip.mode = PKG_MODE_NORMAL;
            p.data.strip.setting.normal.color = lv_color_to16(configuration.setting.normal.color);
        }
        else if (configuration.mode == MODE_BREATHING) {
            p.data.strip.mode = PKG_MODE_BREATHING;
            p.data.strip.setting.breathing.color = lv_color_to16(configuration.setting.breathing.color);
            p.data.strip.setting.breathing.duration = configuration.setting.breathing.duration;
            p.data.strip.setting.breathing.ease = configuration.setting.breathing.ease;
            p.data.strip.setting.breathing.interval = configuration.setting.breathing.interval;
        }
        else if (configuration.mode == MODE_LIGHTBEAM) {
            p.data.strip.mode = PKG_MODE_LIGHTBEAM;
            p.data.strip.setting.lightbeam.color = lv_color_to16(configuration.setting.lightbeam.color);
            p.data.strip.setting.lightbeam.dir = configuration.setting.lightbeam.dir;
            p.data.strip.setting.lightbeam.faded_end = Package::parseFadedEnd(configuration.setting.lightbeam.faded_end);
            p.data.strip.setting.lightbeam.head_len = configuration.setting.lightbeam.head_len;
            p.data.strip.setting.lightbeam.gap = configuration.setting.lightbeam.gap;
            p.data.strip.setting.lightbeam.len = configuration.setting.lightbeam.len;
            p.data.strip.setting.lightbeam.speed = configuration.setting.lightbeam.speed;
            p.data.strip.setting.lightbeam.tail_len = configuration.setting.lightbeam.tail_len;
        }
        else if (configuration.mode == MODE_RAINBOW) {
            p.data.strip.mode = PKG_MODE_RAINBOW;
            p.data.strip.setting.rainbow.speed = configuration.setting.rainbow.speed;
        }
    }
    else if (msg.msg == MSG_READ_CONFIG) {
        Serial.println("Read config message");
        p.cmd = PKG_CMD_READ_SETTING;
    }
    else if (msg.msg == MSG_MATCH) {
        IPAddress ip = WiFi.localIP();
        p.cmd = PKG_CMD_ACK;
        p.data.ip.addr = PP_HTONL(uint32_t(ip));
    }
}

void ConnectHandler::_handle_reply(msg_reply_t& reply) {
    package_t& p = _package.getPackage();
    if (reply.msg == MSG_WRITE_CONFIG) {
        configuration_t* data = (configuration_t*) reply.user_data;
        if (!reply.resp) {
            memcpy(&configuration, data, sizeof(configuration_t));  // if it failed, roll back to the old config
        }
        else {
            xSemaphoreGive(saveConfigMessage);
        }
        heap_caps_free(data);
        data = NULL;
        lv_msg_send(REFRESH_GUI, NULL);
    }
    else if (reply.msg == MSG_READ_CONFIG) {
        if (reply.resp) {
            configuration.power = p.data.strip.power;
            configuration.mode = Package::packMode(p.data.strip.mode);
            if (p.data.strip.mode == PKG_MODE_NORMAL) {
                configuration.setting.normal.color = lv_color_hex(p.data.strip.setting.normal.color);
            }
            else if (p.data.strip.mode == PKG_MODE_BREATHING) {
                configuration.setting.breathing.color = lv_color_hex(p.data.strip.setting.breathing.color);
                configuration.setting.breathing.duration = p.data.strip.setting.breathing.duration;
                configuration.setting.breathing.ease = Package::packEase(p.data.strip.setting.breathing.ease);
                configuration.setting.breathing.interval = p.data.strip.setting.breathing.interval;
            }
            else if (p.data.strip.mode == PKG_MODE_LIGHTBEAM) {
                configuration.setting.lightbeam.color = lv_color_hex(p.data.strip.setting.lightbeam.color);
                configuration.setting.lightbeam.dir = Package::packDirection(p.data.strip.setting.lightbeam.dir);
                configuration.setting.lightbeam.faded_end = Package::packFadedEnd(p.data.strip.setting.lightbeam.faded_end);
                configuration.setting.lightbeam.head_len = p.data.strip.setting.lightbeam.head_len;
                configuration.setting.lightbeam.gap = p.data.strip.setting.lightbeam.gap;
                configuration.setting.lightbeam.len = p.data.strip.setting.lightbeam.len;
                configuration.setting.lightbeam.speed = p.data.strip.setting.lightbeam.speed;
                configuration.setting.lightbeam.tail_len = p.data.strip.setting.lightbeam.tail_len;
            }
            else if (p.data.strip.mode == PKG_MODE_RAINBOW) {
                configuration.setting.rainbow.speed = p.data.strip.setting.rainbow.speed;
            }
        }
        lv_msg_send(REFRESH_GUI, NULL);
    }
    else if (reply.msg == MSG_MATCH) {
        if (reply.resp) {
            connectivity.host_ip = p.data.ip;
            Serial.printf("Host IP: %d.%d.%d.%d\n", ip4_addr1_val(connectivity.host_ip), ip4_addr2_val(connectivity.host_ip), ip4_addr3_val(connectivity.host_ip), ip4_addr4_val(connectivity.host_ip));
            connectivity.connected = true;
            Serial.println("Matched");
            msg_request_t request;
            request.msg = MSG_READ_CONFIG;
            request.user_data = NULL;
            xQueueSend(messageHandler, &request, QUEUE_TIMEOUT_MS);
        }
        lv_msg_send(RES_MATCH, &reply);
    }
    Serial.printf("Send result: %d to GUI\n", reply.resp);
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

    }
    else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {

    }
}

ConnectHandler connHandler;
