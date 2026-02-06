#include "hardware/fingerprintreader/fingerprintreaderinterface.h"

#include <easyqt/communication/commands/command.h>
#include <easyqt/qobject_helper.h>

#include "hardware/fingerprintreader/commands/addfingerprintcommand.h"
#include "hardware/fingerprintreader/commands/checkfingerprintcommand.h"
#include "hardware/fingerprintreader/commands/fingerprintaddingmodecommand.h"
#include "hardware/fingerprintreader/commands/getuserscountcommand.h"
#include "hardware/fingerprintreader/commands/sendusercommand.h"
#include "hardware/fingerprintreader/fingerprintreadercommandsqueue.h"
#include "hardware/fingerprintreader/fingerprintreaderheader.h"


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
    const FingerprintAddingMode mode,
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

void FingerprintReaderInterface::addFingerprint(
    const quint16 user,
    const quint8 permissions,
    const quint8 attempt,
    const int timeout,
    QObject* receiver,
    const SlotNoArgType& slotAnswer,
    const SlotNoArgType& slotError)
{
    FingerprintReaderCommands::Enum command_id;
    if (attempt == 2)
    {
        command_id = FingerprintReaderCommands::AddFingerprint2;
    }
    else if (attempt == 3)
    {
        command_id = FingerprintReaderCommands::AddFingerprint3;
    }
    else
    {
        command_id = FingerprintReaderCommands::AddFingerprint1;
    }

    Command* command = makeRequest(command_id, timeout);
    IF_CAST_OBJECT(AddFingerprintCommand, add_command, command)
    {
        add_command->setUserNumber(user);
        add_command->setUserPermissions(permissions);
    }
    sendRequestEmptyAnswer(command, receiver, slotAnswer, slotError);
}

void FingerprintReaderInterface::removeFingerprint(
    const quint16 user,
    QObject* receiver,
    const SlotNoArgType& slotAnswer,
    const SlotNoArgType& slotError)
{
    sendUserRequest(FingerprintReaderCommands::RemoveFingerprint, user, 0, receiver, slotAnswer, slotError);
}

void FingerprintReaderInterface::removeAllFingerprints(
    QObject* receiver,
    const SlotNoArgType& slotAnswer,
    const SlotNoArgType& slotError)
{
    sendRequestEmptyAnswer(
        makeRequest(FingerprintReaderCommands::RemoveAllFingerprints),
        receiver,
        slotAnswer,
        slotError);
}

void FingerprintReaderInterface::getFingerprintsCount(
    QObject* receiver,
    const std::function<void(quint16)>& slotAnswer,
    const SlotNoArgType& slotError)
{
    Command* command = makeRequest(FingerprintReaderCommands::GetFingerprintsCount);
    IF_CAST_OBJECT(GetUsersCountCommand, get_command, command)
    {
        connect(get_command, &GetUsersCountCommand::usersCountRead, receiver, slotAnswer);
    }
    sendRequest(command, receiver, slotError);
}

void FingerprintReaderInterface::checkFingerprint(
    const quint16 user,
    const int timeout,
    QObject* receiver,
    const SlotNoArgType& slotAnswer,
    const SlotNoArgType& slotError)
{
    sendUserRequest(FingerprintReaderCommands::CheckUserFingerprint, timeout, user, receiver, slotAnswer, slotError);
}

void FingerprintReaderInterface::checkFingerprint(
    const int timeout,
    QObject* receiver,
    const std::function<void(quint16, quint8)>& slotAnswer,
    const SlotNoArgType& slotError)
{
    Command* command = makeRequest(FingerprintReaderCommands::CheckFingerprint, timeout);
    IF_CAST_OBJECT(CheckFingerprintCommand, check_command, command)
    {
        connect(check_command, &CheckFingerprintCommand::userMatched, receiver, slotAnswer);
    }
    sendRequest(command, receiver, slotError);
}

void FingerprintReaderInterface::sendUserRequest(
    FingerprintReaderCommands::Enum command_id,
    const quint16 user,
    const int timeout,
    QObject* receiver,
    const SlotNoArgType& slotAnswer,
    const SlotNoArgType& slotError)
{
    Command* command = makeRequest(command_id, timeout);
    IF_CAST_OBJECT(SendUserCommand, add_command, command)
    {
        add_command->setUserNumber(user);
    }
    sendRequestEmptyAnswer(command, receiver, slotAnswer, slotError);
}

Command* FingerprintReaderInterface::makeRequest(FingerprintReaderCommands::Enum command, const int timeout)
{
    return AbstractCommunicationInterface::makeRequest(std::make_shared<FingerprintReaderHeader>(command), timeout);
}
