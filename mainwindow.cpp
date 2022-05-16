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
#include <QDir>

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
        QStandardItem *newItem = new QStandardItem(filename);
        newItem->setTextAlignment(Qt::AlignCenter);
        stdModel->appendRow(newItem);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{

    //QVBoxLayout *layout = new QVBoxLayout(this);
    QGridLayout *layout = new QGridLayout(this);
    QSignalMapper *signalMapper = new QSignalMapper(this);
    qDebug() << "User clicked list view";

    int row=0, col=0;

    QDir *dir = new QDir();
    QString resourcesFileAbsolute = dir->absolutePath();

    for(int i=0; i<MenuList.size();i++){
        if(MenuList[i].category==CategoriesList[index.row()]){
            QPushButton *newButton = new QPushButton(this);
            QPixmap pic(resourcesFileAbsolute + "/pictures/" + MenuList[i].name + ".png");
            QIcon icon(pic);
            newButton->setStyleSheet("QPushButton{"
                                     "border:0px;"
                                     "padding:0px;"
                                     "image-position:top;"
                                     "text-align:bottom;"
                                     "}"
                                     "QPushButton{"
                                         "background-color: rgb(249, 204, 158);"
                                         "color: rgb(89, 52, 27);"
                                         "border-radius: 10px;"
                                     "}"
                                     "QPushButton::hover{"
                                         "border: 5px solid rgb(138, 80, 42);"
                                     "}"
                                     "QPushButton::pressed{"
                                         "border: 5px solid rgb(89, 52, 27);"
                                     "}");
            //newButton->setText(MenuList[i].name);
            newButton->setIcon(icon);
            newButton->setIconSize(QSize(210,210));
            layout->addWidget(newButton,row,col);
            connect(newButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(newButton, MenuList[i].name + "|" + QString::number(MenuList[i].price) + "|" + QString::number(MenuList[i].supply));
            if(col==1){
                row++;
                col=0;
            }
            else{
                col++;
            }
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

    QWidget *mainWidget = new QWidget(this);
    mainWidget->setLayout(layout);

    ui->scrollArea->setWidget(mainWidget);
    //ui->scrollArea->layout()->update();

    }


void MainWindow::confirmAddtoCart(QString parameters){
    QStringList splitParameters = parameters.split('|');

    if(splitParameters[2].toInt()==0){
        QMessageBox::information(this, "Out of stock",  "This item is currently out of stock!");
        return;
    }

    ConfirmAddToCart *confirmAddtoCartWindow = new ConfirmAddToCart(this, splitParameters[0], splitParameters[1].toDouble(), splitParameters[2].toInt());
    confirmAddtoCartWindow->setWindowModality(Qt::ApplicationModal);
    confirmAddtoCartWindow->exec();

    tuple<QString,double,int> orderParameters = confirmAddtoCartWindow->getOrderParameters();

    if(get<2>(orderParameters)!=0){
        CurrentOrders.push_back(orderParameters);
        updateCartMenu();
    }

}

void MainWindow::updateCartMenu(){
    QStandardItemModel *standardModel = new QStandardItemModel(this);
    standardModel->setColumnCount(3);
    QStringList headerLabels = {"Item", "Amount", "Subtotal"};
    standardModel->setHorizontalHeaderLabels(headerLabels);
    for(int i=0; i<CurrentOrders.size();i++){

        QDir *dir = new QDir();
        QString resourcesFileAbsolute = dir->absolutePath();

        QStandardItem *productName = new QStandardItem(QIcon(resourcesFileAbsolute + "/pictures/" + get<0>(CurrentOrders[i]) + ".png"), get<0>(CurrentOrders[i]));
        QStandardItem *productAmountSold = new QStandardItem(QString::number(get<2>(CurrentOrders[i])));
        QStandardItem *productTotalSales = new QStandardItem(QString::number(get<1>(CurrentOrders[i])));
        productAmountSold->setTextAlignment(Qt::AlignCenter);

        standardModel->setItem(i,0,productName);
        standardModel->setItem(i,1,productAmountSold);
        standardModel->setItem(i,2,productTotalSales);
    }
    ui->treeView->setModel(standardModel);
    ui->treeView->setIconSize(QSize(20,20));
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->header()->setDefaultAlignment(Qt::AlignCenter);
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->treeView->header()->setStretchLastSection(false);
    ui->treeView->setIndentation(0);
}
void MainWindow::on_CheckoutButton_clicked()
{
    if(!CurrentOrders.empty()){
        CheckoutWindow *checkoutWindow = new CheckoutWindow(this);
        checkoutWindow->setWindowModality(Qt::ApplicationModal);
        checkoutWindow->show();
    }
    else{
        QMessageBox::information(this, "Empty cart", "Please select an order before checking out.");
    }

}


void MainWindow::on_pushButton_clicked()
{
    int index = ui->treeView->currentIndex().row();
    if(index ==-1){
        return;
    }
    CurrentOrders.erase(CurrentOrders.begin()+index);
    updateCartMenu();
}

