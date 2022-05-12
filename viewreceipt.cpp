#include "viewreceipt.h"
#include "ui_viewreceipt.h"

ViewReceipt::ViewReceipt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewReceipt)
{
    ui->setupUi(this);
}

ViewReceipt::~ViewReceipt()
{
    delete ui;
}
