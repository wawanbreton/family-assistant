#include "fingerprintreaderheader.h"

#include <easyqt/bitfield.h>


FingerprintReaderHeader::FingerprintReaderHeader(FingerprintReaderCommands::Enum command)
    : CommandHeader()
    , command_(command)
{
}

FingerprintReaderCommands::Enum FingerprintReaderHeader::getCommand() const
{
    return command_;
}

bool FingerprintReaderHeader::matches(const std::shared_ptr<const CommandHeader> &header) const
{
    auto other_fp = std::dynamic_pointer_cast<const FingerprintReaderHeader>(header);
    return other_fp && other_fp->command_ == command_;
}

std::optional<QByteArray> FingerprintReaderHeader::streamData() const
{
    return BitField::toByteArray(static_cast<quint8>(command_));
}

QString FingerprintReaderHeader::toString() const
{
    return QString("ID:%1").arg(BitField::toHex(static_cast<quint8>(command_)));
}
