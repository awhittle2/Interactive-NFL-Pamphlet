QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Abi.cpp \
    Clara.cpp \
    Destiny.cpp \
    Eddie.cpp \
    MeatHandler.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    csv.cpp \
    nfldatatable.cpp \
    sort.cpp \
    utils.cpp

HEADERS += \
    loginwindow.h \
    mainwindow.h \
    csv.h \
    nfldatatable.h \
    sort.h \
    utils.h

FORMS += \
    loginwindow.ui \
    mainwindow.ui


CSV = $$PWD/*.csv

# Some extra magic to allow for us to keep the csv files with the executable.
win32 {
    QMAKE_POST_LINK += $$quote(cmd /c copy /y /B \"$$replace(CSV, /, \\\\)\" \"$$OUT_PWD\\\"$$escape_expand(\n\t))
}

macx {
    QMAKE_POST_LINK += $$quote(cp \"$$CSV\" \"$$OUT_PWD/\"$$escape_expand(\n\t))
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
