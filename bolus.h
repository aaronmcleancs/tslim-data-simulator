#ifndef BOLUS_H
#define BOLUS_H

#include <QDialog>

namespace Ui {
class bolus;
}

class bolus : public QDialog
{
    Q_OBJECT

public:
    explicit bolus(QWidget *parent = nullptr);
    ~bolus();

signals:
    void mainShift();

private slots:


private:
    Ui::bolus *ui;
};

#endif // BOLUS_H
