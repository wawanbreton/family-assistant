#include "playlist.h"

#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>

#include "resource/song.h"


Playlist::Playlist(QObject* parent)
    : QObject(parent)
{
#warning This only works if you have a Synology NAS with a proper configuration of the
#warning PostgreSQL media server database
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("192.168.0.3");
    db.setDatabaseName("mediaserver");
    db.setUserName("AudioStation");

    QTimer* timerDb = new QTimer(this);
    connect(timerDb, &QTimer::timeout, this, &Playlist::checkDatabaseConnection);
    timerDb->start(1000);

    QTimer* timerUpdate = new QTimer(this);
    connect(timerUpdate, &QTimer::timeout, this, &Playlist::checkForUpdate);
    timerDb->start(60 * 1000);

    checkDatabaseConnection();
    checkForUpdate();
}

Song Playlist::playNext()
{
    if (_songs.isEmpty())
    {
        return Song();
    }

    _currentSong++;

    while (_currentSong >= _history.count())
    {
        quint32 totalScore = 0;

        for (int i = 0; i < _scores.count(); i++)
        {
            totalScore += _scores[i];
        }

        quint32 winner = std::rand() % totalScore;
        quint32 currentScore = 0;
        bool winnerFound = false;
        int songId = 0;

        for (int i = 0; i < _songs.count(); i++)
        {
            if (winnerFound)
            {
                _scores[i] += 1;
            }
            else
            {
                currentScore += _scores[i];
                if (currentScore > winner)
                {
                    songId = _songs[i];
                    winnerFound = true;
                    _scores[i] = 0;
                }
                else
                {
                    _scores[i] += 1;
                }
            }
        }

        _history.append(songId);
    }

    while (_history.count() > 100)
    {
        _history.removeFirst();
        _currentSong--;
    }

    emit hasPreviousChanged();

    return songFromId(_history.value(_currentSong));
}

Song Playlist::playPrevious()
{
    if (_history.count() > 1)
    {
        _currentSong = std::max(0, _currentSong - 1);
        emit hasPreviousChanged();
        return songFromId(_history.value(_currentSong));
    }
    else
    {
        return Song();
    }
}

void Playlist::checkDatabaseConnection()
{
    QSqlDatabase database = QSqlDatabase::database();
    if (! database.isOpen())
    {
        database.open();

        if (database.isOpen())
        {
            checkForUpdate();
        }
    }
}

void Playlist::checkForUpdate()
{
    if (getIdsMd5() != _idsMd5) // Check if a song has been added or removed
    {
        updateSongs();
    }
}

void Playlist::updateSongs()
{
    QList<int> songs;

    QSqlQuery query("SELECT id FROM music");
    while (query.next())
    {
        songs << query.value(0).toInt();
    }

    setSongs(songs);

    _idsMd5 = getIdsMd5();
}

QString Playlist::getIdsMd5()
{
    QSqlQuery queryMd5("SELECT md5(array_to_string(array(SELECT id FROM music ORDER BY id), ''))");
    queryMd5.next();
    return queryMd5.value(0).toString();
}

void Playlist::setSongs(const QList<int>& songs)
{
    _songs = songs;

    _scores.clear();
    _scores.reserve(songs.count());

    for (int i = 0; i < songs.count(); ++i)
    {
        _scores << songs.count();
    }
}

QString Playlist::fullSongPath(const QString& songPath)
{
#warning For this to work, musics files should be mounted on a local directory (e.g. via NFS)
    return QDir::homePath() + "/Musique/" + songPath;
}

Song Playlist::songFromId(int id)
{
    QSqlQuery query;
    query.prepare("SELECT path,artist,title FROM music WHERE id = ?");
    query.bindValue(0, id);

    Song song;

    if (query.exec() && query.next())
    {
        QStringList musicFileParts = query.value(0).toString().split("/", Qt::SkipEmptyParts);
        if (musicFileParts.count() >= 3)
        {
            song.path = fullSongPath(QStringList(musicFileParts.mid(2)).join("/"));
            song.artist = query.value(1).toString();
            song.title = query.value(2).toString();
        }
    }

    return song;
}
