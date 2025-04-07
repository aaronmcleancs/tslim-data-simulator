#include "bolus.h"
#include "ui_bolus.h"
#include "headers/CGM.h"
#include "headers/BolusCalculator.h"
#include "mainwindow.h"

bolus::bolus(Pump* pump,ContentWidget* contentWidget, QWidget *parent) :
    ui(new Ui::bolus),
    pump(pump),
    contentWidget(contentWidget)

{
    ui->setupUi(this);
    //initial ui setups
    setLayout();
    checked = false;
    if(pump == nullptr){
        qDebug() << "no pump in bolus construct";
    }
    connect(contentWidget, &ContentWidget::cancelBolusRequested, this, &bolus::cancelBolus);


}

bolus::~bolus()
{
    delete ui;
}


//initial layout setup for the ui.
void bolus :: setLayout(){
    ui->label_8->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_8->setVisible(false);
    ui->checkBox_2->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    ui->lineEdit_4->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->label_10->setVisible(false);
    ui->label_11->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_9->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->checkBox->setEnabled(false);
}

//procedure to be followed when bolus is calculated.
void bolus::on_pushButton_4_clicked()
{
    if (!pump) {
        qDebug() << "Pump is not initialized!";
        return;
    }
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()){
        qDebug()<<"Both information required form manual bolus calculations.... ";
        return;
    }
    ui->label_8->setVisible(true);
    ui->label_4->setVisible(true);
    BolusCalculator b1;
    totalBolus = b1.total_bolus(ui->lineEdit->text().toInt(), ui->lineEdit_2->text().toInt(),pump->getActiveProfile(),pump->getInsulinCartridge()   );
    if(totalBolus>0){

        ui->label_4->setText(QString :: number(totalBolus,'g',5));
        ui->checkBox_2->setVisible(true);
        ui->pushButton->setVisible(true);
        if(totalBolus < pump->getActiveProfile()->getTargetGlucoseRange().first)
            ui->checkBox->setEnabled(true);
    }else{
        ui->label_4->setText("No Bolus Required");

    }

}

// yo is this even real anymore, someone check if it's not then remove i think
// i take this back i hate this button------- this is the automatic button (As the ceo of controliq i approve! :D)

//Procedure to be followed when glucose autopopulation is  requested.
void bolus::on_pushButton_3_clicked()
{


    if (!pump) {
        qDebug() << "Pump is not initialized!";
        return;
    }

    CGM* cgm = pump->getCGM();
    if (!cgm) {
        qDebug() << "Failed to find CGM!";
        return;
    }


    double glucose = cgm->getCurrentGlucoseLevel();

    ui->lineEdit_2->setText(QString::number(glucose));

    qDebug() << "Automatic mode.....";
    cgm->setControlIQActive(true);
    qDebug() << "ControlIQ started!";
}




//Displays the appropriate bolus information at delivery
void bolus::on_pushButton_2_clicked()
{
    double delivery = (totalBolus * ui->lineEdit_3->text().toInt())/100;
    ui->label_7->setVisible(true);
    ui->label_9->setVisible(true);
    ui->label_10->setVisible(true);
    ui->label_11->setVisible(true);
    ui->pushButton->setVisible(true);
    ui->checkBox_2->setEnabled(true);
    ui->label_9->setText(QString :: number(delivery,'g',5));
    ui->label_11->setText(QString :: number(totalBolus - delivery,'g',5));
    ui->pushButton->setEnabled(true);

}



//Procedure when the bolus is initiated
void bolus::on_pushButton_clicked()
{
    pump->recordBolus(totalBolus, "Manual");
    qDebug()<<"Manual bolus Initiated....";
    pump->recordAlert("Manual Bolus Initiated!", totalBolus * 1.0);

    pump->getCGM()->applyInsulinEffect(totalBolus * 1.0);
    emit BolusInitiated();
    bolusState = true;

    setLayout();
    checked = false;
    emit mainShift();

}

//Checks and determines the state of the bolus either extended or quick
void bolus::on_checkBox_2_stateChanged(int arg1)
{

        if(ui->checkBox_2->isChecked()){
            qDebug()<<"Checkeed";
            ui->label_5->setVisible(true);
            ui->label_6->setVisible(true);
            ui->lineEdit_3->setVisible(true);
            ui->lineEdit_4->setVisible(true);
            ui->pushButton_2->setVisible(true);
            ui->pushButton->setEnabled(false);
            checked = !checked;
        }else{

            ui->label_5->setVisible(false);
            ui->label_6->setVisible(false);
            ui->lineEdit_3->setVisible(false);
            ui->lineEdit_4->setVisible(false);
            ui->pushButton_2->setVisible(false);
            ui->label_7->setVisible(false);
            ui->label_9->setVisible(false);
            ui->label_10->setVisible(false);
            ui->label_11->setVisible(false);
            ui->pushButton->setEnabled(true);

        }


}


//Procedure to be followed when cancel is triggered in the contentwidget.
void bolus::cancelBolus() {
    qDebug() << "cancelBolus() triggered. Current state: bolusState=" << bolusState
             << ", totalBolus=" << totalBolus;

    if (bolusState && totalBolus > 0.0) {
        pump->getCGM()->applyInsulinEffect(-totalBolus);
        pump->recordAlert("Bolus Canceled!", totalBolus);
        qDebug() << "Bolus successfully canceled. Units canceled:" << totalBolus;

        totalBolus = 0.0;
        bolusState = false;
    } else {
        qDebug() << "No active bolus found or totalBolus is zero!";
    }
}
