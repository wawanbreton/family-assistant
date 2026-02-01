#pragma once

#include <easyqt/communication/commands/commandheader.h>
#include <easyqt/communication/core/abstractdevicecommandsqueue.h>

class FingerprintReaderCommandsQueue : public AbstractDeviceCommandsQueue
{
    Q_OBJECT

public:
    explicit FingerprintReaderCommandsQueue(QObject* parent, QIODevice* device, const bool logRawData = false);

protected:
    virtual QByteArray streamCommandData(
        const std::shared_ptr<const CommandHeader>& header,
        const QByteArray& commandRawData) const override;

    virtual std::expected<DataParseResult, DataParseError>
        unstreamReceivedData(const QByteArray& buffer) const override;

    virtual void fixBuffer(QByteArray& buffer) const override;

    virtual Command* makeCommandImpl(const std::shared_ptr<const CommandHeader>& header) override;

private:
    static constexpr quint8 magicNumber{ 0xf5 };
    static const QByteArray magicNumberData;
    static constexpr quint8 headerMaxDataSize{ 3 };
    static constexpr quint8 headerTotalDataSize{ 4 };
    static constexpr quint8 headerSize{ 8 };
};
