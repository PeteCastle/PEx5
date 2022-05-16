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
#include <QStandardItemModel>
#include "inventorywindow.h"


QStringList PromoCodesName;
double baseTotal, deliveryCharge, paymentCharge, discount, grandTotal, valueAddedTax;
QString selectedPromoCode = "(none)";
QWidget *previous;

CheckoutWindow::CheckoutWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CheckoutWindow)
{
    ui->setupUi(this);

    previous = parent;

    for(int i=0; i<PaymentsList.size();i++){
        ui->PaymentMethod->addItem(PaymentsList[i].paymentTypeName);
    }
    for(int i=0; i<PromosList.size();i++){
        PromoCodesName.append(PromosList[i].promoName);
    }
    for(int i=0; i<DeliveryList.size();i++){
        ui->DeliveryMethod->addItem(DeliveryList[i].deliveryName);
    }



    QStandardItemModel *standardModel = new QStandardItemModel(this);
    standardModel->setColumnCount(3);
    QStringList headerLabels = {"Item", "Amount", "Subtotal"};
    standardModel->setHorizontalHeaderLabels(headerLabels);

    int i;
    for(i=0; i<CurrentOrders.size();i++){

        QDir *dir = new QDir();
        QString resourcesFileAbsolute = dir->absolutePath();

        QStandardItem *productName = new QStandardItem(QIcon(resourcesFileAbsolute + "/pictures/" + get<0>(CurrentOrders[i]) + ".png"), get<0>(CurrentOrders[i]));
        QStandardItem *productAmountSold = new QStandardItem(QString::number(get<2>(CurrentOrders[i])));
        QStandardItem *productTotalSales = new QStandardItem(QString::number(get<1>(CurrentOrders[i])));
        productAmountSold->setTextAlignment(Qt::AlignCenter);

        standardModel->setItem(i,0,productName);
        standardModel->setItem(i,1,productAmountSold);
        standardModel->setItem(i,2,productTotalSales);

        baseTotal+=get<1>(CurrentOrders[i]);
    }
    standardModel->setItem(i,0,new QStandardItem("SUBTOTAL:"));
    standardModel->setItem(i,1,new QStandardItem(QString::number(baseTotal)));


    ui->treeView->setModel(standardModel);
    ui->treeView->setIconSize(QSize(20,20));
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->treeView->header()->setStretchLastSection(false);
    ui->treeView->setIndentation(0);

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
    if(PaymentsList[index].paymentTypeName!="Cash"){
        ui->PaymentAmount->setVisible(false);
        ui->PaymentAmountLabel->setVisible(false);
    }else{
        ui->PaymentAmount->setVisible(true);
        ui->PaymentAmountLabel->setVisible(true);
    }
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
    if(ui->PaymentMethod->currentText()=="Cash"){
        QString paymentAmountString = ui->PaymentAmount->text();
        if(paymentAmountString==""){
            QMessageBox::warning(nullptr, "Payment Amount Needed", "For cash payments, payment amount shall be provided during checkout.");
            return;
        }
        for(int i=0; i<paymentAmountString.size();i++){
            if(paymentAmountString[i].isLetter()){
                QMessageBox::warning(nullptr,"Error", "Invalid price amount.  Please enter numbers and/or decimals only.");
                return;
            }
        }
        double paymentAmount = paymentAmountString.toDouble();
        if(paymentAmount < grandTotal){
            QMessageBox::warning(nullptr,"Error", "Payment amount must be greater than total price.");
            return;
        }
    }
    QString receipt = generateReceipt();
    QMessageBox orderSuccessful;
    orderSuccessful.setText("Your order is successful");
    orderSuccessful.setInformativeText("Thank you for buying at Tom's Grill. A receipt has been generated for your convenience.  You may opt to view the receipt or enter a new order.");
    orderSuccessful.setWindowTitle("Order Success");


    QAbstractButton *newOrderButton = orderSuccessful.addButton("New Order", QMessageBox::NoRole);
    QAbstractButton *viewReceiptButton = orderSuccessful.addButton("View My Receipt", QMessageBox::YesRole);

    orderSuccessful.setIcon(QMessageBox::Information);
    orderSuccessful.exec();

    if(orderSuccessful.clickedButton()==viewReceiptButton){
        ViewReceipt *viewReceipt = new ViewReceipt(this, receipt);
        viewReceipt->setWindowTitle("Receipt");
        viewReceipt->exec();
    }

    updateInventoryCount();
    CurrentOrders.clear();
    this->close();
    previous->close();
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

    QTextStream output(&logFile);

    output << "Transaction ID:\t" << transactionCode << "\n";
    output << "Transaction Date:\t" << date << "\n";
    output << "Transaction Time:\t" << time << "\n";

    int totalNumberofItems=0;
    for(int i=0; i<CurrentOrders.size();i++){
        output << "Orders:\t" << get<0>(CurrentOrders[i]) << "\t" << get<2>(CurrentOrders[i])  <<" \t" << get<1>(CurrentOrders[i]) / get<2>(CurrentOrders[i]) << "\t" << get<1>(CurrentOrders[i]) << "\t"  << "\n";
        totalNumberofItems++;
    }
    output << "Total Number of Items:\t" << totalNumberofItems << "\n";
    output << "Subtotal:\t" << baseTotal << "\n";
    output << "Delivery Charge:\t" << deliveryCharge<< "\n";
    output << "Payment Charge:\t" << paymentCharge<< "\n";
    output << "Value Added Tax:\t" << valueAddedTax<< "\n";
    output << "Discount:\t" << discount << "\n";
    output << "Grand Total:\t" << grandTotal << "\n";
    if(ui->PaymentMethod->currentText()=="Cash"){
        output << "Paid Amount:\t" << ui->PaymentAmount->text().toDouble() << "\n";
        output << "Change:\t" << ui->PaymentAmount->text().toDouble()-grandTotal << "\n";
    }
    else{
        output << "Paid Amount:\t" << grandTotal << "\n";
        output << "Change:\t" << 0 << "\n";
    }
    output << "Cashier:\t" << hostName << "\n";
    output << "Payment Method:\t" << ui->PaymentMethod->currentText() <<"\n";
    output << "Delivery Method:\t" << ui->DeliveryMethod->currentText()<< "\n";
    output << "Promo Code:\t" << selectedPromoCode << "\n\n";

    logFile.flush();
    logFile.close();

    return logFileName;
}

void CheckoutWindow::updateInventoryCount(){
    for(int i=0; i<CurrentOrders.size();i++){
        QString orderName = get<0>(CurrentOrders[i]);
        auto promoCodeIndex = std::find_if(MenuList.begin(),MenuList.end(),[&orderName](MenuDetails & m){ return m.name == orderName; });
        promoCodeIndex->supply -= get<2>(CurrentOrders[i]);
    }
    InventoryWindow inventoryWindow;
    inventoryWindow.saveSuppliesToFile();
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
