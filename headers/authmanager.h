#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>

class AuthManager : public QObject
{
    Q_OBJECT

public:
    // Singleton instance accessor
    static AuthManager* getInstance();

    // Authentication state methods
    bool isAuthenticated() const;
    void setAuthenticated(bool authenticated);

signals:
    // Signal emitted when auth state changes
    void authStateChanged(bool authenticated);

private:
    // Private constructor for singleton
    explicit AuthManager(QObject *parent = nullptr);

    // Authentication state
    bool m_authenticated;

    // Singleton instance
    static AuthManager* instance;
};

#endif // AUTHMANAGER_H
