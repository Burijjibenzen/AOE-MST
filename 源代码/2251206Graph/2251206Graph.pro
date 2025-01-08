QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    choosewindow.cpp \
    dijkstra.cpp \
    dijkstrawindow.cpp \
    floyd.cpp \
    floydwindow.cpp \
    graph.cpp \
    graphwindow.cpp \
    kruskal.cpp \
    kruskalwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    prim.cpp \
    primwindow.cpp

HEADERS += \
    choosewindow.h \
    dijkstra.h \
    dijkstrawindow.h \
    floyd.h \
    floydwindow.h \
    graph.h \
    graphwindow.h \
    kruskal.h \
    kruskalwindow.h \
    mainwindow.h \
    prim.h \
    primwindow.h

FORMS += \
    choosewindow.ui \
    dijkstrawindow.ui \
    floydwindow.ui \
    graphwindow.ui \
    kruskalwindow.ui \
    mainwindow.ui \
    primwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
