#include "DeviceList.h"

DeviceList::DeviceList(QWidget *parent) : QAbstractScrollArea(parent) {
    viewport = new DeviceListViewport(this);
    itemList.setExclusive(true);
    this->setupUI();
}

void DeviceList::setupUI() {
    this->setObjectName("DeviceList");
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->setFixedWidth(DeviceList::Width);
    this->setFrameShadow(QFrame::Plain);
    this->setFrameShape(QFrame::NoFrame);
    this->setViewport(viewport);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewportMargins(0, 0, 0, 0);

    addItem("Product1");
    addItem("Product2");
    addItem("Product3");
    addItem("Product4");
    addItem("Product5");
    addItem("Product6");
    addItem("Product7");
    addItem("Product8");
}

void DeviceList::addItem(const QString &text, const QString &imgPath) {
    auto* item = new DeviceListItem(viewport, text, imgPath);

    itemList.addButton(item);
    viewport->setFixedHeight(this->viewportSizeHint().height());
    viewport->layout()->addWidget(item);
}

void DeviceList::wheelEvent(QWheelEvent *e) {
    // Viewport's height is smaller than scroll area's, no need to scroll
    if (viewport->height() <= this->height()) {
        e->accept();
        return;
    }

    QPoint p = e->angleDelta();
    QPoint pos = viewport->pos();

    if (!p.isNull()) {
        int scrollStep = p.y();
        int tarY = pos.y() + scrollStep;

        if (scrollStep > 0 && tarY > 0)
            tarY = 0;
        else if (scrollStep < 0 && tarY < 0 - (viewport->height() - this->height()))
            tarY = 0 - (viewport->height() - this->height());
        viewport->move(pos.x(), tarY);
    }
    e->accept();
}

qsizetype DeviceList::itemNum() const {
    return itemList.buttons().size();
}

QSize DeviceList::viewportSizeHint() const {
    int cnt = (int) itemNum();
    return {this->width(), cnt * DeviceListItem::Height + cnt * DeviceListViewport::ItemSpacing};
}

