#pragma once

#include "Definitions.hpp"
#include "Export.hpp"

#include <QRectF>
#include <QSize>
#include <QTransform>

namespace QtNodes
{

    class AbstractGraphModel;

    class NODE_EDITOR_PUBLIC AbstractNodeGeometry
    {
    public:
        AbstractNodeGeometry(AbstractGraphModel &);
        virtual ~AbstractNodeGeometry() {}

        // 节点的大小加上一些额外的边距，以考虑绘图效果(例如阴影)或节点在尺寸矩形之外的部分(例如端口点)。
        // 默认实现在矩形的每边返回QSize + 20%的宽度和高度。
        virtual QRectF boundingRect(NodeId const nodeId) const;

        //定义节点矩形边界的直接矩形。
        virtual QSize size(NodeId const nodeId) const = 0;

        //当一些端口发生更改或嵌入式部件需要更新时，会触发该函数。
        virtual void recomputeSize(NodeId const nodeId) const = 0;

        //端口在节点坐标系中的位置。
        virtual QPointF portPosition(NodeId const nodeId, PortType const portType, PortIndex const index) const = 0;

        //一个使用`portPosition`和给定转换的便捷函数。
        virtual QPointF portScenePosition(NodeId const nodeId, PortType const portType, PortIndex const index, QTransform const &t) const;

        //定义在哪里绘制端口标签。这个点对应字体基线。
        virtual QPointF portTextPosition(NodeId const nodeId, PortType const portType, PortIndex const portIndex) const = 0;

        //定义从哪里开始绘制标题。这个点对应字体基线。
        virtual QPointF captionPosition(NodeId const nodeId) const = 0;

        //标题矩形用于估计节点的总大小。caption：标题
        virtual QRectF captionRect(NodeId const nodeId) const = 0;

        //Position for an embedded widget. Return any value if you don't embed.
        //嵌入小部件的位置。如果不嵌入，则返回任何值。
        virtual QPointF widgetPosition(NodeId const nodeId) const = 0;

        virtual PortIndex checkPortHit(NodeId const nodeId, PortType const portType, QPointF const nodePoint) const;

        virtual QRect resizeHandleRect(NodeId const nodeId) const = 0;

    protected:
        AbstractGraphModel &_graphModel;
    };

} // namespace QtNodes
