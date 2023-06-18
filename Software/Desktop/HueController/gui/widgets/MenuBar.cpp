#include "MenuBar.h"

MenuBar::MenuBar(QWidget *parent) : QWidget(parent) {
    setupUI();
}

MenuBar::~MenuBar() {
    while (!itemList.empty()) {
        auto* item = itemList.front();
        delete item;
        itemList.pop_front();
    }
}

void MenuBar::setupUI() {
    this->setObjectName("MenuBar");
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    this->setFixedWidth(MenuBarItem::Width);

    auto* vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    vLayout->setAlignment(Qt::AlignTop);
    this->setLayout(vLayout);
}

void MenuBar::addMenuItem(const QString &text, const QString &defaultIconPath, const QString &selectedIconPath) {
    auto* item = new MenuBarItem(this, text, defaultIconPath, selectedIconPath);
    itemList.push_back(item);
    if (!itemList.empty()) {
        itemList.front()->setChecked(true);
        selectedIndex = 0;
    }
    this->layout()->addWidget(item);
}
