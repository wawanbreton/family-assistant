#include "simulatedhardware.h"

#include "hardware/simulated/simulatedhardwarewidget.h"


SimulatedHardware::SimulatedHardware(QObject* parent)
    : Hardware{ parent }
    , widget_(new SimulatedHardwareWidget())
{
    connect(widget_, &SimulatedHardwareWidget::parentUnlock, this, &SimulatedHardware::parentUnlock);

    widget_->move(0, 0);
    widget_->show();
}
