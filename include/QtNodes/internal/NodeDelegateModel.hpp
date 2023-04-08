#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include "Definitions.hpp"
#include "Export.hpp"
#include "NodeData.hpp"
#include "NodeStyle.hpp"
#include "Serializable.hpp"

namespace QtNodes
{

    class StyleCollection;

    //这个类包装了特定于节点的数据操作，并将其传播给嵌套的DataFlowGraphModel，它是的子类AbstractGraphModel。
    //这个类与v3之前的NodeDataModel是一样的。
    class NODE_EDITOR_PUBLIC NodeDelegateModel : public QObject, public Serializable
    {
        Q_OBJECT

    public:
        NodeDelegateModel();

        virtual ~NodeDelegateModel() = default;
        // 可以在gui中隐藏标题
        virtual bool captionVisible() const { return true; }
        // 标题在gui中使用
        virtual QString caption() const = 0;
        //在gui中可以隐藏端口标题
        virtual bool portCaptionVisible(PortType, PortIndex) const { return false; }
        //端口标题在gui中用于标记各个端口
        virtual QString portCaption(PortType, PortIndex) const { return QString(); }
        //
        virtual QString name() const = 0;

    public:
        QJsonObject save() const override;

        void load(QJsonObject const &) override;

    public:
        virtual unsigned int nPorts(PortType portType) const = 0;

        virtual NodeDataType dataType(PortType portType, PortIndex portIndex) const = 0;

    public:
        virtual ConnectionPolicy portConnectionPolicy(PortType, PortIndex) const;

        NodeStyle const &nodeStyle() const;

        void setNodeStyle(NodeStyle const &style);

    public:
        virtual void setInData(std::shared_ptr<NodeData> nodeData, PortIndex const portIndex) = 0;

        virtual std::shared_ptr<NodeData> outData(PortIndex const port) = 0;
        //建议对进行延迟初始化
        //嵌入式widget，并在此函数中创建它，而不是在
        //当前模型的构造函数。
        //我们的模型注册中心能够很快按顺序实例化模型
        //调用非静态的`Model::name()`。如果嵌入的小部件是
        //在构造函数中分配，但实际上不会嵌入到某些中
        // QGraphicsProxyWidget，我们将有一个悬空指针。
        virtual QWidget *embeddedWidget() = 0;

        virtual bool resizable() const { return false; }

    public Q_SLOTS:

        virtual void inputConnectionCreated(ConnectionId const &) {}

        virtual void inputConnectionDeleted(ConnectionId const &) {}

        virtual void outputConnectionCreated(ConnectionId const &) {}

        virtual void outputConnectionDeleted(ConnectionId const &) {}

    Q_SIGNALS:

        //触发下游节点的更新。
        void dataUpdated(PortIndex const index);

        //触发空数据向下游传播。
        void dataInvalidated(PortIndex const index);

        void computingStarted();

        void computingFinished();

        void embeddedWidgetSizeUpdated();

        //在删除与端口相关的数据之前调用该函数。
        //该函数通知图模型，并使其删除并重新计算受影响的连接地址。
        void portsAboutToBeDeleted(PortType const portType, PortIndex const first, PortIndex const last);

        //在完成数据和端口修改时调用该函数。
        void portsDeleted();

        //在插入与端口相关的数据之前调用该函数。
        //该函数通知图模型并让它重新计算受影响的对象连接地址。
        void portsAboutToBeInserted(PortType const portType, PortIndex const first, PortIndex const last);

        //在完成数据和端口修改时调用该函数。
        void portsInserted();

    private:
        NodeStyle _nodeStyle;
    };

} // namespace QtNodes
