#include "DevicePanel.h"

DevicePanel::DevicePanel(QWidget *parent) : QWidget(parent) {
    this->setupUI();
}

void DevicePanel::setupUI() {
    this->setObjectName("DevicePanel");
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->setFixedWidth(DevicePanel::Width);

    auto* vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setSpacing(20);

    auto* top = new QWidget(this);

    auto* hLayout = new QHBoxLayout(top);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setAlignment(Qt::AlignVCenter);

    auto* label = new QLabel("Device: ", top);
    label->setFont(QFont("Arial", 16, QFont::Bold));
    hLayout->addWidget(label);

    auto* btn = new IconBtn(top, "Refresh", "icons/refresh_default_30px.png");
    hLayout->addWidget(btn);

    top->setLayout(hLayout);

    vLayout->addWidget(top);

    deviceList = new DeviceList(this);
    vLayout->addWidget(deviceList);

    this->setLayout(vLayout);
}
