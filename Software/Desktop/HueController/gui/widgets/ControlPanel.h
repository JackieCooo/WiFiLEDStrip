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

class ControlPanel : public QWidget {
    Q_OBJECT

public:
    explicit ControlPanel(QWidget* parent);
    [[nodiscard]] bool getState() const;

public slots:
    void setState(bool state);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    bool state = false;

    void setupUI();
};
