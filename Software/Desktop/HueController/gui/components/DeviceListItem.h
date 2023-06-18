#pragma once

#include <QAbstractButton>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QColor>
#include <QEvent>
#include <QCursor>
#include <QFontMetrics>
#include <QSizePolicy>
#include <QPalette>

class DeviceListItem : public QAbstractButton {
    Q_OBJECT

public:
    static const int Height = 100;

    explicit DeviceListItem(QWidget* parent);
    DeviceListItem(QWidget* parent, const QString& text, const QString& imgPath = nullptr);

protected:
    void paintEvent(QPaintEvent* e) override;
    bool event(QEvent* e) override;

private:
    bool hovered = false;
    QImage img;

    void setupUI();
};
