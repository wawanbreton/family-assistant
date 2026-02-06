#pragma once

#include "hardware/fingerprintreader/commands/basefingerprintcommand.h"

class FingerprintReaderHeader;

class SendUserCommand : public BaseFingerprintCommand
{
    Q_OBJECT

public:
    SendUserCommand(const std::shared_ptr<const FingerprintReaderHeader>& header, QObject* parent = nullptr);

    quint16 getUser() const;

    void setUserNumber(quint16 user);

protected:
    virtual std::optional<QByteArray> streamData(CommandDataType::Enum dataType) const override;

private:
    quint16 user_{ 0 };
};
