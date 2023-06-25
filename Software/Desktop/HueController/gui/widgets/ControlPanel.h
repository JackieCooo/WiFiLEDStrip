#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QFontMetrics>
#include <QColor>
#include <QPalette>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QLabel>
#include <QDebug>

#include "DeviceList.h"
#include "PidUtils.h"

class ControlPanel : public QWidget {
    Q_OBJECT

public:
    explicit ControlPanel(QWidget* parent);
    [[nodiscard]] bool getState() const;
    void showControlOption(unsigned int pid);

public slots:
    void setState(bool state);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    bool state = false;

    void setupUI();
    void setupListener();
    void clearLayoutWidgets();

private slots:
    void handleDeviceSelected(unsigned int pid);
};
