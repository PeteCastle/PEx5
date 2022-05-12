#ifndef MENULIST_H
#define MENULIST_H
#include <QString>
#include <QStringList>
#include <QDateTime>
using namespace std;

struct MenuDetails{
    QString name;
    QString category;
    double price;
    int supply;
};
struct CollectiveDetails{
    vector<QString> name;
    vector<int> amount;
    double price;
};
struct PromoCodes{
    QString promoName;
    double discountQuantity;
    int discountType;

    //discountType type has two types:
    // Flat (integer value 1) - subtracted from base subtotal
    // Percentage (integer value 2) - total is subtracted on a percentage

};
struct PaymentMethods{
    QString paymentTypeName;
    double paymentTypeCharge;

};
struct DeliveryMethods{
    QString deliveryName;
    double deliveryCharge;
};
struct ReceiptStructure{
    QString transactionID;
    QDateTime transactionDate;
    QDateTime transactionTime;
    QList<QString> orderNames;
    QList<double> orderPricePerQuantity;
    QList<int> orderQuantity;
    QList<double> orderPrice;
    int totalNumberOfItems;
    double subTotal;
    double deliveryCharge;
    double paymentCharge;
    double valueAddedTax;
    double discount;
    double grandTotal;
    double paidAmount;
    double change;
    QString cashier;
    QString paymentMethod;
    QString deliveryMethod;
    QString promoCode;
};

extern vector<struct MenuDetails> MenuList;
extern vector<tuple<QString,double,int>> CurrentOrders;

extern QStringList CategoriesList;
extern vector<struct CollectiveDetails> CollectiveList;
extern vector<struct PaymentMethods> PaymentsList;
extern vector<struct PromoCodes> PromosList;
extern vector<struct DeliveryMethods> DeliveryList;

#endif // MENULIST_H
