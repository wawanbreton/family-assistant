#include "addfingerprintcommand.h"

#include <easyqt/bitfield.h>

#include "hardware/fingerprintreader/fingerprintreaderheader.h"


AddFingerprintCommand::AddFingerprintCommand(
    const std::shared_ptr<const FingerprintReaderHeader>& header,
    QObject* parent)
    : SendUserCommand{ header, parent }
{
}

void AddFingerprintCommand::setUserPermissions(quint8 permissions)
{
    permissions_ = permissions;
}

std::optional<QByteArray> AddFingerprintCommand::streamData(CommandDataType::Enum dataType) const
{
    QByteArray data;

    data.push_back(BitField::toByteArray(getUser()));
    data.push_back(permissions_);

    return data;
}
