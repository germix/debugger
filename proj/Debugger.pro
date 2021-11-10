#-------------------------------------------------
#
# Project created by QtCreator 2021-11-09T10:39:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#---------------------------------------------------------------------------------------------------
# Version
#---------------------------------------------------------------------------------------------------

DEFINES += MAJOR_VERSION=1
DEFINES += MINOR_VERSION=0

#---------------------------------------------------------------------------------------------------
# Target
#---------------------------------------------------------------------------------------------------
CONFIG(debug, debug|release) {
TARGET = Debugger_d
DEFINES += DEBUG
} else {
TARGET = Debugger
}

#---------------------------------------------------------------------------------------------------
# Libraries
#---------------------------------------------------------------------------------------------------

LIBS += libPsapi

#---------------------------------------------------------------------------------------------------
# Destination folder
#---------------------------------------------------------------------------------------------------
DESTDIR = ../bin

#---------------------------------------------------------------------------------------------------
# Source files
#---------------------------------------------------------------------------------------------------
SOURCES += \
        src/main.cpp \
        src/MainWindow.cpp \
    src/Debugger.cpp \
    src/utils/GetFileNameFromHandle.cpp \
    src/AboutDialog.cpp \
    src/RegistersPanel.cpp \
    src/RecentFilesMenu.cpp \
    src/utils/IsWindowsXP.cpp \
    src/DebuggerHelper.cpp

HEADERS += \
        src/MainWindow.h \
    src/Debugger.h \
    src/utils/GetFileNameFromHandle.h \
    src/AboutDialog.h \
    src/RegistersPanel.h \
    src/RecentFilesMenu.h \
    src/utils/IsWindowsXP.h \
    src/DebuggerHelper.h

FORMS += \
        src/MainWindow.ui \
    src/AboutDialog.ui \
    src/RegistersPanel.ui

RESOURCES += \
    res/resource.qrc

win32:RC_FILE = res/resource_win32.rc
