#pragma once

#include "hardware/fingerprintreader/commands/sendusercommand.h"

class FingerprintReaderHeader;

class AddFingerprintCommand : public SendUserCommand
{
    Q_OBJECT

public:
    AddFingerprintCommand(const std::shared_ptr<const FingerprintReaderHeader>& header, QObject* parent = nullptr);

    void setUserPermissions(quint8 permissions);

protected:
    virtual std::optional<QByteArray> streamData(CommandDataType::Enum dataType) const override;

private:
    quint8 permissions_{ 1 };
};
