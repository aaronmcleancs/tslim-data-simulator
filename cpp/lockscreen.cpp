#include "headers/lockscreen.h"
#include "ui_lockscreen2.h"

LockScreen::LockScreen(StatusBar *sb, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LockScreen)
{
    ui->setupUi(this);
    statusBar = sb;
    ui->statusBarContainer->layout()->addWidget(statusBar);
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
    QString savedPassword = loadPasswordFromJson();

    // If no password is set yet use default
    if (savedPassword.isEmpty()) {
        savePasswordToJson("1234"); // Set default password
        return pin == "1234";
    }

    return pin == savedPassword;
}

void LockScreen::savePasswordToJson(const QString &password) {
    QJsonObject json;
    json["password"] = password;

    QFile file("password.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(json).toJson());
        file.close();
    }
}

QString LockScreen::loadPasswordFromJson() {
    QFile file("password.json");
    if (!file.open(QIODevice::ReadOnly)) {
        return ""; // File doesn't exist yet
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        return "";
    }

    QJsonObject json = doc.object();
    return json["password"].toString();
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

void LockScreen::on_clearButton_clicked()
{
    ui->pinEdit->clear();
    savePasswordToJson("1111");
}

