#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MenuList.h"
#include <QPushButton >
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QIcon>


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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    for(int i=0; i<MenuList.size();i++){
        if(MenuList[i].category==CategoriesList[index.row()]){
            QPushButton *newButton = new QPushButton(this);
            newButton->setText(MenuList[i].name);
            layout->addWidget(newButton);
            qDebug() << MenuList[i].category <<  CategoriesList[index.row()];
        }

    }

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

