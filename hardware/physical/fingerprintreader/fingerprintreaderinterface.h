#pragma once

#include "easyqt/communication/core/abstractcommunicationinterface.h"
#include "hardware/physical/fingerprintreader/fingerprintaddingmode.h"
#include "hardware/physical/fingerprintreader/fingerprintreadercommands.h"

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
        FingerprintAddingMode mode,
        QObject* receiver,
        const SlotNoArgType& slotAnswer = nullptr,
        const SlotNoArgType& slotError = nullptr);

private:
    Command* makeRequest(FingerprintReaderCommands::Enum command);

private:
    QIODevice* const _device;
};
