#include "HeartWidget.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
HeartWidget::HeartWidget(QWidget *parent) :
    QWidget(parent)
{
    initMusic();
    initWindow();
    initTimer();
    m_pen.setBrush(QColor("pink"));
    painter_state =  -2;
    paint_flag = 0;
    replay_count = 0;
    for (int i = 0; i < 6; i ++) {
        show_text[i] = (char*)malloc(512);
        memset(show_text[i], 0, 512);
    }

    initConfig();
    replay_flag = 0;
    shortcut_esc = new QxtGlobalShortcut(this);
    //可以通过
    if(shortcut_esc->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Space)))
    {
        qDebug()<<"setShortcut Ctrl+Space sucess";
        connect(shortcut_esc, &QxtGlobalShortcut::activated,[=]() {
            this->close();
        });
    }
}

HeartWidget::~HeartWidget()
{

}

void HeartWidget::initWindow()
{
    // 设置程序不在任务栏显示且窗体无边框 ^_^
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    // 窗口整体透明，但窗口控件不透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    // 全屏显示
    this->showFullScreen();

    // 设置分别率
    QRect screenRect = QApplication::desktop()->screenGeometry();
    w = screenRect.width();
    h = screenRect.height();
    if (w == 1920)
        resolution = 0;
    else if (w <= 1400 && w >= 1280)
        resolution = 1;
    else
        resolution = 1;
}

void HeartWidget::initTimer()
{
    timer = new QTimer(this);
    //timer->setInterval(100);
    timer->start(8);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

void HeartWidget::initMusic()
{
    player = new QMediaPlayer(this);
    player->setMedia(QMediaContent(QUrl(QCoreApplication::applicationDirPath() + QString("/mp3/vae.mp3"))));
    //player->setMedia(QMediaContent(QUrl("../flowers/images/like.mp3")));
    player->play();
    // 单曲循环
    connect(player, &QMediaPlayer::stateChanged, [=]{
        replay_flag = 1;
        replay_count ++;
        player->play();
        if (paint_flag) {
            /*  reset the play music  */
            /*  do more painting action */
            {
                paint_flag = 0;
                painter_state = 7;
                connect(timer, &QTimer::timeout, [&]{
                    qDebug() << "reset color" ;
                    switch (painter_state) {
                        case 7:
                            m_pen.setBrush(QColor(color_table[(painter_state+replay_count)%8]));
                            break;
                        case 8:
                            m_pen.setBrush(QColor(color_table[(painter_state+replay_count)%8]));
                            break;
                        case 9:
                            m_pen.setBrush(QColor(color_table[(painter_state+replay_count)%8]));
                            break;
                        case 10:
                            m_pen.setBrush(QColor(color_table[(painter_state+replay_count)%8]));
                            break;
                        case 11:
                            m_pen.setBrush(QColor(color_table[(painter_state+replay_count)%8]));
                            break;
                        case 12:
                            m_pen.setBrush(QColor(color_table[(painter_state+replay_count)%8]));
                            break;
                    }
                    update();
                });
                timer->start(1);
            }
        }
    });
}

int HeartWidget::initConfig()
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
                    QJsonArray array = m_obj.value("three_section_poems").toArray();
                    qDebug() << "array size:" << array.size();

                    for (int i = 0; i < array.size(); i++) {
                        strncpy(show_text[i],
                                array.at(i).toString().toStdString().c_str(), 512);
                    }
                    strcpy(show_text[3],
                            m_obj.value("sentiment_text").toString().toStdString().c_str());
                    qDebug()<< show_text[3];
                    strcpy(show_text[4],
                            m_obj.value("self_text").toString().toStdString().c_str());
                    qDebug()<< show_text[4];
                    strcpy(show_text[5],
                            m_obj.value("end_text").toString().toStdString().c_str());
                    qDebug()<< show_text[5];
                }
            }

    }
    qDebug() << "init config finish.";
    return true;
}


void HeartWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == 16777216) {
        //this->parentWidget()->close();
    }
}

void HeartWidget::paintEvent(QPaintEvent *)
{
    painter = new QPainter(this);
    QPixmap pix;

#if 1
    // 限定画爱心的取点
    static int count = 0;
    // 参数方程中的角度
    static double angle = 10.0;
    // 用于绘制I❤Y
    // -1 -2 -> 大爱心
    // 0 1 2 -> I
    // 3 4 5 -> Y
    // 6 -> ❤
    // 7 -> 文字
    static int i = 0;
    double a = 1.15, b = 1.15;
    switch(resolution)
    {
    case 0:
        a = 1.4;
        b = 1.4;
        break;
    case 1:
        a = 1.15;
        b = 1.15;
        break;
    }
    if (angle < 30 && painter_state == -2)
    {
        switch (resolution)
        {
        case 0:
            pix.load(":/new/prefix1/images/hh.gif");
            break;
        case 1:
            pix.load(":/new/prefix1/images/hh1.gif");
            break;
        }
        float t = (float) (angle / 3.14159);
        int x = (int) (19.5 * a * (16 * pow(sin(t), 3)));
        int y = (int) (-20.0 * b * (13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t)));
        // 隔15个点取一次绘制
        if (count % 15 == 0)
        {
            //painter->drawPixmap(550 + x, 400 + y, pix.width(), pix.height(), pix);
            painter->drawPixmap(w / 3.491 + x, h / 2.7 + y, pix.width(), pix.height(), pix);
        }
        count++;
        angle += 0.02;
        if (angle >= 30)
        {
            angle = 10.0;
            painter_state = -1;
        }
    }
    else if (angle < 30 && painter_state == -1)
    {
        float t = (float) (angle / 3.14159);
        int x = (int) (19.5 * a * (16 * pow(sin(t), 3)));
        int y = (int) (-20.0 * b * (13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t)));
        switch (resolution)
        {
        case 0:
            pix.load(":/new/prefix1/images/ff.png");
            break;
        case 1:
            pix.load(":/new/prefix1/images/ff1.png");
            break;
        }
        // 隔15个点取一次绘制
        if (count % 15 == 0)
        {
            int d = 0;
            if (resolution == 1)
            {
                d = 30;
            }
            //painter->drawPixmap(1250 + x, 400 + y, pix.width(), pix.height(), pix);
            painter->drawPixmap(w / 1.536 + d + x,  h / 2.7 + y, pix.width(), pix.height(), pix);
        }
        count++;
        angle += 0.02;
        if (angle >= 30)
        {
            timer->stop();
            timer->start(100);
            painter_state = 0;
        }
    }
    else
    {
        //timer->stop();
        //QPen m_pen;
        switch(resolution)
        {
        case 0:
            painter->setFont(QFont("Microsoft YaHei", 28, QFont::DemiBold));
            break;
        case 1:
             painter->setFont(QFont("Microsoft YaHei", 20, QFont::DemiBold));
            break;
        }

        pix.load(":/new/prefix1/images/rr1.png");
        if (painter_state == 0)
        {
            int d = 0;
            if (resolution == 1)
            {
                d = -50;
            }
            //painter->drawPixmap(300 + i, 325, pix.width(), pix.height(), pix);
            painter->drawPixmap(w / 6.4 + i, h / 3.323, pix.width(), pix.height(), pix);
            i += 25;
            if (i >= (175 + d))
            {
                painter_state = 1;
                i = 0;
            }
        }
        else if (painter_state == 1)
        {
            int d = 0;
            if (resolution == 1)
            {
                d = -50;
            }
            //painter->drawPixmap(375, 325 + i, pix.width(), pix.height(), pix);
            painter->drawPixmap(w / 5.12 + d / 8, h / 3.323 + i, pix.width(), pix.height(), pix);
            i += 25;
            if (i >= (200 + d))
            {
                painter_state = 2;
                i = 0;
            }
        }
        else if (painter_state == 2)
        {
            int d = 0;
            if (resolution == 1)
            {
                d = -50;
            }
            //painter->drawPixmap(300 + i, 525, pix.width(), pix.height(), pix);
            painter->drawPixmap(w / 6.4 + i, h / 2.057, pix.width(), pix.height(), pix);
            i += 25;
            if (i >= (175 + d))
            {
                painter_state = 3;
                i = 0;
            }
        }
        else if (painter_state == 3)
        {
            int d = 0;
            if (resolution == 1)
            {
                d = -50;
            }
            //pix.load("../flowers/images/love.png");
            //painter->drawPixmap(700 + i, 325 + tan(3.141592 / 3.0) * i, pix.width(), pix.height(), pix);
            painter->drawPixmap(w / 2.743 + i, h / 3.323 + tan(3.141592 / 3.0) * i, pix.width(), pix.height(), pix);
            i += 10;
            if (i >= 50)
            {
                painter_state = 4;
                i = 0;

            }
        }
        else if (painter_state == 4)
        {
            //painter->drawPixmap(755 + i, 411 - tan(3.141592 / 3) * i, pix.width(), pix.height(), pix);
            painter->drawPixmap(w / 2.543 + i, h / 2.628 - tan(3.141592 / 3) * i, pix.width(), pix.height(), pix);
            i += 10;
            if (i >= 60)
            {
                painter_state = 5;
                i = 0;
            }
        }
        else if (painter_state == 5)
        {
            //painter->drawPixmap(755, 411 + i, pix.width(), pix.height(), pix);
            painter->drawPixmap(w / 2.543, h / 2.628 + i, pix.width(), pix.height(), pix);
            i += 25;
            if (i >= 125)
            {
                painter_state = 6;
                i = 0;
            }
        }
        else if (painter_state == 6)
        {
            switch (resolution)
            {
            case 0:
                //pix.load(":/new/prefix1/images/love.png");
                pix.load(":/new/prefix1/images/ll.png");
                break;
            case 1:
                pix.load(":/new/prefix1/images/ll1.png");
                break;
            }
            //painter->drawPixmap(500, 350, pix.width(), pix.height(), pix);
            painter->drawPixmap(w / 3.84, h / 3.086, pix.width(), pix.height(), pix);
            painter_state = 7;
            timer->stop();
            timer->start(1000);
        }
        else if (painter_state == 7)
        {
            int d = 0;
            if (resolution == 1)
                d = 60;

            painter->setPen(m_pen);
            //painter->drawText(1100, 400, "眼角眉梢都充满着暖意");
            qDebug() <<"draw:" << QString(show_text[painter_state-7]);
            painter->drawText(w / 1.745 + d, h / 2.7, QString(show_text[painter_state-7]));
            painter_state = 8;
            timer->setInterval(1);
        }
        else if (painter_state == 8)
        {
            int d = 0;
            if (resolution == 1)
                d = 60;
            painter->setPen(m_pen);
            //painter->drawText(1100, 450, "遇上你，就像浩劫余生");
            qDebug() <<"draw:" << QString(show_text[painter_state-7]);
            painter->drawText(w / 1.745 + d, h / 2.40, QString(show_text[painter_state-7]));
            painter_state = 9;
        }
        else if (painter_state == 9)
        {
            int d = 0;
            if (resolution == 1)
                d = 60;

            painter->setPen(m_pen);
            //painter->drawText(1100, 500, "漂流过沧海，终见陆地");
            qDebug() <<"draw:" << QString(show_text[painter_state-7]);
            painter->drawText(w / 1.745 + d, h / 2.16, QString(show_text[painter_state-7]));
            painter_state = 10;
        }
        else if (painter_state == 10)
        {
            int d = 0;
            if (resolution == 1)
                d = 60;
            if (!replay_flag)
                m_pen.setBrush(QColor("skyblue"));
            painter->setPen(m_pen);
            //painter->drawText(1100, 600, "  小姜，七夕快乐！");
            qDebug() <<"draw:" << QString(show_text[painter_state-7]);
            painter->drawText(w / 1.745 + d, h / 1.80, QString(show_text[painter_state-7]));
            painter_state = 11;
        }
        else if (painter_state == 11)
        {
            int d = 0;
            if (resolution == 1)
                d = 60;
            if (!replay_flag)
                m_pen.setBrush(QColor("skyblue"));
            switch (resolution)
            {
            case 0:
                painter->setFont(QFont("Microsoft YaHei", 17, QFont::DemiBold));
                break;
            case 1:
                painter->setFont(QFont("Microsoft YaHei", 13, QFont::DemiBold));
                break;
            default:
                painter->setFont(QFont("Microsoft YaHei", 13, QFont::DemiBold));
                break;
            }
            painter->setPen(m_pen);
            //painter->drawText(1100, 640, "           —— by 小蔡学长");
            qDebug() <<"draw:" << QString(show_text[painter_state-7]);
            painter->drawText(w / 1.745 + d, h / 1.678, QString(show_text[painter_state-7]));
            painter_state = 12;
        }
        else if (painter_state == 12)
        {
            if (!replay_flag)
                m_pen.setBrush(QColor("purple"));
            switch (resolution)
            {
            case 0:
                painter->setFont(QFont("Microsoft YaHei", 40, QFont::DemiBold));
                break;
            case 1:
                painter->setFont(QFont("Microsoft YaHei", 34, QFont::DemiBold));
                break;
            default:
                painter->setFont(QFont("Microsoft YaHei", 34, QFont::DemiBold));
                break;
            }

            painter->setPen(m_pen);
            qDebug() <<"draw:" << QString(show_text[painter_state-7]);
            painter->drawText(w / 10.0, h / 5.0 * 4.5, QString(show_text[painter_state-7]));
            painter_state = 13;
            /*  paint all context   */
            paint_flag = 1;
            timer->stop();
            //disconnect(timer, SIGNAL(timeout()), this, SLOT(update()));
            //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
            qDebug() << "show end";
        }

    }
#endif
}
