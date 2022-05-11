#include "checkoutwindow.h"
#include "ui_checkoutwindow.h"
#include "MenuList.h"
#include <QDebug>
#include <QMessageBox>
#include <iostream>
QStringList PromoCodesName;
double baseTotal, deliveryCharge, paymentCharge, discount, grandTotal, valueAddedTax;

CheckoutWindow::CheckoutWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CheckoutWindow)
{
    ui->setupUi(this);

    for(int i=0; i<PaymentsList.size();i++){
        ui->PaymentMethod->addItem(PaymentsList[i].paymentTypeName);
    }
    for(int i=0; i<PromosList.size();i++){
        PromoCodesName.append(PromosList[i].promoName);
    }
    for(int i=0; i<DeliveryList.size();i++){
        ui->DeliveryMethod->addItem(DeliveryList[i].deliveryName);
    }

    ui->tableWidget->setRowCount(CurrentOrders.size()+1);
    ui->tableWidget->setColumnCount(3);
    for(int i=0; i<CurrentOrders.size();i++){


        QTableWidgetItem *newItemName = new QTableWidgetItem();
        newItemName->setText(get<0>(CurrentOrders[i]));
        ui->tableWidget->setItem(i, 0, newItemName);

        QTableWidgetItem *newItemQuantity = new QTableWidgetItem(QString::number(get<1>(CurrentOrders[i])));
        ui->tableWidget->setItem(i, 2, newItemQuantity);

        QTableWidgetItem *newItemPrice = new QTableWidgetItem(QString::number(get<2>(CurrentOrders[i])));
        ui->tableWidget->setItem(i, 1, newItemPrice);

        baseTotal+=get<1>(CurrentOrders[i]);

    }
    QTableWidgetItem *subTotalLabel = new QTableWidgetItem("SUBTOTAL:");
    ui->tableWidget->setItem(CurrentOrders.size(), 1, subTotalLabel);
    QTableWidgetItem *subTotal = new QTableWidgetItem(QString::number(baseTotal));
    ui->tableWidget->setItem(CurrentOrders.size(), 2, subTotal);

    valueAddedTax=baseTotal*0.12;
    computeChargesAndValues();
}

void CheckoutWindow::computeChargesAndValues(){
    ui->AdditionalCharge->setText(QString::number(deliveryCharge+paymentCharge));
    ui->VAT->setText(QString::number(valueAddedTax));
    ui->Discounts->setText(QString::number(discount));
    grandTotal = baseTotal+deliveryCharge+paymentCharge-discount;

    ui->GrandTotal->setText(QString::number(grandTotal));
}



CheckoutWindow::~CheckoutWindow()
{
    delete ui;
}

void CheckoutWindow::on_PaymentMethod_currentIndexChanged(int index)
{
    paymentCharge=baseTotal*(PaymentsList[index].paymentTypeCharge/100);
    computeChargesAndValues();
}


void CheckoutWindow::on_DeliveryMethod_currentIndexChanged(int index)
{
    deliveryCharge=DeliveryList[index].deliveryCharge;
    computeChargesAndValues();
}


void CheckoutWindow::on_PromoCodeApplyButton_clicked()
{
    QString promoCode = ui->PromoCode->text();
    if(promoCode==""){
        return;
    }

    auto promoCodeIndex = std::find_if(PromosList.begin(),PromosList.end(),[&promoCode](PromoCodes & m){ return m.promoName == promoCode; });
    if(promoCodeIndex!=PromosList.end()){
        qDebug() << "TEST";
        discount = baseTotal*(promoCodeIndex->discountQuantity/100);
        QMessageBox::information(this,"Promo Code Success","A discount of " + QString::number(discount) + " (" + QString::number(promoCodeIndex->discountQuantity) +"%) has been deducted to your subtotal.  Note that promo codes cannot be used in conjunction with other promo codes.");
        computeChargesAndValues();
    }
    else{
        QMessageBox::information(this,"Promo Code not Found","The promo code you've entered is not valid.  Please try again.");
        ui->PromoCode->setText("");
        return;
    }

}

void CheckoutWindow::on_PlacePrderButton_clicked()
{

}

