#pragma once

#include "easyqt/communication/core/abstractcommunicationinterface.h"
#include "hardware/physical/fingerprintreader/fingerprintreadercommands.h"

class QIODevice;

class FingerprintReaderInterface : public AbstractCommunicationInterface
{
    Q_OBJECT

public:
    explicit FingerprintReaderInterface(QObject* parent, QIODevice* device, const bool logRawData = false);

    void sendSleep(
        QObject* receiver = nullptr,
        const SlotAnswerType& slotAnswer = nullptr,
        const SlotNoArgType& slotError = nullptr);

signals:
    // void positionReached();

protected:
    virtual bool onCommandReceivedImpl(Command* command) override;

private:
    void sendRequest(
        const FingerprintReaderCommands::Enum command,
        const QList<QVariant>& dataRequest = QList<QVariant>(),
        QObject* receiver = nullptr,
        const SlotAnswerType& slotAnswer = nullptr,
        const SlotNoArgType& slotError = nullptr,
        const SlotNoArgType& slotSent = nullptr,
        const int& timeout = 0);

private:
    QIODevice* const _device;
};
