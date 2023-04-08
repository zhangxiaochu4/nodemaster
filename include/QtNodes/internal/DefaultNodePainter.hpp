#pragma once

#include <QtGui/QPainter>

#include "AbstractNodePainter.hpp"
#include "Definitions.hpp"

namespace QtNodes {

class BasicGraphicsScene;
class GraphModel;
class NodeGeometry;
class NodeGraphicsObject;
class NodeState;

//封装了paint代码的轻量级类
class NODE_EDITOR_PUBLIC DefaultNodePainter : public AbstractNodePainter
{
public:
    void paint(QPainter *painter, NodeGraphicsObject &ngo) const override;

    void drawNodeRect(QPainter *painter, NodeGraphicsObject &ngo) const;

    void drawConnectionPoints(QPainter *painter, NodeGraphicsObject &ngo) const;

    void drawFilledConnectionPoints(QPainter *painter, NodeGraphicsObject &ngo) const;

    void drawNodeCaption(QPainter *painter, NodeGraphicsObject &ngo) const;

    void drawEntryLabels(QPainter *painter, NodeGraphicsObject &ngo) const;

    void drawResizeRect(QPainter *painter, NodeGraphicsObject &ngo) const;
};
} // namespace QtNodes
