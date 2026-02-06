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

    void setFingerprintReaderEnabled(bool enabled);

    void setUsersCount(const std::optional<int> users);

    void setMatchedUser(const std::optional<int> user);

    void setScanAttemptSuccess(bool success);

signals:
    void parentUnlock();

    void readUsersCount();

    void clearAllFingerprints();

    void matchUser();

    void scanUser(int attempt, int user_id);

private:
    Ui::SimulatedHardwareWidget* ui_;
};
