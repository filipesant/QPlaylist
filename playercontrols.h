#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <QWidget>
#include <QMediaPlayer>

namespace Ui {
class PlayerControls;
}

class PlayerControls : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerControls(QWidget *parent = nullptr);
    QMediaPlayer::State state() const;
    int volume() const;
    bool isMuted() const;
    void setCurrentTime(QString value);
    void setFinalTime(QString value);
    void setMaximumValueSlider(int value);
    void setValueSlider(int value);
    bool setVolumeBlockSignals(bool value);
    ~PlayerControls();

public slots:
    void setState(QMediaPlayer::State state);
    void setVolume(int volume);
    void setMuted(bool muted);

signals:
    void play();
    void pause();
    void next();
    void previous();
    void seek(int pos);
    void changeVolume(int volume);
    void changeMuting(bool muting);

private:
    Ui::PlayerControls *ui;
    QMediaPlayer::State playerState = QMediaPlayer::StoppedState;
    bool playerMuted = false;


private slots:
    void playClicked();
    void nextClicked();
    void previousClicked();
    void muteClicked();
    void onVolumeSliderValueChanged();
    void onSeek();
};

#endif // PLAYERCONTROLS_H
