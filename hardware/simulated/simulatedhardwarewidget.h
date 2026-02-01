#pragma once

#include <QWidget>

namespace Ui
{
class SimulatedHardwareWidget;
}

class SimulatedHardwareWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimulatedHardwareWidget(QWidget* parent = nullptr);
    ~SimulatedHardwareWidget();

    void setBacklight(const qreal power_percent);

signals:
    void parentUnlock();

private:
    Ui::SimulatedHardwareWidget* ui_;
};
