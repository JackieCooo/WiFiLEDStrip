#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
}

void MainWindow::setupUI() {
    this->setObjectName("MainWindow");
    this->resize(800, 600);

    mainWidget = new MainWidget(this);
    this->setCentralWidget(mainWidget);
}
