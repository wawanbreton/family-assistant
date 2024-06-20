#include "utils/json.h"

#include <QJsonObject>
#include <QMetaProperty>


void Json::mapPropertyToObject(const QJsonObject& json_object, QObject* object)
{
    const QMetaObject* meta_object = object->metaObject();

    for (auto iterator = json_object.constBegin(); iterator != json_object.constEnd(); ++iterator)
    {
        const QString json_value_name = iterator.key();

        const int property_index = meta_object->indexOfProperty(json_value_name.toLocal8Bit().data());
        if (property_index >= 0)
        {
            QMetaProperty property = meta_object->property(property_index);

            if (property.typeId() == qMetaTypeId<QDateTime>())
            {
                QDateTime date_time = QDateTime::fromString(iterator.value().toString(), "yyyy-MM-dd hh:mm");
                date_time.setTimeSpec(Qt::UTC);
                property.write(object, date_time);
            }
            else
            {
                property.write(object, iterator.value().toVariant());
            }
        }
    }
}
