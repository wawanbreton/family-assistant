#include "musicmanager.h"

#include <QAudioOutput>
#include <QDebug>
#include <QMediaPlayer>

#include "data/preferences.h"
#include "hardware/music/dlnaplaylist.h"


SINGLETON_IMPL(MusicManager)
SINGLETON_DESTRUCTOR_IMPL(MusicManager)

MusicManager::MusicManager(QObject* parent)
    : AudioManager(PreferenceEntry::MusicAudioDevice, "music", parent)
    , player_(new QMediaPlayer(this))
{
    Preferences::access()->registerPreference(PreferenceEntry::DlnaDeviceDescriptionUrl, QMetaType::QString, QString());
    Preferences::access()->registerPreference(PreferenceEntry::DlnaMainMusicPath, QMetaType::QString, QString());
    Preferences::access()->registerPreference(PreferenceEntry::MusicMinVolume, QMetaType::Double, min_volume_);
    Preferences::access()->registerPreference(PreferenceEntry::MusicMaxVolume, QMetaType::Double, max_volume_);

    min_volume_ = Preferences::access()->getDouble(PreferenceEntry::MusicMinVolume);
    max_volume_ = Preferences::access()->getDouble(PreferenceEntry::MusicMaxVolume);

    if (max_volume_ <= min_volume_)
    {
        qWarning() << "Invalid volume range" << min_volume_ << max_volume_ << "=> using default";
        min_volume_ = 0.0;
        max_volume_ = 1.0;
    }

    auto audio_output = new QAudioOutput(this);
    audio_output->setDevice(getOutputDevice());
    audio_output->setVolume(min_volume_);
    player_->setAudioOutput(audio_output);

    const QUrl device_description_url{
        Preferences::get()->getString(PreferenceEntry::DlnaDeviceDescriptionUrl).trimmed()
    };

    if (! device_description_url.isValid() || device_description_url.scheme().isEmpty())
    {
        qWarning() << "No valid DLNA device description URL configured";
        return;
    }

    playlist_ = new DlnaPlaylist(device_description_url, this);

    connect(playlist_, &DlnaPlaylist::hasPreviousChanged, this, &MusicManager::hasPreviousChanged);
    connect(player_, &QMediaPlayer::mediaStatusChanged, this, &MusicManager::onMediaStatusChanged);
}

bool MusicManager::getPlaying() const
{
    return playing_;
}

const QString& MusicManager::getArtist() const
{
    return song_.artist;
}

const QString& MusicManager::getTitle() const
{
    return song_.title;
}

const QString& MusicManager::getIconPath() const
{
    return song_.icon_path;
}

bool MusicManager::getHasPrevious() const
{
    return playlist_->hasPrevious();
}

qreal MusicManager::getVolume() const
{
    const float actual_volume = player_->audioOutput()->volume();
    return (actual_volume - min_volume_) / (max_volume_ - min_volume_);
}

void MusicManager::setVolume(const qreal& volume)
{
    const float actual_volume = std::lerp(min_volume_, max_volume_, volume);

    if (! qFuzzyCompare(actual_volume, player_->audioOutput()->volume()))
    {
        player_->audioOutput()->setVolume(actual_volume);
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
