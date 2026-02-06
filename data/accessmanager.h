#pragma once

#include <easyqt/singleton.h>

class User;

class AccessManager : public QObject
{
    Q_OBJECT
    SINGLETON(AccessManager)
    Q_PROPERTY(bool has_access_management READ hasAccessManagement NOTIFY hasAccessManagementChanged)

public:
    Q_INVOKABLE void registerFingerprintForUser(User* user);

    bool hasAccessManagement() const;

signals:
    void hasAccessManagementChanged(bool has_access_management);

    void scanFingerprintStart(bool display_progress);

    void scanFingerprintProgress(int step, int total_steps);

    void scanFingerprintDone();

    void scanFingerprintError();

private:
    void registerFingerprint(User* user, int fingerprint_id);

    void onAddFingerprintDone(User* user, int fingerprint_id);

    void setHasAccessManagement(bool has_access_management);

private:
    static constexpr int scan_attempts{ 3 };

private:
    bool has_access_management_{ false };
    int scan_fingerprint_attempt_;
};
