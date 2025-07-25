QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    source/chasing_strategy.cpp \
    source/game.cpp \
    source/game_object.cpp \
    source/ghost.cpp \
    source/main.cpp \
    source/mainwindow.cpp \
    source/pacman.cpp \
    source/startscene.cpp

HEADERS += \
    source/game.h \
    source/gameobject.h \
    source/mainwindow.h \
    source/startscene.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
