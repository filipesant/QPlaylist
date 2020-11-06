QT += core \
      gui \
      network \
      xml \
      multimedia \
      multimediawidgets \
      widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialogcreateplaylist.cpp \
    dialogcredentials.cpp \
    layout.cpp \
    main.cpp \
    mainwindow.cpp \
    playercontroller.cpp \
    playercontrols.cpp \
    playlist.cpp \
    playlistview.cpp \
    playlistdelegate.cpp \
    searchview.cpp \
    sidemenu.cpp \
    song.cpp \
    spotifycredentials.cpp \
    spotifyrequests.cpp

HEADERS += \
    dialogcreateplaylist.h \
    dialogcredentials.h \
    layout.h \
    mainwindow.h \
    playercontroller.h \
    playercontrols.h \
    playlist.h \
    playlistview.h \
    playlistdelegate.h \
    searchview.h \
    sidemenu.h \
    song.h \
    spotifycredentials.h \
    spotifyrequests.h

FORMS += \
    dialogcreateplaylist.ui \
    dialogcredentials.ui \
    mainwindow.ui \
    playercontrols.ui \
    playlistview.ui \
    searchview.ui \
    sidemenu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image_source.qrc
