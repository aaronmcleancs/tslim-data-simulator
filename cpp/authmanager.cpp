#include "headers/authmanager.h"
#include <QDebug>

AuthManager* AuthManager::instance = nullptr;

AuthManager* AuthManager::getInstance() {
    if (!instance) {
        instance = new AuthManager();
    }
    return instance;
}

AuthManager::AuthManager(QObject *parent)
    : QObject(parent)
    , m_authenticated(false)
    , m_settings("TandemPump", "COMP3004A4")
{
    if (!m_settings.contains("pinCode")) {
        setPinCode("1234");
    }
}

bool AuthManager::isAuthenticated() const {
    return m_authenticated;
}

void AuthManager::setAuthenticated(bool authenticated) {
    if (m_authenticated != authenticated) {
        m_authenticated = authenticated;
        emit authStateChanged(authenticated);
    }
}

// get current PIN code
QString AuthManager::getPinCode() const {
    return m_settings.value("pinCode", "1234").toString();
}

// set new PIN code and save
bool AuthManager::setPinCode(const QString &newPin) {
    m_settings.setValue("pinCode", newPin);
    m_settings.sync();

    return m_settings.status() == QSettings::NoError;
}

// validate a PIN against the stored PIN
bool AuthManager::validatePin(const QString &pin) {
    bool valid = (pin == getPinCode());

    // if PIN valid authenticated state is true
    if (valid) {
        setAuthenticated(true);
    }

    return valid;
}
