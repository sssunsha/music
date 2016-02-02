unix:!android {
    isEmpty(target.path) {
        qnx {
            target.path = /tmp/$${TARGET}/bin
        } else {
            target.path = /opt/$${TARGET}/bin
        }
        export(target.path)
    }
    INSTALLS += target
}

export(INSTALLS)

#DEFINES += LOG_ENGINE

DEFINES += SPECTRUM_ANALYSER_SEPARATE_THREAD
#DEFINES += LOG_SPECTRUMANALYSER
