#include "checkfingerprintcommand.h"

#include <easyqt/bitfield.h>

#include "hardware/fingerprintreader/fingerprintreaderheader.h"


CheckFingerprintCommand::CheckFingerprintCommand(
    const std::shared_ptr<const FingerprintReaderHeader>& header,
    QObject* parent)
    : BaseFingerprintCommand{ header, parent }
{
}

bool CheckFingerprintCommand::unstreamCommandDataImpl(const QByteArray& rawData, CommandDataType::Enum dataType)
{
    quint16 user;
    quint8 result;
    if (! BitField::toUInt16(rawData, user) || ! BitField::toUInt8(rawData.mid(2), result))
    {
        return false;
    }

    if (result < 1 || result > 3)
    {
        return false;
    }

    emit userMatched(user, result);

    return true;
}
