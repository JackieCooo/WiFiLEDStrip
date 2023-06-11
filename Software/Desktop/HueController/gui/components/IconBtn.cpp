#include "IconBtn.h"

#define BTN_WIDTH               120
#define BTN_HEIGHT              50
#define BTN_PADDING             10

IconBtn::IconBtn(QWidget *parent) : QAbstractButton(parent) {
    setupUI();
}

void IconBtn::setupUI() {
    this->setObjectName("IconBtn");
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFixedSize(BTN_WIDTH, BTN_HEIGHT);
}

void IconBtn::paintEvent(QPaintEvent *e) {
    QPainter p(this);

    p.drawRect(this->rect());

    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    p.drawImage(0, 0, this->getIcon());

    p.setFont(QFont("Arial", 14, QFont::Normal));
    QFontMetrics fm = p.fontMetrics();
    QRect r = fm.boundingRect(this->text());
    p.drawText(BTN_WIDTH - BTN_PADDING - r.width(), (BTN_HEIGHT - r.height()) / 2 + r.height() - fm.descent(), this->text());
}

void IconBtn::setIcon(const QString &filepath) {
    img.load(filepath);
}

QImage IconBtn::getIcon() const {
    return this->img;
}


