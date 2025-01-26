TEMPLATE = app
TARGET = audiopanning

QT += core gui multimedia widgets spatialaudio

SOURCES = main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/spatialaudio/audiopanning
INSTALLS += target