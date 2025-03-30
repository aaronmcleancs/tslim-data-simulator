#ifndef UI_H
#define UI_H

#include <QObject>
#include "ui_mainwindow.h"

// Example Header File for initial repo
// Feel free to change any of this to your implementation

class MainWindow;

class UI : public QObject
{
    Q_OBJECT
public:
    explicit UI(Ui::MainWindow *ui, QObject *parent = nullptr);

private:
    Ui::MainWindow *ui;

public slots:
    //void displayStatus(const QString &status);
    //void displayError(const QString &error);

signals:
    void userInputReceived(const QString &input);
};

#endif
