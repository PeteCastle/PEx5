#include "mainadminwindow.h"
#include "ui_mainadminwindow.h"
#include <QInputDialog>
#include <QDir>

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

void MainAdminWindow::on_ModifyPromoCodesButton_clicked()
{
    PromoCodesWindow *promoCodesWindow = new PromoCodesWindow(this);
    promoCodesWindow->setWindowModality(Qt::ApplicationModal);
    promoCodesWindow->show();
}


void MainAdminWindow::on_ChangePasswordButton_clicked()
{
    AskCredentials *askCredentials = new AskCredentials(this,2);
    askCredentials->show();
    askCredentials->setWindowModality(Qt::ApplicationModal);
}

void MainAdminWindow::on_AddProductButton_clicked()
{
    ProductsWindow *productsWindow = new ProductsWindow(this);
    productsWindow->setWindowModality(Qt::ApplicationModal);
    productsWindow->show();
}


void MainAdminWindow::on_AnalyticsButton_clicked()
{
    Analytics *analytics = new Analytics(this);
    analytics->setWindowModality(Qt::ApplicationModal);
    analytics->show();
}

