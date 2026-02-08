#pragma once

#include <easyqt/singleton.h>

class Kid;
class User;

class AccessManager : public QObject
{
    Q_OBJECT
    SINGLETON(AccessManager)

public:
    Q_INVOKABLE void registerFingerprintForUser(User* user);

    Q_INVOKABLE void tryLogAdminIn();

    Q_INVOKABLE void tryLogKidIn(const Kid* kid);

signals:
    void scanFingerprintStart();

    void scanFingerprintProgress(int step, int total_steps);

    void scanFingerprintDone();

    void scanFingerprintError();

    void adminLoginStart();

    void adminLoggedIn();

    void adminLoginFailed(const QString& error);

    void kidLoginStart();

    void kidLoggedIn(const Kid* kid);

    void kidLoginFailed(const QString& error);

private:
    void readFingerprintsCount();

    void registerFingerprint(User* user, int fingerprint_id);

    void onAddFingerprintDone(User* user, int fingerprint_id);

    void setHasAccessManagement(bool has_access_management);

    void onTryLogAdminAnswered(quint16 fingerprint_id);

    void onTryLogAdminFailed();

    void onTryLogKidAnswered(const Kid* kid);

    void onTryLogKidFailed();

    void onReadFingerprintsCountsDone(quint16 count);

    void onReadFingerprintsCountsError();

private:
    static constexpr int scan_attempts{ 3 };

private:
    bool has_access_management_{ false };
    int scan_fingerprint_attempt_;
};
