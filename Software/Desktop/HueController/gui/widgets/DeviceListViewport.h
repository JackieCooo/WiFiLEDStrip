#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QSizePolicy>

class DeviceListViewport : public QWidget {
    Q_OBJECT

public:
    static constexpr int ItemSpacing = 20;

    explicit DeviceListViewport(QWidget* parent);

private:
    void setupUI();
};
