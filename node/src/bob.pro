TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpcap -lcurl -lpthread

SOURCES += main.cpp \
    get_mac.cpp \
    pkt.cpp

HEADERS += \
    radiotap_header.h \
    integrated.h

