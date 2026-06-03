#include "announcementeventplay.h"

#include <QAudioOutput>
#include <QBuffer>
#include <QCryptographicHash>
#include <QFuture>
#include <QFutureWatcher>
#include <QMediaPlayer>
#include <QtConcurrent/QtConcurrentRun>
#include <easyqt/bitfield.h>
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
    auto player = new QMediaPlayer(this);
    auto audio_output = new QAudioOutput(this);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &AnnouncementEventPlay::onMediaPlayerStatusChanged);
    player->setAudioOutput(audio_output);
    player->setSource(QUrl::fromLocalFile(wav_filepath_));
    player->play();
}

void AnnouncementEventPlay::onGenerateWavFileDone(bool success)
{
    if (success)
    {
        playAnnouncement();
    }
    else
    {
        emit over();
    }
}

void AnnouncementEventPlay::onMediaPlayerStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia)
    {
        emit over();
    }
}

bool AnnouncementEventPlay::generateWavFile(const QString& text, const QString& wav_filepath)
{
    QElapsedTimer timer;
    timer.start();
    QByteArray sound_data;

    bool success = false;

    piper_synthesizer* synthesizer = AnnouncementManager::access()->getVoiceSynthesizer();
    if (! synthesizer)
    {
        return success;
    }

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
        // Generate a proper WAV file from the RAW PCM data returned by piper
        QByteArray wav_header_data;

        constexpr quint16 channels = 1;
        constexpr quint32 sample_rate = 22050;
        constexpr quint32 bytes_per_sample = sizeof(float);
        constexpr bool invert_bytes = true;

        wav_header_data.push_back(QString("RIFF").toLatin1());
        wav_header_data.push_back(BitField::toByteArray<quint32>(36 + sound_data.size(), invert_bytes));
        wav_header_data.push_back(QString("WAVE").toLatin1());
        wav_header_data.push_back(QString("fmt ").toLatin1());
        wav_header_data.push_back(BitField::toByteArray<quint32>(18, invert_bytes)); // Size of data format
        wav_header_data.push_back(BitField::toByteArray<quint16>(3, invert_bytes));  // Floating-point values
        wav_header_data.push_back(BitField::toByteArray<quint16>(channels, invert_bytes));
        wav_header_data.push_back(BitField::toByteArray<quint32>(sample_rate, invert_bytes));
        wav_header_data.push_back(
            BitField::toByteArray<quint32>(sample_rate * channels * bytes_per_sample, invert_bytes)); // Byte rate
        wav_header_data.push_back(
            BitField::toByteArray<quint16>(channels * bytes_per_sample, invert_bytes)); // Block align
        wav_header_data.push_back(
            BitField::toByteArray<quint16>(bytes_per_sample * 8, invert_bytes));    // Bits per sample
        wav_header_data.push_back(BitField::toByteArray<quint16>(0, invert_bytes)); // Extra format info
        wav_header_data.push_back(QString("data").toLatin1());
        wav_header_data.push_back(BitField::toByteArray<quint32>(sound_data.size(), invert_bytes));

        easyqt::File::writeToFile(wav_filepath, wav_header_data + sound_data);
    }

    qInfo() << "Generating the voice file took" << timer.elapsed() << "ms";

    return success;
}
