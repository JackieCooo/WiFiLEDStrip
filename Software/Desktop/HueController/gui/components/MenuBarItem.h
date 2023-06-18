#pragma once

#include <QAbstractButton>
#include <QWidget>
#include <QSizePolicy>
#include <QImage>
#include <QPaintEvent>
#include <QPainter>

class MenuBarItem : public QAbstractButton {
    Q_OBJECT

public:
    static const int Width = 50;
    static const int Height = 50;

    explicit MenuBarItem(QWidget* parent);
    MenuBarItem(QWidget* parent, const QString& text, const QString& defaultIconPath, const QString& selectedIconPath = nullptr);

protected:
    void paintEvent(QPaintEvent* e) override;
    bool event(QEvent* e) override;

private:
    bool hovered = false;
    QImage defaultIcon;
    QImage selectedIcon;

    void setupUI();
};
