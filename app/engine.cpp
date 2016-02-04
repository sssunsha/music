#include "engine.h"
#include "utils.h"

#include <math.h>

#include <QAudioOutput>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QMetaObject>
#include <QSet>
#include <QThread>

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

const qint64 BufferDurationUs       = 10 * 1000000;
const int    NotifyIntervalMs       = 100;

// Size of the level calculation window in microseconds
const int    LevelWindowUs          = 0.1 * 1000000;

//-----------------------------------------------------------------------------
// Constructor and destructor
//-----------------------------------------------------------------------------

Engine::Engine(QObject *parent)
    :   QObject(parent)
    ,   m_mode(QAudio::AudioInput)
    ,   m_state(QAudio::StoppedState)
    ,   m_generateTone(false)
    ,   m_file(0)
    ,   m_analysisFile(0)
    ,   m_availableAudioOutputDevices
            (QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    ,   m_audioOutputDevice(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_audioOutput(0)
    ,   m_playPosition(0)
    ,   m_bufferPosition(0)
    ,   m_bufferLength(0)
    ,   m_dataLength(0)
    ,   m_levelBufferLength(0)
    ,   m_rmsLevel(0.0)
    ,   m_peakLevel(0.0)
    ,   m_spectrumBufferLength(0)
    ,   m_spectrumAnalyser()
    ,   m_spectrumPosition(0)
    ,   m_ffmpegHelper(new ffmpegHelper())
    ,   m_count(0)
{
    qRegisterMetaType<FrequencySpectrum>("FrequencySpectrum");
    qRegisterMetaType<WindowFunction>("WindowFunction");
    CHECKED_CONNECT(&m_spectrumAnalyser,
                    SIGNAL(spectrumChanged(FrequencySpectrum)),
                    this,
                    SLOT(spectrumChanged(FrequencySpectrum)));

    connect(this, SIGNAL(bufferChanged(qint64, qint64, const QByteArray)),this,SLOT(handleBufferChanged(qint64, qint64, const QByteArray)));

    initialize();

    m_bar.resize(SpectrumNumBands);
}

Engine::~Engine()
{   
    delete m_ffmpegHelper;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

bool Engine::loadFile(const QString &fileName)
{
    reset();
    bool result = false;
    Q_ASSERT(!m_generateTone);
    Q_ASSERT(!m_file);
    Q_ASSERT(!fileName.isEmpty());
    m_file = new WavFile(this);
    if (m_file->open(fileName)) {
        if (isPCMS16LE(m_file->fileFormat())) {
            result = initialize();
        } else {
            emit errorMessage(tr("Audio format not supported"),
                              formatToString(m_file->fileFormat()));
        }
    } else {
        // transform to wav for other audio format
        QString wavFile = this->m_ffmpegHelper->transform2WAV(fileName);
        if(wavFile == NULL)
        {
            // can not transform correctly
            emit errorMessage(tr("Could not open file"), fileName);
        }
        else
        {
            loadFile(wavFile);
        }

    }
    if (result) {
        m_analysisFile = new WavFile(this);
        m_analysisFile->open(fileName);
    }
    return result;
}

qint64 Engine::bufferLength() const
{
    return m_file ? m_file->size() : m_bufferLength;
}


//-----------------------------------------------------------------------------
// Public slots
//-----------------------------------------------------------------------------

void Engine::startPlayback()
{
    if (m_audioOutput) {
        if (QAudio::AudioOutput == m_mode &&
            QAudio::SuspendedState == m_state) {
            m_audioOutput->resume();
        } else {
            m_spectrumAnalyser.cancelCalculation();
            spectrumChanged(0, 0, FrequencySpectrum());
            setPlayPosition(0, true);
            m_mode = QAudio::AudioOutput;
            CHECKED_CONNECT(m_audioOutput, SIGNAL(stateChanged(QAudio::State)),
                            this, SLOT(audioStateChanged(QAudio::State)));
            CHECKED_CONNECT(m_audioOutput, SIGNAL(notify()),
                            this, SLOT(audioNotify()));
            m_count = 0;
            if (m_file) {
                m_file->seek(0);
                m_bufferPosition = 0;
                m_dataLength = 0;
                m_audioOutput->start(m_file);
            } else {
                m_audioOutputIODevice.close();
                m_audioOutputIODevice.setBuffer(&m_buffer);
                m_audioOutputIODevice.open(QIODevice::ReadOnly);
                m_audioOutput->start(&m_audioOutputIODevice);
            }
        }
    }
}

void Engine::suspend()
{
    if (QAudio::ActiveState == m_state ||
        QAudio::IdleState == m_state) {
        switch (m_mode) {
        case QAudio::AudioOutput:
            m_audioOutput->suspend();
            break;
        }
    }
}

void Engine::setAudioOutputDevice(const QAudioDeviceInfo &device)
{
    if (device.deviceName() != m_audioOutputDevice.deviceName()) {
        m_audioOutputDevice = device;
        initialize();
    }
}


//-----------------------------------------------------------------------------
// Private slots
//-----------------------------------------------------------------------------

void Engine::audioNotify()
{
    switch (m_mode) {
    case QAudio::AudioOutput: {
            const qint64 playPosition = audioLength(m_format, m_audioOutput->processedUSecs());
            setPlayPosition(qMin(bufferLength(), playPosition));
            const qint64 levelPosition = playPosition - m_levelBufferLength;
            const qint64 spectrumPosition = playPosition - m_spectrumBufferLength;
            if (m_file) {
                if (levelPosition > m_bufferPosition ||
                    spectrumPosition > m_bufferPosition ||
                    qMax(m_levelBufferLength, m_spectrumBufferLength) > m_dataLength) {
                    m_bufferPosition = 0;
                    m_dataLength = 0;
                    // Data needs to be read into m_buffer in order to be analysed
                    const qint64 readPos = qMax(qint64(0), qMin(levelPosition, spectrumPosition));
                    const qint64 readEnd = qMin(m_analysisFile->size(), qMax(levelPosition + m_levelBufferLength, spectrumPosition + m_spectrumBufferLength));
                    const qint64 readLen = readEnd - readPos + audioLength(m_format, WaveformWindowDuration);
                    qDebug() << "Engine::audioNotify [1]"
                             << "analysisFileSize" << m_analysisFile->size()
                             << "readPos" << readPos
                             << "readLen" << readLen;
                    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength())) {
                        m_buffer.resize(readLen);
                        m_bufferPosition = readPos;
                        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
                        qDebug() << "Engine::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
                    } else {
                        qDebug() << "Engine::audioNotify [2]" << "file seek error";
                    }
                    qDebug() << "Engine::audioNotify [2]" << "m_bufferPosition: " << m_bufferPosition;
                    qDebug() << "Engine::audioNotify [2]" << "m_dataLength: " << m_dataLength;
                    emit bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
                }
            } else {
                if (playPosition >= m_dataLength)
                    stopPlayback();
            }
            if (levelPosition >= 0 && levelPosition + m_levelBufferLength < m_bufferPosition + m_dataLength)
                calculateLevel(levelPosition, m_levelBufferLength);
            if (spectrumPosition >= 0 && spectrumPosition + m_spectrumBufferLength < m_bufferPosition + m_dataLength)
                calculateSpectrum(spectrumPosition);
        }
        break;
    }
}

void Engine::audioStateChanged(QAudio::State state)
{
    ENGINE_DEBUG << "Engine::audioStateChanged from" << m_state
                 << "to" << state;

    if (QAudio::IdleState == state && m_file && m_file->pos() == m_file->size()) {
        stopPlayback();
    } else {
        if (QAudio::StoppedState == state) {
            // Check error
            QAudio::Error error = QAudio::NoError;
            switch (m_mode) {
            case QAudio::AudioOutput:
                error = m_audioOutput->error();
                break;
            }
            if (QAudio::NoError != error) {
                reset();
                return;
            }
        }
        setState(state);
    }
}

void Engine::updateBars()
{
    m_barData.clear();

    FrequencySpectrum::const_iterator i = m_spectrum.begin();
    const FrequencySpectrum::const_iterator end = m_spectrum.end();
    for ( ; i != end; ++i) {
//        qDebug() << "Engine::updateBars:";
        const FrequencySpectrum::Element e = *i;
        if (e.frequency >= SpectrumLowFreq && e.frequency < SpectrumHighFreq) {
            int index = barIndex(e.frequency);
            int value = (int) (e.amplitude * 450);
//            m_bar[index] = qMax(m_bar.at(index), value);
            m_bar[index] = value;
        }
    }

    for(int i=0; i<SpectrumNumBands; i++){
        m_barData.append(m_bar[i]);
//        qDebug() << "Engine::updateBars m_barData:" << m_barData.at(i);
    }

    emit barDataChanged(m_barData);
}

int Engine::barIndex(qreal frequency) const
{
//    qDebug() << "Engine::barIndex frequency:" << frequency;
    Q_ASSERT(frequency >= SpectrumLowFreq && frequency < SpectrumHighFreq);
    const qreal bandWidth = (SpectrumHighFreq - SpectrumLowFreq) / SpectrumNumBands;
    const int index = (frequency - SpectrumLowFreq) / bandWidth;
    if (index <0 || index >= SpectrumNumBands)
        Q_ASSERT(false);
//    qDebug() << "Engine::barIndex index:" << index;
    return index;
}

void Engine::spectrumChanged(const FrequencySpectrum &spectrum)
{
    ENGINE_DEBUG << "Engine::spectrumChanged" << "pos" << m_spectrumPosition;
    m_spectrum = spectrum;
    updateBars();
//    emit spectrumChanged(m_spectrumPosition, m_spectrumBufferLength, spectrum);
}


//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

void Engine::resetAudioDevices()
{
    delete m_audioOutput;
    m_audioOutput = 0;
    setPlayPosition(0);
    m_spectrumPosition = 0;
    setLevel(0.0, 0.0, 0);
}

void Engine::reset()
{
    stopPlayback();
    setState(QAudio::AudioInput, QAudio::StoppedState);
    setFormat(QAudioFormat());
    m_generateTone = false;
    delete m_file;
    m_file = 0;
    delete m_analysisFile;
    m_analysisFile = 0;
    m_buffer.clear();
    m_bufferPosition = 0;
    m_bufferLength = 0;
    m_dataLength = 0;
    emit dataLengthChanged(0);
    resetAudioDevices();
}

bool Engine::initialize()
{
    bool result = false;

    QAudioFormat format = m_format;

    if (selectFormat()) {
        if (m_format != format) {
            resetAudioDevices();
            if (m_file) {
                emit bufferLengthChanged(bufferLength());
                emit dataLengthChanged(dataLength());
//                emit bufferChanged(0, 0, m_buffer);
                result = true;
            } else {
                m_bufferLength = audioLength(m_format, BufferDurationUs);
                m_buffer.resize(m_bufferLength);
                m_buffer.fill(0);
                emit bufferLengthChanged(bufferLength());
                if (m_generateTone) {
                    if (0 == m_tone.endFreq) {
                        const qreal nyquist = nyquistFrequency(m_format);
                        m_tone.endFreq = qMin(qreal(SpectrumHighFreq), nyquist);
                    }
                    // Call function defined in utils.h, at global scope
                    m_dataLength = m_bufferLength;
                    emit dataLengthChanged(dataLength());
//                    emit bufferChanged(0, m_dataLength, m_buffer);
                    result = true;
                } else {
//                    emit bufferChanged(0, 0, m_buffer);
                    result = true;
                }
            }
            m_audioOutput = new QAudioOutput(m_audioOutputDevice, m_format, this);
            m_audioOutput->setNotifyInterval(NotifyIntervalMs);
        }
    } else {
        if (m_file)
            emit errorMessage(tr("Audio format not supported"),
                              formatToString(m_format));
        else if (m_generateTone)
            emit errorMessage(tr("No suitable format found"), "");
        else
            emit errorMessage(tr("No common input / output format found"), "");
    }

    ENGINE_DEBUG << "Engine::initialize" << "m_bufferLength" << m_bufferLength;
    ENGINE_DEBUG << "Engine::initialize" << "m_dataLength" << m_dataLength;
    ENGINE_DEBUG << "Engine::initialize" << "format" << m_format;

    return result;
}

bool Engine::selectFormat()
{
    bool foundSupportedFormat = false;

    if (m_file || QAudioFormat() != m_format) {
        QAudioFormat format = m_format;
        if (m_file)
            // Header is read from the WAV file; just need to check whether
            // it is supported by the audio output device
            format = m_file->fileFormat();
        if (m_audioOutputDevice.isFormatSupported(format)) {
            setFormat(format);
            foundSupportedFormat = true;
        }
    } else {

        QList<int> sampleRatesList;

        if (!m_generateTone)

        sampleRatesList += m_audioOutputDevice.supportedSampleRates();
        sampleRatesList = sampleRatesList.toSet().toList(); // remove duplicates
        qSort(sampleRatesList);
        ENGINE_DEBUG << "Engine::initialize frequenciesList" << sampleRatesList;

        QList<int> channelsList;
        channelsList += m_audioOutputDevice.supportedChannelCounts();
        channelsList = channelsList.toSet().toList();
        qSort(channelsList);
        ENGINE_DEBUG << "Engine::initialize channelsList" << channelsList;

        QAudioFormat format;
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setCodec("audio/pcm");
        format.setSampleSize(16);
        format.setSampleType(QAudioFormat::SignedInt);
        int sampleRate, channels;
        foreach (sampleRate, sampleRatesList) {
            if (foundSupportedFormat)
                break;
            format.setSampleRate(sampleRate);
            foreach (channels, channelsList) {
                format.setChannelCount(channels);
                const bool outputSupport = m_audioOutputDevice.isFormatSupported(format);
                ENGINE_DEBUG << "Engine::initialize checking " << format
                             << "output" << outputSupport;
                if (outputSupport)
                {
                    foundSupportedFormat = true;
                    break;
                }
            }
        }

        if (!foundSupportedFormat)
            format = QAudioFormat();

        setFormat(format);
    }

    return foundSupportedFormat;
}


void Engine::stopPlayback()
{
    if (m_audioOutput) {
        m_audioOutput->stop();
        QCoreApplication::instance()->processEvents();
        m_audioOutput->disconnect();
        setPlayPosition(0);
    }
}

void Engine::setState(QAudio::State state)
{
    const bool changed = (m_state != state);
    m_state = state;
    if (changed)
        emit stateChanged(m_mode, m_state);
}

void Engine::setState(QAudio::Mode mode, QAudio::State state)
{
    const bool changed = (m_mode != mode || m_state != state);
    m_mode = mode;
    m_state = state;
    if (changed)
        emit stateChanged(m_mode, m_state);
}

void Engine::setPlayPosition(qint64 position, bool forceEmit)
{
    const bool changed = (m_playPosition != position);
    m_playPosition = position;
    if (changed || forceEmit)
    {
        emit playPositionChanged(m_playPosition);

        if(m_file)
        {
            qreal currentTime = m_audioOutput->processedUSecs() / 1000000.0;
            qreal durations   = m_file->audioDuration();

            emit elapsedTimeChanged(currentTime, durations);
        }
    }
}

void Engine::calculateLevel(qint64 position, qint64 length)
{
    Q_ASSERT(position + length <= m_bufferPosition + m_dataLength);

    qreal peakLevel = 0.0;

    qreal sum = 0.0;
    const char *ptr = m_buffer.constData() + position - m_bufferPosition;
    const char *const end = ptr + length;
    while (ptr < end) {
        const qint16 value = *reinterpret_cast<const qint16*>(ptr);
        const qreal fracValue = pcmToReal(value);
        peakLevel = qMax(peakLevel, fracValue);
        sum += fracValue * fracValue;
        ptr += 2;
    }
    const int numSamples = length / 2;
    qreal rmsLevel = sqrt(sum / numSamples);

    rmsLevel = qMax(qreal(0.0), rmsLevel);
    rmsLevel = qMin(qreal(1.0), rmsLevel);
    setLevel(rmsLevel, peakLevel, numSamples);

    ENGINE_DEBUG << "Engine::calculateLevel" << "pos" << position << "len" << length
                 << "rms" << rmsLevel << "peak" << peakLevel;
}

void Engine::calculateSpectrum(qint64 position)
{
    Q_ASSERT(position + m_spectrumBufferLength <= m_bufferPosition + m_dataLength);
    Q_ASSERT(0 == m_spectrumBufferLength % 2); // constraint of FFT algorithm

    // QThread::currentThread is marked 'for internal use only', but
    // we're only using it for debug output here, so it's probably OK :)
    ENGINE_DEBUG << "Engine::calculateSpectrum" << QThread::currentThread()
                 << "count" << m_count << "pos" << position << "len" << m_spectrumBufferLength
                 << "spectrumAnalyser.isReady" << m_spectrumAnalyser.isReady();

    if (m_spectrumAnalyser.isReady()) {
        m_spectrumBuffer = QByteArray::fromRawData(m_buffer.constData() + position - m_bufferPosition,
                                                   m_spectrumBufferLength);
        m_spectrumPosition = position;
        m_spectrumAnalyser.calculate(m_spectrumBuffer, m_format);
    }
}

void Engine::setFormat(const QAudioFormat &format)
{
    const bool changed = (format != m_format);
    m_format = format;
    m_levelBufferLength = audioLength(m_format, LevelWindowUs);
    m_spectrumBufferLength = SpectrumLengthSamples *
                            (m_format.sampleSize() / 8) * m_format.channelCount();
    if (changed)
        emit formatChanged(m_format);
}

void Engine::setLevel(qreal rmsLevel, qreal peakLevel, int numSamples)
{
    m_rmsLevel = rmsLevel;
    m_peakLevel = peakLevel;
    emit levelChanged(m_rmsLevel, m_peakLevel, numSamples);
}

void Engine::handleBufferChanged(qint64 position, qint64 length, const QByteArray &buffer)
{
    qreal progressBarValue;

    int fileSize = m_analysisFile->size();
    qDebug() << "Engine::handleBufferChanged" << "fileSize: " << fileSize;
    qDebug() << "Engine::handleBufferChanged" << "position: " << position;
    progressBarValue = ((qreal)position) / ((qreal)fileSize);
    qDebug() << "Engine::handleBufferChanged" << "progressBarValue: " << progressBarValue;
    emit progressChanged(progressBarValue);
}
