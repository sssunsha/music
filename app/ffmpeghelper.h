#ifndef FFMPEGHELPER_H
#define FFMPEGHELPER_H

#include <QObject>
#include <QString>
#include <QProcess>

const QString OutPutPath = "/tmp/tietomusic/";

class ffmpegHelper : public QObject
{
    Q_OBJECT
public:
    ffmpegHelper();
    ~ffmpegHelper();
    QString transform2WAV(QString fileName);
    bool isFFmpegSupport();

private slots:
    void	handle_error(QProcess::ProcessError error);
    void	handle_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void	handle_readyReadStandardError();
    void	handle_readyReadStandardOutput();
    void	handle_started();
    void	handle_stateChanged(QProcess::ProcessState newState);


private:
    void init();
    void CheckFFmpegSupportOrNot();
    QString audioFileNameFormat(QString fileName);


    bool m_isFFmpegSupport;
    QProcess* m_process;

    enum ffmepg_state {
        ENONE,
        ECHECK,
        ETRANSFORM
    };

    ffmepg_state m_state;
};

#endif // FFMPEGHELPER_H
