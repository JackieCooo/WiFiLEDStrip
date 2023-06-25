#include "DeviceList.h"

DeviceList::DeviceList(QWidget *parent) : QAbstractScrollArea(parent) {
    viewport = new DeviceListViewport(this);
    itemList.setExclusive(true);
    this->setupUI();
    this->setupListener();
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

    addItem(0x00, "Product1");
    addItem(0x01, "Product2");
    addItem(0x02, "Product3");
    addItem(0x03, "Product4");
    addItem(0x04, "Product5");
    addItem(0x05, "Product6");
    addItem(0x06, "Product7");
    addItem(0x07, "Product8");
}

void DeviceList::addItem(const unsigned int pid, const QString &text, const QString &imgPath) {
    auto* item = new DeviceListItem(viewport, pid, text, imgPath);

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

void DeviceList::setupListener() {
    connect(&itemList, SIGNAL(idClicked(int)), this, SLOT(handleItemSelected(int)));
}

void DeviceList::handleItemSelected(int id) {
    if (id == this->lastSelected) return;
//    qDebug("id: %d", id);
    auto* item = dynamic_cast<DeviceListItem*>(itemList.button(id));
    emit deviceSelected(item->getPid());
    this->lastSelected = id;
//    qDebug("pid: %d", item->getPid());
}
