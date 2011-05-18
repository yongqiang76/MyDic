//This file uses simple factory design patter.

#include <QString>
#include <QWebElementCollection>

#include <QWebFrame>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QDir>
#include <QWebView>
#include <QEventLoop>
#include <string>
#include <iostream>


class OnlineDictionary
{
public:
	OnlineDictionary() 
	{
		m_Word = "";
		m_Url = "";
	}
	OnlineDictionary(QString word):m_Word(word),m_Url("") {};
	void SetWord(QString word) {m_Word = word;}
	void SetUrl(QString url) { m_Url = url; }
	QString GetWord() { return m_Word; }
	QString GetUrl() {return m_Url;}
	

	QWebView* GetWebView(QString& url)
	{
		QWebView *view = new QWebView;
		view->setUrl(url);
		QEventLoop loop;
		QObject::connect(view, SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
		loop.exec();
		return view;
	}
	
	virtual void Lookup() {};

	void CreateHTMLFileHead(QFile& file)
	{
		if(file.open(QFile::WriteOnly ) )
		{
			QTextStream out(&file);
			out<<"<html>"<<"\n";
			out<<" <meta http-equiv=\"Content-Type\" content=\"text/html; charset=GB18030\">"<<"\n";
			out<<" <meta http-equiv=\"Content-Language\" content=\"Chinese\">"<<"\n";
			out<<"<body>"<<"\n";
			out<<"<title> </title>"<<"\n";
		}
	}

private:
	QString m_Word;
	QString m_Url;
};

class OnlineGoogleDictionary : public OnlineDictionary
{
public:
	void Lookup()
	{
		QString url;
		url="http://www.google.com/dictionary?aq=f&langpair=en|zh-CN&q=";
		url+=this->GetWord();
		url+="&hl=en";
		
		QWebElementCollection elements;

		elements = this->GetWebView(url)->page()->currentFrame()->documentElement().findAll(QObject::tr("li")); //.dct-em"));
		

		if(elements.count() == 0)
		{
			this->SetUrl( "about::blank");
			return ;
		}

		QString l_url= this->GetWord() +"_"+ "Google.html";
		QFile file(l_url);
		if(!file.exists())
		{
			this->CreateHTMLFileHead(file);
//			if(file.open(QFile::WriteOnly ) )
			{
				QTextStream out(&file);

				foreach (QWebElement paraElement, elements)
				{
					if(paraElement.hasClass("dct-em"))
						out<<"<p>"<<paraElement.toOuterXml()<<"</p>\n";
				}
				out<<"</body>"<<"\n";
				out<<"</html>"<<"\n";
			}
		}
		//std::cout<<"enter"<<std::endl;
		this->SetUrl("file:///"+QDir::currentPath()+"/"+l_url);
	}
};

class OnlineUrbanDictionary : public OnlineDictionary
{
public:
	void Lookup() {
		QString url;
		url="http://www.urbandictionary.com/define.php?term=";
		url+= this->GetWord();

		QWebElementCollection elements;

		elements = this->GetWebView(url)->page()->currentFrame()->documentElement().findAll(QObject::tr("div")); //.dct-em"));


		if(elements.count() == 0)
			return ;

		QString l_url= this->GetWord() +"_"+ "Urban.html";
		QFile file(l_url);
		if(!file.exists())
		{
			this->CreateHTMLFileHead(file);
	//		if(file.open(QFile::WriteOnly ) )
			{
				QTextStream out(&file);

				foreach (QWebElement paraElement, elements)
				{
					if(paraElement.hasClass("definition"))
						out<<"<p>"<<paraElement.toOuterXml()<<"</p>\n";
				}
				out<<"</body>"<<"\n";
				out<<"</html>"<<"\n";
			}
		}

		this->SetUrl( "file:///"+QDir::currentPath()+"/"+l_url );
	}
};

class OnlineLongmanDictionary : public OnlineDictionary
{

};

class OnlineCollinsDictionary : public OnlineDictionary
{
	void Lookup() {
		QString url;
		url="http://www.collinslanguage.com/results.aspx?context=3&reversed=False&action=define&homonym=-1&text=";
		url+= this->GetWord();

		QWebElementCollection elements;

		elements = this->GetWebView(url)->page()->currentFrame()->documentElement().findAll(QObject::tr("span")); //.dct-em"));


		if(elements.count() == 0)
			return ;

		QString l_url= this->GetWord() +"_"+ "Collins.html";
		QFile file(l_url);
		if(!file.exists())
		{
			this->CreateHTMLFileHead(file);
	//		if(file.open(QFile::WriteOnly ) )
			{
				QTextStream out(&file);

				foreach (QWebElement paraElement, elements)
				{
					if(paraElement.hasClass("term"))
						out<<"<p>"<<paraElement.toOuterXml()<<"</p>\n";
				}
				out<<"</body>"<<"\n";
				out<<"</html>"<<"\n";
			}
		}

		this->SetUrl( "file:///"+QDir::currentPath()+"/"+l_url );
	}
};

class DictionaryFactory
{
public:
	DictionaryFactory(void);
	~DictionaryFactory(void);

public:
	//Lookup(QString&);
	static OnlineDictionary* CreateOnlineDictionary(std::string type) 
	{

		if (type=="Google")
			return new OnlineGoogleDictionary();
		else if(type=="Urban")
			return new OnlineUrbanDictionary();
		else if(type=="Longman")
			return new OnlineLongmanDictionary();
		else if( type=="Collins" )
			return new OnlineCollinsDictionary();
	}

};
