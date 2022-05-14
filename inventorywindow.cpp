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


vector<struct MenuDetails> menuTemp;
int mode1;



InventoryWindow::InventoryWindow(QWidget *parent, int mode) :
    QMainWindow(parent),
    ui(new Ui::InventoryWindow)
{
    ui->setupUi(this);

    ui->CategoriesList->addItems(CategoriesList);
    menuTemp = MenuList;
    mode1 = mode;
    if(mode==2){
        ui->SaveButton->setText("Close");
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
            menuName->setAlignment(Qt::AlignCenter);
            QLabel *menuPicture = new QLabel("Placeholder Picture");
            QPixmap picture(":/pictures/pictures/NoThumbnail.jpg"); //Change later to suit menus
            picture = picture.scaledToWidth(250, Qt::SmoothTransformation);
            menuPicture->setAlignment(Qt::AlignCenter);
            menuPicture->setPixmap(picture);

            QSpacerItem *spacer = new QSpacerItem(0,0,QSizePolicy::Fixed,QSizePolicy::Expanding);
            subMenuLayout->addItem(spacer);
            subMenuLayout->addWidget(menuName);
            subMenuLayout->addWidget(menuPicture);
            QHBoxLayout *subQuantityMenuLayout = new QHBoxLayout();


            QPushButton *addButton = new QPushButton("+");
            QLabel *menuSupplyQuantity = new QLabel(QString::number(menuTemp[i].supply));
            menuSupplyQuantity->setAlignment(Qt::AlignCenter);
            QPushButton *subtractButton = new QPushButton("-");

            if(mode1==1){
                subQuantityMenuLayout->addWidget(subtractButton);
            }
            subQuantityMenuLayout->addWidget(menuSupplyQuantity);
            if(mode1==1){
                subQuantityMenuLayout->addWidget(addButton);
            }


            subQuantityMenuLayout->setContentsMargins(QMargins(20,0,20,0));

            subMenuLayout->addLayout(subQuantityMenuLayout);
            QSpacerItem *spacer1 = new QSpacerItem(0,0,QSizePolicy::Fixed,QSizePolicy::Expanding);
            subMenuLayout->addItem(spacer1);

            QWidget *subMenuWidget = new QWidget();
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
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Save", "Are you sure you want to save any changes made?", QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){
            MenuList = menuTemp;

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
        else{
            QMessageBox::information(this,"Information","Any changes were not saved.");
        }
    }
    this->close();
}

