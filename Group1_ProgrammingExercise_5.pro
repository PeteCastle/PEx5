QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    checkoutwindow.cpp \
    confirmaddtocart.cpp \
    inventorywindow.cpp \
    landingpage.cpp \
    main.cpp \
    mainadminwindow.cpp \
    mainwindow.cpp \
    promocodeswindow.cpp \
    viewreceipt.cpp

HEADERS += \
    MenuList.h \
    checkoutwindow.h \
    confirmaddtocart.h \
    inventorywindow.h \
    landingpage.h \
    mainadminwindow.h \
    mainwindow.h \
    promocodeswindow.h \
    viewreceipt.h

FORMS += \
    checkoutwindow.ui \
    confirmaddtocart.ui \
    inventorywindow.ui \
    landingpage.ui \
    mainadminwindow.ui \
    mainwindow.ui \
    promocodeswindow.ui \
    viewreceipt.ui
    confirmaddtocart.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
