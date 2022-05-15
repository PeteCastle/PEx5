#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <QMainWindow>
#include <QCalendarWidget>
#include "calendardialog.h"
#include "viewreceipt.h"


namespace Ui {
class Analytics;
}

class Analytics : public QMainWindow
{
    Q_OBJECT

public:
    explicit Analytics(QWidget *parent = nullptr);
    ~Analytics();

private slots:
    void on_StartDateButton_clicked();

    void on_EndDateButton_clicked();



    void on_UpdateButton_clicked();

    void on_ViewTransactionButton_clicked();

    void on_CloseButton_clicked();

private:
    Ui::Analytics *ui;
    CalendarDialog *calendarDialog;
};

#endif // ANALYTICS_H
