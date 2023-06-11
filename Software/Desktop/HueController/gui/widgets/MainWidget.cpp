#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void MainWidget::setupUI() {
    this->setObjectName("MainWidget");

    centralWidget = new CentralWidget(this);
}


