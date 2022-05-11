#ifndef CONFIRMADDTOCART_H
#define CONFIRMADDTOCART_H

#include <QDialog>
using namespace std;
namespace Ui {
class ConfirmAddToCart;
}

class ConfirmAddToCart : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmAddToCart(QWidget *parent = nullptr, QString name="", double price=0, int supply=0);
    ~ConfirmAddToCart();
    tuple<QString,double,int> getOrderParameters();

private slots:
    void on_addButton_clicked();

    void on_subtractButton_clicked();

    void on_AddToCart_clicked();


    void on_backButton_clicked();

private:
    Ui::ConfirmAddToCart *ui;

};

#endif // CONFIRMADDTOCART_H
