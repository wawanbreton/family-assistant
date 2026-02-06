#pragma once

#include "hardware/fingerprintreader/commands/basefingerprintcommand.h"


class FingerprintReaderHeader;

class GetUsersCountCommand : public BaseFingerprintCommand
{
    Q_OBJECT

public:
    GetUsersCountCommand(const std::shared_ptr<const FingerprintReaderHeader>& header, QObject* parent = nullptr);

signals:
    void usersCountRead(quint16 users);

protected:
    virtual bool unstreamCommandDataImpl(const QByteArray& rawData, CommandDataType::Enum dataType) override;
};
