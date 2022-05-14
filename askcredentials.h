#ifndef ASKCREDENTIALS_H
#define ASKCREDENTIALS_H

#include <QDialog>

namespace Ui {
class AskCredentials;
}

class AskCredentials : public QDialog
{
    Q_OBJECT

public:
    explicit AskCredentials(QWidget *parent = nullptr, int mode=0);
    ~AskCredentials();
    bool getResult();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AskCredentials *ui;
};

#endif // ASKCREDENTIALS_H
