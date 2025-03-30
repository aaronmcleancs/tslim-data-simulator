#include "headers/UI.h"
#include "mainwindow.h"
#include <QDebug>

UI::UI(Ui::MainWindow *ui, QObject *parent)
    : QObject(parent), ui(ui)
{
    //Store reference to UI elements
}
