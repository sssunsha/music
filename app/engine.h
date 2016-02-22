#ifndef ENGINE_H
#define ENGINE_H

#include "spectrum.h"
#include "spectrumanalyser.h"
#include "wavfile.h"
#include "ffmpeghelper.h"

#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QBuffer>
#include <QByteArray>
#include <QDir>
#include <QObject>
#include <QVector>


class FrequencySpectrum;
QT_BEGIN_NAMESPACE
class QAudioOutput;
QT_END_NAMESPACE

class Engine : public QObject
{
    Q_OBJECT

    Q_DISABLE_COPY(Engine)
public:
    explicit Engine(QObject *parent = 0);
    ~Engine();

    const QList<QAudioDeviceInfo> &availableAudioOutputDevices() const
    { return m_availableAudioOutputDevices; }

    QAudio::Mode mode() const { return m_mode; }
    QAudio::State state() const { return m_state; }

    /**
     * \return Current audio format
     * \note May be QAudioFormat() if engine is not initialized
     */
    const QAudioFormat& format() const { return m_format; }

    /**
     * Stop any ongoing recording or playback, and reset to ground state.
     */
    void reset();

    /**
     * Load data from WAV file
     */
    Q_INVOKABLE bool loadFile(const QString &fileName);

    /**
     * RMS level of the most recently processed set of audio samples.
     * \return Level in range (0.0, 1.0)
     */
    qreal rmsLevel() const { return m_rmsLevel; }

    /**
     * Peak level of the most recently processed set of audio samples.
     * \return Level in range (0.0, 1.0)
     */
    qreal peakLevel() const { return m_peakLevel; }

    /**
     * Position of the audio output device.
     * \return Position in bytes.
     */
    qint64 playPosition() const { return m_playPosition; }

    /**
     * Length of the internal engine buffer.
     * \return Buffer length in bytes.
     */
    qint64 bufferLength() const;

    /**
     * Amount of data held in the buffer.
     * \return Data length in bytes.
     */
    qint64 dataLength() const { return m_dataLength; }


    enum EOS { Linux, Windows, Android, Ios, Mac };
    Q_ENUM(EOS)

    Q_INVOKABLE Engine::EOS get_os();

public slots:
    Q_INVOKABLE void startPlayback();
    Q_INVOKABLE void suspend();
    void setAudioOutputDevice(const QAudioDeviceInfo &device);

signals:
    void stateChanged(QAudio::Mode mode, QAudio::State state);

    /**
     * Informational message for non-modal display
     */
    void infoMessage(const QString &message, int durationMs);

    /**
     * Error message for modal display
     */
    void errorMessage(const QString &heading, const QString &detail);

    /**
     * Format of audio data has changed
     */
    void formatChanged(const QAudioFormat &format);

    /**
     * Length of buffer has changed.
     * \param duration Duration in microseconds
     */
    void bufferLengthChanged(qint64 duration);

    /**
     * Amount of data in buffer has changed.
     * \param Length of data in bytes
     */
    void dataLengthChanged(qint64 duration);

    /**
     * Position of the audio output device has changed.
     * \param position Position in bytes
     */
    void playPositionChanged(qint64 position);

    /**
     * Level changed
     * \param rmsLevel RMS level in range 0.0 - 1.0
     * \param peakLevel Peak level in range 0.0 - 1.0
     * \param numSamples Number of audio samples analyzed
     */
    void levelChanged(qreal rmsLevel, qreal peakLevel, int numSamples);

    /**
     * Spectrum has changed.
     * \param position Position of start of window in bytes
     * \param length   Length of window in bytes
     * \param spectrum Resulting frequency spectrum
     */
    void spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum);

    /**
     * Buffer containing audio data has changed.
     * \param position Position of start of buffer in bytes
     * \param buffer   Buffer
     */
    void bufferChanged(qint64 position, qint64 length, const QByteArray &buffer);

    void barDataChanged(QList<int> barData);

    void progressChanged(qreal position);  

    void elapsedTimeChanged(qreal current, qreal duration);

private slots:
    void audioNotify();
    void audioStateChanged(QAudio::State state);
    void spectrumChanged(const FrequencySpectrum &spectrum);
    void handleBufferChanged(qint64 position, qint64 length, const QByteArray &buffer);

private:
    void resetAudioDevices();
    bool initialize();
    bool selectFormat();
    void stopPlayback();
    void setState(QAudio::State state);
    void setState(QAudio::Mode mode, QAudio::State state);
    void setFormat(const QAudioFormat &format);
    void setPlayPosition(qint64 position, bool forceEmit = false);
    void calculateLevel(qint64 position, qint64 length);
    void calculateSpectrum(qint64 position);
    void setLevel(qreal rmsLevel, qreal peakLevel, int numSamples);

    void updateBars();
    int barIndex(qreal frequency) const;

private:
    QAudio::Mode        m_mode;
    QAudio::State       m_state;

    bool                m_generateTone;
    SweptTone           m_tone;

    WavFile*            m_file;
    // We need a second file handle via which to read data into m_buffer
    // for analysis
    WavFile*            m_analysisFile;

    ffmpegHelper*       m_ffmpegHelper;

    QAudioFormat        m_format;

    const QList<QAudioDeviceInfo> m_availableAudioOutputDevices;
    QAudioDeviceInfo    m_audioOutputDevice;
    QAudioOutput*       m_audioOutput;
    qint64              m_playPosition;
    QBuffer             m_audioOutputIODevice;

    QByteArray          m_buffer;
    qint64              m_bufferPosition;
    qint64              m_bufferLength;
    qint64              m_dataLength;

    int                 m_levelBufferLength;
    qreal               m_rmsLevel;
    qreal               m_peakLevel;

    int                 m_spectrumBufferLength;
    QByteArray          m_spectrumBuffer;
    SpectrumAnalyser    m_spectrumAnalyser;
    qint64              m_spectrumPosition;

    int                 m_count;

    FrequencySpectrum   m_spectrum;
    QVector<int>        m_bar;
    QList<int>          m_barData;

    EOS _OS;
};

#endif // ENGINE_H
