#include "simulatedhardware.h"

#include <QFile>
#include <QSerialPort>

#include "hardware/physical/fingerprintreader/fingerprintreaderinterface.h"
#include "hardware/simulated/simulatedhardwarewidget.h"


SimulatedHardware::SimulatedHardware(QObject* parent)
    : Hardware{ parent }
    , widget_(new SimulatedHardwareWidget())
{
    connect(widget_, &SimulatedHardwareWidget::parentUnlock, this, &SimulatedHardware::parentUnlock);

    widget_->move(0, 0);
    widget_->show();

    auto serial_port = new QSerialPort("/dev/ttyUSB0", this);
    if (serial_port->open(QIODevice::ReadWrite))
    {
        serial_port->setBaudRate(QSerialPort::Baud19200);
        serial_port->setDataBits(QSerialPort::Data8);
        serial_port->setStopBits(QSerialPort::OneStop);
        serial_port->setFlowControl(QSerialPort::NoFlowControl);
        serial_port->setParity(QSerialPort::NoParity);
        constexpr const bool log_raw_data = true;
        auto fingerprint_reader = new FingerprintReaderInterface(this, serial_port, log_raw_data);

        fingerprint_reader->readFingerprintAddingMode(
            this,
            [](FingerprintAddingMode mode) { qDebug() << "mode1" << static_cast<quint8>(mode); },
            []() { qDebug() << "il veut toujours pas repondre"; });

        fingerprint_reader->setFingerprintAddingMode(
            FingerprintAddingMode::EnableRepeat,
            this,
            []() { qDebug() << "c'est change"; },
            []() { qDebug() << "il veut toujours pas repondre2"; });

        fingerprint_reader->readFingerprintAddingMode(
            this,
            [](FingerprintAddingMode mode) { qDebug() << "mode2" << static_cast<quint8>(mode); },
            []() { qDebug() << "il veut toujours pas repondre3"; });
    }
    else
    {
        qWarning() << "Unable to open fingerprint reader on" << serial_port->portName()
                   << ", make sure that it is connected";
        delete serial_port;
    }
}

void SimulatedHardware::setBacklight(const qreal power_percent)
{
    widget_->setBacklight(power_percent);
}
