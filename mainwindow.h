#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "confirmaddtocart.h"
#include "checkoutwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listView_clicked(const QModelIndex &index);
    void confirmAddtoCart(QString parameters);
    void updateCartMenu();
    void on_CheckoutButton_clicked();

    void on_pushButton_clicked();

private:

    Ui::MainWindow *ui;
    ConfirmAddToCart *confirmAddtoCartWindow;
    CheckoutWindow *checkoutWindow;
};
#endif // MAINWINDOW_H
