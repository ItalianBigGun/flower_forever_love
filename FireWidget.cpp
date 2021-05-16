#include "FireWidget.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
FireWidget::FireWidget(QWidget *parent) : QWidget(parent)
{
    initConfig();
    // 初始化音乐
    initMusic();

    // 设置程序不在任务栏显示且窗体无边框 ^_^
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

    // 全屏显示
    QRect screenRect = QApplication::desktop()->screenGeometry();
    //QRect screenRect = QGuiApplication::screens()[0]->geometry();
    w = screenRect.width();
    h = screenRect.height();
    // 设置分别率
    if (w == 1920)
        resolution = 0;
    else if (w <= 1400 && w >= 1280)
        resolution = 1;
    else
        resolution = 1;
    resize(w, h);

    // 设置背景全黑
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(5, 4, 3));
    setAutoFillBackground(true);
    setPalette(palette);

    QPixmap pixmap;
    switch (resolution)
    {
    case 0:
        pixmap.load(":/new/prefix1/images/fire.gif");
        break;
    case 1:
        pixmap.load(":/new/prefix1/images/fire1.gif");
        break;
    }

    int mw = pixmap.size().width();
    int mh = pixmap.size().height();

    QLabel *label1 = new QLabel(this);
    label1->move(w / 2 - mw / 2, h / 2 - mh / 2);
    playMovie(label1);

    QLabel *label2 = new QLabel(this);
    label2->move(w / 2 - mw * 1.5, h / 2 - mh);
    playMovie(label2);

    QLabel *label3 = new QLabel(this);
    label3->move(w / 2 + mw / 2, h / 2 - mh);
    playMovie(label3);

    QLabel *label4 = new QLabel(this);
    label4->move(w / 2 - mw * 1.5, h / 2);
    playMovie(label4);

    QLabel *label5 = new QLabel(this);
    label5->move(w / 2 + mw / 2, h / 2);
    playMovie(label5);


    QTimer *timer = new QTimer(this);
    timer->start(7000);
    connect(timer, &QTimer::timeout, [=]{
        timer->stop();
        player->stop();
        this->hide();
        HeartWidget *w = new HeartWidget(this);
        w->show();
    });

    // 文字
    QLabel *textLabel = new QLabel(this);
    textLabel->move(80, 100);
    QFont ft;
    switch (resolution)
    {
    case 0:
        ft.setPointSize(28);
        break;
    case 1:
        ft.setPointSize(22);
        break;
    default:
        ft.setPointSize(22);
    }

    textLabel->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::white);
    textLabel->setPalette(pa);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignTop);
    textLabel->setText(QString::fromUtf8(text));

//    QTimer *timer1 = new QTimer(this);
//    timer1->start(500);
//    connect(timer1, &QTimer::timeout, [=]{

//    });
}

void FireWidget::initMusic()
{
    player = new QMediaPlayer(this);
    player->setMedia(QMediaContent(QUrl(QString(QCoreApplication::applicationDirPath() + QString("/mp3/fire.mp3")))));
    player->play();
}

void FireWidget::playMovie(QLabel *label)
{
    QMovie *movie = new QMovie(this);
    switch (resolution)
    {
    case 0:
        movie->setFileName(":/new/prefix1/images/fire.gif");
        break;
    case 1:
        movie->setFileName(":/new/prefix1/images/fire1.gif");
        break;
    default:
        movie->setFileName(":/new/prefix1/images/fire1.gif");
    }
    // 动画
    label->setMovie(movie);
    movie->start();
}

int FireWidget::initConfig()
{
    qDebug() << "init config start.";
    QFile  config_file(QDir::currentPath()+QString("/config.json"));
    qDebug() << config_file.fileName();
    if (!config_file.exists()) {//文件不存在
        return false;
    }
    if (!config_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QByteArray data = config_file.readAll();

    {
            QJsonParseError jsonError;
            QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // 转化为 JSON 文档
            if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) { // 解析未发生错误
                if (doucment.isObject()) { // JSON 文档为数组
                    QJsonObject m_obj = doucment.object();  // 转化为数组
                    if (m_obj.contains("main_text")) {
                        memset(text, 0, 128);
                        qDebug() << m_obj.value("main_text").toString();
                        strcpy(text, m_obj.value("main_text").toString().toStdString().c_str());
                    }
                }
            }

    }
    qDebug() << "init config finish.";
    return true;
}

