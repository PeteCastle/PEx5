#include "viewreceipt.h"
#include "ui_viewreceipt.h"
#include <QFile>
#include <QMessageBox>
#include <QTableWidgetItem>

ViewReceipt::ViewReceipt(QWidget *parent, QString filePath) :
    QDialog(parent),
    ui(new Ui::ViewReceipt)
{
    ui->setupUi(this);


    //GET BUSINESS INFO
    QString restaurantName = "TEST RESTAURANT";
    QString businessName = "TEST COMPANY";

    ReceiptStructure receipt = readReceipt(filePath);
    ui->RestaurantName->setText(restaurantName);
    ui->CompanyName->setText(businessName);
    ui->TransactionIDLabel->setText("Transaction ID: " + receipt.transactionID);
    ui->TransactionDateLabel->setText("Transaction Date: " + receipt.transactionDate.toString("yyyy-MM-dd"));
    ui->TransactionTimeLabel->setText("Transaction Time: " + receipt.transactionTime.toString("hh-mm-ss"));
    ui->TotalNumberofItemsLabel->setText("Total Number of Items: " + QString::number(receipt.totalNumberOfItems));
    ui->SubtotalLabel->setText("Subtotal: " + QString::number(receipt.subTotal));
    ui->DeliveryChargeLabel->setText("Delivery Charge: " + QString::number(receipt.deliveryCharge));
    ui->PaymentChargeLabel->setText("Payment Charge: " +QString::number(receipt.paymentCharge));
    ui->ValueAddedTaxLabel->setText("VAT: "+ QString::number(receipt.valueAddedTax));
    ui->DiscountLabel->setText("Discount: "+ QString::number(receipt.discount));
    ui->GrandTotalLabel->setText("Grand Total: " + QString::number(receipt.grandTotal));
    ui->PaidAmountLabel->setText("Paid Amount: " + QString::number(receipt.paidAmount));
    ui->ChangeLabel->setText("Change: " + QString::number(receipt.change));
    ui->CashierLabel->setText("Cashier: " + receipt.cashier);
    ui->PaymentMethodLable->setText("Payment Method: " + receipt.paymentMethod);
    ui->PromoCodeLabel->setText("Promo Code: " + receipt.promoCode);
    ui->DeliveryMethodLabel->setText("Delivery Method: " + receipt.deliveryMethod);

    ui->tableWidget->setRowCount(receipt.orderNames.size());
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList labels = {"Name","Price","Amount","Subtotal"};
    ui->tableWidget->setHorizontalHeaderLabels(labels);



    for(int i=0; i<receipt.orderNames.size();i++){
        QTableWidgetItem *newItemName = new QTableWidgetItem();
        newItemName->setText(receipt.orderNames[i]);
        newItemName->setFlags(newItemName->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 0, newItemName);


        QTableWidgetItem *newItemPricePerQuantity = new QTableWidgetItem(QString::number(receipt.orderPricePerQuantity[i]));
        newItemPricePerQuantity->setFlags(newItemPricePerQuantity->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 1, newItemPricePerQuantity);

        QTableWidgetItem *newItemQuantity = new QTableWidgetItem(QString::number(receipt.orderQuantity[i]));
        newItemQuantity->setFlags(newItemQuantity->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 2, newItemQuantity);

        QTableWidgetItem *newItemPrice = new QTableWidgetItem(QString::number(receipt.orderPrice[i]));
        newItemPrice->setFlags(newItemPrice->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 3, newItemPrice);

    }
}

ViewReceipt::~ViewReceipt()
{
    delete ui;
}

ReceiptStructure ViewReceipt::readReceipt(QString filePath){
    ReceiptStructure receipt;

    QFile readFile(filePath);

    if(!readFile.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,"","TEST");
    }

    QTextStream input(&readFile);
    while(!input.atEnd()){
        QStringList receiptDetails = input.readLine().split('\t');
        qDebug() << receiptDetails;
        if(receiptDetails[0]=="Transaction ID:"){receipt.transactionID=receiptDetails[1];}
        else if(receiptDetails[0]=="Transaction Date:"){receipt.transactionDate=QDateTime::fromString(receiptDetails[1],"yyyy-MM-dd");}
        else if(receiptDetails[0]=="Transaction Time:"){receipt.transactionTime=QDateTime::fromString(receiptDetails[1],"hh-mm-ss");}
        else if(receiptDetails[0]=="Orders:"){
            receipt.orderNames.append(receiptDetails[1]);
            receipt.orderPricePerQuantity.append(receiptDetails[2].toDouble());
            receipt.orderQuantity.append(receiptDetails[3].toInt());
            receipt.orderPrice.append(receiptDetails[4].toDouble());
        }
        else if(receiptDetails[0]=="Total Number of Items:"){receipt.totalNumberOfItems=receiptDetails[1].toInt();}
        else if(receiptDetails[0]=="Subtotal:"){receipt.subTotal=receiptDetails[1].toDouble();}
        else if(receiptDetails[0]=="Delivery Charge:"){receipt.deliveryCharge=receiptDetails[1].toDouble();}
        else if(receiptDetails[0]=="Payment Charge:"){receipt.paymentCharge=receiptDetails[1].toDouble();}
        else if(receiptDetails[0]=="Value Added Tax:"){receipt.valueAddedTax=receiptDetails[1].toDouble();}
        else if(receiptDetails[0]=="Discount:"){receipt.discount=receiptDetails[1].toDouble();}
        else if(receiptDetails[0]=="Grand Total:"){receipt.grandTotal=receiptDetails[1].toDouble();}
        else if(receiptDetails[0]=="Paid Amount:"){receipt.paidAmount=receiptDetails[1].toDouble();}
        else if(receiptDetails[0]=="Change:"){receipt.change=receiptDetails[1].toDouble();}
        else if(receiptDetails[0]=="Cashier:"){receipt.cashier=receiptDetails[1];}
        else if(receiptDetails[0]=="Payment Method:"){receipt.paymentMethod=receiptDetails[1];}
        else if(receiptDetails[0]=="Delivery Method:"){receipt.deliveryMethod=receiptDetails[1];}
        else if(receiptDetails[0]=="Promo Code:"){receipt.promoCode=receiptDetails[1];}
        else{continue;}
    }
    //qDebug() << receipt.cashier << receipt.change << receipt.deliveryCharge << receipt.deliveryMethod << receipt.cashier
             //<< receipt.discount << receipt.grandTotal;
    return receipt;
}

void ViewReceipt::on_pushButton_clicked()
{
    this->close();
}

