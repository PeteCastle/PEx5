#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MenuList.h"
#include <QPushButton >
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QIcon>
#include <QSignalMapper>
#include <QMessageBox>
#include <QTableWidgetItem>

vector<tuple<QString,double,int>> CurrentOrders;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStandardItemModel *stdModel = new QStandardItemModel(this);
    ui->listView->setModel(stdModel);
    ui->listView->setIconSize(QSize(50,50));

    foreach(QString filename, CategoriesList) {
        /*
        QStringListModel *stdModel = new QStringListModel(this);

        ui->listView->setModel(stdModel);
        qDebug() << filename;*/

        stdModel->appendRow(new QStandardItem(filename));
    }

    QStringList headerNames = {"Product Name","Amount","Subtotal"};
    stdModel->setHorizontalHeaderLabels(headerNames);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setRowCount(1);
    ui->tableView->setColumnCount(3);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QSignalMapper *signalMapper = new QSignalMapper(this);

    for(int i=0; i<MenuList.size();i++){
        if(MenuList[i].category==CategoriesList[index.row()]){
            QPushButton *newButton = new QPushButton(this);
            newButton->setText(MenuList[i].name);
            layout->addWidget(newButton);
            qDebug() << MenuList[i].category <<  CategoriesList[index.row()];

            connect(newButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(newButton, MenuList[i].name + "|" + QString::number(MenuList[i].price) + "|" + QString::number(MenuList[i].supply));

        }
    }
    connect (signalMapper, SIGNAL(mappedString(QString)), this, SLOT(confirmAddtoCart(QString))) ;

    if ( ui->scrollArea->layout() != NULL ){
        QLayoutItem* item;
        while ( ( item = ui->scrollArea->layout()->takeAt( 0 ) ) != NULL ){
            delete item->widget();
            delete item;
        }
        delete ui->scrollArea->layout();
    }

    ui->scrollArea->setLayout(layout);
    ui->scrollArea->layout()->update();

    }


void MainWindow::confirmAddtoCart(QString parameters){
    QStringList splitParameters = parameters.split('|');
    //qDebug() << splitParameters;

    if(splitParameters[2].toInt()==0){
        QMessageBox::information(this, "Out of stock",  "This item is currently out of stock!");
        return;
    }

    ConfirmAddToCart *confirmAddtoCartWindow = new ConfirmAddToCart(this, splitParameters[0], splitParameters[1].toDouble(), splitParameters[2].toInt());
    confirmAddtoCartWindow->setWindowModality(Qt::ApplicationModal);
    confirmAddtoCartWindow->exec();


    tuple<QString,double,int> orderParameters = confirmAddtoCartWindow->getOrderParameters();

    if(get<2>(orderParameters)!=0){

        int currentRow = ui->tableView->model()->rowCount()-1;
        ui->tableView->setRowCount(currentRow+2);
        qDebug() << currentRow;

        QTableWidgetItem *newItemName = new QTableWidgetItem();
        newItemName->setText(get<0>(orderParameters));
        ui->tableView->setItem(currentRow, 0, newItemName);

        QTableWidgetItem *newItemQuantity = new QTableWidgetItem(QString::number(get<1>(orderParameters)));
        ui->tableView->setItem(currentRow, 1, newItemQuantity);

        QTableWidgetItem *newItemPrice = new QTableWidgetItem(QString::number(get<2>(orderParameters)));
        ui->tableView->setItem(currentRow, 2, newItemPrice);

        CurrentOrders.push_back(orderParameters);
    }
}

