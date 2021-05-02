TEMPLATE = subdirs
#CONFIG += ordered
SUBDIRS += \
           BorgarsWorkPunchLib \
           BorgarsWorkPunchApp \
           TestApplication \
           tests

BorgarsWorkPunchApp.depends = BorgarsWorkPunchLib
TestApplication.depends = BorgarsWorkPunchLib

DISTFILES += \
   BorgarsWorkPunchApp/qml/DeveloperPage.qml


