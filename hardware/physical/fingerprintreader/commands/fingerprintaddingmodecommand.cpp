#include "fingerprintaddingmodecommand.h"

#include <easyqt/bitfield.h>

#include "hardware/physical/fingerprintreader/fingerprintreaderheader.h"
#include "hardware/physical/fingerprintreader/fingerprintreaderresultcodes.h"


FingerprintAddingModeCommand::FingerprintAddingModeCommand(const std::shared_ptr<const FingerprintReaderHeader> &header, QObject* parent)
    : Command{ header, parent }
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
    quint8 current_mode;
    quint8 acknowledge;

    if (! BitField::toUInt8(rawData.mid(1), current_mode) || ! BitField::toUInt8(rawData.mid(2), acknowledge))
    {
        return false;
    }

    if (acknowledge != static_cast<quint8>(FingerprintReaderResultCodes::Success))
    {
        return false;
    }

    mode_ = static_cast<FingerprintAddingMode>(current_mode);

    emit readDone(mode_);

    return true;
}
