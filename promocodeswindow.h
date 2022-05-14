#ifndef PROMOCODESWINDOW_H
#define PROMOCODESWINDOW_H

#include <QMainWindow>

namespace Ui {
class PromoCodesWindow;
}

class PromoCodesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PromoCodesWindow(QWidget *parent = nullptr);
    ~PromoCodesWindow();

private slots:
    void on_RemovePromoCodeButton_clicked();
    void updateTempPromoCodes();
    void on_SaveAndCloseButton_clicked();

    void on_AddPromoCodeButton_clicked();

private:
    Ui::PromoCodesWindow *ui;
};

#endif // PROMOCODESWINDOW_H
