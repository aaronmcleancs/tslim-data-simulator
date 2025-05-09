#include "headers/lockscreen.h"
#include "ui_lockscreen2.h"

LockScreen::LockScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LockScreen)
{
    ui->setupUi(this);
    authManager = AuthManager::getInstance();
    for (int i = 0; i <= 9; i++) {
        QString buttonName = "button" + QString::number(i);
        QPushButton* button = findChild<QPushButton*>(buttonName);

        if (button) {
            connect(button, &QPushButton::clicked, this, &LockScreen::onNumericButtonClicked);
        }
    }
}

LockScreen::~LockScreen()
{
    delete ui;
}

void LockScreen::on_confirmButton_clicked()
{
    // get entered pin
    QString enteredPIN = ui->pinEdit->text();

    // validate pin
    if (validatePIN(enteredPIN)) {
        // set authenticated state to true
        authManager->setAuthenticated(true);

        emit unlocked();
    } else {
        // if pin is incorrect
        ui->messageLabel->setText("Incorrect PIN. Please try again.");
        ui->pinEdit->clear();
    }
}

bool LockScreen::validatePIN(const QString &pin) {
    return authManager->validatePin(pin);
}

void LockScreen::onNumericButtonClicked()
{
    ui->messageLabel->setText("Enter PIN to Unlock");
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString digit = button->text();
        QString currentPin = ui->pinEdit->text();

        if (currentPin.length() < 4) {
            ui->pinEdit->setText(currentPin + digit);
        }
    }
}

void LockScreen::clearPin() {
    ui->pinEdit->clear();
}

void LockScreen::on_clearButton_clicked()
{
    clearPin();
}

