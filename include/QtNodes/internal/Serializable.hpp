#pragma once

#include <QtCore/QJsonObject>

namespace QtNodes {
//序列化，可串行的
class Serializable
{
public:
    virtual ~Serializable() = default;

    virtual QJsonObject save() const { return {}; }

    virtual void load(QJsonObject const & /*p*/) {}
};
} // namespace QtNodes
