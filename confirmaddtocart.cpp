#include "confirmaddtocart.h"
#include "ui_confirmaddtocart.h"
#include <QMessageBox>
#include <QListView>


int currentSupply,currentAmount=1;
double currentPrice, pricePerAmount;
QString currentName;
QWidget *previousWindow;

using namespace std;

ConfirmAddToCart::ConfirmAddToCart(QWidget   *parent,QString name, double price, int supply) :
    QDialog (parent),
    ui(new Ui::ConfirmAddToCart)
{
    ui->setupUi(this);
    ui->Name->setText(name);
    ui->Price->setText(QString::number(price));



    previousWindow = parent;

    qDebug() << name << price << supply;
    currentAmount=1;
    currentSupply = supply;
    currentPrice = price;
    pricePerAmount = price;
    currentName = name;

}

ConfirmAddToCart::~ConfirmAddToCart()
{
    delete ui;
}

void ConfirmAddToCart::on_addButton_clicked()
{
    if(currentAmount!=currentSupply){
        currentAmount+=1;
        ui->Amount->setText(QString::number(currentAmount));
        currentPrice+=pricePerAmount;
        ui->Price->setText(QString::number(currentPrice));
    }
    else{
        QMessageBox::information(this, "Out of stock",  "Order amount has alread reached the stock available");
        return;
    }
}


void ConfirmAddToCart::on_subtractButton_clicked()
{
    if(currentAmount!=1){
        currentAmount-=1;
        ui->Amount->setText(QString::number(currentAmount));
        currentPrice-=pricePerAmount;
        ui->Price->setText(QString::number(currentPrice));
    }
}

tuple<QString,double,int> ConfirmAddToCart::getOrderParameters(){
    return make_tuple(currentName, currentPrice, currentAmount);
}

void ConfirmAddToCart::on_AddToCart_clicked()
{
    this->close();
}

void ConfirmAddToCart::on_backButton_clicked()
{
    currentAmount=0;
    this->close();
}

