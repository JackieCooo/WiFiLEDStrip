#include "ControlPanel.h"

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent) {
    this->setupUI();
}

void ControlPanel::setupUI() {
    this->setObjectName("ControlPanel");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    auto* vLayout = new QVBoxLayout(this);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setSpacing(10);
    vLayout->setContentsMargins(10, 10, 10, 10);

    auto* colorLabel = new QLabel("Color: ", this);
    colorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    colorLabel->setFont(QFont("Arial", 14, QFont::Medium));
    vLayout->addWidget(colorLabel);
}

void ControlPanel::paintEvent(QPaintEvent *event) {
    QPainter p(this);

    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    p.setBrush(this->palette().dark());
    p.setPen(QPen(Qt::PenStyle::NoPen));
    p.drawRoundedRect(event->rect(), 10, 10);

    p.setPen(QPen(Qt::white));
    p.setFont(QFont("Arial", 10, QFont::Normal));
    QFontMetrics fm = this->fontMetrics();
    QRect rect = fm.boundingRect("Select a product");
    p.drawText((event->rect().width() - rect.width()) / 2, (event->rect().height() - rect.height()) / 2 + fm.ascent(), "Select a product");
}

void ControlPanel::setState(bool state) {
    this->state = state;
}

bool ControlPanel::getState() const {
    return this->state;
}
