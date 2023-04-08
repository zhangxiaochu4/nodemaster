#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QString>

#include "Export.hpp"

namespace QtNodes
{
    struct NODE_EDITOR_PUBLIC NodeDataType
    {
        QString id;
        QString name;
    };
    class NODE_EDITOR_PUBLIC NodeData
    {
    public:
        virtual ~NodeData() = default;
        virtual bool sameType(NodeData const &nodeData) const { return (this->type().id == nodeData.type().id); }
        virtual NodeDataType type() const = 0;
    };

} // namespace QtNodes
Q_DECLARE_METATYPE(QtNodes::NodeDataType)
Q_DECLARE_METATYPE(std::shared_ptr<QtNodes::NodeData>)
