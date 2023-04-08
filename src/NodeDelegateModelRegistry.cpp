#include "NodeDelegateModelRegistry.hpp"

#include <QtCore/QFile>
#include <QtWidgets/QMessageBox>

using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::NodeDelegateModelRegistry;

std::unique_ptr<NodeDelegateModel> NodeDelegateModelRegistry::create(QString const &modelName)
{
    auto it = _registeredItemCreators.find(modelName);

    if (it != _registeredItemCreators.end())
    {
        return it->second();
    }

    return nullptr;
}

std::unordered_map<QString, std::function<std::unique_ptr<NodeDelegateModel>()>> const &NodeDelegateModelRegistry::registeredModelCreators() const
{
    return _registeredItemCreators;
}

std::unordered_map<QString, QString> const &NodeDelegateModelRegistry::registeredModelsCategoryAssociation() const
{
    return _registeredModelsCategory;
}

std::set<QString> const &NodeDelegateModelRegistry::categories() const
{
    return _categories;
}
