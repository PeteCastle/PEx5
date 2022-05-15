#ifndef VIEWRECEIPT_H
#define VIEWRECEIPT_H
#include "MenuList.h"
#include <QDialog>


namespace Ui {
class ViewReceipt;
}

class ViewReceipt : public QDialog
{
    Q_OBJECT

public:
    explicit ViewReceipt(QWidget *parent = nullptr, QString filename = "");
    ~ViewReceipt();
    ReceiptStructure readReceipt(QString filePath);

private:
    Ui::ViewReceipt *ui;

public slots:

private slots:
    void on_pushButton_clicked();
};

#endif // VIEWRECEIPT_H
