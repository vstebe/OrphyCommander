TEMPLATE = app
TARGET = 
DEPENDPATH += . \

include(qextserialport/src/qextserialport.pri)

QT += script
INCLUDEPATH += . src  qextserialport/src
LIBS+=-lqwt


# Input
HEADERS += src/config.h \
           src/CustMathFuncWindow.h \
           src/Graph.h \
           src/GraphData.h \
           src/MainWindow.h \
           src/OrphyCommander.h \
           src/PeriodicFunction.h \
           src/SignalMaker.h \
           src/SpinBoxFocusSignal.h \

FORMS += src/CustMathFuncWindow.ui src/MainWindow.ui
SOURCES += src/CustMathFuncWindow.cpp \
        src/Graph.cpp \
        src/GraphData.cpp \
        src/main.cpp \
        src/MainWindow.cpp \
        src/OrphyCommander.cpp \
        src/PeriodicFunction.cpp \


RESOURCES += src/resources.qrc
