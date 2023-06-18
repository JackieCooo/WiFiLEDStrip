#include "DeviceListViewport.h"

DeviceListViewport::DeviceListViewport(QWidget *parent) : QWidget(parent) {
    this->setupUI();
}

void DeviceListViewport::setupUI() {
    this->setObjectName("DeviceListViewport");
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    this->setFixedHeight(0);

    auto* vLayout = new QVBoxLayout(this);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(DeviceListViewport::ItemSpacing);

    this->setLayout(vLayout);
}
