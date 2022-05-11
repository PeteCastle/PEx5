#ifndef MENULIST_H
#define MENULIST_H
#include <QString>
#include <QStringList>
using namespace std;


//This header file lists all products located in products.csv file

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
extern vector<struct MenuDetails> MenuList;
extern vector<tuple<QString,double,int>> CurrentOrders;

extern QStringList CategoriesList;
extern vector<struct CollectiveDetails> CollectiveList;

#endif // MENULIST_H
