#pragma once

#include <easyqt/communication/commands/command.h>

#include "hardware/physical/fingerprintreader/fingerprintaddingmode.h"

class FingerprintReaderHeader;

class FingerprintAddingModeCommand : public Command
{
    Q_OBJECT

public:
    enum class Action
    {
        Set = 0,
        Read = 1
    };

public:
    FingerprintAddingModeCommand(
        const std::shared_ptr<const FingerprintReaderHeader>& header,
        QObject* parent = nullptr);

    void setAction(const Action action);

    void setMode(const FingerprintAddingMode mode);

signals:
    void readDone(FingerprintAddingMode mode);

protected:
    virtual std::optional<QByteArray> streamData(CommandDataType::Enum dataType) const override;

    virtual bool unstreamCommandDataImpl(const QByteArray& rawData, CommandDataType::Enum dataType) override;

private:
private:
    Action action_{ Action::Read };
    FingerprintAddingMode mode_{ FingerprintAddingMode::DisableRepeat };
};
