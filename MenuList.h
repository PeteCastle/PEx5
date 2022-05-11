#ifndef MENULIST_H
#define MENULIST_H
#include <QString>
#include <QStringList>
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

extern vector<struct MenuDetails> MenuList;
extern vector<tuple<QString,double,int>> CurrentOrders;

extern QStringList CategoriesList;
extern vector<struct CollectiveDetails> CollectiveList;
extern vector<struct PaymentMethods> PaymentsList;
extern vector<struct PromoCodes> PromosList;
extern vector<struct DeliveryMethods> DeliveryList;

#endif // MENULIST_H
