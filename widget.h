#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<QVideoWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
static QString fmtTime(qint64 ms);

private slots:
    void on_pushButton_open_clicked();

    void on_pushButton_play_clicked();

    void on_plause_clicked();

    void on_pushButton_stop_clicked();
protected:
    void paintEvent(QPaintEvent* e);    
private:
    bool seek;
    Ui::Widget *ui;
    QMediaPlayer *player;
    QMediaPlaylist *list;
    QVideoWidget *vdwgt;
};
#endif // WIDGET_H
