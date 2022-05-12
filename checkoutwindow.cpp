#include "checkoutwindow.h"
#include "ui_checkoutwindow.h"
#include "MenuList.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QDateTime>
#include <QTimeZone>
#include <QSysInfo>
#include <QDir>
#include <QTextStream>

QStringList PromoCodesName;
double baseTotal, deliveryCharge, paymentCharge, discount, grandTotal, valueAddedTax;
QString selectedPromoCode = "(none)";

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
        selectedPromoCode = promoCodeIndex->promoName;
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
    QString receipt = generateReceipt();
    QMessageBox orderSuccessful;
    orderSuccessful.setText("Your order is successful");
    orderSuccessful.setInformativeText("Thank you for buying at <restaurantname>. A recipe has been generated for your convenience.  You may opt to view the recipe or enter a new order.");
    orderSuccessful.setWindowTitle("Order Success");


    QAbstractButton *newOrderButton = orderSuccessful.addButton("New Order", QMessageBox::YesRole);
    QAbstractButton *viewReceiptButton = orderSuccessful.addButton("View My Receipt", QMessageBox::NoRole);


    orderSuccessful.setIcon(QMessageBox::Information);
    orderSuccessful.exec();

}

QString CheckoutWindow::generateReceipt(){
    //DATE AND TIME
    QDateTime now = QDateTime::currentDateTime();
    QString date = now.date().toString("yyyy-MM-dd");
    QString time = now.time().toString("hh-mm-ss");
    //auto timeZone = now.timeZone();

    //ORDER TRANSACTION NUMBER
    QString transactionCode = generateTransactionCode();

    //GET CASHER NAME
    QString hostName = QSysInfo::machineHostName();

    qDebug() << now.toString() << transactionCode << hostName;

    //GET BUSINESS INFO
    QString restaurantName = "TEST RESTAURANT";
    QString businessName = "TEST COMPANY";
    QString taxIdentificationNumber = "TEST TIN";


    //GENERATE LOG INFO
    QDir dir;
    QString logFilePathAbsolute = dir.absolutePath();
    QString logFilePath = logFilePathAbsolute + "/transactionHistory/" + date;
    QString logFileName = logFilePath + "/order-" + transactionCode + ".dat";

    if(!QDir(logFilePath).exists()){
        dir.mkpath(logFilePath);
        qDebug() << "Created new file path at " << logFilePath;
    }
    qDebug() << "Writing file at " << logFilePath;

    QFile logFile(logFileName);
    if(!logFile.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,"","TEST");
    }

    QTextStream input(&logFile);

    input << "Transaction ID:\t" << transactionCode << "\n";
    input << "Transaction Date:\t" << date << "\n";
    input << "Transaction Time:\t" << time << "\n\n";
    input << "Orders:\n";

    int totalNumberofItems=0;
    for(int i=0; i<CurrentOrders.size();i++){
        input << "\t" << get<0>(CurrentOrders[i]) << "\t" << get<2>(CurrentOrders[i])  <<" \t" << get<1>(CurrentOrders[i]) / get<2>(CurrentOrders[i]) << "\t" << get<1>(CurrentOrders[i]) << "\t"  << "\n";
        totalNumberofItems++;
    }
    input << "Total Number of Items:\t" << totalNumberofItems << "\n\n";

    input << "Subtotal:\t" << baseTotal << "\n";
    input << "Delivery Charge:\t" << deliveryCharge<< "\n";
    input << "Payment Charge:\t" << paymentCharge<< "\n";
    input << "Value Added Tax:\t" << valueAddedTax<< "\n";
    input << "Discount:\t" << discount << "\n";
    input << "Grand Total:\t" << grandTotal << "\n";
    input << "Paid Amount:\t" << 0 << "\n";
    input << "Change:\t" << 0 << "\n\n";



    input << "Cashier:\t" << hostName << "\n";
    input << "Payment Method:\t" << ui->PaymentMethod->currentText() <<"\n";
    input << "Delivery Method:\t" << ui->DeliveryMethod->currentText()<< "\n";

    input << "Promo Code:\t" << selectedPromoCode << "\n\n";




    logFile.flush();
    logFile.close();

    return logFileName;

}

QString CheckoutWindow::generateTransactionCode(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    QString transactionCode;
    transactionCode.reserve(len);

    for (int i = 0; i < len; ++i) {
        transactionCode += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return transactionCode;
}
