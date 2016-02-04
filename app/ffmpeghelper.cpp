#include "ffmpeghelper.h"
#include <QDebug>
#include <QDir>
#include <QFile>


ffmpegHelper::ffmpegHelper()
{
    this->m_isFFmpegSupport = false;
    this->m_process = new QProcess();
    init();
}

ffmpegHelper::~ffmpegHelper()
{
    delete this->m_process;
    this->m_process = NULL;
}

void ffmpegHelper::init()
{
    // create the dir for transform audio
    QDir dir;
    dir.mkdir(OutPutPath);

    // connect the signal and slot
    connect(m_process, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(handle_error(QProcess::ProcessError)));
    connect(m_process, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(handle_finished(int,QProcess::ExitStatus)));
    connect(m_process, SIGNAL(readyReadStandardError()),
            this, SLOT(handle_readyReadStandardError()));
    connect(m_process, SIGNAL(readyReadStandardOutput()),
            this, SLOT(handle_readyReadStandardOutput()));
    connect(m_process, SIGNAL(stateChanged(QProcess::ProcessState)),
            this, SLOT(handle_stateChanged(QProcess::ProcessState)));


    // set the state to ENONE
    this->m_state = ENONE;

    // start to check the ffmpeg is support or not
    this->CheckFFmpegSupportOrNot();
}


// check the ffmpeg is support
void ffmpegHelper::CheckFFmpegSupportOrNot()
{
    QString command = "ffmpeg";
    this->m_state = ECHECK;
    int result = this->m_process->execute(command);
    switch(result)
    {
        case -2:
            this->m_isFFmpegSupport = false;
            break;
        case -1:
            this->m_isFFmpegSupport = true;
            break;
        case 1:
            this->m_isFFmpegSupport = true;
            break;
        default:
            this->m_isFFmpegSupport = false;
    }
}

QString ffmpegHelper::transform2WAV(QString fileName){
    if(!this->m_isFFmpegSupport)
        return NULL;


    // check the wav is already existed or not
    QString outFileName = audioFileNameFormat(fileName);
    if(outFileName == NULL)
        return NULL;

    QFile lookForFile(outFileName);
    if(lookForFile.exists())
        return outFileName;

    QString command = "ffmpeg";
    QStringList args;
    args<<"-i"<< fileName << "-f" <<"wav" << outFileName;

    int result = m_process->execute(command,args);

    switch(result)
    {
        case 0:
        case 1:
            return outFileName;
            break;
        default:
            return NULL;
    }
}

// format to /tmp/xxx.wav
QString ffmpegHelper::audioFileNameFormat(QString fileName)
{
    int splashCount = fileName.count("/");
    if(splashCount > 0)
    {
        return OutPutPath + fileName.section("/", splashCount) + ".wav";
    }
    else
    {
        return NULL;
    }
}


bool ffmpegHelper::isFFmpegSupport()
{
    return this->m_isFFmpegSupport;
}

// private slots

void	ffmpegHelper::handle_error(QProcess::ProcessError error)
{
    if(m_state == ECHECK)
    {
        qDebug() << "check handle_error--> error :  " << error;
    }
    else if(m_state == ETRANSFORM)
    {
        qDebug() << "transform handle_error--> error :  " << error;
    }
    else
    {

    }
}

void	ffmpegHelper::handle_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(m_state == ECHECK)
    {
        qDebug() << "check handle_finished-->  exitCode : " << exitCode << " exitStatus : " << exitStatus;
    }
    else if(m_state == ETRANSFORM)
    {
        qDebug() << "transform handle_finished-->  exitCode : " << exitCode << " exitStatus : " << exitStatus;
    }
    else
    {

    }


}

void	ffmpegHelper::handle_readyReadStandardError()
{

    if(m_state == ECHECK)
    {
        qDebug() << "check handle_readyReadStandardError";
    }
    else if(m_state == ETRANSFORM)
    {
        qDebug() << "transform handle_readyReadStandardError";
    }
    else
    {

    }

}

void	ffmpegHelper::handle_readyReadStandardOutput()
{
    if(m_state == ECHECK)
    {
        qDebug() << "check handle_readyReadStandardOutput";
    }
    else if(m_state == ETRANSFORM)
    {
        qDebug() << "transform handle_readyReadStandardOutput";
    }
    else
    {

    }

}

void	ffmpegHelper::handle_started()
{

    if(m_state == ECHECK)
    {
        qDebug() << "check handle_started";
    }
    else if(m_state == ETRANSFORM)
    {
        qDebug() << "transform handle_started";
    }
    else
    {

    }

}

void	ffmpegHelper::handle_stateChanged(QProcess::ProcessState newState)
{
    if(m_state == ECHECK)
    {
        qDebug() << "check handle_stateChanged--> newState : " << newState;
    }
    else if(m_state == ETRANSFORM)
    {
        qDebug() << "transform handle_stateChanged--> newState : " << newState;
    }
    else
    {

    }

}

