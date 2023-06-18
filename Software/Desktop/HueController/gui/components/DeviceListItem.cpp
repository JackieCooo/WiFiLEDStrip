#include "DeviceListItem.h"

DeviceListItem::DeviceListItem(QWidget *parent) : QAbstractButton(parent) {
    this->setupUI();
}

DeviceListItem::DeviceListItem(QWidget *parent, const QString &text, const QString &imgPath) : QAbstractButton(parent) {
    this->setText(text);
    if (imgPath != nullptr) img.load(imgPath);
    this->setupUI();
}

void DeviceListItem::setupUI() {
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    this->setFixedHeight(DeviceListItem::Height);
    this->setCheckable(true);
    this->setMouseTracking(true);
}

void DeviceListItem::paintEvent(QPaintEvent *e) {
    QPainter p(this);

    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    if (this->isChecked()) {
        p.setBrush(this->palette().base());
        p.setPen(QPen(Qt::PenStyle::NoPen));
        p.drawRoundedRect(e->rect(), 10, 10);
    }
    else if (hovered) {
        p.setBrush(this->palette().mid());
        p.setPen(QPen(Qt::PenStyle::NoPen));
        p.drawRoundedRect(e->rect(), 10, 10);
    }
    else {
        p.setBrush(this->palette().dark());
        p.setPen(QPen(Qt::PenStyle::NoPen));
        p.drawRoundedRect(e->rect(), 10, 10);
    }

    // Draw image
    if (img.isNull()) {
        p.setBrush(QBrush(Qt::gray));
        p.drawRoundedRect(10, 10, 80, 80, 10, 10);
    }
    else {
        p.drawImage(10, (DeviceListItem::Height - img.height()) / 2, img);
    }

    // Draw text
    p.setFont(QFont("Arial", 20, QFont::Bold));
    p.setPen(QPen(Qt::white));
    QFontMetrics fm = p.fontMetrics();
    p.drawText(10 + 80 + 20, 20 + fm.ascent(), this->text());
}

bool DeviceListItem::event(QEvent *e) {
    if (e->type() == QEvent::Enter) {
        this->hovered = true;
        this->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        update();
    }
    else if (e->type() == QEvent::Leave) {
        this->hovered = false;
        this->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        update();
    }

    return QAbstractButton::event(e);
}
