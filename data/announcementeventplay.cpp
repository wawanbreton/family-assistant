#include "announcementeventplay.h"

#include <QAudioFormat>
#include <QAudioSink>
#include <QBuffer>
#include <QCryptographicHash>
#include <QFuture>
#include <QFutureWatcher>
#include <QMediaPlayer>
#include <QtConcurrent/QtConcurrentRun>
#include <easyqt/datastorage.h>
#include <easyqt/file.h>
#include <easyqt/qobject_helper.h>
#include <piper.h>

#include "data/announcementmanager.h"


AnnouncementEventPlay::AnnouncementEventPlay(const QString& text, QObject* parent)
    : AbstractAnnouncementEvent{ parent }
    , text_(text)
{
    const QByteArray text_hash = QCryptographicHash::hash(text_.toUtf8(), QCryptographicHash::Md5).toHex();
    const QDir wav_files_dir = easyqt::DataStorage::cacheDir().filePath("announcement-wavs");
    const QString wav_filename = QString("%1.wav").arg(text_hash);
    wav_filepath_ = wav_files_dir.absoluteFilePath(wav_filename);

    if (! wav_files_dir.exists() && ! QDir().mkpath(wav_files_dir.absolutePath()))
    {
        qWarning() << "Unable to create WAV cache directory" << wav_files_dir;
    }
}

AnnouncementEventPlay::~AnnouncementEventPlay()
{
    qInfo() << "Destroy me";
}

void AnnouncementEventPlay::start()
{
    if (QFile(wav_filepath_).exists())
    {
        playAnnouncement();
        return;
    }

    QFuture<bool> generate_result = QtConcurrent::run([this]() { return generateWavFile(text_, wav_filepath_); });
    auto* generate_watcher = new QFutureWatcher<bool>(this);
    generate_watcher->setFuture(generate_result);
    QObject::connect(
        generate_watcher,
        &QFutureWatcher<bool>::finished,
        this,
        [this, generate_watcher] { onGenerateWavFileDone(generate_watcher->result()); });
}

void AnnouncementEventPlay::playAnnouncement()
{
    QAudioFormat format;
    format.setSampleRate(22050);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Float);

    auto file = new QFile(wav_filepath_, this);
    file->open(QIODevice::ReadOnly);

    auto audio_sink = new QAudioSink(format, this);
    audio_sink->setVolume(1.0);

    connect(audio_sink, &QAudioSink::stateChanged, this, &AnnouncementEventPlay::onAudioSinkStateChanged);
    audio_sink->start(file);
}

void AnnouncementEventPlay::onGenerateWavFileDone(bool success)
{
    if (success)
    {
        playAnnouncement();
    }
}

void AnnouncementEventPlay::onAudioSinkStateChanged(QAudio::State state)
{
    if (state == QAudio::IdleState)
    {
        IF_CAST_SENDER(QAudioSink, audio_sink)
        {
            // This really looks silly, but destroying the audio sink while not explicitely stopped leads to
            // segfaults...
            audio_sink->stop();
        }

        emit over();
    }
}

bool AnnouncementEventPlay::generateWavFile(const QString& text, const QString& wav_filepath)
{
    QElapsedTimer timer;
    timer.start();
    QByteArray sound_data;

    piper_synthesizer* synthesizer = AnnouncementManager::access()->getVoiceSynthesizer();
    bool success = false;
    if (piper_synthesize_start(synthesizer, text.toStdString().c_str(), nullptr) == PIPER_OK)
    {
        piper_audio_chunk chunk;
        int get_next_chunk;
        while ((get_next_chunk = piper_synthesize_next(synthesizer, &chunk)) == PIPER_OK)
        {
            sound_data.append(reinterpret_cast<const char*>(chunk.samples), chunk.num_samples * sizeof(float));
        }

        success = get_next_chunk == PIPER_DONE;
    }

    if (success)
    {
        easyqt::File::writeToFile(wav_filepath, sound_data);
    }

    qInfo() << "Generating the voice file took" << timer.elapsed() << "ms";

    return success;
}
