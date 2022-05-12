#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QMainWindow>

namespace Ui {
class LandingPage;
}

class LandingPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit LandingPage(QWidget *parent = nullptr);
    ~LandingPage();

private:
    Ui::LandingPage *ui;
};

#endif // LANDINGPAGE_H
