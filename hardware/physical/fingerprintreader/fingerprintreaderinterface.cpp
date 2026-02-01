#include "hardware/physical/fingerprintreader/fingerprintreaderinterface.h"

#include <easyqt/communication/commands/command.h>
#include <easyqt/qobject_helper.h>

#include "hardware/physical/fingerprintreader/commands/fingerprintaddingmodecommand.h"
#include "hardware/physical/fingerprintreader/fingerprintreadercommandsqueue.h"
#include "hardware/physical/fingerprintreader/fingerprintreaderheader.h"


FingerprintReaderInterface::FingerprintReaderInterface(QObject* parent, QIODevice* device, const bool logRawData)
    : AbstractCommunicationInterface(new FingerprintReaderCommandsQueue(nullptr, device, logRawData), parent)
    , _device(device)
{
}

void FingerprintReaderInterface::switchToSleep(
    QObject* receiver,
    const SlotNoArgType& slotAnswer,
    const SlotNoArgType& slotError)
{
    sendRequestEmptyAnswer(makeRequest(FingerprintReaderCommands::Sleep), receiver, slotAnswer, slotError);
}

void FingerprintReaderInterface::readFingerprintAddingMode(
    QObject* receiver,
    const std::function<void(FingerprintAddingMode)>& slotAnswer,
    const SlotNoArgType& slotError)
{
    Command* command = makeRequest(FingerprintReaderCommands::SetReadFingerprintAddingMode);
    IF_CAST_OBJECT(FingerprintAddingModeCommand, mode_command, command)
    {
        mode_command->setAction(FingerprintAddingModeCommand::Action::Read);
        connect(mode_command, &FingerprintAddingModeCommand::readDone, receiver, slotAnswer);
    }
    sendRequest(command, receiver, slotError);
}

void FingerprintReaderInterface::setFingerprintAddingMode(
    FingerprintAddingMode mode,
    QObject* receiver,
    const SlotNoArgType& slotAnswer,
    const SlotNoArgType& slotError)
{
    Command* command = makeRequest(FingerprintReaderCommands::SetReadFingerprintAddingMode);
    IF_CAST_OBJECT(FingerprintAddingModeCommand, mode_command, command)
    {
        mode_command->setAction(FingerprintAddingModeCommand::Action::Set);
        mode_command->setMode(mode);
    }
    sendRequestEmptyAnswer(command, receiver, slotAnswer, slotError);
}

Command* FingerprintReaderInterface::makeRequest(FingerprintReaderCommands::Enum command)
{
    return AbstractCommunicationInterface::makeRequest(std::make_shared<FingerprintReaderHeader>(command));
}
