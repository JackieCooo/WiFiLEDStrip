#include "IconBtn.h"

IconBtn::IconBtn(QWidget *parent) : QAbstractButton(parent) {
    this->setupUI();
}

IconBtn::IconBtn(QWidget *parent, const QString &text, const QString &iconPath) : QAbstractButton(parent) {
    this->setText(text);
    this->setIcon(iconPath);
    this->setupUI();
}

void IconBtn::setupUI() {
    this->setObjectName("IconBtn");
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFixedSize(IconBtn::Width, IconBtn::Height);
    this->setMouseTracking(true);
}

void IconBtn::paintEvent(QPaintEvent *e) {
    QPainter p(this);

    if (hovered) {
        p.setBrush(QBrush(QColor(0x303030)));
        p.setPen(QPen(Qt::PenStyle::NoPen));
        p.drawRoundedRect(e->rect(), 10, 10);
        this->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    }
    else {
        p.setBrush(QBrush(QColor(0x262727)));
        p.setPen(QPen(Qt::PenStyle::NoPen));
        p.drawRoundedRect(e->rect(), 10, 10);
        this->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
    }

    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    p.drawImage(IconBtn::Padding, (IconBtn::Height - this->getIcon().height()) / 2, this->getIcon());

    p.setPen(QPen(Qt::white));
    p.setFont(QFont("Arial", 12, QFont::Normal));
    QFontMetrics fm = p.fontMetrics();
    QRect r = fm.boundingRect(this->text());
    p.drawText(IconBtn::Width - IconBtn::Padding - r.width(), (IconBtn::Height - r.height()) / 2 + r.height() - fm.descent(), this->text());
}

void IconBtn::setIcon(const QString &filepath) {
    this->img.load(filepath);
}

QImage IconBtn::getIcon() const {
    return this->img;
}

bool IconBtn::event(QEvent *e) {
    if (e->type() == QEvent::Enter) {
        this->hovered = true;
        update();
    }
    else if (e->type() == QEvent::Leave) {
        this->hovered = false;
        update();
    }

    return QAbstractButton::event(e);
}

