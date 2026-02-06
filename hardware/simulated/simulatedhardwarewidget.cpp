#include "simulatedhardwarewidget.h"

#include "ui_simulatedhardwarewidget.h"


SimulatedHardwareWidget::SimulatedHardwareWidget(QWidget* parent)
    : QWidget(parent)
    , ui_(new Ui::SimulatedHardwareWidget)
{
    ui_->setupUi(this);

    connect(ui_->buttonClearFingerprints, &QPushButton::clicked, this, &SimulatedHardwareWidget::clearAllFingerprints);
    connect(ui_->buttonReadUsersCount, &QPushButton::clicked, this, &SimulatedHardwareWidget::readUsersCount);
    connect(ui_->buttonMatchAny, &QPushButton::clicked, this, &SimulatedHardwareWidget::matchAny);
    connect(
        ui_->buttonMatchUser,
        &QPushButton::clicked,
        this,
        [this] { emit matchUser(ui_->spinBoxMatchUserId->value()); });
    connect(
        ui_->buttonScanUser,
        &QPushButton::clicked,
        this,
        [this] { emit scanUser(ui_->spinBoxAttempt->value(), ui_->spinBoxScanUserId->value()); });
}

SimulatedHardwareWidget::~SimulatedHardwareWidget()
{
    delete ui_;
}

void SimulatedHardwareWidget::setBacklight(const qreal power_percent)
{
    ui_->sliderBacklight->setValue(std::llrint(power_percent));
}

void SimulatedHardwareWidget::setFingerprintReaderEnabled(bool enabled)
{
    ui_->groupBoxFingerprint->setEnabled(enabled);
}

void SimulatedHardwareWidget::setUsersCount(const std::optional<int> users)
{
    ui_->labelUsersCount->setText(users.has_value() ? QString::number(*users) : "#ERROR");
}

void SimulatedHardwareWidget::setMatchedAny(const std::optional<int> user)
{
    ui_->labelMatchedAny->setText(user.has_value() ? QString::number(*user) : "#NONE");
}

void SimulatedHardwareWidget::setMatchedUser(bool matched)
{
    ui_->labelMatchedUser->setText(matched ? "MATCH" : "#ERROR");
}

void SimulatedHardwareWidget::setScanAttemptSuccess(bool success)
{
    if (success)
    {
        ui_->labelScanResult->setText("OK");
        ui_->spinBoxAttempt->setValue((ui_->spinBoxAttempt->value() % 3) + 1);
    }
    else
    {
        ui_->labelScanResult->setText("#ERROR");
        ui_->spinBoxAttempt->setValue(1);
    }
}
