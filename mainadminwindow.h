#ifndef MAINADMINWINDOW_H
#define MAINADMINWINDOW_H

#include <QMainWindow>
#include "inventorywindow.h"
#include "promocodeswindow.h"
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

private:
    Ui::MainAdminWindow *ui;
    InventoryWindow *inventoryWindow;
    PromoCodesWindow *promoCodesWindow;
};

#endif // MAINADMINWINDOW_H
