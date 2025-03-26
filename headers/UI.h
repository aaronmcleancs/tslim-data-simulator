#ifndef UI_H
#define UI_H

#include <QObject>

// Example Header File for initial repo
// Feel free to change any of this to your implementation

class UI : public QObject
{
    Q_OBJECT
public:
    explicit UI(QObject *parent = nullptr);

public slots:
    void displayStatus(const QString &status);
    void displayError(const QString &error);

signals:
    void userInputReceived(const QString &input);
};

#endif
