#pragma once

#include <QWidget>
#include <QPalette>
#include <QColor>
#include <QHBoxLayout>

#include "MenuBar.h"
#include "CentralWidget.h"

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent);

private:
    MenuBar* menuBar = nullptr;
    CentralWidget* centralWidget = nullptr;

    void setupUI();
};
