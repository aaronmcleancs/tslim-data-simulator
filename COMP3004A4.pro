QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    cpp/Battery.cpp \
#    cpp/CGM.cpp \
#    cpp/InsulinCartridge.cpp \
#    cpp/Profile.cpp \
#    cpp/Pump.cpp \
#    cpp/UI.cpp \
    cpp/Battery.cpp \
    cpp/BolusCalculator.cpp \
    cpp/CGM.cpp \
    cpp/InsulinCartridge.cpp \
    cpp/Profile.cpp \
    cpp/Pump.cpp \
    cpp/UI.cpp \
    main.cpp \
    mainwindow.cpp \
    statusbar.cpp
HEADERS += \
#    headers/Battery.h \
#    headers/CGM.h \
#    headers/InsulinCartridge.h \
#    headers/Profile.h \
#    headers/Pump.h \
#    headers/UI.h \
    headers/Battery.h \
    headers/BolusCalculator.h \
    headers/CGM.h \
    headers/InsulinCartridge.h \
    headers/Profile.h \
    headers/Pump.h \
    headers/UI.h \
    mainwindow.h \
    statusbar.h

FORMS += \
    mainwindow.ui \
    statusbar.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
