#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void MainWidget::setupUI() {
    this->setObjectName("MainWidget");

    auto* hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);

    menuBar = new MenuBar(this);

    menuBar->addMenuItem("Home", "icons/home_default_40px.png", "icons/home_selected_40px.png");
    menuBar->addMenuItem("Setting", "icons/setting_default_40px.png", "icons/setting_selected_40px.png");

    hLayout->addWidget(menuBar);

    centralWidget = new CentralWidget(this);
    hLayout->addWidget(centralWidget);
}


