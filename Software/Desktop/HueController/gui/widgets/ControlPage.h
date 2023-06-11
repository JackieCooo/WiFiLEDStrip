#pragma once

#include <QWidget>
#include <QVBoxLayout>

#include "TopPanel.h"

class ControlPage : public QWidget {
    Q_OBJECT

public:
    explicit ControlPage(QWidget* parent = nullptr);

private:
    TopPanel* topPanel = nullptr;

    void setupUI();
};
