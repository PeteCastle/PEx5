#ifndef CHECKOUTWINDOW_H
#define CHECKOUTWINDOW_H

#include <QMainWindow>

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

private slots:
    void computeChargesAndValues();
    void on_PaymentMethod_currentIndexChanged(int index);
    void on_DeliveryMethod_currentIndexChanged(int index);
    void on_PromoCodeApplyButton_clicked();
};

#endif // CHECKOUTWINDOW_H
