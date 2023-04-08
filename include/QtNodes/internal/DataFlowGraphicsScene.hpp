#pragma once

#include "BasicGraphicsScene.hpp"
#include "DataFlowGraphModel.hpp"
#include "Export.hpp"

namespace QtNodes {

//使用数据传播图的高级场景。
//类表示存在于v2中的一个场景。X，但是在构建时考虑到了新的模型-视图方法。
class NODE_EDITOR_PUBLIC DataFlowGraphicsScene : public BasicGraphicsScene
{
    Q_OBJECT
public:
    DataFlowGraphicsScene(DataFlowGraphModel &graphModel, QObject *parent = nullptr);

    ~DataFlowGraphicsScene() = default;

public:
    std::vector<NodeId> selectedNodes() const;

public:
    QMenu *createSceneMenu(QPointF const scenePos) override;

public Q_SLOTS:
    void save() const;

    void load();

Q_SIGNALS:
    void sceneLoaded();

private:
    DataFlowGraphModel &_graphModel;
};

} // namespace QtNodes
