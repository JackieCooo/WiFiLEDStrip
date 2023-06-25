#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QFont>

#include "IconBtn.h"
#include "DeviceList.h"

class DevicePanel : public QWidget {
    Q_OBJECT

public:
    static constexpr int Width = 300;

    explicit DevicePanel(QWidget* parent);

private:
    DeviceList* deviceList = nullptr;

    void setupUI();
};
