#pragma once

#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QMenu>

#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>

#include "AbstractGraphModel.hpp"
#include "AbstractNodeGeometry.hpp"
#include "ConnectionIdHash.hpp"
#include "Definitions.hpp"
#include "Export.hpp"

#include "QUuidStdHash.hpp"

class QUndoStack;

namespace QtNodes
{

    class AbstractGraphModel;
    class AbstractNodePainter;
    class ConnectionGraphicsObject;
    class NodeGraphicsObject;
    class NodeStyle;
    // QGraphicsScene的一个实例，保存连接和节点。
    class NODE_EDITOR_PUBLIC BasicGraphicsScene : public QGraphicsScene
    {
        Q_OBJECT
    public:
        BasicGraphicsScene(AbstractGraphModel &graphModel, QObject *parent = nullptr);
        BasicGraphicsScene() = delete;
        ~BasicGraphicsScene();

    public:
        // 引用也能像指针一样进行上行或下行转换吗？
        AbstractGraphModel const &graphModel() const;
        AbstractGraphModel &graphModel();
        AbstractNodeGeometry &nodeGeometry();
        AbstractNodePainter &nodePainter();
        void setNodePainter(std::unique_ptr<AbstractNodePainter> newPainter);
        QUndoStack &undoStack();

    public:
        // 创建一个ConnectionGraphicsObject的“草图”实例。
        // 场景缓存了一个“草图”连接，它有一个松动的端点。
        // 在附件之后，“draft”实例被删除，取而代之的是一个普通的“full”连接被创建。
        // 函数@返回“draft”实例以进行进一步的几何操作。
        std::unique_ptr<ConnectionGraphicsObject> const &makeDraftConnection(ConnectionId const newConnectionId);

        // Deletes "draft" connection.
        // 当用户在构建新连接期间释放鼠标按钮而没有将其附加到任何节点时，该函数将被调用。
        void resetDraftConnection();

        // Deletes all the nodes. Connections are removed automatically.
        void clearScene();

    public:
        NodeGraphicsObject *nodeGraphicsObject(NodeId nodeId);
        ConnectionGraphicsObject *connectionGraphicsObject(ConnectionId connectionId);
        // orientation: 目标定位
        Qt::Orientation orientation() const { return _orientation; }
        void setOrientation(Qt::Orientation const orientation);

    public:
        virtual QMenu *createSceneMenu(QPointF const scenePos);

    Q_SIGNALS:
        void nodeMoved(NodeId const nodeId, QPointF const &newLocation);
        void nodeClicked(NodeId const nodeId);
        void nodeSelected(NodeId const nodeId);
        void nodeDoubleClicked(NodeId const nodeId);
        void nodeHovered(NodeId const nodeId, QPoint const screenPos);
        void nodeHoverLeft(NodeId const nodeId);
        void connectionHovered(ConnectionId const connectionId, QPoint const screenPos);
        void connectionHoverLeft(ConnectionId const connectionId);
        // 信号允许在点击一个节点时显示自定义上下文菜单。
        void nodeContextMenu(NodeId const nodeId, QPointF const pos);

    private:
        // Creates Node and Connection graphics objects.
        // 函数用于在构造函数中填充一个空场景。
        // 我们执行深度优先的AbstractGraphModel遍历。
        // 连接是通过检查非空节点的`Out`端口创建的。
        void traverseGraphAndPopulateGraphicsObjects();

        // 为给定的`connectionId`重绘相邻节点
        void updateAttachedNodes(ConnectionId const connectionId, PortType const portType);

    public Q_SLOTS:
        // 当从AbstractGraphModel中删除`connectionId`时调用。
        void onConnectionDeleted(ConnectionId const connectionId);

        // 当在AbstractGraphModel中创建`connectionId`时调用。
        void onConnectionCreated(ConnectionId const connectionId);
        void onNodeDeleted(NodeId const nodeId);
        void onNodeCreated(NodeId const nodeId);
        void onNodePositionUpdated(NodeId const nodeId);
        void onNodeUpdated(NodeId const nodeId);
        void onNodeClicked(NodeId const nodeId);
        void onModelReset();

    private:
        AbstractGraphModel &_graphModel;

        using UniqueNodeGraphicsObject = std::unique_ptr<NodeGraphicsObject>;

        using UniqueConnectionGraphicsObject = std::unique_ptr<ConnectionGraphicsObject>;

        std::unordered_map<NodeId, std::unique_ptr<NodeGraphicsObject>> _nodeGraphicsObjects;

        std::unordered_map<ConnectionId, std::unique_ptr<ConnectionGraphicsObject>> _connectionGraphicsObjects;

        std::unique_ptr<ConnectionGraphicsObject> _draftConnection;

        std::unique_ptr<AbstractNodeGeometry> _nodeGeometry;

        std::unique_ptr<AbstractNodePainter> _nodePainter;

        bool _nodeDrag;

        QUndoStack *_undoStack;

        Qt::Orientation _orientation;
    };

} // namespace QtNodes
