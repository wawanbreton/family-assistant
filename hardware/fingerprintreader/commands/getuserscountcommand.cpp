#include "getuserscountcommand.h"

#include <easyqt/bitfield.h>

#include "hardware/fingerprintreader/fingerprintreaderheader.h"


GetUsersCountCommand::GetUsersCountCommand(
    const std::shared_ptr<const FingerprintReaderHeader>& header,
    QObject* parent)
    : BaseFingerprintCommand{ header, parent }
{
}

bool GetUsersCountCommand::unstreamCommandDataImpl(const QByteArray& rawData, CommandDataType::Enum dataType)
{
    if (! BaseFingerprintCommand::unstreamCommandDataImpl(rawData, dataType))
    {
        return false;
    }

    quint16 users_count;

    if (! BitField::toUInt16(rawData, users_count))
    {
        return false;
    }

    emit usersCountRead(users_count);

    return true;
}
