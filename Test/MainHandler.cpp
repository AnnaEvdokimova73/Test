#include "MainHandler.h"

/**
 * @brief MainHandler::MainHandler
 * Create classes objects and connects
 */
MainHandler::MainHandler()
{
    m_fileChosen = false;
    m_thread = new QThread();
    m_textHandler = new TextHandler();
    m_textHandler->moveToThread(m_thread);

    connect(m_thread, &QThread::started, m_textHandler, &TextHandler::runRead);
    connect(m_thread, &QThread::finished, m_textHandler, &TextHandler::stopRead);
    connect(m_textHandler, &TextHandler::thPaintHist, this, &MainHandler::regroupData);
    connect(m_textHandler, SIGNAL(progressData(int, int, bool)), this, SIGNAL(setProgress(int, int, bool)));
}

/**
 * @brief MainHandler::~MainHandler
 * Quit thread in the end
 */
MainHandler::~MainHandler()
{
    m_textHandler->stopRead();
    m_thread->quit();
}

/**
 * @brief MainHandler::setFileName
 * @param path - name of file got from form
 */
void MainHandler::setFileName(const QString& path)
{
    m_fileChosen = m_textHandler->nameIsSet(path);
}

/**
 * @brief MainHandler::startRead
 * If file was chosen then start thread
 */
void MainHandler::startRead()
{
    if (m_fileChosen)
    {
        m_thread->start();
    }
    else
    {
        emit needChooseFile();
    }
}

/**
 * @brief MainHandler::cancelRead
 * Call function in TextHandler to set stop flag
 * Quit Thread
 * and emit signal for clear data on form
 */
void MainHandler::cancelRead()
{
    m_textHandler->stopRead();
    m_thread->quit();
    emit clearHist();
}

/**
 * @brief MainHandler::regroupData
 * @param data - convenient conteiner for task in c++
 * Regroup map data to format for sending to QML
 * And left only 15 maxes
 */
void MainHandler::regroupData(const MapIntStr& data)
{
    int count = 0;
    QVariantMap qVarData; // Select container to send to qml
    for (auto& [countInText, word] : data)
    {
        if (count == 15)
            break;

        qVarData[word] = QString::number(countInText);
        ++count;
    }

    int maxValue = data.begin()->first;
    emit paintHist(qVarData, maxValue);
}


