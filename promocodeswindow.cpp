#include "promocodeswindow.h"
#include "ui_promocodeswindow.h"
#include "MenuList.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStringListModel>

vector<struct PromoCodes> TempPromosList;

PromoCodesWindow::PromoCodesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PromoCodesWindow)
{
    ui->setupUi(this);

    TempPromosList = PromosList;
    updateTempPromoCodes();
}

PromoCodesWindow::~PromoCodesWindow()
{
    delete ui;
}

void PromoCodesWindow::on_RemovePromoCodeButton_clicked()
{
    if(ui->PromoCodesList->model()->rowCount()!=0){
        int index = ui->PromoCodesList->currentIndex().row();
        TempPromosList.erase(TempPromosList.begin()+index);
        updateTempPromoCodes();
    }
}
void PromoCodesWindow::updateTempPromoCodes(){




    QStringList promosList;

    for(int i=0; i<TempPromosList.size();i++){
        if (TempPromosList[i].discountType==1){
            promosList.append(TempPromosList[i].promoName + " (Php " + QString::number(TempPromosList[i].discountQuantity));
            //model->insertRow(i, new QModelIndex())
            //ui->PromoCodesList->addItem(TempPromosList[i].promoName + " (Php " + QString::number(TempPromosList[i].discountQuantity) + ")");
        }
        else if(TempPromosList[i].discountType==2){
            promosList.append(TempPromosList[i].promoName + " (" + QString::number(TempPromosList[i].discountQuantity) + "%)");
            //ui->PromoCodesList->addItem(TempPromosList[i].promoName + " (" + QString::number(TempPromosList[i].discountQuantity) + "%)");
        }
        qDebug() << promosList;
    }
    //ui->PromoCodesList->model()->removeRows(0,ui->PromoCodesList->model()->rowCount());
    ui->PromoCodesList->setModel(new QStringListModel(promosList));
}

void PromoCodesWindow::on_SaveAndCloseButton_clicked()
{
    PromosList = TempPromosList;

    QDir *dir = new QDir();
    QString resourcesFileAbsolute = dir->absolutePath();
    QString promosFileLocation = resourcesFileAbsolute + "/resources/promocodes.csv";
    QFile PromosFile(promosFileLocation);

    PromosFile.open( QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text);
    QTextStream output(&PromosFile);

    for(int i=0; i <PromosList.size(); i++){
        output << PromosList[i].promoName << "," << PromosList[i].discountQuantity << "," << PromosList[i].discountType << "\n";
    }

    PromosFile.flush();
    PromosFile.close();

    this->close();
}


void PromoCodesWindow::on_AddPromoCodeButton_clicked()
{
    PromoCodes tempPromo;

    QString promoNameTemp = ui->PromoNameLineEdit->text();
    QString discountAmountTemp = ui->DiscountAmountLineEdit->text();

    if(promoNameTemp!="" && discountAmountTemp!=""){
        for(int i=0; i<discountAmountTemp.size();i++){
            if(discountAmountTemp[i].isLetter()){
                QMessageBox::warning(this,"Error", "Invalid discount amount.  Please enter numbers and/or decimals only.");
                return;
            }
        }
    }
    else{
        QMessageBox::warning(this,"Error", "Please enter values/texts in Promo Name and Discount Amount before proceeding.");
        return;
    }

    tempPromo.promoName = promoNameTemp;
    tempPromo.discountQuantity = discountAmountTemp.toDouble();

    if(ui->FlatDiscountButton->isChecked()){
        tempPromo.discountType = 1;
    }
    else if(ui->PercentageButton->isChecked()){
        tempPromo.discountType = 2;
    }
    else{
        QMessageBox::warning(this,"Error", "You must select discount type before proceeding");
        return;
    }

    ui->PromoNameLineEdit->setText("");
    ui->DiscountAmountLineEdit->setText("");

    TempPromosList.push_back(tempPromo);
    updateTempPromoCodes();
}

