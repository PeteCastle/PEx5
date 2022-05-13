#include "mainadminwindow.h"
#include "ui_mainadminwindow.h"

MainAdminWindow::MainAdminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainAdminWindow)
{
    ui->setupUi(this);
}

MainAdminWindow::~MainAdminWindow()
{
    delete ui;
}



void MainAdminWindow::on_ModifyInventoryButton_clicked()
{
    InventoryWindow *inventoryWindow = new InventoryWindow(this, 1);
    inventoryWindow->setWindowModality(Qt::ApplicationModal);
    inventoryWindow->show();
}


void MainAdminWindow::on_ShowInventoryButton_clicked()
{
    InventoryWindow *inventoryWindow = new InventoryWindow(this, 2);
    inventoryWindow->setWindowModality(Qt::ApplicationModal);
    inventoryWindow->show();
}

