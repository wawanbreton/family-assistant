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

signals:
    void parentUnlock();

private:
    Ui::SimulatedHardwareWidget* ui_;
};
