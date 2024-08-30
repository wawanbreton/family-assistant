#include "hardware.h"

#ifdef ENV_SIMULATOR
#include "hardware/simulated/simulatedhardware.h"
#else
#error Define physical hardware manager
#endif


SINGLETON_IMPL_COMMON(Hardware)
SINGLETON_DESTROY(Hardware)
SINGLETON_DESTRUCTOR_IMPL(Hardware)

Hardware::Hardware(QObject* parent)
    : QObject{ parent }
{
}

void Hardware::init(QObject* parent)
{
    if (! instance_)
    {
        qInfo() << "Init start" << Hardware::staticMetaObject.className();
#ifdef ENV_SIMULATOR
        instance_ = new SimulatedHardware(parent);
#else
#endif
        qInfo() << "Init done" << Hardware::staticMetaObject.className();
    }
    else
    {
        qCritical() << "There is already a Hardware instance";
    }
}
