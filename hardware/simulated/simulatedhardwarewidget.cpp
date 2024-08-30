#include "simulatedhardwarewidget.h"

#include "ui_simulatedhardwarewidget.h"


SimulatedHardwareWidget::SimulatedHardwareWidget(QWidget* parent)
    : QWidget(parent)
    , ui_(new Ui::SimulatedHardwareWidget)
{
    ui_->setupUi(this);

    connect(ui_->buttonParentUnlock, &QPushButton::clicked, this, &SimulatedHardwareWidget::parentUnlock);
}

SimulatedHardwareWidget::~SimulatedHardwareWidget()
{
    delete ui_;
}
