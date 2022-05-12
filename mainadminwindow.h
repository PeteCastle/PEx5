#ifndef MAINADMINWINDOW_H
#define MAINADMINWINDOW_H

#include <QMainWindow>
#include "inventorywindow.h"
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

private:
    Ui::MainAdminWindow *ui;
    InventoryWindow *inventoryWindow;
};

#endif // MAINADMINWINDOW_H
