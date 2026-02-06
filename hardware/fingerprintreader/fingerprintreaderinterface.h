#pragma once

#include "easyqt/communication/core/abstractcommunicationinterface.h"
#include "hardware/fingerprintreader/fingerprintaddingmode.h"
#include "hardware/fingerprintreader/fingerprintreadercommands.h"

class QIODevice;

class FingerprintReaderInterface : public AbstractCommunicationInterface
{
    Q_OBJECT

public:
    explicit FingerprintReaderInterface(QObject* parent, QIODevice* device, const bool logRawData = false);

    void switchToSleep(
        QObject* receiver = nullptr,
        const SlotNoArgType& slotAnswer = nullptr,
        const SlotNoArgType& slotError = nullptr);

    void readFingerprintAddingMode(
        QObject* receiver,
        const std::function<void(FingerprintAddingMode)>& slotAnswer,
        const SlotNoArgType& slotError = nullptr);

    /*!
     * \brief Set the fingerprint adding mode, to enable or disable repeat
     * \note This setting is not saved after a device reboot
     */
    void setFingerprintAddingMode(
        const FingerprintAddingMode mode,
        QObject* receiver = nullptr,
        const SlotNoArgType& slotAnswer = nullptr,
        const SlotNoArgType& slotError = nullptr);

    /*!
     * \brief Add a user fingerprint
     * \param user The user ID, which should be between 1 and 4095
     * \param permissions The user permissions, which should be 1, 2 or 3
     * \param attempt Since this operations should be repeated 3 times, this is the attempt number, thus 1, 2 or 3
     * \param timeout The actual delay for the user to touch the reader, after which it will be canceled
     */
    void addFingerprint(
        const quint16 user,
        const quint8 permissions,
        const quint8 attempt,
        const int timeout,
        QObject* receiver = nullptr,
        const SlotNoArgType& slotAnswer = nullptr,
        const SlotNoArgType& slotError = nullptr);

    void removeFingerprint(
        const quint16 user,
        QObject* receiver = nullptr,
        const SlotNoArgType& slotAnswer = nullptr,
        const SlotNoArgType& slotError = nullptr);

    void removeAllFingerprints(
        QObject* receiver = nullptr,
        const SlotNoArgType& slotAnswer = nullptr,
        const SlotNoArgType& slotError = nullptr);

    void getFingerprintsCount(
        QObject* receiver,
        const std::function<void(quint16)>& slotAnswer,
        const SlotNoArgType& slotError = nullptr);

    void checkFingerprint(
        const quint16 user,
        const int timeout,
        QObject* receiver,
        const SlotNoArgType& slotAnswer,
        const SlotNoArgType& slotError);

    void checkFingerprint(
        const int timeout,
        QObject* receiver,
        const std::function<void(quint16, quint8)>& slotAnswer,
        const SlotNoArgType& slotError);

private:
    void sendUserRequest(
        FingerprintReaderCommands::Enum command_id,
        const quint16 user,
        const int timeout,
        QObject* receiver,
        const SlotNoArgType& slotAnswer,
        const SlotNoArgType& slotError);

    Command* makeRequest(FingerprintReaderCommands::Enum command, const int timeout = 0);

private:
    QIODevice* const _device;
};
