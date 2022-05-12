#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include "MenuList.h"
#include <QDebug>
#include <QFontDatabase>

vector<struct MenuDetails> MenuList={};
vector<struct CollectiveDetails> CollectiveList={};
vector<struct PaymentMethods> PaymentsList;
vector<struct PromoCodes> PromosList;
vector<struct DeliveryMethods> DeliveryList;

MenuDetails tempDetails;
PaymentMethods tempPayment;
PromoCodes tempPromo;
DeliveryMethods tempDelivery;

QStringList CategoriesList={};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/fonts/Monaco.ttf");

    QString productsFileLocation = ":/files/resources/products.csv";
    QString deliveryFileLocation = ":/files/resources/deliverymethods.csv";
    QString paymentsFileLocation = ":/files/resources/paymentmethods.csv";
    QString promosFileLocation = ":/files/resources/promocodes.csv";

    QFile ProductsFile(productsFileLocation);
    if (!ProductsFile.open(QIODevice::ReadOnly | QIODevice::Text)){
          QMessageBox::warning(nullptr, "File not found error", "CSV file " + productsFileLocation + " not found.");
          return 0;
    }
    while (!ProductsFile.atEnd()) {
        QString line = ProductsFile.readLine();
        QStringList details = line.split(',');


        tempDetails.name = details[0];
        tempDetails.price = details[1].toDouble();
        tempDetails.category = details[2];
        tempDetails.supply = details[3].toInt();


        MenuList.push_back(tempDetails);
        if(!CategoriesList.contains(details[2])){
            CategoriesList.append(details[2]);
        }
    }
    ProductsFile.close();

    QFile DeliveryMethodsFile(deliveryFileLocation);
    if (!DeliveryMethodsFile.open(QIODevice::ReadOnly | QIODevice::Text)){
          QMessageBox::warning(nullptr, "File not found error", "CSV file " + deliveryFileLocation + " not found.");
          return 0;
    }
    while (!DeliveryMethodsFile.atEnd()) {
        QString line = DeliveryMethodsFile.readLine();
        QStringList details = line.split(',');

        tempDelivery.deliveryName = details[0];
        tempDelivery.deliveryCharge = details[1].toDouble();

        DeliveryList.push_back(tempDelivery);
    }
    DeliveryMethodsFile.close();

    QFile PromoCodesFile(promosFileLocation);
    if (!PromoCodesFile.open(QIODevice::ReadOnly | QIODevice::Text)){
          QMessageBox::warning(nullptr, "File not found error", "CSV file " + promosFileLocation + " not found.");
          return 0;
    }
    while (!PromoCodesFile.atEnd()) {
        QString line = PromoCodesFile.readLine();
        QStringList details = line.split(',');

        tempPromo.promoName = details[0];
        tempPromo.discountQuantity = details[1].toDouble();
        tempPromo.discountType = details[2].toInt();

        PromosList.push_back(tempPromo);
    }
    DeliveryMethodsFile.close();

    QFile PaymentsListFile(paymentsFileLocation);
    if (!PaymentsListFile.open(QIODevice::ReadOnly | QIODevice::Text)){
          QMessageBox::warning(nullptr, "File not found error", "CSV file " + paymentsFileLocation + " not found.");
          return 0;
    }
    while (!PaymentsListFile.atEnd()) {
        QString line = PaymentsListFile.readLine();
        QStringList details = line.split(',');

        tempPayment.paymentTypeName = details[0];
        tempPayment.paymentTypeCharge = details[1].toDouble();

        PaymentsList.push_back(tempPayment);
    }
    PaymentsListFile.close();


    MainWindow w;
    w.show();
    return a.exec();
}
