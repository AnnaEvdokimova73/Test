#ifndef TEXTHANDLER_H
#define TEXTHANDLER_H

#include <QObject>
#include <QFile>
#include <QDebug>

#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include <QMetaType>

typedef std::multimap<int, QString, std::greater<>> MapIntStr;

class TextHandler : public QObject
{
    Q_OBJECT
public:
    TextHandler();

public slots:
    bool nameIsSet(const QString& path);
    void runRead();
    void stopRead();

signals:
    void thPaintHist(const MapIntStr& data);
    void progressData(int curProg, int totalProg, bool stop);

private:
    void readData();
    void getMaxes();
    int getWordsCount();

private:
    std::string m_filePath;
    std::map<QString, int> m_wordsMap;

    std::mutex m_mutex;
    std::condition_variable m_condVar;

    bool m_stopFlag; // Flag to stop threads
    bool m_fileFinished;

    int m_totalProgress;
};

#endif // TEXTHANDLER_H
