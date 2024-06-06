#ifndef MAINHANDLER_H
#define MAINHANDLER_H

#include <QObject>
#include <QThread>

#include "TextHandler.h"

class MainHandler : public QObject
{
    Q_OBJECT
public:
    MainHandler();
    ~MainHandler();

public slots:
    void setFileName(const QString& path);
    void startRead();
    void cancelRead();
    void regroupData(const MapIntStr &data);

signals:
    void needChooseFile();
    void paintHist(QVariantMap data, int maxValue);
    void setProgress(int currentProg, int totalProg, bool stop);
    void clearHist();
    void clearProgBar();

private:
    QThread* m_thread;
    TextHandler* m_textHandler;
    bool m_fileChosen;
};

#endif // MAINHANDLER_H
