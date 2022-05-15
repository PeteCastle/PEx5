#include "productswindow.h"
#include "ui_productswindow.h"
#include "MenuList.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QIcon>
#include <QModelIndex>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QModelIndex>


vector<struct MenuDetails> tempMenuList;
QString fileNamePath;

ProductsWindow::ProductsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProductsWindow)
{
    ui->setupUi(this);

    tempMenuList = MenuList;
    updateProductsList();

}

void ProductsWindow::updateProductsList(){
    QStandardItemModel *standardModel = new QStandardItemModel(this);
    standardModel->setColumnCount(4);

    QStringList headerLabels = {"Product Name", "Price", "Category", "Supply"};
    standardModel->setHorizontalHeaderLabels(headerLabels);

    for(int i=0; i<tempMenuList.size();i++){
        QStandardItem *menuIcon = new QStandardItem(QIcon(":/pictures/pictures/NoThumbnail.jpg"),tempMenuList[i].name);
        QStandardItem *menuPrice = new QStandardItem(QString::number(tempMenuList[i].price));
        QStandardItem *menuCategory = new QStandardItem(tempMenuList[i].category);
        QStandardItem *menuSupply = new QStandardItem(QString::number(tempMenuList[i].supply));

        standardModel->setItem(i,0,menuIcon);
        standardModel->setItem(i,1,menuPrice);
        standardModel->setItem(i,2,menuCategory);
        standardModel->setItem(i,3,menuSupply);

    }


    ui->productsList->setModel(standardModel);
    ui->productsList->setIconSize(QSize(50,50));
    ui->productsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->productsList->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->productsList->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->productsList->header()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->productsList->header()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->productsList->header()->setStretchLastSection(false);

    QModelIndex defaultIndex = ui->productsList->model()->index(0,0);
    ui->productsList->setCurrentIndex(defaultIndex);
}

ProductsWindow::~ProductsWindow()
{
    delete ui;
}

void ProductsWindow::on_pushButton_clicked()
{
   if(ui->productsList->model()->rowCount()!=0){
    int index = ui->productsList->currentIndex().row();
    ui->productsList->model()->removeRow(index);
    tempMenuList.erase(tempMenuList.begin()+index);
   }
}


void ProductsWindow::on_AddMenuButton_clicked()
{
    MenuDetails tempMenu;

    auto tempMenuName = ui->MenuNameLineEdit->text();
    auto tempMenuCategory = ui->MenuCategoryLineEdit->text();
    auto tempMenuPrice = ui->MenuPriceLineEdit->text();
    auto tempMenuSupplyQuantity = ui->MenuSupplyQuantityLineEdit->text();

    if(tempMenuName!="" && tempMenuCategory!="" && tempMenuPrice!="" && tempMenuSupplyQuantity!="" && ui->MenuFilePath->text() != "(Select a picture)"){
        for(int i=0; i<tempMenuPrice.size();i++){
            if(tempMenuPrice[i].isLetter()){
                QMessageBox::warning(this,"Error", "Invalid price amount.  Please enter numbers and/or decimals only.");
                return;
            }
        }
        for(int i=0; i<tempMenuSupplyQuantity.size();i++){
            if(tempMenuSupplyQuantity[i].isLetter()){
                QMessageBox::warning(this,"Error", "Invalid supply amount.  Please enter numbers and/or decimals only.");
                return;
            }
        }
        tempMenu.name = tempMenuName;
        tempMenu.category = tempMenuCategory;
        tempMenu.price = tempMenuPrice.toDouble();
        tempMenu.supply = tempMenuSupplyQuantity.toInt();

        QDir *dir = new QDir();
        QString menuPictureDirectory = dir->absolutePath() + "/pictures/" + tempMenuName + ".png";

        if(QFile::exists(menuPictureDirectory)){
            QFile::remove(menuPictureDirectory);
        }
        QFile::copy(fileNamePath, menuPictureDirectory);

        tempMenuList.push_back(tempMenu);
        updateProductsList();
        ui->productsList->scrollToBottom();
    }
    else{
        QMessageBox::warning(this,"Error", "Incomplete input.  Please fill all elements above.");
    }

}


void ProductsWindow::on_AddImageButton_clicked()
{
    fileNamePath = QFileDialog::getOpenFileName(this, tr("Select Image"),QStandardPaths::writableLocation((QStandardPaths::DownloadLocation)),tr("Image Files (*.png)"));

    if(fileNamePath!=""){
        QPixmap image(fileNamePath);
        QPixmap imageScaled = image.scaled(200,200,Qt::KeepAspectRatio,Qt::FastTransformation);
        ui->MenuPicture->setPixmap(imageScaled);
        ui->MenuFilePath->setText(fileNamePath.section("/",-1,-1));
    }
}


void ProductsWindow::on_SaveCloseButton_clicked()
{
    MenuList = tempMenuList;

    QDir *dir = new QDir();
    QString resourcesFileAbsolute = dir->absolutePath();
    QString productsFileLocation = resourcesFileAbsolute + "/resources/products.csv";
    QFile ProductsFile(productsFileLocation);

    ProductsFile.open( QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text);
    QTextStream output(&ProductsFile);

    for(int i=0; i <MenuList.size(); i++){
        output << MenuList[i].name << "," << MenuList[i].price << "," << MenuList[i].category << "," << MenuList[i].supply << "\n";
    }

    ProductsFile.flush();
    ProductsFile.close();


    QMessageBox::information(this, "Success!", "Succesfully saved all product changes.");
    this->close();
}

