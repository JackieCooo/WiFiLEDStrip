#include "MenuBarItem.h"

MenuBarItem::MenuBarItem(QWidget *parent) : QAbstractButton(parent) {
    this->setupUI();
}

MenuBarItem::MenuBarItem(QWidget *parent, const QString &text, const QString &defaultIconPath, const QString &selectedIconPath) : QAbstractButton(parent) {
    this->setText(text);
    this->defaultIcon.load(defaultIconPath);
    if (selectedIconPath != nullptr) this->selectedIcon.load(selectedIconPath);
    this->setupUI();
}

void MenuBarItem::setupUI() {
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFixedSize(MenuBarItem::Width, MenuBarItem::Height);
    this->setCheckable(true);
    this->setMouseTracking(true);
}

void MenuBarItem::paintEvent(QPaintEvent *e) {
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing);

    if (hovered || this->isChecked()) {
        p.setBrush(QBrush(QColor(Qt::white)));
        p.setPen(QPen(Qt::PenStyle::NoPen));
        p.drawRect(e->rect());

        p.drawImage((MenuBarItem::Width - this->selectedIcon.width()) / 2, (MenuBarItem::Height - this->selectedIcon.height()) / 2, this->selectedIcon);
    }
    else {
        p.drawImage((MenuBarItem::Width - this->defaultIcon.width()) / 2, (MenuBarItem::Height - this->defaultIcon.height()) / 2, this->defaultIcon);
    }
}

bool MenuBarItem::event(QEvent *e) {
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
