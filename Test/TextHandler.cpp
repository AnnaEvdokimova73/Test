#include "TextHandler.h"

TextHandler::TextHandler()
{

}

/**
 * @brief TextHandler::runRead
 * Function creates threads and run them
 * with little pause
 */
void TextHandler::runRead()
{
    m_fileFinished = false;
    m_stopFlag = false;
    m_totalProgress = getWordsCount();

    std::thread thGetMaxes(&TextHandler::getMaxes, this);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::thread thRead(&TextHandler::readData, this);

    thGetMaxes.join();
    thRead.join();

}

/**
 * @brief TextHandler::getWordsCount
 * Open file and count words
 * @return count of words in file
 */
int TextHandler::getWordsCount()
{
    std::ifstream file(m_filePath);

    if (!file.is_open())
    {
        qDebug() << "File wasn't opened!";
        return 0;
    }

    std::string word;
    int countWord = 0;

    // Read words until the end check stop flag
    while(file >> word)
    {
       ++countWord;
    }

    return countWord;
}

/**
 * @brief TextHandler::readData
 * Function read words from file,
 * filter them by removing punctuation
 * and fill wordsMap with word count
 */
void TextHandler::readData()
{
    // create object of ifstream and open file
    std::ifstream file(m_filePath);
    std::string word;

    if (!file.is_open())
    {
        qDebug() << "File wasn't opened!";
        return;
    }

    int currentProgress = 0;
    // Read words until the end check stop flag
    while(file >> word && !m_stopFlag)
    {
        // emit signal which word number program processes now
        emit progressData(++currentProgress, m_totalProgress, m_stopFlag);

        std::unique_lock<std::mutex> lock(m_mutex, std::try_to_lock);

        // Remove punctuation marks
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        if (word.empty())
            continue;

        QString qWord = QString::fromUtf8(word.c_str());
        ++m_wordsMap[qWord];

        m_condVar.notify_one();
        m_condVar.wait(lock);
    }
    // emit signal for final progress setting
    emit progressData(m_totalProgress, m_totalProgress, m_stopFlag);

    m_fileFinished = true;
    m_condVar.notify_one(); // Notify for case when file is finished

    file.close();
}


/**
 * @brief TextHandler::getMaxes
 * Function rewrites from struct std::map<QString, int>
 * to std::multimap<int, QString, std::greater<>>
 * to make sorting more convenient and emit signal to MainHandler
 */
void TextHandler::getMaxes()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(m_mutex, std::try_to_lock);
        m_condVar.wait(lock);

        if (m_fileFinished)
            break;

        // Get map with correct sort of words count
        MapIntStr countMap;
        for (auto& [word, count] : m_wordsMap)
        {
            int ind = count;
            countMap.insert({ind, word});
        }

        // signal to update graphic
        emit thPaintHist(countMap);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        m_condVar.notify_one();
    }
}

/**
 * @brief TextHandler::stopRead
 * Set stop flag and clear map with current words data
 */
void TextHandler::stopRead()
{
    m_stopFlag = true;
    m_wordsMap.clear();
}


/**
 * @brief TextHandler::nameIsSet
 * @param path
 * @return
 * false - if file is empty and
 * true - if it's not
 */
bool TextHandler::nameIsSet(const QString& path)
{
    QString qPath;
    qPath = path;
    qPath.remove(0, 8);
    m_filePath = qPath.toStdString();

    if (!m_filePath.empty())
    {
//        m_totalProgress = getWordsCount();
        return true;
    }

    return false;
}
