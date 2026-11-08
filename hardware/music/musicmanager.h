#pragma once

#include <QMediaPlayer>
#include <QObject>

#include "hardware/music/song.h"

class Playlist;

class MusicManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool playing READ getPlaying NOTIFY playingChanged)
    Q_PROPERTY(QString artist READ getArtist NOTIFY songChanged)
    Q_PROPERTY(QString title READ getTitle NOTIFY songChanged)
    Q_PROPERTY(bool hasPrevious READ getHasPrevious NOTIFY hasPreviousChanged)
    Q_PROPERTY(qreal volumeBathroom READ getVolumeBathroom WRITE setVolumeBathroom NOTIFY volumeBathroomChanged)
    Q_PROPERTY(qreal volumeLivingRoom READ getVolumeLivingRoom WRITE setVolumeLivingRoom NOTIFY volumeLivingRoomChanged)

public:
    explicit MusicManager(QObject* parent = nullptr);

    bool getPlaying() const;

    const QString& getArtist() const;

    const QString& getTitle() const;

    bool getHasPrevious() const;

    qreal getVolume() const;

    void setVolume(const qreal& volume);

    Q_INVOKABLE void mainButtonClicked();

    Q_INVOKABLE void nextButtonClicked();

    Q_INVOKABLE void previousButtonClicked();

signals:
    void playingChanged();

    void songChanged();

    void hasPreviousChanged();

    void volumeChanged();

private:
    bool playNext();

    void playSong(const Song& song);

    void setPlaying(const bool playing);

    void stopMusic();

    void onMediaStatusChanged(const QMediaPlayer::MediaStatus& status);

private:
    bool playing_{ false };
    QMediaPlayer* player_{ nullptr };
    Song song_;
    Playlist* playlist_{ nullptr };
};
