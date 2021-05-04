QT += gui
QT += network
QT += sql

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
   src/CppInterface.cpp \
   src/DayReport.cpp \
   src/Formatter.cpp \
   src/FirebaseAuthHandler.cpp \
   src/FirebaseInterface.cpp \
   src/ReportParser.cpp \
   src/SQLInterface.cpp \
   src/TimeRegistration.cpp \
   src/Util.cpp \
   src/WeekData.cpp \
   src/WeekDatesModel.cpp \
   src/WeekReport.cpp \
   src/WeekReportModel.cpp

HEADERS += \
   src/Constants.h \
   src/CppInterface.h \
   src/DayReport.h \
   src/Formatter.h \
   src/FirebaseAuthHandler.h \
   src/FirebaseInterface.h \
   src/ReportParser.h \
   src/SQLInterface.h \
   src/TimeRegistration.h \
   src/Util.h \
   src/WeekData.h \
   src/WeekDatesModel.h \
   src/WeekReport.h \
   src/WeekReportModel.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
