#pragma once

#include <easyqt/singleton.h>

class QApplication;
class QTimer;

class MetaManager : public QObject
{
    Q_OBJECT
    SINGLETON(MetaManager)

public:
    void load(const QString& file_path = QString());

private:
    void save();

private:
    QString data_file_path_;
    QTimer* const timer_data_changed_;
};
