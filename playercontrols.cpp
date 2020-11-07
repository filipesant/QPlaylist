#include <QStyle>
#include <QDebug>
#include <QHoverEvent>
#include "playercontrols.h"
#include "ui_playercontrols.h"

PlayerControls::PlayerControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerControls)
{
    ui->setupUi(this);

    ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButtonNext->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->pushButtonBack->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->pushButtonVolume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    ui->volumeSlider->setValue(100);

    connect(ui->pushButtonPlay, &QAbstractButton::clicked, this, &PlayerControls::playClicked);
    connect(ui->pushButtonNext, &QAbstractButton::clicked, this, &PlayerControls::nextClicked);
    connect(ui->pushButtonBack, &QAbstractButton::clicked, this, &PlayerControls::previousClicked);
    connect(ui->volumeSlider, &QSlider::sliderReleased, this, &PlayerControls::onVolumeSliderValueChanged);
    connect(ui->timeLineSlider, &QSlider::sliderReleased, this, &PlayerControls::onSeek);
}

PlayerControls::~PlayerControls()
{
    delete ui;
}

QMediaPlayer::State PlayerControls::state() const
{
    return playerState;
}

void PlayerControls::setState(QMediaPlayer::State state)
{
    if (state != playerState) {
        playerState = state;

        switch (state) {
        case QMediaPlayer::StoppedState:
        case QMediaPlayer::PlayingState:
            ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

void PlayerControls::playClicked()
{
   if(playerState == QMediaPlayer::StoppedState || playerState == QMediaPlayer::PausedState){
       emit play();
   } else {
       emit pause();
   }
}

void PlayerControls::nextClicked()
{
    emit next();
}


void PlayerControls::previousClicked()
{
    emit previous();
}

void PlayerControls::onSeek()
{
    emit seek(ui->timeLineSlider->value());
}

void PlayerControls::setVolume(int volume)
{
    qreal logarithmicVolume = QAudio::convertVolume(volume / qreal(100),
                                                    QAudio::LinearVolumeScale,
                                                    QAudio::LogarithmicVolumeScale);

    ui->volumeSlider->setValue(qRound(logarithmicVolume * 100));
}

bool PlayerControls::isMuted() const
{
    return playerMuted;
}

void PlayerControls::setMuted(bool muted)
{
    if (muted != playerMuted) {
        playerMuted = muted;

        ui->pushButtonVolume->setIcon(style()->standardIcon(muted
                                                            ? QStyle::SP_MediaVolumeMuted
                                                            : QStyle::SP_MediaVolume));
    }
}

void PlayerControls::muteClicked()
{
    emit changeMuting(!playerMuted);
}

void PlayerControls::onVolumeSliderValueChanged()
{
    emit changeVolume(ui->volumeSlider->value());
}

void PlayerControls::setCurrentTime(QString value)
{
    ui->labelCurrentTime->setText(value);
}
void PlayerControls::setFinalTime(QString value)
{
    ui->labelTotalTime->setText(value);
}

void PlayerControls::setValueSlider(int value)
{
    ui->timeLineSlider->setValue(value);
}
void PlayerControls::setMaximumValueSlider(int value)
{
    ui->timeLineSlider->setMaximum(value);
}

bool PlayerControls::setVolumeBlockSignals(bool value)
{
    return ui->volumeSlider->blockSignals(value);
}
