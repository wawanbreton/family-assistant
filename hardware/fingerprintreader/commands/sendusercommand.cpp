#include "sendusercommand.h"

#include <easyqt/bitfield.h>

#include "hardware/fingerprintreader/fingerprintreaderheader.h"


SendUserCommand::SendUserCommand(const std::shared_ptr<const FingerprintReaderHeader>& header, QObject* parent)
    : BaseFingerprintCommand{ header, parent }
{
}

quint16 SendUserCommand::getUser() const
{
    return user_;
}

void SendUserCommand::setUserNumber(quint16 user)
{
    user_ = user;
}

std::optional<QByteArray> SendUserCommand::streamData(CommandDataType::Enum dataType) const
{
    return BitField::toByteArray(user_);
}
