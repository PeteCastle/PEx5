#include "inventorywindow.h"
#include "ui_inventorywindow.h"
#include "MenuList.h"

InventoryWindow::InventoryWindow(QWidget *parent, int mode) :
    QMainWindow(parent),
    ui(new Ui::InventoryWindow)
{
    ui->setupUi(this);

    ui->CategoriesList->addItems(CategoriesList);

    for(int i=0; i<MenuList.size();i++){

    }
}

InventoryWindow::~InventoryWindow()
{
    delete ui;
}
