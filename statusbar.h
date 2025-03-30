#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QFrame>

namespace Ui {
class StatusBar;
}

class StatusBar : public QFrame
{
    Q_OBJECT

public:
    explicit StatusBar(QWidget *parent = nullptr);
    ~StatusBar();

private:
    Ui::StatusBar *ui;
};

#endif // STATUSBAR_H
