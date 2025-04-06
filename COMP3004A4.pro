QT += core gui widgets printsupport  # Add printsupport module

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bolus.cpp \
    cpp/Battery.cpp \
    cpp/ControlIQ.cpp \
    cpp/BolusCalculator.cpp \
    cpp/CGM.cpp \
    cpp/InsulinCartridge.cpp \
    cpp/Profile.cpp \
    cpp/PumpHistory.cpp \
    cpp/Pump.cpp \
    cpp/authmanager.cpp \
    cpp/contentwidget.cpp \
    cpp/lockscreen.cpp \
    cpp/powerstatemachine.cpp \
    cpp/statusmodel.cpp \
    cpp/QCustomPlot.cpp \
    main.cpp \
    mainwindow.cpp \
    optionswindow.cpp \
    poweroff.cpp \
    statusbar.cpp
HEADERS += \
    bolus.h \
    headers/Battery.h \
    headers/ControlIQ.h \
    headers/CGM.h \
    headers/InsulinCartridge.h \
    headers/BolusCalculator.h \
    headers/Profile.h \
    headers/PumpHistory.h \
    headers/Pump.h \
    headers/authmanager.h \
    headers/contentwidget.h \
    headers/lockscreen.h \
    headers/powerstatemachine.h \
    headers/statusmodel.h \
    headers/QCustomPlot.h \
    mainwindow.h \
    options.h \
    optionswindow.h \
    poweroff.h \
    statusbar.h

FORMS += \
    bolus.ui \
    contentwidget.ui \
    lockscreen2.ui \
    mainwindow.ui \
    optionswindow.ui \
    optionswindow.ui \
    poweroff.ui \
    statusbar.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
