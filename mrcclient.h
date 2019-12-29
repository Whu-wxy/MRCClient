#ifndef MRCCLIENT_H
#define MRCCLIENT_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QApplication>
#include <QMouseEvent>
#include <msgbox.h>
#include <QJsonArray>
#include <QJsonParseError>
#include <QTime>

//   Who designs both the "50" as well as the Trophy?

#define PREDICTURL "http://127.0.0.1:8000/predict"

class MRCClient : public QDialog
{
    Q_OBJECT

public:
    MRCClient(QWidget *parent = 0);
    ~MRCClient();

#ifdef ANDROID
    void    setupUIAND();

#else
    void    setupUIWIN();
#endif

    void    setPassageText(int index);
    void    MendText(QString& text);
    void    sentMsg(QString json); //上传自定义文章和问题,或用服务器数据库上的文章

    double  findMaxProbs(QJsonArray probs);

#ifdef WIN32
public:
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
#endif

private:
    bool  mousePressed;
    QPoint mousePoint;

    QPushButton* minWinBtn;
    QPushButton* closeWinBtn;

    QTextEdit *m_PEdit; //passage
    QComboBox* m_PBox;
    QLineEdit *m_QEdit; //question
    QPushButton* m_submitBtn;

    QNetworkAccessManager *m_qnam;
    QNetworkReply* m_reply;

    QTime m_time;


protected slots:
    void    minDialog();
    void    onSubmit();
    void    receiveResult();
    void    selectBox(int index);

};

#endif // MRCCLIENT_H
