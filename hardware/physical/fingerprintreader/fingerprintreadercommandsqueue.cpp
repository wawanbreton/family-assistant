#include "hardware/physical/fingerprintreader/fingerprintreadercommandsqueue.h"

#include <easyqt/bitfield.h>

#include "hardware/physical/fingerprintreader/fingerprintreadercommanddatastreamer.h"
// #include "platform/utils/qt/assertutils.h"


const QByteArray FingerprintReaderCommandsQueue::magicNumberData{ BitField::toByteArray(magicNumber) };

FingerprintReaderCommandsQueue::FingerprintReaderCommandsQueue(
    QObject* parent,
    QIODevice* device,
    const bool logRawData)
    : AbstractDeviceCommandsQueue(
          new FingerprintReaderCommandDataStreamer(nullptr),
          device,
          false,
#ifdef ENV_SIMULATOR
          10000,
#else
          1000,
#endif
          parent,
          logRawData)
{
}

QByteArray FingerprintReaderCommandsQueue::streamCommandData(
    const CommandHeader* header,
    const QByteArray& commandRawData) const
{
    QByteArray headerData;
    QByteArray headerChecksumedData;

    QByteArray payloadData;

    if (commandRawData.size() <= headerMaxDataSize)
    {
        headerData.append(magicNumberData);

        QByteArray fixedSizeRawData = commandRawData.leftJustified(headerTotalDataSize, '\0');
        headerChecksumedData.append(BitField::toByteArray(static_cast<quint8>(header->getId())));
        headerChecksumedData.append(fixedSizeRawData);
        headerData.append(headerChecksumedData);

        quint8 checksum = 0;
        for (const char& byte : headerChecksumedData)
        {
            checksum ^= static_cast<quint8>(byte);
        }

        headerData.append(checksum);
        headerData.append(magicNumberData);
    }
    else
    {
    }

    return headerData + payloadData;
}

DataParseResult FingerprintReaderCommandsQueue::unstreamReceivedData(
    const QByteArray& buffer,
    quint16& consumedBytes,
    CommandHeader*& header,
    QByteArray& commandRawData) const
{
    DataParseResult result = DataParseResult::WrongData;

    if (buffer.size() >= headerSize)
    {
        if (buffer.startsWith(magicNumberData) && buffer.mid(7).startsWith(magicNumberData))
        {
            const quint8 received_checksum = buffer.at(6);
            quint8 expected_checksum = buffer.at(1);
            for (qsizetype i = 2; i <= 5; ++i)
            {
                expected_checksum ^= static_cast<quint8>(buffer.at(i));
            }

            if (received_checksum == expected_checksum)
            {
                // Data is consistent
                quint8 commandId;
                BitField::toUInt8(buffer.mid(1), commandId);

                commandRawData = buffer.mid(2, headerMaxDataSize);
                header = new CommandHeader(static_cast<quint32>(commandId));

                consumedBytes = headerSize;
                result = DataParseResult::Success;
            }
        }
    }
    else
    {
        // Wait a bit more
        result = DataParseResult::NotEnoughData;
    }

    return result;
}

void FingerprintReaderCommandsQueue::fixBuffer(QByteArray& buffer) const
{
    fixBufferStandard(buffer, magicNumberData, magicNumberData);
}

Command* FingerprintReaderCommandsQueue::matchAnsweredCommand(const CommandHeader* header)
{
    return matchAnsweredCommandBasic(header);
}
