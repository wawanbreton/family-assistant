#include "hardware/physical/fingerprintreader/fingerprintreaderinterface.h"

#include <easyqt/communication/commands/command.h>

#include "hardware/physical/fingerprintreader/fingerprintreadercommandsqueue.h"


FingerprintReaderInterface::FingerprintReaderInterface(QObject* parent, QIODevice* device, const bool logRawData)
    : AbstractCommunicationInterface(new FingerprintReaderCommandsQueue(nullptr, device, logRawData), parent)
    , _device(device)
{
}

void FingerprintReaderInterface::sendSleep(
    QObject* receiver,
    const SlotAnswerType& slotAnswer,
    const SlotNoArgType& slotError)
{
    sendRequest(FingerprintReaderCommands::Sleep, {}, receiver, slotAnswer, slotError);
}

bool FingerprintReaderInterface::onCommandReceivedImpl(Command* command)
{
    // if (command->getId() == Tmcm1161Commands::RequestPositionReached)
    // {
    //     emit positionReached();
    //     return false;
    // }
    // else
    {
        return AbstractCommunicationInterface::onCommandReceivedImpl(command);
    }
}

void FingerprintReaderInterface::sendRequest(
    const FingerprintReaderCommands::Enum command,
    const QList<QVariant>& dataRequest,
    QObject* receiver,
    const SlotAnswerType& slotAnswer,
    const SlotNoArgType& slotError,
    const SlotNoArgType& slotSent,
    const int& timeout)
{
    AbstractCommunicationInterface::sendRequest(
        static_cast<quint32>(command),
        dataRequest,
        receiver,
        slotAnswer,
        slotError,
        slotSent,
        timeout);
}
