#include "inventorywindow.h"
#include "ui_inventorywindow.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QPixmap>
#include <QSize>
#include <QSpacerItem>
#include <QProgressDialog>
#include <QSignalMapper>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStandardItemModel>
#include <QStringListModel>


vector<struct MenuDetails> menuTemp;
int mode1;



InventoryWindow::InventoryWindow(QWidget *parent, int mode) :
    QMainWindow(parent),
    ui(new Ui::InventoryWindow)
{
    ui->setupUi(this);

    QStringListModel *categoriesListModel = new QStringListModel(this);
    categoriesListModel->setStringList(CategoriesList);
    ui->CategoriesList->setItemAlignment(Qt::AlignCenter);
    ui->CategoriesList->setModel(categoriesListModel);


    menuTemp = MenuList;
    mode1 = mode;
    if(mode==2){
        ui->SaveButton->setText("Close");

    }
    else{
        ui->MenuListLabel->setText("Modify Inventory Count");
        ui->MenuListLabel->setAlignment(Qt::AlignCenter);
        ui->SaveButton->setText("Save and Close");
    }

}

InventoryWindow::~InventoryWindow()
{
    delete ui;
}

void InventoryWindow::on_CategoriesList_clicked(const QModelIndex &index)
{
    QGridLayout *mainLayout = new QGridLayout();

    int row=0, col=0;
    for(int i=0; i<menuTemp.size();i++){
        if(menuTemp[i].category==CategoriesList[index.row()]){
            QVBoxLayout *subMenuLayout = new QVBoxLayout();

            QLabel *menuName = new QLabel(menuTemp[i].name);
            menuName->setStyleSheet("QLabel{	font:  16pt 'Georgia';padding: 5px 5px 5px 5px; font-style:bold}");
            menuName->setAlignment(Qt::AlignCenter);
            QLabel *menuPicture = new QLabel("Placeholder Picture");

            QDir *dir = new QDir();
            QString resourcesFileAbsolute = dir->absolutePath();
            QPixmap pic(resourcesFileAbsolute + "/pictures/" + menuTemp[i].name + ".png");
            auto pics = pic.scaled(QSize(250,250),Qt::KeepAspectRatio,Qt::FastTransformation);

            menuPicture->setAlignment(Qt::AlignCenter);
            menuPicture->setPixmap(pics);

            QSpacerItem *spacer = new QSpacerItem(0,0,QSizePolicy::Fixed,QSizePolicy::Expanding);
            subMenuLayout->addItem(spacer);
            subMenuLayout->addWidget(menuName);
            subMenuLayout->addWidget(menuPicture);
            QHBoxLayout *subQuantityMenuLayout = new QHBoxLayout();


            QPushButton *addButton = new QPushButton();
            addButton->setIcon(QIcon(":/pictures/pictures/697342_plus_512x512.png"));
            addButton->setStyleSheet("QPushButton::hover{background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(255, 255, 255, 255), stop:0.1 rgba(255, 255, 255, 255), stop:0.2 rgba(255, 176, 176, 167), stop:0.3 rgba(255, 151, 151, 92), stop:0.4 rgba(255, 125, 125, 51), stop:0.5 rgba(255, 76, 76, 205), stop:0.52 rgba(255, 76, 76, 205), stop:0.6 rgba(255, 180, 180, 84), stop:1 rgba(255, 255, 255, 0));}");
            addButton->setFixedSize(QSize(30,30));
            QLabel *menuSupplyQuantity = new QLabel(QString::number(menuTemp[i].supply));
            menuSupplyQuantity->setStyleSheet("QLabel{	font:  14pt 'Georgia';padding: 5px 5px 5px 5px;}");
            menuSupplyQuantity->setAlignment(Qt::AlignCenter);
            QPushButton *subtractButton = new QPushButton();
            subtractButton->setIcon(QIcon(":/pictures/pictures/minus icon.png"));
            subtractButton->setStyleSheet("QPushButton::hover{background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(255, 255, 255, 255), stop:0.1 rgba(255, 255, 255, 255), stop:0.2 rgba(255, 176, 176, 167), stop:0.3 rgba(255, 151, 151, 92), stop:0.4 rgba(255, 125, 125, 51), stop:0.5 rgba(255, 76, 76, 205), stop:0.52 rgba(255, 76, 76, 205), stop:0.6 rgba(255, 180, 180, 84), stop:1 rgba(255, 255, 255, 0));}");
            subtractButton->setFixedSize(QSize(30,30));

            subQuantityMenuLayout->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Fixed));
            if(mode1==1){
                subQuantityMenuLayout->addWidget(subtractButton);
            }
            subQuantityMenuLayout->addWidget(menuSupplyQuantity);
            if(mode1==1){
                subQuantityMenuLayout->addWidget(addButton);
            }
            subQuantityMenuLayout->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Fixed));

            subQuantityMenuLayout->setContentsMargins(QMargins(20,0,20,0));

            subMenuLayout->addLayout(subQuantityMenuLayout);
            QSpacerItem *spacer1 = new QSpacerItem(0,0,QSizePolicy::Fixed,QSizePolicy::Expanding);
            subMenuLayout->addItem(spacer1);

            QWidget *subMenuWidget = new QWidget();
            //subMenuWidget->setStyleSheet("QWidget{border: 5px solid rgb(212, 108, 78);border-radius: 5px}");
            subMenuWidget->setLayout(subMenuLayout);

            mainLayout->addWidget(subMenuWidget,row,col);

            if(col==1){
               row++;
               col=0;
            }
            else{
              col++;
            }

            int one=1,two=2;
            connect(addButton, &QPushButton::clicked, [this, menuSupplyQuantity, i, one] { updateQuantity(menuSupplyQuantity,i,one); });
            connect(subtractButton, &QPushButton::clicked, [this, menuSupplyQuantity, i, two] { updateQuantity(menuSupplyQuantity,i,two); });
       }
    }



    QWidget *scrollAreaWidget = new QWidget();
    scrollAreaWidget->setLayout(mainLayout);

    ui->scrollArea->setWidget(scrollAreaWidget);
}

void InventoryWindow::updateQuantity(QLabel *quantityLabel, int index, int mode){
    if(mode==1){

        menuTemp[index].supply +=1;
        quantityLabel->setText(QString::number(menuTemp[index].supply));
    }
    else if(mode==2 && menuTemp[index].supply!=0){
        menuTemp[index].supply -=1;
        quantityLabel->setText(QString::number(menuTemp[index].supply));
    }

}

void InventoryWindow::on_SaveButton_clicked()
{
    if(mode1==1){
        QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Confirm Save", "Are you sure you want to save any changes made?", QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){
            MenuList = menuTemp;
            saveSuppliesToFile();
        }
        else{
            QMessageBox::information(nullptr,"Information","Any changes were not saved.");
        }
    }
    this->close();
}


void InventoryWindow::saveSuppliesToFile(){
    QDir *dir = new QDir();
    QString resourcesFileAbsolute = dir->absolutePath();

    QFile logFile(resourcesFileAbsolute + "/resources/products.csv");
    if(!logFile.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,"","TEST");
    }
    QTextStream output(&logFile);

    for(int i=0; i<MenuList.size();i++){
        output<< MenuList[i].name << "," << MenuList[i].price << "," << MenuList[i].category << "," << MenuList[i].supply << "\n";
    }
    logFile.flush();
    logFile.close();
}


