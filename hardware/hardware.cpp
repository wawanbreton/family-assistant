#include "hardware.h"

#include <QCoreApplication>
#include <QFile>
#include <QSerialPort>

#include "hardware/backlightmanager.h"
#include "hardware/fingerprintreader/fingerprintreaderinterface.h"
#ifdef ENV_SIMULATOR
#include "hardware/simulated/simulatedhardware.h"
#else
#include "hardware/physical/physicalhardware.h"
#endif


SINGLETON_IMPL_COMMON(Hardware)
SINGLETON_DESTROY(Hardware)
SINGLETON_DESTRUCTOR_IMPL(Hardware)

Hardware::Hardware(QObject* parent)
    : QObject{ parent }
{
    auto power_manager = new BacklightManager(this);
    connect(power_manager, &BacklightManager::setBacklight, this, &Hardware::setBacklight);
    qApp->installEventFilter(power_manager);

    auto serial_port = new QSerialPort("/dev/ttyUSB0", this);
    if (serial_port->open(QIODevice::ReadWrite))
    {
        serial_port->setBaudRate(QSerialPort::Baud19200);
        serial_port->setDataBits(QSerialPort::Data8);
        serial_port->setStopBits(QSerialPort::OneStop);
        serial_port->setFlowControl(QSerialPort::NoFlowControl);
        serial_port->setParity(QSerialPort::NoParity);

        constexpr bool log_raw_data = false;
        fingerprint_ = new FingerprintReaderInterface(this, serial_port, log_raw_data);
    }
    else
    {
        qWarning() << "Unable to open fingerprint reader on" << serial_port->portName()
                   << ", make sure that it is connected";
        delete serial_port;
    }
}

void Hardware::init(QObject* parent)
{
    if (! instance_)
    {
        qInfo() << "Init start" << Hardware::staticMetaObject.className();
#ifdef ENV_SIMULATOR
        instance_ = new SimulatedHardware(parent);
#else
        instance_ = new PhysicalHardware(parent);
#endif
        qInfo() << "Init done" << Hardware::staticMetaObject.className();
    }
    else
    {
        qCritical() << "There is already a Hardware instance";
    }
}

bool Hardware::hasFingerprintReader() const
{
    return fingerprint_;
}

void Hardware::registerFingerprint(
    const quint16 user,
    const quint8 attempt,
    QObject* receiver,
    const SlotNoArgType& slotSuccess,
    const SlotNoArgType& slotError)
{
    if (fingerprint_)
    {
        fingerprint_->addFingerprint(user, 1, attempt, 10000, receiver, slotSuccess, slotError);
    }
}

void Hardware::removeFingerprint(
    const quint16 user,
    QObject* receiver,
    const SlotNoArgType& slotSuccess,
    const SlotNoArgType& slotError)
{
    if (fingerprint_)
    {
        fingerprint_->removeFingerprint(user, receiver, slotSuccess, slotError);
    }
}

void Hardware::readFingerprintsCount(
    QObject* receiver,
    const std::function<void(quint16)>& slotSuccess,
    const SlotNoArgType& slotError)
{
    if (fingerprint_)
    {
        fingerprint_->getFingerprintsCount(receiver, slotSuccess, slotError);
    }
}

FingerprintReaderInterface* Hardware::getFingerprint()
{
    return fingerprint_;
}
