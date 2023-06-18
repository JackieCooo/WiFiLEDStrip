#include "ControlPage.h"

ControlPage::ControlPage(QWidget *parent) : QWidget(parent) {
    this->setupUI();
}

void ControlPage::setupUI() {
    this->setObjectName("ControlPage");

    auto* hLayout = new QHBoxLayout(this);
    hLayout->setAlignment(Qt::AlignLeft);
    hLayout->setContentsMargins(10, 10, 10, 10);
    hLayout->setSpacing(20);

    devicePanel = new DevicePanel(this);
    hLayout->addWidget(devicePanel);

    controlPanel = new ControlPanel(this);
    hLayout->addWidget(controlPanel);

    this->setLayout(hLayout);
}


