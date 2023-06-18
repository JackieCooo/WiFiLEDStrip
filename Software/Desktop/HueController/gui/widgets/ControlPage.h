#pragma once

#include <QWidget>
#include <QHBoxLayout>

#include "DevicePanel.h"
#include "ControlPanel.h"

class ControlPage : public QWidget {
    Q_OBJECT

public:
    explicit ControlPage(QWidget* parent = nullptr);

private:
    DevicePanel* devicePanel = nullptr;
    ControlPanel* controlPanel = nullptr;

    void setupUI();
};
