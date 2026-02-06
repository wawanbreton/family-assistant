#include "fingerprintaddingmodecommand.h"

#include <easyqt/bitfield.h>

#include "hardware/fingerprintreader/fingerprintreaderheader.h"


FingerprintAddingModeCommand::FingerprintAddingModeCommand(
    const std::shared_ptr<const FingerprintReaderHeader>& header,
    QObject* parent)
    : BaseFingerprintCommand{ header, parent }
{
}

void FingerprintAddingModeCommand::setAction(const Action action)
{
    action_ = action;
}

void FingerprintAddingModeCommand::setMode(const FingerprintAddingMode mode)
{
    mode_ = mode;
}

std::optional<QByteArray> FingerprintAddingModeCommand::streamData(CommandDataType::Enum dataType) const
{
    QByteArray data;

    data.push_back('\0');
    data.push_back(action_ == Action::Set ? static_cast<quint8>(mode_) : quint8(0));
    data.push_back(static_cast<quint8>(action_));

    return data;
}

bool FingerprintAddingModeCommand::unstreamCommandDataImpl(const QByteArray& rawData, CommandDataType::Enum dataType)
{
    if (! BaseFingerprintCommand::unstreamCommandDataImpl(rawData, dataType))
    {
        return false;
    }

    quint8 current_mode;

    if (! BitField::toUInt8(rawData.mid(1), current_mode))
    {
        return false;
    }

    mode_ = static_cast<FingerprintAddingMode>(current_mode);

    if (action_ == Action::Read)
    {
        emit readDone(mode_);
    }

    return true;
}
