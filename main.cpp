#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include "MenuList.h"
#include <QDebug>

vector<struct MenuDetails> MenuList={};
QStringList CategoriesList={};
vector<struct CollectiveDetails> CollectiveList={};
MenuDetails tempDetails;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Reads all menu from the CSV file
    QFile file(":/files/resources/products.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
          QMessageBox::warning(nullptr, "File not found error", "CSV folder not found.");
          return 0;
    }
    while (!file.atEnd()) {
        QString line = file.readLine();
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
    /*
    for(int i=0; i<MenuList.size();i++){
        qDebug() << MenuList[i].category <<  MenuList[i].name <<  MenuList[i].price <<  MenuList[i].supply;
    }
    qDebug() << CategoriesList;
    */


    MainWindow w;
    w.show();
    return a.exec();
}
