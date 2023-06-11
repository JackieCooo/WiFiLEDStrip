#include "TopPanel.h"

TopPanel::TopPanel(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void TopPanel::setupUI() {
    this->setObjectName("TopPanel");
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto* hLayout = new QHBoxLayout(this);
    this->setLayout(hLayout);

    refreshBtn = new IconBtn(this);
    refreshBtn->setText("Refresh");
    refreshBtn->setIcon("icons/refresh_default_40px.ico");
    hLayout->addWidget(refreshBtn);

    settingBtn = new IconBtn(this);
    settingBtn->setText("Setting");
    settingBtn->setIcon("icons/setting_default_40px.ico");
    hLayout->addWidget(settingBtn);
}


