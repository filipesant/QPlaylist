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
    void changeVolume(int volume);
    void changeMuting(bool muting);

private:
    Ui::PlayerControls *ui;
    QMediaPlayer::State playerState = QMediaPlayer::StoppedState;
    bool playerMuted = false;


private slots:
    void playClicked();
    void muteClicked();
    void onVolumeSliderValueChanged();
};

#endif // PLAYERCONTROLS_H
