#include "CentralWidget.h"

CentralWidget::CentralWidget(QWidget *parent) : QStackedWidget(parent) {
    setupUI();
}

void CentralWidget::setupUI() {
    this->setObjectName("CentralWidget");

    controlPage = new ControlPage(this);
    this->addWidget(controlPage);
}
