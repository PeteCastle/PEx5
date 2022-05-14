#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QMainWindow>
#include "mainwindow.h"
#include "mainadminwindow.h"

namespace Ui {
class LandingPage;
}

class LandingPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit LandingPage(QWidget *parent = nullptr);
    ~LandingPage();

private slots:
    void on_customerButton_clicked();

    void on_adminButton_clicked();

private:
    Ui::LandingPage *ui;
    MainWindow *customerWindow;
    MainAdminWindow *adminWindow;
};

#endif // LANDINGPAGE_H
