#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QDialog>
#include "headers/Pump.h"
class ContentWidget;
namespace Ui {
class OptionsWindow;
}

class OptionsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsWindow(Pump* pump, ContentWidget* contentWidget, QWidget *parent = nullptr);
    ~OptionsWindow();

signals:
    void powerOff();
private slots:
    void on_acceptButton_clicked();
    void on_pushButton_clicked();

private:
    Ui::OptionsWindow *ui;
    Pump* pump;
    ContentWidget* contentWidget;
};

#endif // OPTIONSWINDOW_H
