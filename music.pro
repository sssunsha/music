# Default rules for deployment.
include(deployment.pri)

TEMPLATE = subdirs

CONFIG  += ordered

SUBDIRS += 3rdparty/fftreal
SUBDIRS += app

TARGET = music
