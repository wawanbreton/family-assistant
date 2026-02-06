#pragma once

#include "hardware/fingerprintreader/commands/basefingerprintcommand.h"

class FingerprintReaderHeader;

class CheckFingerprintCommand : public BaseFingerprintCommand
{
    Q_OBJECT

public:
    CheckFingerprintCommand(const std::shared_ptr<const FingerprintReaderHeader>& header, QObject* parent = nullptr);

signals:
    void userMatched(quint16 user, quint8 permissions);

protected:
    virtual bool unstreamCommandDataImpl(const QByteArray& rawData, CommandDataType::Enum dataType) override;
};
