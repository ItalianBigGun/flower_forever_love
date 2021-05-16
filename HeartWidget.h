#ifndef HEAERWIDGET_H
#define HEAERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QMediaPlayer>
#include <QApplication>
#include <QDesktopWidget>
#include <cmath>
#include <QDebug>
#include <QLabel>
#include <QKeyEvent>
#include "qxtglobalshortcut.h"

class HeartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeartWidget(QWidget *parent = 0);
    ~HeartWidget();
    void initWindow();
    void initTimer();
    void initMusic();
    int initConfig();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QxtGlobalShortcut *shortcut_esc;
    const double PI = 3.1415926;
    QTimer *timer;
    QPainter *painter;
    QMediaPlayer *player;
    int w, h;
    int painter_state;
    char *show_text[10];
    QPen m_pen;
    int paint_flag;
    int replay_flag;
    int replay_count;
    const char *color_table[8] = {"skyblue","purple","blue","black","red","pink","green","white"};
    // 定义分辨率标识,适配多种分别率
    // 0 -> 1920 * 1080
    // 1 -> 1366 * 768
    int resolution;
};

#endif // HEAERWIDGET_H
