#include "landingpage.h"
#include "ui_landingpage.h"

LandingPage::LandingPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LandingPage)
{
    ui->setupUi(this);
}

LandingPage::~LandingPage()
{
    delete ui;
}

void LandingPage::on_customerButton_clicked()
{
    MainWindow *customerWindow = new MainWindow(this);
    customerWindow->show();
    this->hide();
}


void LandingPage::on_adminButton_clicked()
{
    MainAdminWindow *adminWindow = new MainAdminWindow(this);
    adminWindow->show();
    this->hide();
}

