include(../deployment.pri)

TEMPLATE = app

TARGET = music

QT += qml quick widgets multimedia

HEADERS += engine.h \
    utils.h \
    wavfile.h \
    frequencyspectrum.h \
    spectrumanalyser.h \
    spectrum.h \
    ffmpeghelper.h

SOURCES += main.cpp \
    utils.cpp \
    engine.cpp \
    wavfile.cpp \
    frequencyspectrum.cpp \
    spectrumanalyser.cpp \
    ffmpeghelper.cpp

OTHER_FILES += qml/main.qml \
               BarItem.qml \
               BarGraphArea.qml \
               MainForum.ui.qml

RESOURCES += qml/qml.qrc

fftreal_dir = ../3rdparty/fftreal

INCLUDEPATH += $${fftreal_dir}

# Dynamic linkage against FFTReal DLL

macx {
    # Link to fftreal framework
    LIBS += -F$${fftreal_dir}
    LIBS += -framework fftreal
} else {
    LIBS += -L..$${music_build_dir}
    LIBS += -lfftreal
}


target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/spectrum
INSTALLS += target

# Deployment

DESTDIR = ..$${music_build_dir}
macx {
    !contains(DEFINES, DISABLE_FFT) {
        # Relocate fftreal.framework into spectrum.app bundle
        framework_dir = ../spectrum.app/Contents/Frameworks
        framework_name = fftreal.framework/Versions/1/fftreal
        QMAKE_POST_LINK = \
            mkdir -p $${framework_dir} &&\
            rm -rf $${framework_dir}/fftreal.framework &&\
            cp -R $${fftreal_dir}/fftreal.framework $${framework_dir} &&\
            install_name_tool -id @executable_path/../Frameworks/$${framework_name} \
                                $${framework_dir}/$${framework_name} &&\
            install_name_tool -change $${framework_name} \
                                @executable_path/../Frameworks/$${framework_name} \
                                ../spectrum.app/Contents/MacOS/spectrum
    }
} else {
    linux-g++*: {
        # Provide relative path from application to fftreal library
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
    }
}
