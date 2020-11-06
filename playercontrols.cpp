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

    ui->pushButtonVolume->setAttribute(Qt::WA_Hover);

    connect(ui->pushButtonPlay, &QAbstractButton::clicked, this, &PlayerControls::playClicked);
    connect(ui->pushButtonNext, &QAbstractButton::clicked, this, &PlayerControls::nextClicked);
    connect(ui->pushButtonBack, &QAbstractButton::clicked, this, &PlayerControls::previousClicked);
    connect(ui->pushButtonVolume, &QAbstractButton::clicked, this, &PlayerControls::muteClicked);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &PlayerControls::onVolumeSliderValueChanged);
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
    switch (playerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
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

int PlayerControls::volume() const
{
    qreal linearVolume =  QAudio::convertVolume(ui->volumeSlider->value() / qreal(100),
                                                QAudio::LogarithmicVolumeScale,
                                                QAudio::LinearVolumeScale);

    return qRound(linearVolume * 100);
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
    emit changeVolume(volume());
}
