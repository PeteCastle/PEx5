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
    customerWindow->setWindowModality(Qt::ApplicationModal);
}


void LandingPage::on_adminButton_clicked(){

    AskCredentials *askCredentials = new AskCredentials(this,1);
    askCredentials->setWindowModality(Qt::ApplicationModal);
    askCredentials-
    askCredentials->exec();

    bool loginSuccess = askCredentials->getResult();
    if(loginSuccess){
        MainAdminWindow *adminWindow = new MainAdminWindow(this);

        adminWindow->show();
        adminWindow->setWindowModality(Qt::ApplicationModal);
    }
}

