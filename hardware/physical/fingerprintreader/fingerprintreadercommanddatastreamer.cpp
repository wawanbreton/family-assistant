#include "hardware/physical/fingerprintreader/fingerprintreadercommanddatastreamer.h"

#include <easyqt/bitfield.h>

#include "hardware/physical/fingerprintreader/fingerprintreadercommands.h"


FingerprintReaderCommandDataStreamer::FingerprintReaderCommandDataStreamer(QObject* parent)
    : AbstractCommandDataStreamer(parent)
{
}

QByteArray FingerprintReaderCommandDataStreamer::streamCommandData(
    const quint32& commandId,
    const QList<QVariant>& userData,
    CommandDataType::Enum dataType) const
{
    Q_UNUSED(dataType)

    QByteArray commandData;

    switch (static_cast<FingerprintReaderCommands::Enum>(commandId))
    {
        case FingerprintReaderCommands::Sleep:
            // No data
            break;
    }

    return commandData;
}

QList<QVariant> FingerprintReaderCommandDataStreamer::unstreamCommandData(
    const quint32& commandId,
    const QByteArray& rawData,
    CommandDataType::Enum dataType,
    const QList<QVariant>& dataSend) const
{
    Q_UNUSED(rawData)
    Q_UNUSED(dataType)
    Q_UNUSED(dataSend)

    QList<QVariant> userData;

    switch (static_cast<FingerprintReaderCommands::Enum>(commandId))
    {
        case FingerprintReaderCommands::Sleep:
            // No data
            break;
    }

    return userData;
}
