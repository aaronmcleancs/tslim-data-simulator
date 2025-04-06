#ifndef POWEROFF_H
#define POWEROFF_H

#include <QWidget>

namespace Ui {
class PowerOff;
}

class PowerOff : public QWidget
{
    Q_OBJECT

public:
    explicit PowerOff(QWidget *parent = nullptr);
    ~PowerOff();

private slots:
    void on_powerOnButton_clicked();

signals:
    void powerOn();
private:
    Ui::PowerOff *ui;
};

#endif // POWEROFF_H
