#include "ControlPage.h"

ControlPage::ControlPage(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void ControlPage::setupUI() {
    this->setObjectName("ControlPage");

    auto* vLayout = new QVBoxLayout(this);

    topPanel = new TopPanel(this);
    vLayout->addWidget(topPanel);
}


