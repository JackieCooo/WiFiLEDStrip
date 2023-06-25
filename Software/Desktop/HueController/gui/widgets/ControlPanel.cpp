#include "ControlPanel.h"

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent) {
    this->setupUI();
    this->setupListener();
}

void ControlPanel::setupUI() {
    this->setObjectName("ControlPanel");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    auto* vLayout = new QVBoxLayout(this);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setSpacing(10);
    vLayout->setContentsMargins(10, 10, 10, 10);

    this->setLayout(vLayout);
}

void ControlPanel::paintEvent(QPaintEvent *event) {
    QPainter p(this);

    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    p.setBrush(this->palette().dark());
    p.setPen(QPen(Qt::PenStyle::NoPen));
    p.drawRoundedRect(event->rect(), 10, 10);

    if (!state) {
        p.setPen(QPen(Qt::white));
        p.setFont(QFont("Arial", 10, QFont::Normal));
        QFontMetrics fm = this->fontMetrics();
        QRect rect = fm.boundingRect("Select a product");
        p.drawText((event->rect().width() - rect.width()) / 2, (event->rect().height() - rect.height()) / 2 + fm.ascent(), "Select a product");
    }
}

void ControlPanel::setState(bool state) {
    this->state = state;
    update();
}

bool ControlPanel::getState() const {
    return this->state;
}

void ControlPanel::handleDeviceSelected(unsigned int pid) {
    qDebug("pid: %d", pid);
    this->setState(true);
    this->showControlOption(pid);
}

void ControlPanel::setupListener() {
    auto* deviceList = this->parent()->findChild<DeviceList*>("DeviceList");
    connect(deviceList, SIGNAL(deviceSelected(uint)), this, SLOT(handleDeviceSelected(uint)));
}

void ControlPanel::showControlOption(unsigned int pid) {
    auto* layout = this->layout();
    clearLayoutWidgets();

    if (pid == PidUtils::SmartLight) {
        auto* colorLabel = new QLabel("Color: ", this);
        colorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        colorLabel->setFont(QFont("Arial", 14, QFont::Medium));
        layout->addWidget(colorLabel);
    }
}

void ControlPanel::clearLayoutWidgets() {
    auto* layout = this->layout();
    auto widgets = layout->children();
    for (auto widget : widgets) {
        layout->removeWidget(dynamic_cast<QWidget*>(widget));
    }
}
