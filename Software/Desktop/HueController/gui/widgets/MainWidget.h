#pragma once

#include <QWidget>

#include "CentralWidget.h"

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent);

private:
    CentralWidget* centralWidget = nullptr;

    void setupUI();
};
