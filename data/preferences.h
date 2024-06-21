#pragma once

#include <easyqt/preferences.h>
#include <easyqt/singleton.h>

#include "data/preferenceentry.h"


class Preferences : public easyqt::Preferences
{
    Q_OBJECT
    SINGLETON(Preferences)

public:
    void registerPreference(
        const PreferenceEntry::Enum entry,
        const QMetaType::Type type,
        const QVariant& default_value);

    QString getString(const PreferenceEntry::Enum entry) const;

    int getInt(const PreferenceEntry::Enum entry) const;

    QVariant getValue(const PreferenceEntry::Enum entry) const;

    void setValue(const PreferenceEntry::Enum entry, const QVariant& value);

signals:
    void valueChanged(const PreferenceEntry::Enum entry);
};
