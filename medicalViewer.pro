#-------------------------------------------------
#
# Project created by QtCreator 2017-12-12T14:12:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = medicalViewer
TEMPLATE = app
INCLUDEPATH+=D:\opencv2\release\install\include
LIBS+=D:\opencv2\release\bin\libopencv_core249.dll
LIBS+=D:\opencv2\release\bin\libopencv_highgui249.dll
LIBS+=D:\opencv2\release\bin\libopencv_imgproc249.dll
LIBS+=D:\opencv2\release\bin\libopencv_calib3d249.dll
LIBS+=D:\opencv2\release\bin\libopencv_calib3d249.dll

LIBS+=D:\opencv2\release\bin\libopencv_video249.dll



SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES +=

DISTFILES += \
    helloWorld.py
