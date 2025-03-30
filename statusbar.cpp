#include "statusbar.h"
#include "ui_statusbar.h"

StatusBar::StatusBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::StatusBar)
{
    ui->setupUi(this);
}

StatusBar::~StatusBar()
{
    delete ui;
}
