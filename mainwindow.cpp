#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QtWebKit>

#include <QWebElementCollection>

#include <QWebFrame>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QDir>

#include <iostream>
#include "DictionaryFactory.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),START(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_QInputWord_textChanged(QString )
{

    ui->qGoogle->setEnabled(false);
    ui->qUrban->setEnabled(false);
	ui->qLongman->setEnabled(false);
	ui->qCollins->setEnabled(false);
    ui->qDefWeb->setUrl(tr("about::blank"));
}


void MainWindow::on_QInputWord_editingFinished()
{
    m_Word=ui->QInputWord->text();
    if(!m_Word.isEmpty())
    {
		/*
        QString dic1="Google";
		QString dic2="Urban";
		QString dic3="Longman";
		QString dic4="Collins";
        */

		OnlineDictionary* dic1;
		dic1 = DictionaryFactory::CreateOnlineDictionary("Google");
		dic1->SetWord(m_Word);
		dic1->Lookup();
		m_GoogleUrl=dic1->GetUrl();
		//std::cout<<m_GoogleUrl.toStdString()<<std::endl;


		dic1 = DictionaryFactory::CreateOnlineDictionary("Urban");
		dic1->SetWord(m_Word);
		dic1->Lookup();
		m_UrbanUrl=dic1->GetUrl();

		dic1 = DictionaryFactory::CreateOnlineDictionary("Collins");
		dic1->SetWord(m_Word);
		dic1->Lookup();
		m_CollinsUrl=dic1->GetUrl();

		/*
		m_UrbanUrl=this->LookupDictionary(dic2,m_Word);

		m_LongmanUrl=this->LookupDictionary(dic3, m_Word);

		m_CollinsUrl=this->LookupDictionary(dic4, m_Word);
		*/

		ui->qGoogle->setEnabled(true);
		ui->qUrban->setEnabled(true);
		ui->qLongman->setEnabled(true);
		ui->qCollins->setEnabled(true);
    }
}

void MainWindow::on_qGoogle_clicked()
{
    //m_GoogleUrl = m_GoogleThread.GetUrl();
    ui->qDefWeb->setUrl(m_GoogleUrl);
}

void MainWindow::on_qUrban_clicked()
{
    //m_UrbanUrl = m_UrbanThread.GetUrl();
    if(m_UrbanUrl.isEmpty() )
        m_UrbanUrl="about::blank";
    ui->qDefWeb->setUrl(m_UrbanUrl);
}

void MainWindow::on_qCollins_clicked()
{
	//m_UrbanUrl = m_UrbanThread.GetUrl();
	if(m_CollinsUrl.isEmpty() )
		m_CollinsUrl="about::blank";
	ui->qDefWeb->setUrl(m_CollinsUrl);
}


QString MainWindow::LookupDictionary(QString& dic, QString& word)
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
	else if(dic == "Collins")
	{
		url="http://www.collinslanguage.com/results.aspx?context=3&reversed=False&action=define&homonym=-1&text=";
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
	else if(dic == "Collins")
	{
		elements = view->page()->currentFrame()->documentElement().findAll(tr("span"));
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
			out<<" <meta http-equiv=\"Content-Type\" content=\"text/html; charset=GB18030\">"<<"\n";
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
			else if(dic == "Collins")
			{
				foreach (QWebElement paraElement, elements)
				{
					if(paraElement.hasClass("term"))
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

	url = "file:///"+QDir::currentPath()+"/"+l_url;
	//std::cout<<url.toStdString()<<std::endl;
	//ui->qDefWeb->setUrl(m_GoogleUrl);
	//ui->qGoogle->setEnabled(true);
	//file.close();
	delete view;
	return url;
}
