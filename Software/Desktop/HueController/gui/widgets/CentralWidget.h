#pragma once

#include <QStackedWidget>

#include "ControlPage.h"

class CentralWidget : public QStackedWidget {
    Q_OBJECT

public:
    explicit CentralWidget(QWidget* parent = nullptr);

private:
    ControlPage* controlPage = nullptr;

    void setupUI();
};

