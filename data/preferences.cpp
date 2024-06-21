#include "preferences.h"


SINGLETON_IMPL(Preferences)
SINGLETON_DESTRUCTOR_IMPL(Preferences)

Preferences::Preferences(QObject* parent)
    : easyqt::Preferences{ parent }
{
    connect(
        this,
        &easyqt::Preferences::valueChanged,
        this,
        [this](const int entry)
        {
            emit valueChanged(static_cast<PreferenceEntry::Enum>(entry));
        });
}

void Preferences::registerPreference(
    const PreferenceEntry::Enum entry,
    const QMetaType::Type type,
    const QVariant& default_value)
{
    return easyqt::Preferences::registerPreference(
        static_cast<int>(entry),
        type,
        default_value,
        PreferenceEntry::toString(entry));
}

QString Preferences::getString(const PreferenceEntry::Enum entry) const
{
    return easyqt::Preferences::getTypedValue<QString>(static_cast<int>(entry));
}

int Preferences::getInt(const PreferenceEntry::Enum entry) const
{
    return easyqt::Preferences::getTypedValue<int>(static_cast<int>(entry));
}

QVariant Preferences::getValue(const PreferenceEntry::Enum entry) const
{
    return easyqt::Preferences::getValue(static_cast<int>(entry));
}

void Preferences::setValue(const PreferenceEntry::Enum entry, const QVariant& value)
{
    return easyqt::Preferences::setValue(static_cast<int>(entry), value);
}
