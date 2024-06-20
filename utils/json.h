#pragma once

class QJsonObject;
class QObject;

namespace Json
{
void mapPropertyToObject(const QJsonObject& json_object, QObject* object);
};
