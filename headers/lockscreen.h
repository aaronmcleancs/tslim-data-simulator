#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include <QWidget>
#include <QSettings>
#include "authmanager.h"

namespace Ui {
class LockScreen;
}

class LockScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LockScreen(QWidget *parent = nullptr);
    void clearPin();
    ~LockScreen();
signals:
    // signal when unlock is successful
    void unlocked();

private slots:
    void on_confirmButton_clicked();
    // add other button handlers as needed

    void on_clearButton_clicked();

private:
    Ui::LockScreen *ui;
    AuthManager *authManager;

    // PIN validation
    bool validatePIN(const QString &pin);
    void onNumericButtonClicked();
};

#endif // LOCKSCREEN_H
