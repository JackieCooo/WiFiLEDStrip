#pragma once

#include <QAbstractButton>
#include <QSizePolicy>
#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QFontMetrics>
#include <QRect>
#include <iostream>

class IconBtn : public QAbstractButton {
    Q_OBJECT

public:
    explicit IconBtn(QWidget* parent);
    void setIcon(const QString& filepath);
    [[nodiscard]] QImage getIcon() const;

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QImage img;

    void setupUI();
};
