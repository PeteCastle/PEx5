#include "analytics.h"
#include "ui_analytics.h"
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "MenuList.h"
#include "viewreceipt.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableWidget>
#include <QHash>
#include <QVector>


QDate startDate, endDate;
QList<QDate> filteredDate; // Dates from directory between start date and end date
QList<QString> filteredTransactionFiles;
QList<ReceiptStructure> filteredTransactions;
QHash<QString, double> summaryStatistics;

struct ProductStatistic{
    QString name;
    int amountSold;
    double sale;
};
QList<ProductStatistic>productStatistics;
QList<QString> productNames; //for search only

Analytics::Analytics(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Analytics)
{
    ui->setupUi(this);

    startDate = QDate::currentDate();
    endDate = QDate::currentDate();
    ui->StartDate->setText(startDate.toString());
    ui->EndDate->setText(endDate.toString());

    on_UpdateButton_clicked();
}

Analytics::~Analytics()
{
    delete ui;
}

void Analytics::on_StartDateButton_clicked()
{
    CalendarDialog *calendarDialog = new CalendarDialog(this);
    calendarDialog->setWindowTitle("Select Date");
    calendarDialog->exec();
    if(endDate>=calendarDialog->getDate()){
        startDate = calendarDialog->getDate();
        ui->StartDate->setText(startDate.toString());
    }
    else{
        QMessageBox::warning(this, "Date Selection Error", "Start date must be earlier than end date!");
    }
    qDebug() << startDate << endDate;

}


void Analytics::on_EndDateButton_clicked()
{
    CalendarDialog *calendarDialog = new CalendarDialog(this);
    calendarDialog->setWindowTitle("Select Date");
    calendarDialog->exec();
    if(startDate<=calendarDialog->getDate()){
        endDate = calendarDialog->getDate();
        ui->EndDate->setText(endDate.toString());
    }
    else{
        QMessageBox::warning(this, "Date Selection Error", "End date must be later than start date!");
    }
    qDebug() << startDate << endDate;

}

void Analytics::on_UpdateButton_clicked()
{
    QDir *dir = new QDir();

    filteredDate.clear();
    filteredTransactionFiles.clear();
    filteredTransactions.clear();
    productStatistics.clear();
    productNames.clear();
    summaryStatistics.clear();

    QString transactionHistoryFolderLocation = dir->absolutePath() +  "/transactionHistory";

    QDir *transactionHistoryDir = new QDir(transactionHistoryFolderLocation);
    QStringList dateList = transactionHistoryDir->entryList(QDir::Dirs|QDir::NoDotAndDotDot);

    for(int i=0; i<dateList.size();i++){
        QDate tempDate = QDate::fromString(dateList[i],"yyyy-MM-dd");
        if(tempDate>=startDate && tempDate<=endDate){
            filteredDate.append(tempDate);
        }
    }

    //Set default value for every widget
    if(filteredDate.isEmpty()){
        QMessageBox::information(nullptr, "Empty Transactions List", "No recorded transactions between " + startDate.toString("yyyy-MM-dd") + " and " + endDate.toString("yyyy-MM-dd"));
        ui->SummaryStatistics->clearContents();
        ui->OrderQuantitiesList->setModel(nullptr);
        ui->TransactionsList->setModel(nullptr);
        return;
    }

    for(int i=0; i<filteredDate.size();i++){
        QDir *transactionFilesDir = new QDir(transactionHistoryFolderLocation + "/" + filteredDate[i].toString("yyyy-MM-dd"));
        QStringList transactionFiles = transactionFilesDir->entryList(QDir::Files);
        for(int j=0; j<transactionFiles.size();j++){
            filteredTransactionFiles.append(transactionFilesDir->path() + "/" + transactionFiles[j]);
        }
    }
    for(int i=0; i<filteredTransactionFiles.size();i++){

        ViewReceipt *viewReceipt = new ViewReceipt();
        ReceiptStructure tempReceipt = viewReceipt->readReceipt(filteredTransactionFiles[i]);
        filteredTransactions.append(tempReceipt);
    }

    QStandardItemModel *model = new QStandardItemModel(this);
    for(int i=0; i<filteredTransactions.size();i++){
        model->appendRow(new QStandardItem(filteredTransactions[i].transactionID
                                           + "(" + filteredTransactions[i].transactionDate.toString("yyyy-MM-dd") + ")"));
        for(int j=0; j<filteredTransactions[i].orderNames.size();j++){
            int index = productNames.indexOf(filteredTransactions[i].orderNames[j]);

            if(index!=-1){
                productStatistics[index].amountSold +=  filteredTransactions[i].orderQuantity[j];
                productStatistics[index].sale += filteredTransactions[i].orderPrice[j];
            }
            else{
                ProductStatistic tempStatistic;
                tempStatistic.name = filteredTransactions[i].orderNames[j];
                tempStatistic.amountSold = filteredTransactions[i].orderQuantity[j];
                tempStatistic.sale = filteredTransactions[i].orderPrice[j];
                productNames.append(filteredTransactions[i].orderNames[j]);
                productStatistics.append(tempStatistic);
            }
            summaryStatistics["Total Items Sold: "] += filteredTransactions[i].totalNumberOfItems;
            summaryStatistics["Sales (without charges and discounts): "]= filteredTransactions[i].subTotal;
            summaryStatistics["Total Taxes: "] += filteredTransactions[i].valueAddedTax;
            summaryStatistics["Total Delivery Charge: "] += filteredTransactions[i].deliveryCharge;
            summaryStatistics["Total Discounts: "] += filteredTransactions[i].discount;
            summaryStatistics["Sales (with charges and discounts): "]+= filteredTransactions[i].grandTotal;
        }
    }

    QStandardItemModel *standardModel = new QStandardItemModel(this);
    standardModel->setColumnCount(3);
    QStringList headerLabels = {"Product", "Amount", "Total Sales"};
    standardModel->setHorizontalHeaderLabels(headerLabels);
    for(int i=0; i<productStatistics.size();i++){

        QDir *dir = new QDir();
        QString resourcesFileAbsolute = dir->absolutePath();

        QStandardItem *productName = new QStandardItem(QIcon(resourcesFileAbsolute + "/pictures/" + productStatistics[i].name + ".png"), QString::number(i) + " - " + productStatistics[i].name);
        QStandardItem *productAmountSold = new QStandardItem(QString::number(productStatistics[i].amountSold));
        QStandardItem *productTotalSales = new QStandardItem(QString::number(productStatistics[i].sale));
        productAmountSold->setTextAlignment(Qt::AlignCenter);

        standardModel->setItem(i,0,productName);
        standardModel->setItem(i,1,productAmountSold);
        standardModel->setItem(i,2,productTotalSales);
    }
    ui->OrderQuantitiesList->setModel(standardModel);
    ui->OrderQuantitiesList->setIconSize(QSize(20,20));
    ui->OrderQuantitiesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->OrderQuantitiesList->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->OrderQuantitiesList->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->OrderQuantitiesList->header()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->OrderQuantitiesList->header()->setStretchLastSection(false);
    ui->TransactionsList->setModel(model);


    ui->SummaryStatistics->setRowCount(6);
    ui->SummaryStatistics->setColumnCount(2);
    ui->SummaryStatistics->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->SummaryStatistics->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->SummaryStatistics->verticalHeader()->setVisible(false);
    QStringList labels = {"Description"," "};
    ui->SummaryStatistics->setHorizontalHeaderLabels(labels);
    QHash<QString, double>::const_iterator statisticsIndex = summaryStatistics.constBegin();
    int tempIndex=0;
    while(statisticsIndex != summaryStatistics.constEnd()){
        QTableWidgetItem *newStatisticDescription = new QTableWidgetItem();
        newStatisticDescription->setText(statisticsIndex.key());
        newStatisticDescription->setFlags(newStatisticDescription->flags() & ~Qt::ItemIsEditable);
        ui->SummaryStatistics->setItem(tempIndex, 0, newStatisticDescription);

        QTableWidgetItem *newStatisticValue = new QTableWidgetItem(QString::number(statisticsIndex.value()));
        newStatisticValue->setFlags(newStatisticValue->flags() & ~Qt::ItemIsEditable);
        ui->SummaryStatistics->setItem(tempIndex, 1, newStatisticValue);

        statisticsIndex++;
        tempIndex++;
    }

    //Generate Graph
    QVector<double> xAxis(productStatistics.size());
    QVector<double> yAxis(productStatistics.size());

    double maxY=0, maxX=0;
    for(int i=0; i<productStatistics.size();i++){
        xAxis[i] = i; //productStatistics[i].name
        yAxis[i] = productStatistics[i].sale;

        if(productStatistics[i].sale>maxY){
            maxY = productStatistics[i].sale;
        }
        maxX++;
    }



    //This shall be uncommented in final version
    /*

    QCPBars *salesGraph = new QCPBars(ui->Graph->xAxis,ui->Graph->yAxis);
    //salesGraph->setName("Test");
    salesGraph->setData(xAxis,yAxis);

    ui->Graph->yAxis->setRange(0,maxY);
    ui->Graph->xAxis->setRange(-0.5,maxX);

    //Removed x-axis labels as they are too large to be fit in the graph.  Instead use indeces and reference to the menu below.
    //QSharedPointer<QCPAxisTickerText> textTicker (new QCPAxisTickerText);
    //textTicker->addTicks(yAxis, productNames);
    ui->Graph->xAxis->setTickLabels(true);
    //ui->Graph->xAxis->setTicker(textTicker);
    //ui->Graph->xAxis->setTickLabelRotation(45);
    ui->Graph->xAxis->setLabel("Products (indeces)");

    ui->Graph->yAxis->setLabel("Sales (in Php)");

    //ui->Graph->addGraph(ui->Graph->yAxis,ui->Graph->xAxis);



    ui->Graph->replot();

    */
}


void Analytics::on_ViewTransactionButton_clicked()
{
    if(ui->TransactionsList->model()->rowCount()==0){
        return;
    }
    int index = ui->TransactionsList->currentIndex().row();
    if(index==-1){
        return;
    }

    ViewReceipt *viewReceipt = new ViewReceipt(this, filteredTransactionFiles[index]);
    viewReceipt->setWindowTitle("Recipe");
    viewReceipt->exec();
}


void Analytics::on_CloseButton_clicked()
{
    this->close();
}

