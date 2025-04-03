#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <QSettings>

class AuthManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool authenticated READ isAuthenticated WRITE setAuthenticated NOTIFY authStateChanged)

public:
    static AuthManager* getInstance();

    bool isAuthenticated() const;
    void setAuthenticated(bool authenticated);

    // PIN code management methods
    QString getPinCode() const;
    bool setPinCode(const QString &newPin);
    bool validatePin(const QString &pin);

signals:
    void authStateChanged(bool authenticated);

private:
    AuthManager(QObject *parent = nullptr);
    static AuthManager* instance;
    bool m_authenticated;
    QSettings m_settings;
};

#endif // AUTHMANAGER_H
