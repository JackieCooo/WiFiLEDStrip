#pragma once

#include <iostream>
#include <QAbstractScrollArea>
#include <QWidget>
#include <QSizePolicy>
#include <QList>
#include <QWheelEvent>
#include <QPoint>
#include <QSize>
#include <QButtonGroup>

#include "DeviceListViewport.h"
#include "DeviceListItem.h"

class DeviceList : public QAbstractScrollArea {
    Q_OBJECT

public:
    static const int Width = 300;

    explicit DeviceList(QWidget* parent);
    void addItem(const QString& text, const QString& imgPath = nullptr);
    [[nodiscard]] qsizetype itemNum() const;

protected:
    void wheelEvent(QWheelEvent* e) override;
    [[nodiscard]] QSize viewportSizeHint() const override;

private:
    QButtonGroup itemList;
    DeviceListViewport* viewport = nullptr;

    void setupUI();
};
