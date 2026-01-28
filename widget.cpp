#include "widget.h"
#include "ui_widget.h"
#include<QFileDialog>
#include<QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("播放器");
    player=new QMediaPlayer;
    vdwgt=new QVideoWidget(ui->label);
    player->setVideoOutput(vdwgt);
    seek=false;
    connect(player, &QMediaPlayer::durationChanged, this, [=](qint64 dur){
       ui->horizontalSlider->setRange(0, (int)dur);
        // 更新总时长显示
        ui->timeLabel->setText(QString("%1 / %2").arg(fmtTime(player->position()),fmtTime(dur)));
    });
    //更新进度条和时间
    connect(player, &QMediaPlayer::positionChanged, this, [=](qint64 pos){
        if(!seek)
       ui->horizontalSlider->setValue((int)pos);
       ui->timeLabel->setText(QString("%1 / %2").arg(fmtTime(pos), fmtTime(player->duration())));
            });
    connect(ui->horizontalSlider, &QSlider::sliderPressed, this, [=](){
                seek = true;
            });
    connect(ui->horizontalSlider, &QSlider::sliderReleased, this, [=](){
                seek = false;
                player->setPosition(ui->horizontalSlider->value());
            });
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item){
        QString path = item->data(Qt::UserRole).toString();
        if (path.isEmpty()) return;

        player->setMedia(QUrl::fromLocalFile(path));
        player->play();
    });
}
QString Widget::fmtTime(qint64 ms)
{
    int s = int(ms / 1000);
    int m = s / 60; s %= 60;
    int h = m / 60; m %= 60;
    if (h > 0)
        return QString("%1:%2:%3").arg(h,2,10,QChar('0')).arg(m,2,10,QChar('0')).arg(s,2,10,QChar('0'));
    return QString("%1:%2").arg(m,2,10,QChar('0')).arg(s,2,10,QChar('0'));
}
Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_open_clicked()
{
   QString filename=QFileDialog::getOpenFileName(this,"打开视频","/home/my","视频(*.mp4)");
 //  qDebug()<<filename<<endl;
   QUrl url=QUrl::fromLocalFile(filename);
   player->setMedia(url);
   vdwgt->resize(ui->label->size());
   QFileInfo info(filename);
     auto *item = new QListWidgetItem(info.fileName());
     item->setToolTip(filename);
     item->setData(Qt::UserRole, filename);   // 关键：把完整路径塞进 UserRole
     ui->listWidget->addItem(item);

     // 2) 立刻播放新打开的
     player->setMedia(QUrl::fromLocalFile(filename));
     player->play();

     // 3) 可选：选中刚添加的条目
     ui->listWidget->setCurrentItem(item);
}

void Widget::on_pushButton_play_clicked()
{
    player->play();
}

void Widget::on_plause_clicked()
{
    player->pause();
}

void Widget::on_pushButton_stop_clicked()
{
    player->stop();
}
void Widget::paintEvent(QPaintEvent* e)
{
    vdwgt->resize(ui->label->size());
}
