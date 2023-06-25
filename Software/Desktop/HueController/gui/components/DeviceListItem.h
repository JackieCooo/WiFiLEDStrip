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
    static constexpr int Height = 100;

    explicit DeviceListItem(QWidget* parent);
    DeviceListItem(QWidget* parent, unsigned int pid, const QString& text, const QString& imgPath = nullptr);
    [[nodiscard]] unsigned int getPid() const;
    void setPid(unsigned int pid);

protected:
    void paintEvent(QPaintEvent* e) override;
    bool event(QEvent* e) override;

private:
    bool hovered = false;
    QImage img;
    unsigned int pid;

    void setupUI();
};
