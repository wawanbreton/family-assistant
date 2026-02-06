#pragma once

#include <easyqt/communication/commands/command.h>

class FingerprintReaderHeader;

class BaseFingerprintCommand : public Command
{
    Q_OBJECT

public:
    BaseFingerprintCommand(const std::shared_ptr<const FingerprintReaderHeader>& header, QObject* parent = nullptr);

protected:
    virtual std::optional<QByteArray> streamData(CommandDataType::Enum dataType) const override;

    virtual bool unstreamCommandDataImpl(const QByteArray& rawData, CommandDataType::Enum dataType) override;
};
