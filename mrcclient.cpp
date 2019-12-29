#include "mrcclient.h"
#include <QJsonObject>
#include <QJsonDocument>

MRCClient::MRCClient(QWidget *parent)
    : QDialog(parent)
{
    m_qnam = new QNetworkAccessManager(this);

#ifdef ANDROID
    setupUIAND();
#else
    setupUIWIN();
#endif
}

MRCClient::~MRCClient()
{

}

#ifdef ANDROID
void MRCClient::setupUIAND()
{
    //应用样式
       QFile file(":/style/androidEnterstyle.qss");
       file.open(QFile::ReadOnly);
       QTextStream filetext(&file);
       QString stylesheet = filetext.readAll();
       this->setStyleSheet(stylesheet);
       file.close();

    //手机显示及字体
    QScreen* screen = qApp->primaryScreen();
    QRect rect = screen->availableGeometry();
    setFixedSize(rect.width(),rect.height());
    //setGeometry(rect);
    setFont(qApp->font());
    setContentsMargins(20,20,20,20);

    setAutoFillBackground(true);
    QPalette palette;
    QString backPicture = QString(":/image/background.jpg");
    QPixmap pixmap(backPicture);
    pixmap = pixmap.scaled(rect.width(),rect.height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    setPalette(palette);

}
#else
void MRCClient::setupUIWIN()
{
    QFont font  = qApp->font();
    font.setPointSize(13);
    qApp->setFont(font);

    //应用样式
    QFile file(":/style/winEnterWhite.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    minWinBtn = new QPushButton;
    minWinBtn->setIcon(QIcon(":/image/min.ico"));
    minWinBtn->setObjectName("min");
    closeWinBtn = new QPushButton;
    closeWinBtn->setIcon(QIcon(":/image/close.ico"));
    closeWinBtn->setObjectName("close");
    connect(closeWinBtn, SIGNAL(clicked()), this, SLOT(close()));
    connect(minWinBtn,SIGNAL(clicked(bool)),this,SLOT(minDialog()));
    QHBoxLayout* titleLay = new QHBoxLayout;
    titleLay->addStretch();
    titleLay->addWidget(minWinBtn);
    titleLay->addSpacing(15);
    titleLay->addWidget(closeWinBtn);

    this->setFixedWidth(600);
    this->setFixedHeight(400);
    setContentsMargins(20,0,20,20);

    m_PEdit = new QTextEdit(this);
//    m_PEdit->setEnabled(false);
    setPassageText(0);
    m_PBox = new QComboBox(this);
    QStringList items;
    items.append("Super Bowl");
    items.append("Private_school");
    items.append("kenya");
    items.append("自定义文章");
    m_PBox->insertItems(0, items);
    m_QEdit = new QLineEdit(this);
    m_QEdit->setFocus();
    m_QEdit->setPlaceholderText("请根据文章提出一个问题");
    m_submitBtn = new QPushButton("提问", this);

    QVBoxLayout* leftLay = new QVBoxLayout();
    leftLay->addWidget(m_PEdit);
    leftLay->addWidget(m_QEdit);

    QVBoxLayout* rightLay = new QVBoxLayout();
    rightLay->addWidget(m_PBox);
    rightLay->addStretch();
    rightLay->addWidget(m_submitBtn);

    QHBoxLayout* bottomLay = new QHBoxLayout();
    bottomLay->addLayout(leftLay);
    bottomLay->addLayout(rightLay);

    QVBoxLayout* mainLay = new QVBoxLayout(this);
    mainLay->addLayout(titleLay);
    mainLay->addSpacing(15);
    mainLay->addLayout(bottomLay);

    connect(m_PBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectBox(int)));
    connect(m_submitBtn, SIGNAL(clicked(bool)), this, SLOT(onSubmit()));
}
#endif

void MRCClient::minDialog()
{
    if(windowState() ==Qt::WindowNoState )
    {
        setWindowState(Qt::WindowMinimized);
    }
}

//上传自定义文章和问题,或用服务器数据库上的文章
void MRCClient::sentMsg(QString json)
{
    QNetworkRequest req;
    req.setUrl(QUrl(PREDICTURL));
    req.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json;charset=utf-8"));

    m_reply = m_qnam->post(req, json.toUtf8());
    connect(m_reply,SIGNAL(finished()),this,SLOT(receiveResult()));
}

void MRCClient::receiveResult()
{
    QByteArray bytes = m_reply->readAll();
//    qDebug()<<bytes;
    QJsonParseError json_error;
    QJsonDocument document;
    document = QJsonDocument::fromJson(bytes, &json_error);  //注意是static函数
//    qDebug()<<json_error.errorString();
    QJsonObject json = document.object();
    QString SpanStr = json.value("best_span_str").toString();
    QJsonArray arrayStart = json.value("span_start_probs").toArray();
    QJsonArray arrayEnd = json.value("span_end_probs").toArray();
    double startProb = findMaxProbs(arrayStart);
    double endProb = findMaxProbs(arrayEnd);
//    qDebug()<<startProb<<";"<<endProb;
    double ave = (startProb + endProb) / 2;
    ave *= 100;

    qDebug()<<"from send to receive time:"<<m_time.elapsed()<<"ms";

    QString msgText = QString("我有%1%的概率判断答案为:\n%2").arg(ave).arg(SpanStr);
    msgBox msgbox;
    msgbox.setText(msgText);
    msgbox.exec();
}

double MRCClient::findMaxProbs(QJsonArray probs)
{
    double max = 1e-10;
    for(int i=0; i<probs.count(); i++)
    {
        QJsonValue value = probs.at(i);
        double val = value.toDouble();
        if(max < val)
            max = val;
    }
    return max;
}

void MRCClient::onSubmit()
{
    m_time.start();

    QString passage = m_PEdit->document()->toPlainText();
    QString question = m_QEdit->text();

    QJsonObject json;

    QString strJson = "";
//    int index = m_PBox->currentIndex();
//    if(index != 3) ////////////////////////TODO:利用服务器上数据库存的文章
//    {
        json.insert("passage", passage);
        json.insert("question", question);
        QJsonDocument document;
        document.setObject(json);
        QByteArray byteArray = document.toJson(QJsonDocument::Compact);
        strJson = QString(byteArray);
//    }
//    else
//    {
//        json.insert("passage", index);
//        json.insert("question", question);
//        QJsonDocument document;
//        document.setObject(json);
//        QByteArray byteArray = document.toJson(QJsonDocument::Compact);
//        strJson = QString(byteArray);
//    }

    sentMsg(strJson);
}

void MRCClient::selectBox(int index)
{
    if(index == 3)
    {
        m_PEdit->setEnabled(true);
        m_PEdit->clear();
    }
    else
    {
//        m_PEdit->setEnabled(false);
        setPassageText(index);
    }
    m_QEdit->clear();
}

void MRCClient::setPassageText(int index)
{
    QString passage;  //双引号用中文的，post需要
    if(index == 0)
    {
        passage = "In addition to the Vince Lombardi Trophy that all "
                  "Super Bowl champions receive, the winner of Super "
                  "Bowl 50 will also receive a large, 18-karat gold-plated "
                  "“50”. Each digit will weigh 33 lb (15 kg) for a total "
                  "of 66 lb (30 kg). Like the Lombardi Trophy, the “50” "
                  "will be designed by Tiffany & Co.";
    }
    else if(index == 1)
    {
        passage = "In India, private schools are called independent schools, "
                  "but since some private schools receive financial aid from "
                  "the government, it can be an aided or an unaided school. "
                  "So, in a strict sense, a private school is an unaided "
                  "independent school. For the purpose of this definition, "
                  "only receipt of financial aid is considered, not land "
                  "purchased from the government at a subsidized rate. It "
                  "is within the power of both the union government and "
                  "the state governments to govern schools since Education "
                  "appears in the Concurrent list of legislative subjects in "
                  "the constitution. The practice has been for the union "
                  "government to provide the broad policy directions while "
                  "the states create their own rules and regulations for "
                  "the administration of the sector. Among other things, "
                  "this has also resulted in 30 different Examination "
                  "Boards or academic authorities that conduct examinations "
                  "for school leaving certificates. Prominent Examination "
                  "Boards that are present in multiple states are the CBSE "
                  "and the CISCE, NENBSE";
    }
    else if(index == 2)
    {
        passage = "The Republic of Kenya is named after Mount Kenya. The "
                  "origin of the name Kenya is not clear, but perhaps linked "
                  "to the Kikuyu, Embu and Kamba words Kirinyaga, Kirenyaa "
                  "and Kiinyaa which mean \"God's resting place\" in all "
                "three languages. If so, then the British may not so much "
                "have mispronounced it ('Keenya'), as misspelled it. Prehistoric "
                  "volcanic eruptions of Mount Kenya (now extinct) may have "
                  "resulted in its association with divinity and creation "
                  "among the indigenous Bantu ethnic groups, who are the "
                  "native inhabitants of the agricultural land surrounding "
                  "Mount Kenya.[original research?]";
    }
    m_PEdit->setText(passage);
}

//字符串中的英文标点符号变成中文的
void MRCClient::MendText(QString& text)
{
    int j = 0;
    QString array = text;
    for(int i=0; i<array.count(); i++)
    {
        QChar c = array.at(i);
        if(c == '\"')
        {
            if(j % 2 == 0)
            {
                array.replace(i, 1, '\\\"');
                j++;
                continue;
            }
            else if(j % 2 == 1)
            {
                array.replace(i, 1, '\\\"');
                j++;
                continue;
            }
        }
    }
    text = array;
    qDebug()<<text;
}

#ifdef WIN32
void MRCClient::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() == Qt::LeftButton) )
    {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void MRCClient::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void MRCClient::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}
#endif
