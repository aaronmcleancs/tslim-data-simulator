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

// Get the current PIN code
QString AuthManager::getPinCode() const {
    return m_settings.value("pinCode", "1234").toString();
}

// Set a new PIN code
bool AuthManager::setPinCode(const QString &newPin) {
    // You could add validation here if needed
    m_settings.setValue("pinCode", newPin);
    m_settings.sync(); // Make sure to save to disk immediately

    // Return true if save was successful
    return m_settings.status() == QSettings::NoError;
}

// Validate a PIN against the stored PIN
bool AuthManager::validatePin(const QString &pin) {
    bool valid = (pin == getPinCode());

    // If PIN is valid, set the authenticated state to true
    if (valid) {
        setAuthenticated(true);
    }

    return valid;
}
