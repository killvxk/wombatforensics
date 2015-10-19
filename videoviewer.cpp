#include "videoviewer.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

VideoViewer::VideoViewer(QWidget* parent) : QDialog(parent), ui(new Ui::VideoViewer)
{
    ui->setupUi(this);
    QtAV::Widgets::registerRenderers();
    vplayer = new QtAV::AVPlayer(this);
    vout = new QtAV::VideoOutput(this);
    vplayer->setRenderer(vout);
    ui->horizontalLayout->addWidget(vout->widget());
    connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(Seek(int)));
    connect(vplayer, SIGNAL(positionChanged(qint64)), this, SLOT(UpdateSlider()));
    connect(vplayer, SIGNAL(started()), this, SLOT(UpdateSlider()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(PlayPause()));
    ui->pushButton->setText("Pause");
    tskptr = &tskobj;
    tskptr->readimginfo = NULL;
    tskptr->readfsinfo = NULL;
    tskptr->readfileinfo = NULL;
    this->hide();
}

VideoViewer::~VideoViewer()
{
}

void VideoViewer::Seek(int pos)
{
    if(!vplayer->isPlaying())
        return;
    vplayer->seek(pos*1000LL);
}

void VideoViewer::PlayPause()
{
    if(!vplayer->isPlaying())
    {
        ui->pushButton->setText("Pause");
        vplayer->play();
        return;
    }
    ui->pushButton->setText("Play");
    vplayer->pause(!vplayer->isPaused());
}

void VideoViewer::UpdateSlider()
{
    ui->label3->setText(QTime(0, 0, 0).addMSecs(vplayer->mediaStopPosition()).toString("HH:mm:ss"));
    ui->horizontalSlider->setRange(0, int(vplayer->duration()/1000LL));
    ui->horizontalSlider->setValue(int(vplayer->position()/1000LL));
    ui->label->setText(QTime(0, 0, 0).addMSecs(vplayer->position()).toString("HH:mm:ss"));
}

void VideoViewer::GetVideo(QString tmpfilepath, unsigned long long objectid)
{
    // OpenParentImage
    std::vector<std::string> pathvector;
    unsigned long long imgid = 0;
    unsigned long long fsid = 0;
    unsigned long long fsoffset = 0;
    unsigned long long address = 0;
    pathvector.clear();
    QSqlQuery pimgquery(fcasedb);
    pimgquery.prepare("SELECT parimgid, parfsid, address FROM Data WHERE objectid = ?;");
    pimgquery.addBindValue(objectid);
    pimgquery.exec();
    pimgquery.next();
    imgid = pimgquery.value(0).toULongLong();
    fsid = pimgquery.value(1).toULongLong();
    address = pimgquery.value(2).toULongLong();
    pimgquery.finish();
    pimgquery.prepare("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum;");
    pimgquery.addBindValue(imgid);
    if(pimgquery.exec())
    {
        while(pimgquery.next())
        {
            pathvector.push_back(pimgquery.value(0).toString().toStdString());
        }
    }
    pimgquery.finish();
    tskptr->imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
    for(uint i=0; i < pathvector.size(); i++)
    {
        tskptr->imagepartspath[i] = pathvector.at(i).c_str();
    }
    tskptr->readimginfo = tsk_img_open(pathvector.size(), tskptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    free(tskptr->imagepartspath);
    // OpenParentFileSystem
    pimgquery.prepare("SELECT byteoffset FROM data where objectid = ?;");
    pimgquery.addBindValue(fsid);
    pimgquery.exec();
    pimgquery.next();
    fsoffset = pimgquery.value(0).toULongLong();
    pimgquery.finish();
    tskptr->readfsinfo = tsk_fs_open_img(tskptr->readimginfo, fsoffset, TSK_FS_TYPE_DETECT);
    // OpenFile
    tskptr->readfileinfo = tsk_fs_file_open_meta(tskptr->readfsinfo, NULL, address);
    // ReadFileToImageUsingByteArray
    if(tskptr->readfileinfo->meta != NULL)
    {
        char* ibuffer = new char[tskptr->readfileinfo->meta->size];
        ssize_t imglen = tsk_fs_file_read(tskptr->readfileinfo, 0, ibuffer, tskptr->readfileinfo->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
        QString tmppath = tmpfilepath + QString::number(objectid) + "-tmp";
        QFile tmpfile(tmppath);
        if(tmpfile.open(QIODevice::WriteOnly))
        {
            QDataStream outbuffer(&tmpfile);
            outbuffer.writeRawData(ibuffer, imglen);
            tmpfile.close();
        }
        vplayer->setFile(tmppath);
    }
}

void VideoViewer::ShowVideo(QString tmpfilepath, const QModelIndex &index)
{
    this->show();
    ui->label_2->setVisible(true);
    QtConcurrent::run(this, &VideoViewer::GetVideo, tmpfilepath, index.sibling(index.row(), 0).data().toULongLong());
    vplayer->play();
    ui->label_2->setVisible(false);
}
void VideoViewer::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        vplayer->stop();
        vplayer->unload();
        this->hide();
    }
}
