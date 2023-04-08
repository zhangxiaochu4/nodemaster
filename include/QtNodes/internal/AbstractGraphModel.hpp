#pragma once

#include "Export.hpp"

#include <unordered_map>
#include <unordered_set>

#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QVariant>

#include "ConnectionIdHash.hpp"
#include "Definitions.hpp"

namespace QtNodes
{
    //模型-视图方法中的中心类。它提供各种各样的来自表示的后台用户数据结构的信息的图。该类允许修改图形结构:create并删除节点和连接。
    //我们使用两种类型的唯一id来进行图形操作:
    class NODE_EDITOR_PUBLIC AbstractGraphModel : public QObject
    {
        Q_OBJECT
    public:
        virtual NodeId newNodeId() = 0;
        //模型生成器负责生成唯一的`unsigned int`图中所有节点的id。从Id它应该是可以追溯到模型的内部表示的节点。
        virtual std::unordered_set<NodeId> allNodeIds() const = 0;

        //给定`nodeId`的所有输入和输出连接的集合。
        virtual std::unordered_set<ConnectionId> allConnectionIds(NodeId const nodeId) const = 0;

        //返回给定端口的所有连接节点id。
        //返回的节点和端口索引集合对应于类型与给定的`portType`相反。
        virtual std::unordered_set<ConnectionId> connections(NodeId nodeId, PortType portType, PortIndex index) const = 0;

        //检查具有给定`connectionId`的两个节点是否连接。
        virtual bool connectionExists(ConnectionId const connectionId) const = 0;

        //模型负责生成唯一的`NodeId`。@param[in] nodeType可以自由使用和解释
        //模型本身，它有助于区分可能的节点类型并在其中创建一个正确的实例。
        virtual NodeId addNode(QString const nodeType = QString()) = 0;

        //模型判断具有给定连接Id的连接是否可能。
        //默认实现比较相应的数据类型。可以覆盖该函数并连接non-equal数据类型。
        virtual bool connectionPossible(ConnectionId const connectionId) const = 0;

        //定义是否可以断开连接。
        virtual bool detachPossible(ConnectionId const) const { return true; }

        /// Creates a new connection between two nodes.
        /**
         * Default implementation emits signal
         * `connectionCreated(connectionId)`
         *
         * In the derived classes user must emite the signal to notify the
         * scene about the changes.
         */
        //创建两个节点之间的新连接。
        //默认实现发出信号“connectionCreated (connectionId)”在派生类中，用户必须发出信号以通知关于变化的场景。
        virtual void addConnection(ConnectionId const connectionId) = 0;

        virtual bool nodeExists(NodeId const nodeId) const = 0;

        //返回被请求的NodeRole的节点相关数据。
        //QVariant 节点标题，节点标题可见性，节点位置等。
        virtual QVariant nodeData(NodeId nodeId, NodeRole role) const = 0;

        //解包返回的`QVariant`值的实用函数标准的`QVariant AbstractGraphModel::nodeData(NodeId, nodeole)`函数。
        template<typename T>
        T nodeData(NodeId nodeId, NodeRole role) const
        {
            return nodeData(nodeId, role).value<T>();
        }

        virtual NodeFlags nodeFlags(NodeId nodeId) const
        {
            Q_UNUSED(nodeId);
            return NodeFlag::NoFlags;
        }

        /// @brief Sets node properties.
        /**
         * Sets: Node Caption, Node Caption Visibility,
         * Shyle, State, Node Position etc.
         * @see NodeRole.
         */
        virtual bool setNodeData(NodeId nodeId, NodeRole role, QVariant value) = 0;

        /// @brief Returns port-related data for requested NodeRole.
        /**
         * @returns Port Data Type, Port Data, Connection Policy, Port
         * Caption.
         */
        virtual QVariant portData(NodeId nodeId, PortType portType, PortIndex index, PortRole role) const = 0;

        /**
         * A utility function that unwraps the `QVariant` value returned from the
         * standard `QVariant AbstractGraphModel::portData(...)` function.
         */
        template<typename T>
        T portData(NodeId nodeId, PortType portType, PortIndex index, PortRole role) const
        {
            return portData(nodeId, portType, index, role).value<T>();
        }

        virtual bool setPortData(NodeId nodeId, PortType portType, PortIndex index, QVariant const &value,
                                 PortRole role = PortRole::Data) = 0;

        virtual bool deleteConnection(ConnectionId const connectionId) = 0;

        virtual bool deleteNode(NodeId const nodeId) = 0;

        /**
         * Reimplement the function if you want to store/restore the node's
         * inner state during undo/redo node deletion operations.
         */
        virtual QJsonObject saveNode(NodeId const) const { return {}; }

        /**
         * Reimplement the function if you want to support:
         *
         *   - graph save/restore operations,
         *   - undo/redo operations after deleting the node.
         *
         * QJsonObject must contain following fields:
         *
         *
         * ```
         * {
         *   id : 5,
         *   position : { x : 100, y : 200 },
         *   internal-data {
         *     "your model specific data here"
         *   }
         * }
         * ```
         *
         * The function must do almost exacly the same thing as the normal addNode().
         * The main difference is in a model-specific `inner-data` processing.
         */
        virtual void loadNode(QJsonObject const &) {}

    public:
        /**
         * Function clears connections attached to the ports that are scheduled to be
         * deleted. It must be called right before the model removes its old port data.
         *
         * @param nodeId Defines the node to be modified
         * @param portType Is either PortType::In or PortType::Out
         * @param first Index of the first port to be removed
         * @param last Index of the last port to be removed
         */
        void portsAboutToBeDeleted(NodeId const nodeId, PortType const portType, PortIndex const first, PortIndex const last);

        /**
         * Signal emitted when model no longer has the old data associated with the
         * given port indices and when the node must be repainted.
         */
        void portsDeleted();

        /**
         * Signal emitted when model is about to create new ports on the given node.
         * @param first Is the first index of the new port after insertion.
         * @param last Is the last index of the new port after insertion.
         *
         * Function caches existing connections that are located after the `last` port
         * index. For such connections the new "post-insertion" addresses are computed
         * and stored until the function AbstractGraphModel::portsInserted is called.
         */
        void portsAboutToBeInserted(NodeId const nodeId, PortType const portType, PortIndex const first, PortIndex const last);

        /**
         * Function re-creates the connections that were shifted during the port
         * insertion. After that the node is updated.
         */
        void portsInserted();

    Q_SIGNALS:
        void connectionCreated(ConnectionId const connectionId);

        void connectionDeleted(ConnectionId const connectionId);

        void nodeCreated(NodeId const nodeId);

        void nodeDeleted(NodeId const nodeId);

        void nodeUpdated(NodeId const nodeId);

        void nodeFlagsUpdated(NodeId const nodeId);

        void nodePositionUpdated(NodeId const nodeId);

        void modelReset();

    private:
        std::vector<ConnectionId> _shiftedByDynamicPortsConnections;
    };

} // namespace QtNodes
