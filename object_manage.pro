QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adddialog.cpp \
    addmemberdialog.cpp \
    classtable.cpp \
    createinstancedialog.cpp \
    finddialog.cpp \
    main.cpp \
    mainwindow.cpp \
    object.cpp \
    object_member.cpp \
    object_member_2dialog.cpp \
    object_memberdialog.cpp \
    objectinstance.cpp \
    readonlydelegate.cpp

HEADERS += \
    adddialog.h \
    addmemberdialog.h \
    classtable.h \
    createinstancedialog.h \
    finddialog.h \
    mainwindow.h \
    object.h \
    object_member.h \
    object_member_2dialog.h \
    object_memberdialog.h \
    objectinstance.h \
    readonlydelegate.h

FORMS += \
    adddialog.ui \
    addmemberdialog.ui \
    createinstancedialog.ui \
    finddialog.ui \
    mainwindow.ui \
    object_member_2dialog.ui \
    object_memberdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
