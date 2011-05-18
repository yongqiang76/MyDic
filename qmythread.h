#ifndef QMYTHREAD_H
#define QMYTHREAD_H

#include <QThread>

class QMyThread : public QThread
{
    Q_OBJECT
public:
    QMyThread();
    ~QMyThread();

    void SetWord(QString& word)
    { m_Word = word;}
    void SetDic(QString& dic)
    { m_Dic = dic;}
    QString GetUrl()
    {return m_Url;}

signals:
    void activate();

protected:
    void run();

private:
    QString m_Word;
    QString m_Dic;
    QString m_Url;
    QString LookupDictionary(QString& dir, QString& word);

public slots:

};

#endif // QMYTHREAD_H
