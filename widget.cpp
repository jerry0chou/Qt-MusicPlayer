#include <windows.h>
#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QPalette>
#include <QFile>
#include <QTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QPixmap>
#include <QScrollBar>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{


    ui->setupUi(this);

    trayIcon = new QSystemTrayIcon(this);


    //设置列表样式
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条

    //设置右边表格为透明
    QPalette pll = ui->tableWidget->palette();
    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
    ui->tableWidget->setPalette(pll);

    //设置表头不可见
    ui->tableWidget->horizontalHeader()->setVisible(false);// 水平不可见
    ui->tableWidget->verticalHeader()->setVisible(false);// 垂直不可见

    //设置列宽度
    ui->tableWidget->setColumnWidth(0, 372);
    //ui->tableWidget->setColumnWidth(1,108);

    //设置列高度
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(66);

    //设置垂直滚动条
   ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"        //垂直滑块整体
                                                              "background:#FFFFFF;"  //背景色
                                                              "padding-top:20px;"    //上预留位置（放置向上箭头）
                                                              "padding-bottom:20px;" //下预留位置（放置向下箭头）
                                                              "padding-left:3px;"    //左预留位置（美观）
                                                              "padding-right:3px;"   //右预留位置（美观）
                                                              "border-left:0px solid #d7d7d7;}"//左分割线
                                                              "QScrollBar::handle:vertical{"//滑块样式
                                                              "background:#dbdbdb;"  //滑块颜色
                                                              "border-radius:8px;"   //边角圆润
                                                              "min-height:40px;}"    //滑块最小高度
                                                              "QScrollBar::handle:vertical:hover{"//鼠标触及滑块样式
                                                              "background:#d0d0d0;}" //滑块颜色
                                                              );
    QFont font;
    font.setBold(true);
    font.setPointSize(10);

    QFont font1;
    font1.setBold(true);
    font1.setPointSize(12);

    setWindowIcon(QIcon(":/new/prefix1/icon/tray.png"));
    //当前歌词显示
    ui->lrcLabel->setAttribute(Qt::WA_TranslucentBackground);
    ui->lrcLabel->setFont(font1);
    ui->lrcLabel->setAlignment(Qt::AlignCenter);
    ui->lrcLabel->setStyleSheet("color: rgb(56, 212, 255)");
   // ui->label_3->setAttribute(Qt::WA_TranslucentBackground);
    //当前播放音乐歌曲名
    //ui->musicName->setAttribute(Qt::WA_TranslucentBackground);
    ui->musicName->setAlignment(Qt::AlignCenter);
    ui->musicName->setFont(font);
    ui->musicName->setText(tr("歌曲名"));
    ui->musicName->setStyleSheet("color: rgb(208,125,60)");
    //前一行歌词
    //ui->privouslrc->setAttribute(Qt::WA_TranslucentBackground);
    ui->privouslrc->setFont(font);
    ui->privouslrc->setAlignment(Qt::AlignCenter);
    ui->privouslrc->setStyleSheet("color: rgb(255,255,255)");
    //后一行歌词
    //ui->laterlrc->setAttribute(Qt::WA_TranslucentBackground);
    ui->laterlrc->setAlignment(Qt::AlignCenter);
    ui->laterlrc->setFont(font);
    ui->laterlrc->setStyleSheet("color: rgb(255,255,255)");
    //进度条当前时间
    ui->currTime->setStyleSheet("font-size:16px;color:rgb(208,125,60)");
    //歌曲总时间
    ui->totalTime->setStyleSheet("font-size:16px;color:rgb(208,125,60)");

    ui->pauseBtn->setToolTip("播放");
    ui->backBtn->setToolTip("上一首");
    ui->nextBtn->setToolTip("下一首");
    ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/play_hover.png);}"
                                "QPushButton:hover{border-image: url(:/new/prefix1/icon/play_hover.png);}"
                                "QPushButton:pressed{border-image: url(:/new/prefix1/icon/play_hover.png);}"
                                "QPushButton:focus{padding:-1;}");
    ui->backBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/pre_hover.png);}"
                               "QPushButton:hover{border-image: url(:/new/prefix1/icon/pre_hover.png);}"
                               "QPushButton:pressed{border-image: url(:/new/prefix1/icon/pre_press.png);}"
                               "QPushButton:focus{padding:-1;}");
    ui->nextBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/next_hover.png);}"
                               "QPushButton:hover{border-image: url(:/new/prefix1/icon/next_hover.png);}"
                               "QPushButton:pressed{border-image: url(:/new/prefix1/icon/next_press.png);}"
                               "QPushButton:focus{padding:-1;}");
    ui->folder->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/folder.png);}"
                                "QPushButton:hover{border-image: url(:/new/prefix1/icon/folder.png);}"
                                "QPushButton:pressed{border-image: url(:/new/prefix1/icon/folder.png);}"
                                "QPushButton:focus{padding:-1;}");

    ui->modelBtn->setToolTip("顺序播放");
    ui->modelBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/order.png);}"
                            "QPushButton:hover{border-image: url(:/new/prefix1/icon/order.png);}"
                            "QPushButton:pressed{border-image: url(:/new/prefix1/icon/order.png);}"
                            "QPushButton:focus{padding:-1;}");
    ui->lrcShow->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/lrc.png);}"
                                "QPushButton:hover{border-image: url(:/new/prefix1/icon/lrc.png);}"
                                "QPushButton:pressed{border-image: url(:/new/prefix1/icon/lrc.png);}"
                                "QPushButton:focus{padding:-1;}");

    //定时
    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(setPosition()));

    myLrc = new MyLrc();

    play = new QMediaPlayer(this);
    playList = new QMediaPlaylist(this);
//    playList->addMedia(QUrl::fromLocalFile("D:/CodeSpace/QtSpace/MusicPlayer/music/城府 - 许嵩.mp3"));
//    playList->addMedia(QUrl::fromLocalFile("D:/CodeSpace/QtSpace/MusicPlayer/music/shakira - try everything.mp3"));
//    playList->addMedia(QUrl::fromLocalFile("D:/CodeSpace/QtSpace/MusicPlayer/music/Maroon 5 - Maps.mp3"));
    play->setPlaylist(playList);
    play->setVolume(50);


    ui->slider->setCursor(QCursor(Qt::PointingHandCursor));
    ui->slider->setStyleSheet("  \
    QSlider::add-page:Horizontal\
    {  \
        background-color: rgb(116, 116, 110);\
       height:9px;\
    }\
    QSlider::sub-page:Horizontal \
   {\
        background-color: rgb(208, 125, 60);\
        height:8px;\
    }\
   QSlider::groove:Horizontal \
   {\
       background:transparent;\
       height:10px;\
   }\
   QSlider::handle:Horizontal \
   {\
       height:35px;\
       width:25px;\
       border-image: url(:/new/prefix1/icon/progress_thumb_2.png);\
       margin: -8 -6px; \
   }\
   ");

    ui->volumSlider->setCursor(QCursor(Qt::PointingHandCursor));
    ui->volumSlider->setRange(1,100);
    ui->volumSlider->setSliderPosition(play->volume());
    ui->volumSlider->setToolTip("音量:50");
    ui->volumSlider->setVisible(false);
    ui->volumSlider->setStyleSheet("  \
                                    QSlider::add-page:Horizontal\
                                    {  \
                                        background-color: rgb(116, 116, 110);\
                                       height:6px;\
                                    }\
                                    QSlider::sub-page:Horizontal \
                                   {\
                                        background-color: rgb(208, 125, 60);\
                                        height:6px;\
                                    }\
                                   QSlider::groove:Horizontal \
                                   {\
                                       background:transparent;\
                                       height:9px;\
                                   }\
                                   QSlider::handle:Horizontal \
                                   {\
                                       height:30px;\
                                       width:20px;\
                                       border-image: url(:/new/prefix1/icon/progress_thumb_2.png);\
                                       margin: -8 -6px; \
                                   }\
                                   ");
    //播放默认地址文件
    readDefaultFile();

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //信号与槽
    connect(ui->pauseBtn,SIGNAL(clicked()),this,SLOT(pauseBtnClicked()));
    connect(play,SIGNAL(positionChanged(qint64)),this,SLOT(UpdateTime(qint64)));
    connect(play,SIGNAL(metaDataChanged()),this,SLOT(btnUpdate()));
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(tableClick(int,int)));
    //定时器退出
    timer1 = new QTimer;
    connect(timer1,SIGNAL(timeout()),this,SLOT(subtime()));


}
void Widget::start(int i)
{
    switch (i)
    {
    case 0:
        timer->stop();
        return;
        break;
    case 1:
        settime(0,1,0);
        break;
    case 2:
        settime(0,3,0);
        break;
    case 3:
        settime(0,5,0);
        break;
    case 4:
        settime(0,10,0);
        break;
    case 5:
        settime(0,30,0);
        break;
    default:
        break;
    }
        //设定m_timer为1s发送一次信号
        while(m_time&&!timer1->isActive())
        {
            timer1->start(1000);
        }
}

//显示时间
void Widget::showtime()
{   //将剩余时间转化为时，分，秒
    int h = m_time/3600;
    int m_time1 = m_time%3600;
    int m = m_time1/60;
    int s = m_time1%60;


    if(m_time<=0)
    {
        timer1->stop();
        exit(0);
    }
}
//提交当前剩余时间
void Widget::subtime()
{
    m_time--;//剩余时间减少一秒
    showtime();
}

//设置倒计时
void Widget::settime(const unsigned int time0, const unsigned int time, const unsigned int time1)
{
   m_time = time0*3600+time*60+time1;
}

void Widget::tableClick(int row, int colum)
{
    qDebug()<<"playList->currentIndex() in tableClick "<<playList->currentIndex();
    playList->setCurrentIndex(row);
    qDebug()<<"tableClick: "<<row<<colum;
    qDebug()<<ui->tableWidget->item(row,colum)->text();
    pauseBtnClicked();
}

void Widget::setPosition()
{
    qint64 position = play->position();
    ui->slider->setMaximum(play->duration());
    ui->slider->setSliderPosition(position);

    ui->slider->setValue(position);
    QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));

    qint64 maxPosition = play->duration();
    QTime sumTime(0, maxPosition / 60000, qRound((maxPosition % 60000) / 1000.0));
    ui->totalTime->setText(sumTime.toString(tr("mm:ss")));
    ui->currTime->setText(duration.toString(tr("mm:ss")));

    if(ui->pauseBtn->toolTip() == tr("播放"))
    {
        ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/pause_press.png);}"
                                    "QPushButton:focus{padding:-1;}");
       ui->pauseBtn->setToolTip("暂停");
    }
}
void Widget::btnUpdate()
{
    QString title = play->metaData("Title").toString();
    QString author = play->metaData("Author").toString();
    QString Lyric=play->metaData("Lyrics").toString();
    QString musicname=author+" - "+title;
    qDebug()<<"musicname in btnUpdate"<<musicname;
    qDebug()<<"Lyric in btnUpdate"<<Lyric;
    ui->musicName->setText(musicname);
    resolve_lrc(musicname);
}

//void Widget::likefile()
//{
//    QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("另存为"), curFile, tr("(*.txt)"));
//    if (!fileName.isEmpty())
//    {
//        QFile file(fileName);
//        if (!file.open(QFile::WriteOnly | QFile::Text)){
//            QMessageBox::warning(this, QStringLiteral("打开用户保存的列表"), QStringLiteral("无法写入文件 %1:\n%2.").arg(fileName).arg(file.errorString()));
//        }
//        QTextStream out(&file);
//        QApplication::setOverrideCursor(Qt::WaitCursor);
//        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
//        {
//            out << ui->tableWidget->item(i, 2)->text();
//            if (i < ui->tableWidget->rowCount() - 1)
//            out << endl;
//        }
//        QApplication::restoreOverrideCursor();
//        setCurrentFile(fileName);
//    }
//}

void Widget::setCurrentFile(const QString &fileName)
{
    // canonicalFilePath()可以除去路径中的符号链接，“.”和“..”等符号
    curFile = QFileInfo(fileName).canonicalFilePath();
    qDebug()<<"curFile in setCurrentFile :"<<curFile;
    // 文档没有被更改过
    //savebutton->setDisabled(true);
}
void Widget::resolvePath(QString path)
{

}

void Widget::readDefaultFile()
{
    QDir dir("C:/Users/Jerry/Music");
    int index;
    QString title,author,path;
    QFileInfoList list;
    QFileInfo fileInfo;
    list=dir.entryInfoList();
    for(index=0;index<list.size();index++)
    {
        fileInfo=list.at(index);
        path=fileInfo.filePath();
        title=fileInfo.fileName().split(".").first().split("-").last();
        author=fileInfo.fileName().split(".").first().split("-").first();
        if(fileInfo.suffix()=="mp3")
        {
            int rownum=ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rownum);
            QTableWidgetItem *col1 = new QTableWidgetItem(fileInfo.fileName().split(".").front());
            col1->setTextAlignment(Qt::AlignCenter);
            if(rownum%2)
            {
                col1->setBackground(QBrush(QPixmap(":/new/prefix1/icon/gray1.png")));
            }
            ui->tableWidget->setItem(rownum, 0, col1);

            playList->addMedia(QUrl::fromLocalFile(path));
             qDebug()<<"readDefaultFile"<<path;
        }
    }
}
void Widget::dislikefile()
{
    QString initialName=QDir::homePath();
    QString playListFileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"), initialName, tr("(*.txt)"));
    bool flag = false;
    if (playList->isEmpty())
        flag = true;
    QFile file(playListFileName);
    QString path;
    if (!file.open(QFile::ReadOnly | QFile::Text))
        QMessageBox::warning(this, QStringLiteral("打开用户保存列表"), QStringLiteral("无法打开文件 %1:\n%2.").arg(playListFileName).arg(file.errorString()));
    else{
        QTextStream stream(&file);
        int i =0;
        while (!stream.atEnd()){
            path = stream.readLine();
            playList->addMedia(QUrl::fromLocalFile(path));
            QString fileName = path.split("\\").last();
            int rownum = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rownum);
            ui->tableWidget->setItem(rownum, 0, new QTableWidgetItem(fileName.split(".").front()));
//            ui->tableWidget->setItem(rownum, 1, new QTableWidgetItem(fileName.split(".").last()));
//            ui->tableWidget->setItem(rownum, 2, new QTableWidgetItem(QDir::toNativeSeparators(path)));
            i = i + 1;
        }
        if (i != 0){
            initialName = path.left(path.lastIndexOf('/', -1));
        }
    }

}

////函数功能:解析歌词
void Widget::resolve_lrc(const QString &source_file_name)
{
    lrc_map.clear();
    //判断文件是否存在
    if(source_file_name.isEmpty())
        return;
    QString lrc_file_name = "C:/Users/Jerry/Music/" + source_file_name + ".lrc";
    qDebug()<<"lrc_file_name in resolve_lrc"<<lrc_file_name;
    QFile file(lrc_file_name);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"lrc_file_name in resolve_lrc"<<" QIODevice::ReadOnly 歌词未找到";
        ui->lrcLabel->setText(tr("歌词未找到"));
        return;
    }

    //读取文件所有内容
    QString all_text = QString(file.readAll());
    file.close();
    QStringList lrc_line = all_text.split("\n");
    //正则表达式d{2}表示匹配2个数字
    QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
    //遍历
    foreach (QString oneline, lrc_line) {
        QString temp = oneline;
        temp.replace(rx,"");
        int pos = rx.indexIn(oneline,0);
        while (pos != -1) {
            QString cap = rx.cap(0);//返回第0个表达式匹配的内容
            // 将时间标签转换为时间数值，以毫秒为单位
            QRegExp regexp;
            regexp.setPattern("\\d{2}(?=:)");
            regexp.indexIn(cap);
            int minute = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\.)");
            regexp.indexIn(cap);
            int second = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\])");
            regexp.indexIn(cap);
            int millisecond = regexp.cap(0).toInt();
            qint64 totalTime = minute * 60000 + second * 1000 + millisecond * 10;
            lrc_map.insert(totalTime, temp);
            pos += rx.matchedLength();
            pos = rx.indexIn(oneline, pos);//匹配全部
        }
    }
    if(lrc_map.isEmpty()){
        qDebug()<<"lrc_file_name in resolve_lrc"<<"lrc_map.isEmpty() 歌词未找到";
        ui->lrcLabel->setText(tr("歌词未找到"));
        return;
    }
}
//////将歌词显示在主界面myMusic上
void Widget::UpdateTime(qint64 time)
{
    qint64 total_time_value = play->duration();
    QTime total_time(0, (total_time_value/60000)%60, (total_time_value/1000)%60);
    QTime current_time(0, (time/60000)%60, (time/1000)%60);
    QString str = current_time.toString("mm:ss") + "/" + total_time.toString("mm:ss");
    if(!lrc_map.isEmpty()) {
        // 获取当前时间在歌词中的前后两个时间点
        qint64 previous = 0;
        qint64 later = 0;
        //keys()方法返回lrc_map列表
        qint64 count = 0;
        foreach (qint64 value, lrc_map.keys()) {
            if (time >= value) {
                count ++;
                previous = value;
            } else {
                later = value;
                break;
            }
        }

     if (later == 0)
            later = total_time_value;
     qint64 front=0;
     qint64 front1 = 0;
     qint64 later1;
     qint64 a=0;
     foreach ( qint64 item, lrc_map.keys()) {
         a++;
         if( ((a==(count-1)) && item < previous) )
                    front = item;
         if( ((a==(count-2)) && item < previous) )
                    front1 = item;
         if( ((a==(count+2)) && item > previous) )
                    later1 = item;
     }
     QString frontLrc = lrc_map.value(front)+"\n"+"\n"+lrc_map.value(front1);
     QString later_lrc = lrc_map.value(later)+"\n"+"\n"+lrc_map.value(later1);
     QString current_lrc = lrc_map.value(previous);
     if(current_lrc != myLrc->text()) {
                ui->laterlrc->setText(later_lrc);
                myLrc->setText(current_lrc);
                ui->lrcLabel->setText(current_lrc);
                ui->privouslrc->setText(frontLrc);
                qint64 interval_time = later - previous;
                myLrc->start_lrc_mask(interval_time);
            }
      }
    else {  // 如果没有歌词文件，则在顶部标签中显示歌曲标题
            QString title = play->metaData("Title").toString()+".mp3";
            QString author=play->metaData("Author").toString();
            QString musicname =  author+ " - " + title;
            qDebug()<<"musicname in UpdateTime"<<musicname;
            ui->musicName->setText(musicname);
     }
}
void Widget::pauseBtnClicked()
{


    if(ui->pauseBtn->toolTip() == tr("暂停"))
    {
        timer->stop();
        play->pause();
        ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/play_hover.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/play_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/play_press.png);}"
                                    "QPushButton:focus{padding:-1;}");
        ui->pauseBtn->setToolTip("播放");
    }
    else
    {
        QString title = play->metaData("Title").toString();
        QString author = play->metaData("Author").toString();
        QString musicname = author + " - " + title;
        qDebug()<<"musicname in pauseBtnClicked:"<<musicname;

        ui->musicName->setText(musicname);
        resolve_lrc(musicname);

        timer->start();
        play->play();
        ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/pause_press.png);}"
                                    "QPushButton:focus{padding:-1;}");
        ui->pauseBtn->setToolTip("暂停");

    }

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_lrcShow_clicked()
{


    if(myLrc->isHidden())
        myLrc->show();
    else
        myLrc->hide();
}

void Widget::on_nextBtn_clicked()
{
    playList->next();
    if(ui->pauseBtn->toolTip() == tr("播放"))
    {
        ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/pause_press.png);}"
                                    "QPushButton:focus{padding:-1;}");
        ui->pauseBtn->setToolTip("暂停");
    }
    timer->start();
    play->play();
    //updaInfo();
    QString title = play->metaData("Title").toString();
    QString author = play->metaData("Author").toString();
    QString musicname = author + " - " + title;
    qDebug()<<"musicname in on_nextBtn_clicked:"<<musicname;
    //qDebug()<<title;
    ui->musicName->setText(musicname);
    resolve_lrc(musicname);
}

void Widget::on_backBtn_clicked()
{
    play->stop();
    playList->previous();

    if(ui->pauseBtn->toolTip() == tr("播放"))
    {
        ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/pause_press.png);}"
                                    "QPushButton:focus{padding:-1;}");
        ui->pauseBtn->setToolTip("暂停");
    }
    timer->start();
    play->play();
    QString title = play->metaData("Title").toString();
    QString author = play->metaData("Author").toString();
    QString musicname = author + " - " + title;
    qDebug()<<"musicname in on_backBtn_clicked:"<<musicname;
    //qDebug()<<title;
    ui->musicName->setText(musicname);
    resolve_lrc(musicname);

}

void Widget::on_volumBtn_clicked()
{
    if(ui->volumSlider->isVisible())
    {
        ui->volumSlider->setVisible(false);

    }
    else
    {
        ui->volumSlider->setVisible(true);

    }
}


void Widget::on_volumSlider_valueChanged(int value)
{
    play->setVolume(value);
    QString valueStr = QString("%1").arg(value);
    ui->volumSlider->setToolTip("音量:"+valueStr);
    ui->volumBtn->setToolTip("音量:"+valueStr);
}

void Widget::on_slider_sliderMoved(int position)
{
    play->stop();
    timer->stop();
    play->setPosition(position);
    timer->start();
    play->play();

    if(ui->pauseBtn->toolTip() == tr("播放"))
    {
        ui->pauseBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/pause_hover.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/pause_press.png);}"
                                    "QPushButton:focus{padding:-1;}");
        ui->pauseBtn->setToolTip("暂停");
    }

}
void Widget::mousePressEvent(QMouseEvent *event)
{
     if(event->button()==Qt::LeftButton)
     {
        dragPos = event->globalPos()-frameGeometry().topLeft();
        event->accept();
     }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        move(event->globalPos()-dragPos);
        event->accept();
    }
}

void Widget::on_close_clicked()
{
    exit(0);
}

void Widget::on_mini_clicked()
{
    myTrayIcon();//托盘
    //this->showMinimized();
    this->hide();
}

void Widget::on_folder_clicked()
{
    QString initialName=QDir::homePath();

    QStringList pathList=QFileDialog::getOpenFileNames(this, tr("选择文件"), initialName, tr("*.mp3 *.wma *.m4a *.ogg *.aac *.ape"));
    for(int i=0; i<pathList.size(); ++i) {
        QString path=QDir::toNativeSeparators(pathList.at(i));
        if(!path.isEmpty()) {
            playList->addMedia(QUrl::fromLocalFile(path));
            QString fileName=path.split("\\").last();
            int rownum=ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rownum);

            QTableWidgetItem *col1 = new QTableWidgetItem(fileName.split(".").front());
            col1->setTextAlignment(Qt::AlignCenter);
            if(rownum%2)
            {
                col1->setBackground(QBrush(QPixmap(":/new/prefix1/icon/gray1.png")));
            }

            ui->tableWidget->setItem(rownum, 0, col1);
            ui->tableWidget->setItem(rownum, 1, new QTableWidgetItem(fileName.split(".").last()));
            ui->tableWidget->setItem(rownum, 2, new QTableWidgetItem(QDir::toNativeSeparators(pathList.at(i))));
        }
    }
}

void Widget::on_modelBtn_clicked()
{
    if(ui->modelBtn->toolTip() == tr("顺序播放"))
        {
            playList->setPlaybackMode(QMediaPlaylist::Random);//随机
            ui->modelBtn->setToolTip("随机播放");
            ui->modelBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/random.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/random.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/random.png);}"
                                    "QPushButton:focus{padding:-1;}");
        }
       else if(ui->modelBtn->toolTip() == tr("随机播放"))
        {
            playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//单曲
            ui->modelBtn->setToolTip("单曲循环");
            ui->modelBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/cycle.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/cycle.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/cycle.png);}"
                                    "QPushButton:focus{padding:-1;}");
        }
      else
        {
            playList->setPlaybackMode(QMediaPlaylist::Loop); //顺序
            ui->modelBtn->setToolTip("顺序播放");
            ui->modelBtn->setStyleSheet("QPushButton:!hover{border-image: url(:/new/prefix1/icon/order.png);}"
                                    "QPushButton:hover{border-image: url(:/new/prefix1/icon/order.png);}"
                                    "QPushButton:pressed{border-image: url(:/new/prefix1/icon/order.png);}"
                                    "QPushButton:focus{padding:-1;}");
        }
}




//托盘

void Widget::myTrayIcon()
{

    QIcon icon = QIcon(":/new/prefix1/icon/tray.png");
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("隐藏到托盘");
    trayIcon->show();
    //弹出气泡提示
    trayIcon->showMessage("随心而动","隐藏到托盘",QSystemTrayIcon::Information,5000);
    //添加单/双击鼠标相应
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayiconActivated(QSystemTrayIcon::ActivationReason)));
    //行为
    minimizeAction = new QAction(tr("最小化 (&I)"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    before = new QAction(tr("上一曲 (&B)"), this);
    connect(before, SIGNAL(triggered()), this, SLOT(on_backBtn_clicked()));
    pause = new QAction(tr("播放/暂停 (&P)"), this);
    connect(pause, SIGNAL(triggered()), this, SLOT(pauseBtnClicked()));
    next = new QAction(tr("下一曲 (&N)"), this);
    connect(next, SIGNAL(triggered()), this, SLOT(on_nextone_clicked()));
    restoreAction = new QAction(tr("还原 (&R)"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("退出 (&Q)"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    //创建右键弹出菜单
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);

    trayIconMenu->addAction(before);
    trayIconMenu->addAction(pause);
    trayIconMenu->addAction(next);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);


}

//列表右键选择
void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    //menu.addAction(QStringLiteral("保存播放列表"), this, SLOT(likefile()));
    menu.addAction(QStringLiteral("删除歌曲"), this, SLOT(deleteSelectedMusic()));
    menu.addAction(QStringLiteral("清空列表"), this, SLOT(clearPlaylist()));
    menu.exec(event->globalPos());
}

void Widget::deleteSelectedMusic()
{
    QTableWidgetItem * item = ui->tableWidget->currentItem();
    if (item == Q_NULLPTR)
        return;
    playList->removeMedia(ui->tableWidget->currentRow());
    ui->tableWidget->removeRow(item->row());
    //emit isModified();
    //emit playlistReduced();
}

void Widget::clearPlaylist()
{
    while (ui->tableWidget->rowCount()){
        ui->tableWidget->removeRow(0);
        playList->removeMedia(0);
    }
    //emit isModified();
    //emit playlistReduced();
}

//鼠标响应
void Widget::trayiconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        //单击托盘图标
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标
        this->showNormal();
        this->raise();
        break;
    default:
        break;
    }
}


