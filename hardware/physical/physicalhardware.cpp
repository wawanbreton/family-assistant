#include "physicalhardware.h"

#include <easyqt/file.h>

#include "hardware/physical/physicalhardware.h"

PhysicalHardware::PhysicalHardware(QObject* parent)
    : Hardware{ parent }
{
}

void PhysicalHardware::setBacklight(const qreal power_percent)
{
    const int brightness_value = std::llrint(power_percent * 0.01 * 255);
    easyqt::File::writeToFile(
        "/sys/class/backlight/10-0045/brightness",
        QString::number(brightness_value),
        easyqt::WriteFileMode::Basic);
}
