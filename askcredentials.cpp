#include "askcredentials.h"
#include "ui_askcredentials.h"
#include "MenuList.h"
#include <QDir>
#include <QMessageBox>

bool validity;
int mode2;

AskCredentials::AskCredentials(QWidget *parent, int mode) :
    QDialog(parent),
    ui(new Ui::AskCredentials)
{
    ui->setupUi(this);
    if(mode==1){
        ui->UsernameLabel->setText("Enter your username:");
        ui->PasswordLabel->setText("Enter your password:");

        ui->pushButton->setText("Login");
    }
    else if(mode==2){
        ui->UsernameLabel->setText("Enter new username:");
        ui->PasswordLabel->setText("Enter new password:");

        ui->pushButton->setText("Update credentials");
    }
    mode2=mode;
}

AskCredentials::~AskCredentials()
{
    delete ui;
}

bool AskCredentials::getResult(){
    qDebug() << "THISPART" << validity;
    return validity;
}
void AskCredentials::on_pushButton_clicked()
{
    if(mode2==2){
        username = ui->UsernameLineEdit->text();
        password = ui->PasswordLineEdit->text();

        if(username=="" || password==""){
            QMessageBox::warning(nullptr, "Empty Input", "Username and password cannot be empty!");
            return;
        }

        QDir *dir = new QDir();
        QString resourcesFileAbsolute = dir->absolutePath();
        QString credentialsFileLocation = resourcesFileAbsolute + "/resources/credentials.dat";
        QFile CredentialsFile(credentialsFileLocation);

        if (!CredentialsFile.open(QIODevice::WriteOnly | QIODevice::Text)){
              QMessageBox::warning(nullptr, "File not found error", "DAT file " + credentialsFileLocation + " not found.");
        }

        QTextStream output(&CredentialsFile);
        output << username << "," << password;

        CredentialsFile.flush();
        CredentialsFile.close();


        QMessageBox::information(nullptr, "Success", "Successfully changed username and password.");
        this->close();
    }
    else if(mode2==1){
        if(ui->UsernameLineEdit->text()==username && ui->PasswordLineEdit->text()==password){
            QMessageBox::information(nullptr,"Login Success", "Success!");
            validity = 1;
            this->close();
        }
        else{
            QMessageBox::information(nullptr,"Wrong Credentials", "Username and Password are incorrect");
            validity = 0;
        }
    }
}

