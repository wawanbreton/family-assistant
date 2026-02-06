#include "accessmanager.h"

#include <ranges>

#include "data/user.h"
#include "data/usermanager.h"
#include "hardware/hardware.h"


SINGLETON_IMPL(AccessManager)
SINGLETON_DESTRUCTOR_IMPL(AccessManager)

AccessManager::AccessManager(QObject* parent)
    : QObject{ parent }
{
    if (Hardware::access()->hasFingerprintReader())
    {
        Hardware::access()->readFingerprintsCount(
            this,
            [this](quint16 count)
            {
                if (count > 0)
                {
                    qInfo() << "Access management is active";
                    has_access_management_ = true;
                }
            });
    }
}

void AccessManager::registerFingerprintForUser(User* user)
{
    int first_available_fingerprint_id;

    auto users_with_fingerprints
        = UserManager::get()->getUsers()
        | std::views::filter([](const User* user) { return user->getFingerprintId().has_value(); });
    if (users_with_fingerprints.empty())
    {
        first_available_fingerprint_id = 0;
    }
    else
    {
        first_available_fingerprint_id = std::ranges::max(
            users_with_fingerprints
            | std::views::transform([](const User* user) { return user->getFingerprintId().value(); }));
        first_available_fingerprint_id++;
    }

    scan_fingerprint_attempt_ = 1;
    emit scanFingerprintStart(true);
    emit scanFingerprintProgress(0, scan_attempts);
    registerFingerprint(user, first_available_fingerprint_id);
}

bool AccessManager::hasAccessManagement() const
{
    return has_access_management_;
}

void AccessManager::registerFingerprint(User* user, int fingerprint_id)
{
    Hardware::access()->registerFingerprint(
        fingerprint_id,
        scan_fingerprint_attempt_,
        this,
        [this, user, fingerprint_id] { onAddFingerprintDone(user, fingerprint_id); },
        std::bind(&AccessManager::scanFingerprintError, this));
}

void AccessManager::onAddFingerprintDone(User* user, int fingerprint_id)
{
    emit scanFingerprintProgress(scan_fingerprint_attempt_, scan_attempts);

    if (scan_fingerprint_attempt_ == scan_attempts)
    {
        if (user->getFingerprintId().has_value())
        {
            Hardware::access()->removeFingerprint(*user->getFingerprintId());
        }

        user->setFingerprintId(fingerprint_id);
        emit scanFingerprintDone();
        setHasAccessManagement(true);
    }
    else
    {
        scan_fingerprint_attempt_++;
        registerFingerprint(user, fingerprint_id);
    }
}

void AccessManager::setHasAccessManagement(bool has_access_management)
{
    if (has_access_management != has_access_management_)
    {
        has_access_management_ = has_access_management;
        emit hasAccessManagementChanged(has_access_management_);
    }
}
