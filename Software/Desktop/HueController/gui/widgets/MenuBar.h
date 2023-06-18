#pragma once

#include <QWidget>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QImage>
#include <QString>
#include <QList>

#include "PathUtils.h"
#include "MenuBarItem.h"

class MenuBar : public QWidget {
    Q_OBJECT

public:
    explicit MenuBar(QWidget* parent);
    ~MenuBar() override;
    void addMenuItem(const QString& text, const QString& defaultIconPath, const QString& selectedIconPath = nullptr);

private:
    int selectedIndex = -1;
    QList<MenuBarItem*> itemList;

    void setupUI();
};
