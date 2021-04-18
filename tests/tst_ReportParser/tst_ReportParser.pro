QT += testlib
QT += network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_ReportParser.cpp

INCLUDEPATH +=  ../../

LIBS += -L../../BorgarsWorkPunchLib -lBorgarsWorkPunchLib
PRE_TARGETDEPS += $$OUT_PWD/../../BorgarsWorkPunchLib/libBorgarsWorkPunchLib.a

