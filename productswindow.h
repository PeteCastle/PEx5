#ifndef PRODUCTSWINDOW_H
#define PRODUCTSWINDOW_H

#include <QMainWindow>

namespace Ui {
class ProductsWindow;
}

class ProductsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProductsWindow(QWidget *parent = nullptr);
    ~ProductsWindow();

private slots:
    void on_pushButton_clicked();

    void on_AddMenuButton_clicked();

    void on_AddImageButton_clicked();

    void updateProductsList();

    void on_SaveCloseButton_clicked();

private:
    Ui::ProductsWindow *ui;
};

#endif // PRODUCTSWINDOW_H
