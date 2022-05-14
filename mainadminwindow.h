#ifndef MAINADMINWINDOW_H
#define MAINADMINWINDOW_H

#include <QMainWindow>
#include "inventorywindow.h"
#include "promocodeswindow.h"
#include "askcredentials.h"
#include "productswindow.h"

namespace Ui {
class MainAdminWindow;
}

class MainAdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainAdminWindow(QWidget *parent = nullptr);
    ~MainAdminWindow();

private slots:


    void on_ModifyInventoryButton_clicked();

    void on_ShowInventoryButton_clicked();



    void on_ModifyPromoCodesButton_clicked();

    void on_ChangePasswordButton_clicked();

    void on_AddProductButton_clicked();

private:
    Ui::MainAdminWindow *ui;
    InventoryWindow *inventoryWindow;
    PromoCodesWindow *promoCodesWindow;
    AskCredentials *askCredentials;
    ProductsWindow *productsWindow;
};

#endif // MAINADMINWINDOW_H
