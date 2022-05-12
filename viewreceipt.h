#ifndef VIEWRECEIPT_H
#define VIEWRECEIPT_H

#include <QDialog>

namespace Ui {
class ViewReceipt;
}

class ViewReceipt : public QDialog
{
    Q_OBJECT

public:
    explicit ViewReceipt(QWidget *parent = nullptr);
    ~ViewReceipt();

private:
    Ui::ViewReceipt *ui;
};

#endif // VIEWRECEIPT_H
