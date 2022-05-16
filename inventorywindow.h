#ifndef INVENTORYWINDOW_H
#define INVENTORYWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include "MenuList.h"
/*
struct MenuContainer{
    QLabel *menuName;
    QLabel *menuPicture;
    QPushButton *addButton;
    QLabel *menuSupplyQuantity;
    QPushButton *subtractButton;
    int supplyCache;
};*/


namespace Ui {
class InventoryWindow;
}

class InventoryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit InventoryWindow(QWidget *parent = nullptr, int mode=0); //MODE 0 -> VIEW  //MODE 1 -> MODIFY
    ~InventoryWindow();
    void saveSuppliesToFile();

private slots:
    void on_CategoriesList_clicked(const QModelIndex &index);
    void updateQuantity(QLabel *quantityLabel, int index, int mode);

    void on_SaveButton_clicked();

private:
    Ui::InventoryWindow *ui;
};

#endif // INVENTORYWINDOW_H
