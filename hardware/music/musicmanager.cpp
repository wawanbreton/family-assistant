#include "musicmanager.h"

#include <QAudioOutput>
#include <QDebug>
#include <QMediaPlayer>

#include "hardware/music/playlist.h"


MusicManager::MusicManager(QObject* parent)
    : QObject(parent)
    , player_(new QMediaPlayer(this))
    , playlist_(new Playlist(this))
{
    connect(playlist_, &Playlist::hasPreviousChanged, this, &MusicManager::hasPreviousChanged);
    connect(player_, &QMediaPlayer::mediaStatusChanged, this, &MusicManager::onMediaStatusChanged);
}

const QString& MusicManager::getArtist() const
{
    return song_.artist;
}

const QString& MusicManager::getTitle() const
{
    return song_.title;
}

bool MusicManager::getHasPrevious() const
{
    return playlist_->hasPrevious();
}

qreal MusicManager::getVolume() const
{
    return player_->audioOutput()->volume();
}

void MusicManager::setVolume(const qreal& volume)
{
    if (volume != getVolume())
    {
        player_->audioOutput()->setVolume(volume);
        emit volumeChanged();
    }
}

void MusicManager::mainButtonClicked()
{
    if (playing_)
    {
        stopMusic();
        setPlaying(false);
    }
    else
    {
        if (playNext())
        {
            setPlaying(true);
        }
    }
}

void MusicManager::nextButtonClicked()
{
    if (playing_)
    {
        playNext();
    }
}

void MusicManager::previousButtonClicked()
{
    if (playing_)
    {
        Song song = playlist_->playPrevious();
        if (! song.path.isEmpty())
        {
            playSong(song);
        }
    }
}

bool MusicManager::playNext()
{
    Song song = playlist_->playNext();
    if (! song.path.isEmpty())
    {
        playSong(song);
        return true;
    }
    else
    {
        return false;
    }
}

void MusicManager::playSong(const Song& song)
{
    song_ = song;
    player_->stop();

    if (! song.path.isEmpty())
    {
        player_->setSource(song.path);
        player_->play();
    }

    emit songChanged();
}

void MusicManager::setPlaying(const bool playing)
{
    if (playing != playing_)
    {
        playing_ = playing;
        emit playingChanged();
    }
}

void MusicManager::stopMusic()
{
    player_->stop();
    playlist_->clearHistory();
    song_ = Song();
    emit songChanged();
}

void MusicManager::onMediaStatusChanged(const QMediaPlayer::MediaStatus& status)
{
    if (status == QMediaPlayer::EndOfMedia)
    {
        playNext();
    }
}
