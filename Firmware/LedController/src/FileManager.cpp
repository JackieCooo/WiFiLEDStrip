#include "FileManager.h"

void FileManager::begin() {
    // SPIFFS.format();
    if (SPIFFS.begin(true)) {
        Serial.println("SPIFFS init done");

        checkFile();
        loadConfiguration();
    }
    else {
        Serial.println("SPIFFS init failed");
    }
}

void FileManager::checkFile() {
    if (!SPIFFS.exists(CONFIG_FILE_PATH)) {
        Serial.println("Missing configuration file, creating...");
        config_t f;
        initConfigurationFile(f);
        save(CONFIG_FILE_PATH, (uint8_t*)&f, sizeof(config_t));
        Serial.println("Local configuration file created");
    }
    else {
        config_t f;
        bool ret = load(CONFIG_FILE_PATH, (uint8_t*)&f, sizeof(config_t));
        if (!ret) {
            Serial.println("Config file broken, fixing...");
            initConfigurationFile(f);
            save(CONFIG_FILE_PATH, (uint8_t*)&f, sizeof(config_t));
            Serial.println("Config file fixed");
        }
    }

    if (!SPIFFS.exists(CONNECT_FILE_PATH)) {
        Serial.println("Missing connectivity file, creating...");
        connectivity_t f;
        initConnectivityFile(f);
        save(CONNECT_FILE_PATH, (uint8_t*)&f, sizeof(connectivity_t));
        Serial.println("Local connectivity file created");
    }
}

void FileManager::initConfigurationFile(config_t& file) {
    file.power = 0;
    file.mode = 0;

    file.setting.normal.color = 0x001f;

    file.setting.breathing.color = 0x001f;
    file.setting.breathing.duration = 2000;
    file.setting.breathing.interval = 1000;
    file.setting.breathing.ease = 0;

    file.setting.lightbeam.color = 0x001f;
    file.setting.lightbeam.len = 2;
    file.setting.lightbeam.gap = 2;
    file.setting.lightbeam.fade = 3;
    file.setting.lightbeam.head = 4;
    file.setting.lightbeam.tail = 4;
    file.setting.lightbeam.dir = 0;
    file.setting.lightbeam.speed = 8;

    file.setting.rainbow.speed = 8;

    file.setting.rhythm.color = 0x001f;
    file.setting.rhythm.freq = 2;
}

void FileManager::initConnectivityFile(connectivity_t& file) {
    memset(file.ssid, 0x00, sizeof(file.ssid));
    memset(file.password, 0x00, sizeof(file.password));
}

void FileManager::saveConfiguration() {
    config_t f;

    f.power = stripHandler.getPower();
    f.mode = stripHandler.getMode();

    NormalData nd;
    stripHandler.getData(nd);
    f.setting.normal.color = Rgb16Color(RgbColor(nd.color)).Color565;

    BreathingData bd;
    stripHandler.getData(bd);
    f.setting.breathing.color = Rgb16Color(RgbColor(bd.color)).Color565;
    f.setting.breathing.duration = bd.duration;
    f.setting.breathing.interval = bd.interval;
    f.setting.breathing.ease = bd.ease;

    LightbeamData ld;
    stripHandler.getData(ld);
    f.setting.lightbeam.color = Rgb16Color(RgbColor(ld.color)).Color565;
    f.setting.lightbeam.len = ld.len;
    f.setting.lightbeam.gap = ld.gap;
    f.setting.lightbeam.fade = (uint8_t)ld.fade;
    f.setting.lightbeam.head = ld.head;
    f.setting.lightbeam.tail = ld.tail;
    f.setting.lightbeam.dir = ld.dir;
    f.setting.lightbeam.speed = ld.speed;

    RainbowData rb;
    stripHandler.getData(rb);
    f.setting.rainbow.speed = rb.speed;

    RhythmData rhb;
    stripHandler.getData(rhb);
    f.setting.rhythm.color = Rgb16Color(RgbColor(rhb.color)).Color565;
    f.setting.rhythm.freq = rhb.freq;

    _dump_buf((uint8_t*)&f, sizeof(config_t));
    save(CONFIG_FILE_PATH, (uint8_t*)&f, sizeof(config_t));
    Serial.println("Config file saved");
}

void FileManager::loadConfiguration() {
    config_t f;

    load(CONFIG_FILE_PATH, (uint8_t*)&f, sizeof(config_t));
    _dump_buf((uint8_t*)&f, sizeof(config_t));

    stripHandler.setPower(f.power);
    stripHandler.setMode(static_cast<led_mode_t>(f.mode));
    stripHandler.setData(
            NormalData(
                    RgbColor(Rgb16Color(f.setting.normal.color))
            )
    );
    stripHandler.setData(
            BreathingData(
                    RgbColor(Rgb16Color(f.setting.breathing.color)),
                    f.setting.breathing.duration,
                    f.setting.breathing.interval,
                    static_cast<ease_t>(f.setting.breathing.ease)
            )
    );
    stripHandler.setData(
            LightbeamData(
                    RgbColor(Rgb16Color(f.setting.lightbeam.color)),
                    f.setting.lightbeam.len,
                    f.setting.lightbeam.gap,
                    Fade(f.setting.lightbeam.fade),
                    f.setting.lightbeam.head,
                    f.setting.lightbeam.tail,
                    static_cast<dir_t>(f.setting.lightbeam.dir),
                    f.setting.lightbeam.speed
            )
    );
    stripHandler.setData(
            RainbowData(
                    f.setting.rainbow.speed
            )
    );
    stripHandler.setData(
            RhythmData(
                    RgbColor(Rgb16Color(f.setting.rhythm.color)),
                    static_cast<freq_enum>(f.setting.rhythm.freq)
            )
    );
}

FileManager fileManager;
