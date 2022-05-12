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
