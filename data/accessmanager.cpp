#include "accessmanager.h"

#include <QTimer>
#include <ranges>

#include "data/kid.h"
#include "data/usermanager.h"
#include "hardware/hardware.h"


SINGLETON_IMPL(AccessManager)
SINGLETON_DESTRUCTOR_IMPL(AccessManager)

AccessManager::AccessManager(QObject* parent)
    : QObject{ parent }
{
    if (Hardware::access()->hasFingerprintReader())
    {
        QTimer::singleShot(0, this, &AccessManager::readFingerprintsCount);
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
    emit scanFingerprintStart();
    emit scanFingerprintProgress(0, scan_attempts);
    registerFingerprint(user, first_available_fingerprint_id);
}

void AccessManager::tryLogAdminIn()
{
    if (has_access_management_)
    {
        emit adminLoginStart();
        Hardware::access()->matchFingerprint(
            this,
            std::bind(&AccessManager::onTryLogAdminAnswered, this, std::placeholders::_1),
            std::bind(&AccessManager::onTryLogAdminFailed, this));
    }
    else
    {
        emit adminLoggedIn();
    }
}

void AccessManager::tryLogKidIn(const Kid* kid)
{
    if (has_access_management_)
    {
        if (kid->getFingerprintId().has_value())
        {
            emit kidLoginStart();
            Hardware::access()->matchFingerprint(
                *kid->getFingerprintId(),
                this,
                [this, kid] { onTryLogKidAnswered(kid); },
                std::bind(&AccessManager::onTryLogKidFailed, this));
        }
        else
        {
            emit kidLoginFailed("Pas d'empreinte définie");
        }
    }
    else
    {
        emit kidLoggedIn(kid);
    }
}

void AccessManager::readFingerprintsCount()
{
    Hardware::access()->readFingerprintsCount(
        this,
        std::bind(&AccessManager::onReadFingerprintsCountsDone, this, std::placeholders::_1),
        std::bind(&AccessManager::onReadFingerprintsCountsError, this));
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
    has_access_management_ = has_access_management;
}

void AccessManager::onTryLogAdminAnswered(quint16 fingerprint_id)
{
    const QList<User*>& users = UserManager::get()->getUsers();
    auto iterator = std::ranges::find_if(
        users,
        [fingerprint_id](const User* user) { return user->getFingerprintId() == fingerprint_id; });
    if (iterator == users.end())
    {
        emit adminLoginFailed("Empreinte non associée");
        return;
    }

    if (! (*iterator)->isAdmin())
    {
        emit adminLoginFailed("Accès refusé");
        return;
    }

    emit adminLoggedIn();
}

void AccessManager::onTryLogAdminFailed()
{
    emit adminLoginFailed("Erreur de lecture");
}

void AccessManager::onTryLogKidAnswered(const Kid* kid)
{
    emit kidLoggedIn(kid);
}

void AccessManager::onTryLogKidFailed()
{
    emit kidLoginFailed("Empreinte non reconnue");
}

void AccessManager::onReadFingerprintsCountsDone(quint16 count)
{
    if (count > 0)
    {
        qInfo() << "Access management is active";
        has_access_management_ = true;
    }
    else
    {
        qInfo() << "No registered fingerprint, disabling access management";
    }
}

void AccessManager::onReadFingerprintsCountsError()
{
    qWarning() << "Error when reading fingerprints count, retrying later";
    QTimer::singleShot(2000, this, &AccessManager::readFingerprintsCount);
}
