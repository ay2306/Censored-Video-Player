#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    defautVolume = 100;
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    vw = new QVideoWidget(this);
    player->setVideoOutput(vw);
    this->setCentralWidget(vw);
    bar = new QProgressBar(this);
    slider = new QSlider(this);

    slider->setOrientation(Qt::Horizontal);
    ui->statusBar->addPermanentWidget(slider);
    ui->statusBar->addPermanentWidget(bar);

    connect(player,&QMediaPlayer::durationChanged,slider,&QSlider::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,slider,&QSlider::setValue);
    connect(slider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);

    connect(player,&QMediaPlayer::durationChanged,bar,&QProgressBar::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,bar,&QProgressBar::setValue);
    connect(bar,&QProgressBar::valueChanged,[this](int value){
//        if(value > 10 && value < 30){
//            apply_volume(0);
//        }else{
//            apply_volume(defautVolume);
//        }
        value/=1000;
//        QTextStream out(stdout);
//        out << value << "\n";
        if(value > 2 && value < 7)apply_volume(0);
        else apply_volume(defautVolume);
    });
//    apply_volume(0);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open A File","","Video File(*.*)");
    on_actionStop_triggered();

    player->setMedia(QUrl::fromLocalFile(filename));
    on_actionPlay_triggered();
}

void MainWindow::on_actionPlay_triggered()
{
    player->play();
    ui->statusBar->showMessage("Playing");
}

void MainWindow::on_actionPause_triggered()
{
    player->pause();
    ui->statusBar->showMessage("Paused");
}

void MainWindow::on_actionStop_triggered()
{
    player->stop();
    ui->statusBar->showMessage("Stopped");
}

void MainWindow::apply_volume(int volume){
    qreal linearVolume = QAudio::convertVolume(volume/qreal(100.0), QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
    player->setVolume(qRound(linearVolume*100));
}
