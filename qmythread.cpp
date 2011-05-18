#include "qmythread.h"
#include <QWebView>
#include <QEventLoop>
#include <QWebElementCollection>
#include <QWebElement>
#include <QFile>
#include <QTextStream>
#include <QWebFrame>
#include <QDir>

QMyThread::QMyThread()
{
    this->start();
}

QMyThread::~QMyThread()
{}

void QMyThread::run()
{
    while(!m_Dic.isEmpty() && !m_Word.isEmpty())
    {
        this->m_Url = this->LookupDictionary(m_Dic, m_Word);
        emit activate();
    }
}


QString QMyThread::LookupDictionary(QString& dic, QString& word)
{
    QString url;
    if(dic == "Google")
    {
        url="http://www.google.com/dictionary?aq=f&langpair=en|zh-CN&q=";
        url+=word;
        url+="&hl=en";
    }
    else if(dic == "Urban")
    {
        url="http://www.urbandictionary.com/define.php?term=";
        url+=word;
    }

    QWebView *view = new QWebView;
    view->setUrl(url);
    QEventLoop loop;
    connect(view, SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
    loop.exec();

    QWebElementCollection elements;

    if(dic == "Google")
    {
        elements = view->page()->currentFrame()->documentElement().findAll(tr("li")); //.dct-em"));
    }
    else if(dic == "Urban")
    {
        elements = view->page()->currentFrame()->documentElement().findAll(tr("div"));  //.definition"));
    }

    if(elements.count() == 0)
        return QString::null;

    QString l_url=word+"_"+ dic + ".html";
    QFile file(l_url);
    if(!file.exists())
    {
        if(file.open(QFile::WriteOnly ) )
        {
            QTextStream out(&file);
            out<<"<html>"<<"\n";
            out<<" <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"<<"\n";
            out<<" <meta http-equiv=\"Content-Language\" content=\"Chinese\">"<<"\n";
            out<<"<body>"<<"\n";
            out<<"<title> </title>"<<"\n";

            if(dic == "Urban")
            {
               foreach (QWebElement paraElement, elements)
                {
                    if(paraElement.hasClass("definition"))
                     out<<"<p>"<<paraElement.toOuterXml()<<"</p>\n";
                }
            }
            else if (dic == "Google")
            {
                foreach (QWebElement paraElement, elements)
                 {
                     if(paraElement.hasClass("dct-em"))
                      out<<"<p>"<<paraElement.toOuterXml()<<"</p>\n";
                 }
            }

            out<<"</body>"<<"\n";
            out<<"</html>"<<"\n";
        }
           // g_file.write(html);
        else
            return QString::null;
    }

    url = "file://"+QDir::currentPath()+"/"+l_url;
    //ui->qDefWeb->setUrl(m_GoogleUrl);
    //ui->qGoogle->setEnabled(true);
    //file.close();
    delete view;
    return url;
}
