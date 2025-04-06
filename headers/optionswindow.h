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


private slots:
    void on_okorcancel_accepted();
    void on_okorcancel_rejected();

private:
    Ui::OptionsWindow *ui;
    Pump* pump;
    ContentWidget* contentWidget;
};

#endif // OPTIONSWINDOW_H
