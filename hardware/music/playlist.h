#pragma once

#include <QObject>

struct Song;

class Playlist : public QObject
{
    Q_OBJECT

    public:
        explicit Playlist(QObject *parent = nullptr);

        Song playNext();

        Song playPrevious();

        bool hasPrevious() const { return _currentSong > 0; }

        void clearHistory() { _history.clear(); _currentSong = -1; emit hasPreviousChanged(); }

    signals:
        void hasPreviousChanged();

    private:
        void checkForUpdate();

        void checkDatabaseConnection();

        void updateSongs();

        QString getIdsMd5();

        void setSongs(const QList<int> &songs);

        static QString fullSongPath(const QString &songPath);

        static Song songFromId(int id);

    private:
        QString _idsMd5;
        QList<int> _songs;
        QList<int> _scores;
        QList<int> _history;
        int _currentSong{-1};
};

