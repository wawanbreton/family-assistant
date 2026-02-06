#include "basefingerprintcommand.h"

#include <easyqt/bitfield.h>

#include "hardware/fingerprintreader/fingerprintreaderheader.h"
#include "hardware/fingerprintreader/fingerprintreaderresultcodes.h"


BaseFingerprintCommand::BaseFingerprintCommand(
    const std::shared_ptr<const FingerprintReaderHeader>& header,
    QObject* parent)
    : Command{ header, parent }
{
}

std::optional<QByteArray> BaseFingerprintCommand::streamData(CommandDataType::Enum dataType) const
{
    return QByteArray();
}

bool BaseFingerprintCommand::unstreamCommandDataImpl(const QByteArray& rawData, CommandDataType::Enum dataType)
{
    quint8 acknowledge;
    if (! BitField::toUInt8(rawData.mid(2), acknowledge))
    {
        return false;
    }

    if (acknowledge != static_cast<quint8>(FingerprintReaderResultCodes::Success))
    {
        return false;
    }

    return true;
}
