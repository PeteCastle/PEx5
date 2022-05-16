#ifndef CHECKOUTWINDOW_H
#define CHECKOUTWINDOW_H

#include <QMainWindow>
#include "viewreceipt.h"

namespace Ui {
class CheckoutWindow;
}

class CheckoutWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CheckoutWindow(QWidget *parent = nullptr);
    ~CheckoutWindow();

private:
    Ui::CheckoutWindow *ui;
    ViewReceipt *viewReceipt;

private slots:
    void computeChargesAndValues();
    void on_PaymentMethod_currentIndexChanged(int index);
    void on_DeliveryMethod_currentIndexChanged(int index);
    void on_PromoCodeApplyButton_clicked();
    void on_PlacePrderButton_clicked();
    QString generateReceipt();
    void updateInventoryCount();

public slots:
    QString generateTransactionCode(const int len=12);
};

#endif // CHECKOUTWINDOW_H
