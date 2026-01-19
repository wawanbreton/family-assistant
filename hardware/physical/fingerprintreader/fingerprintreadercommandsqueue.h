#pragma once

#include <easyqt/communication/commands/commandheader.h>
#include <easyqt/communication/core/abstractdevicecommandsqueue.h>

class FingerprintReaderCommandsQueue : public AbstractDeviceCommandsQueue
{
    Q_OBJECT

public:
    explicit FingerprintReaderCommandsQueue(QObject* parent, QIODevice* device, const bool logRawData = false);

protected:
    virtual QByteArray streamCommandData(const CommandHeader* header, const QByteArray& commandRawData) const override;

    virtual DataParseResult unstreamReceivedData(
        const QByteArray& buffer,
        quint16& consumedBytes,
        CommandHeader*& header,
        QByteArray& commandRawData) const override;

    virtual void fixBuffer(QByteArray& buffer) const override;

    virtual Command* matchAnsweredCommand(const CommandHeader* header) override;

private:
    static constexpr quint8 magicNumber{ 0xf5 };
    static const QByteArray magicNumberData;
    static constexpr quint8 headerMaxDataSize{ 3 };
    static constexpr quint8 headerTotalDataSize{ 4 };
    static constexpr quint8 headerSize{ 8 };
};
