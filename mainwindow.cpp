#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(StatusBar *statusBar, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , statusBar(statusBar)
    , currentWidget(nullptr)
{
    ui->setupUi(this);

    // setting up status bar
    this->statusBar->setParent(this);
    
    // adding status bar to the UI
    if (ui->statusBarContainer && ui->statusBarContainer->layout()) {
        ui->statusBarContainer->layout()->addWidget(this->statusBar);
    }
    
    // in MainWindow constructor instead of creating a new layout:
    routerLayout = qobject_cast<QVBoxLayout*>(ui->routerOutlet->layout());

    // initialize screens
    lockScreen = new LockScreen(this->statusBar);
    contentWidget = new ContentWidget();
    pump = contentWidget->getPump();
    optionsWidget = new OptionsWindow(pump, contentWidget);

    bolusWidget = new bolus(pump, contentWidget ,nullptr);
    powerOffWidget = new PowerOff(nullptr);
    // Connecting authentication state changes

    StatusModel* statusModel = StatusModel::getInstance();
    powerStateMachine = new PowerStateMachine(statusModel);

    AuthManager* authManager = AuthManager::getInstance();
    connect(authManager, &AuthManager::authStateChanged, this, &MainWindow::onAuthStateChanged);
    connect(powerStateMachine, &PowerStateMachine::poweredChanged, this, &MainWindow::onPowerStateChanged);
    connect(powerOffWidget, &PowerOff::powerOn, powerStateMachine, &PowerStateMachine::powerOn);
    connect(optionsWidget, &OptionsWindow::powerOff, this, &MainWindow::handlePowerOff);

    // connect lock screen unlock signal
    connect(contentWidget, &ContentWidget::openBolus, [this]() {
        navigateToRoute(Route::BOLUS);
    });
    connect(contentWidget, &ContentWidget::openOptions, [this]() {
        navigateToRoute(Route::OPTIONS);
    });

    if (powerStateMachine->isPowered()) {
        if (authManager->isAuthenticated()) {
            navigateToRoute(Route::CONTENT);
        } else {
            navigateToRoute(Route::LOCK_SCREEN);
        }
    } else {
        navigateToRoute(Route::POWER_OFF);
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

bolus* MainWindow::getBolus() const{
    return bolusWidget;
}
void MainWindow::navigateToRoute(Route route)
{
    // remove current widget from layout if exists
    if (currentWidget) {
        routerLayout->removeWidget(currentWidget);
        currentWidget->hide();
    }
    
    // switch to the widget based on the route
    switch (route) {
        case Route::LOCK_SCREEN:
            currentWidget = lockScreen;
            lockScreen->clearPin();
            break;

        case Route::OPTIONS:
            currentWidget = optionsWidget;
        break;

        case Route::BOLUS:
            currentWidget = bolusWidget;
            break;
            
        case Route::SETTINGS:
            break;

        case Route::POWER_OFF:
            currentWidget = powerOffWidget;
            break;
        case Route::CONTENT:
        default:
            // default
            currentWidget = contentWidget;
            break;
    }
    
    // add/show the current widget
    routerLayout->addWidget(currentWidget);
    currentWidget->show();
}

Pump* MainWindow::getPump() const {
    Q_CHECK_PTR(pump);
    return pump;
}

void MainWindow::onAuthStateChanged(bool authenticated)
{
    if (authenticated) {
        navigateToRoute(Route::CONTENT);
    } else {
        navigateToRoute(Route::LOCK_SCREEN);
    }
}

void MainWindow::onPowerStateChanged(bool power) {
    if (power) {
        on_homeButton_clicked();
    } else {
        navigateToRoute(Route::POWER_OFF);
    }
}

ContentWidget* MainWindow::getContentWidget() const{
    return contentWidget;
}

void MainWindow::on_homeButton_clicked()
{
    AuthManager* authManager = AuthManager::getInstance();
    onAuthStateChanged(authManager->isAuthenticated());
}

void MainWindow::handlePowerOff()
{
    AuthManager* authManager = AuthManager::getInstance();
    authManager->setAuthenticated(false);
    powerStateMachine->powerOff();
}
