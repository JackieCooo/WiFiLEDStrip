#pragma once

#include <QWidget>
#include <QSizePolicy>
#include <QHBoxLayout>
#include <QImage>

#include "IconBtn.h"

class TopPanel : public QWidget {
    Q_OBJECT

public:
    explicit TopPanel(QWidget* parent);

private:
    IconBtn* refreshBtn = nullptr;
    IconBtn* settingBtn = nullptr;

    void setupUI();
};
