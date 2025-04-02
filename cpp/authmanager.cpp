#include "headers/authmanager.h"

AuthManager* AuthManager::instance = nullptr;

AuthManager* AuthManager::getInstance()
{
    if (!instance) {
        instance = new AuthManager();
    }
    return instance;
}

AuthManager::AuthManager(QObject *parent)
    : QObject(parent)
    , m_authenticated(false)
{
}

bool AuthManager::isAuthenticated() const
{
    return m_authenticated;
}

void AuthManager::setAuthenticated(bool authenticated)
{
    if (m_authenticated != authenticated) {
        m_authenticated = authenticated;
        emit authStateChanged(authenticated);
    }
}
