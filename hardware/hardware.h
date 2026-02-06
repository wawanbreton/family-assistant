#pragma once

#include <QObject>
#include <easyqt/communication/core/abstractcommunicationinterface.h>
#include <easyqt/singleton.h>

class FingerprintReaderInterface;

class Hardware : public QObject
{
    Q_OBJECT
    SINGLETON(Hardware)

public:
    bool hasFingerprintReader() const;

    virtual void registerFingerprint(
        const quint16 user,
        const quint8 attempt,
        QObject* receiver = nullptr,
        const SlotNoArgType& slotSuccess = nullptr,
        const SlotNoArgType& slotError = nullptr);

    virtual void removeFingerprint(
        const quint16 user,
        QObject* receiver = nullptr,
        const SlotNoArgType& slotSuccess = nullptr,
        const SlotNoArgType& slotError = nullptr);

    virtual void readFingerprintsCount(
        QObject* receiver,
        const std::function<void(quint16)>& slotSuccess,
        const SlotNoArgType& slotError = nullptr);

    virtual void matchFingerprint(
        const quint16 user,
        QObject* receiver,
        const SlotNoArgType& slotSuccess,
        const SlotNoArgType& slotError);

    virtual void matchFingerprint(
        QObject* receiver,
        const std::function<void(quint16)>& slotSuccess,
        const SlotNoArgType& slotError);

protected:
    FingerprintReaderInterface* getFingerprint();

    virtual void setBacklight(const qreal power_percent) = 0;

private:
    FingerprintReaderInterface* fingerprint_{ nullptr };
};
