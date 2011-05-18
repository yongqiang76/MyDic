#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    volatile bool START;

private:
    Ui::MainWindow *ui;
    QString m_Word;
    QString m_GoogleUrl;
    QString m_UrbanUrl;
	QString m_LongmanUrl;
	QString m_CollinsUrl;

private:
	QString LookupDictionary(QString&, QString&);

private slots:
    void on_qUrban_clicked();
    void on_qGoogle_clicked();
	void on_qCollins_clicked();
    void on_QInputWord_editingFinished();
    void on_QInputWord_textChanged(QString );
};

#endif // MAINWINDOW_H
