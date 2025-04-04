#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(StatusBar *statusBar, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , statusBar(statusBar)
    , currentWidget(nullptr)
{
    ui->setupUi(this);
    
    // Set up status bar
    this->statusBar->setParent(this);
    
    // Add status bar to the UI
    if (ui->statusBarContainer && ui->statusBarContainer->layout()) {
        ui->statusBarContainer->layout()->addWidget(this->statusBar);
    }
    
    // In MainWindow constructor - instead of creating a new layout:
    routerLayout = qobject_cast<QVBoxLayout*>(ui->routerOutlet->layout());

    // Initialize screens
    lockScreen = new LockScreen(this->statusBar);
    contentWidget = new ContentWidget();
    this->pump = contentWidget->getPump();
    
    // Connect authentication state changes
    AuthManager* authManager = AuthManager::getInstance();
    connect(authManager, &AuthManager::authStateChanged, this, &MainWindow::onAuthStateChanged);
    
    // Connect lock screen unlock signal
    connect(lockScreen, &LockScreen::unlocked, [this]() {
        navigateToRoute(Route::CONTENT);
    });
    
    // Connect content widget's bolus button to MainWindow's bolusShift signal
    connect(contentWidget, &ContentWidget::bolusRequested, this, &MainWindow::bolusShift);
    
    // Set initial screen based on authentication
    if (authManager->isAuthenticated()) {
        navigateToRoute(Route::CONTENT);
    } else {
        navigateToRoute(Route::LOCK_SCREEN);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::navigateToRoute(Route route)
{
    // Remove current widget from layout if it exists
    if (currentWidget) {
        routerLayout->removeWidget(currentWidget);
        currentWidget->hide();
    }
    
    // Switch to the appropriate widget based on the route
    switch (route) {
        case Route::LOCK_SCREEN:
            currentWidget = lockScreen;
            break;
            
        case Route::CONTENT:
            currentWidget = contentWidget;
            break;
            
        case Route::BOLUS:
            // Emit signal to show bolus screen (handled outside this class)
            emit bolusShift();
            return;  // Return early as we're not changing the current widget
            
        case Route::SETTINGS:
            // Add settings screen handling here
            break;
            
        // Add more cases as needed
        
        default:
            // Default to content if unknown route
            currentWidget = contentWidget;
            break;
    }
    
    // Add and show the current widget
    routerLayout->addWidget(currentWidget);
    currentWidget->show();
}

Pump* MainWindow::getPump() const {
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
