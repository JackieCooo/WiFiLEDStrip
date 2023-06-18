#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
}

void MainWindow::setupUI() {
    this->setObjectName("MainWindow");
    this->resize(800, 600);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(0x1D1E1F));
    palette.setColor(QPalette::Dark, QColor(0x262727));
    palette.setColor(QPalette::Mid, QColor(0x303030));
    palette.setColor(QPalette::Midlight, QColor(0x39393A));
    palette.setColor(QPalette::Light, QColor(0x424243));
    palette.setColor(QPalette::Base, QColor(0x409EFF));
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::white);
    this->setPalette(palette);

    mainWidget = new MainWidget(this);
    this->setCentralWidget(mainWidget);
}
