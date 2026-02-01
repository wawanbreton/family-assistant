#pragma once

#include <easyqt/communication/commands/commandheader.h>

#include "hardware/physical/fingerprintreader/fingerprintreadercommands.h"

class FingerprintReaderHeader : public CommandHeader
{
public:
    explicit FingerprintReaderHeader(FingerprintReaderCommands::Enum command);

    FingerprintReaderCommands::Enum getCommand() const;

    virtual bool matches(const std::shared_ptr<const CommandHeader> &header) const override;

    virtual std::optional<QByteArray> streamData() const override;

    virtual QString toString() const override;

private:
    const FingerprintReaderCommands::Enum command_;
};
