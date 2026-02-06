#include "hardware/fingerprintreader/fingerprintreadercommandsqueue.h"

#include <easyqt/bitfield.h>
#include <easyqt/qobject_helper.h>

#include "hardware/fingerprintreader/commands/addfingerprintcommand.h"
#include "hardware/fingerprintreader/commands/checkfingerprintcommand.h"
#include "hardware/fingerprintreader/commands/fingerprintaddingmodecommand.h"
#include "hardware/fingerprintreader/commands/getuserscountcommand.h"
#include "hardware/fingerprintreader/commands/sendusercommand.h"
#include "hardware/fingerprintreader/fingerprintreaderheader.h"

// #include "platform/utils/qt/assertutils.h"


const QByteArray FingerprintReaderCommandsQueue::magicNumberData{ BitField::toByteArray(magicNumber) };

FingerprintReaderCommandsQueue::FingerprintReaderCommandsQueue(
    QObject* parent,
    QIODevice* device,
    const bool logRawData)
    : AbstractDeviceCommandsQueue(
          device,
          false,
#ifdef ENV_SIMULATOR
          1000,
#else
          1000,
#endif
          parent,
          logRawData)
{
}

QByteArray FingerprintReaderCommandsQueue::streamCommandData(
    const std::shared_ptr<const CommandHeader>& header,
    const QByteArray& commandRawData) const
{
    QByteArray headerData;
    QByteArray headerChecksumedData;

    QByteArray payloadData;

    if (commandRawData.size() <= headerMaxDataSize)
    {
        headerData.append(magicNumberData);

        QByteArray fixedSizeRawData = commandRawData.leftJustified(headerTotalDataSize, '\0');
        headerChecksumedData.append(header->streamData().value_or(QByteArray()));
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

std::expected<AbstractDeviceCommandsQueue::DataParseResult, DataParseError>
    FingerprintReaderCommandsQueue::unstreamReceivedData(const QByteArray& buffer) const
{
    std::expected<DataParseResult, DataParseError> result = std::unexpected(DataParseError::WrongData);

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
                DataParseResult parse_result;

                parse_result.commandRawData = buffer.mid(2, headerMaxDataSize);
                parse_result.consumedBytes = headerSize;

                quint8 command_id;
                BitField::toUInt8(buffer.mid(1), command_id);
                parse_result.header = std::make_shared<FingerprintReaderHeader>(
                    static_cast<FingerprintReaderCommands::Enum>(command_id));

                result = parse_result;
            }
        }
    }
    else
    {
        // Wait a bit more
        result = std::unexpected(DataParseError::NotEnoughData);
    }

    return result;
}

void FingerprintReaderCommandsQueue::fixBuffer(QByteArray& buffer) const
{
    fixBufferStandard(buffer, magicNumberData, magicNumberData);
}

Command* FingerprintReaderCommandsQueue::makeCommandImpl(const std::shared_ptr<const CommandHeader>& header)
{
    IF_CAST_SHARED_PTR(const FingerprintReaderHeader, fp_header, header)
    {
        switch (fp_header->getCommand())
        {
            case FingerprintReaderCommands::SetReadFingerprintAddingMode:
                return new FingerprintAddingModeCommand(fp_header, this);

            case FingerprintReaderCommands::AddFingerprint1:
            case FingerprintReaderCommands::AddFingerprint2:
            case FingerprintReaderCommands::AddFingerprint3:
                return new AddFingerprintCommand(fp_header, this);

            case FingerprintReaderCommands::RemoveFingerprint:
            case FingerprintReaderCommands::CheckUserFingerprint:
                return new SendUserCommand(fp_header, this);

            case FingerprintReaderCommands::GetFingerprintsCount:
                return new GetUsersCountCommand(fp_header, this);

            case FingerprintReaderCommands::CheckFingerprint:
                return new CheckFingerprintCommand(fp_header, this);

            case FingerprintReaderCommands::Sleep:
            case FingerprintReaderCommands::RemoveAllFingerprints:
                return new BaseFingerprintCommand(fp_header, this);
        }
    }

    return AbstractCommandsQueue::makeCommandImpl(header);
}
