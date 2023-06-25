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
#include <QDebug>

#include "DeviceListViewport.h"
#include "DeviceListItem.h"

class DeviceList : public QAbstractScrollArea {
    Q_OBJECT

public:
    static constexpr int Width = 300;

    explicit DeviceList(QWidget* parent);
    void addItem(unsigned int pid, const QString& text, const QString& imgPath = nullptr);
    [[nodiscard]] qsizetype itemNum() const;

signals:
    void deviceSelected(unsigned int pid);

protected:
    void wheelEvent(QWheelEvent* e) override;
    [[nodiscard]] QSize viewportSizeHint() const override;

private:
    int lastSelected = -1;
    QButtonGroup itemList;
    DeviceListViewport* viewport = nullptr;

    void setupUI();
    void setupListener();

private slots:
    void handleItemSelected(int id);
};
