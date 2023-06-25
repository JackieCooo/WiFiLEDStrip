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
#include <QColor>
#include <QPalette>
#include <QEvent>

class IconBtn : public QAbstractButton {
    Q_OBJECT

public:
    static constexpr int Width = 120;
    static constexpr int Height = 50;
    static constexpr int Padding = 10;

    explicit IconBtn(QWidget* parent);
    IconBtn(QWidget* parent, const QString& text, const QString& iconPath);
    void setIcon(const QString& filepath);
    [[nodiscard]] QImage getIcon() const;

protected:
    void paintEvent(QPaintEvent *e) override;
    bool event(QEvent* e) override;

private:
    bool hovered = false;
    QImage img;

    void setupUI();
};
