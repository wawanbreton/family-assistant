#pragma once

#include <easyqt/communication/core/abstractcommanddatastreamer.h>

class FingerprintReaderCommandDataStreamer : public AbstractCommandDataStreamer
{
    Q_OBJECT

    public:
        explicit FingerprintReaderCommandDataStreamer(QObject *parent);

        virtual QByteArray streamCommandData(const quint32 &commandId,
                                             const QList<QVariant> &userData,
                                             CommandDataType::Enum dataType) const override;

        virtual QList<QVariant> unstreamCommandData(const quint32 &commandId,
                                                    const QByteArray &rawData,
                                                    CommandDataType::Enum dataType,
                                                    const QList<QVariant> &dataSend) const override;
};

