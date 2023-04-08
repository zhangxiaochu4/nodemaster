#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>

#include "DecimalData.hpp"

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

class QLabel;
//该模型规定了节点的输入和输出数量。在这个例子中，它没有逻辑。
class NumberDisplayDataModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    NumberDisplayDataModel();

    ~NumberDisplayDataModel() = default;

public:
    QString caption() const override { return QStringLiteral("Result"); }

    bool captionVisible() const override { return false; }

    QString name() const override { return QStringLiteral("Result"); }

public:
    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

    QWidget *embeddedWidget() override;

    double number() const;

private:
    std::shared_ptr<DecimalData> _numberData;

    QLabel *_label;
};
