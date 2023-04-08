#pragma once

#include "Export.hpp"
#include "NodeData.hpp"
#include "NodeDelegateModel.hpp"
#include "QStringStdHash.hpp"

#include <QtCore/QString>

#include <functional>
#include <memory>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace QtNodes
{

    /// Class uses map for storing models (name, model)
    class NODE_EDITOR_PUBLIC NodeDelegateModelRegistry
    {
    public:
        using RegistryItemPtr = std::unique_ptr<NodeDelegateModel>;
        using RegistryItemCreator = std::function<RegistryItemPtr()>;
        using RegisteredModelCreatorsMap = std::unordered_map<QString, RegistryItemCreator>;
        using RegisteredModelsCategoryMap = std::unordered_map<QString, QString>;
        using CategoriesSet = std::set<QString>;

        // using RegisteredTypeConvertersMap = std::map<TypeConverterId, TypeConverter>;

        NodeDelegateModelRegistry() = default;
        ~NodeDelegateModelRegistry() = default;

        NodeDelegateModelRegistry(NodeDelegateModelRegistry const &) = delete;
        NodeDelegateModelRegistry(NodeDelegateModelRegistry &&) = default;

        NodeDelegateModelRegistry &operator=(NodeDelegateModelRegistry const &) = delete;

        NodeDelegateModelRegistry &operator=(NodeDelegateModelRegistry &&) = default;

    public:
        template<typename ModelType>
        void registerModel(std::function<std::unique_ptr<NodeDelegateModel>()> creator, QString const &category = "Nodes")
        {
            QString const name = computeName<ModelType>(HasStaticMethodName<ModelType>{}, creator);
            if (!_registeredItemCreators.count(name))
            {
                _registeredItemCreators[name] = std::move(creator);
                _categories.insert(category);
                _registeredModelsCategory[name] = category;
            }
        }
        // NumberSourceDataModel
        template<typename ModelType>
        void registerModel(QString const &category = "Nodes")
        {
            //RegistryItemPtr = NodeDelegateModel
            RegistryItemPtr tmp;
            qDebug()<<QString::fromStdString(std::string(typeid(tmp).name()));
            std::function<std::unique_ptr<NodeDelegateModel>()> creator = []() { return std::make_unique<ModelType>(); };
            //std::function<RegistryItemPtr()> creator = std::make_unique<ModelType>();
            registerModel<ModelType>(std::move(creator), category);
        }

        std::unique_ptr<NodeDelegateModel> create(QString const &modelName);

        std::unordered_map<QString, RegistryItemCreator> const &registeredModelCreators() const;

        std::unordered_map<QString, QString> const &registeredModelsCategoryAssociation() const;

        CategoriesSet const &categories() const;

#if 0
  TypeConverter
  getTypeConverter(NodeDataType const& d1,
                   NodeDataType const& d2) const;
#endif

    private:
        std::unordered_map<QString, QString> _registeredModelsCategory;

        std::set<QString> _categories;

        std::unordered_map<QString, std::function<std::unique_ptr<NodeDelegateModel>()>> _registeredItemCreators;

#if 0
  RegisteredTypeConvertersMap _registeredTypeConverters;
#endif

    private:
        //如果注册的ModelType类具有静态成员方法`static QString Name();`，使用它。否则，使用非static方法:`虚拟QString名称()const;`
        template<typename T, typename = void>
        struct HasStaticMethodName : std::false_type
        {
        };

        template<typename T>
        struct HasStaticMethodName<T, typename std::enable_if<std::is_same<decltype(T::Name()), QString>::value>::type> : std::true_type
        {
        };

        template<typename ModelType>
        static QString computeName(std::true_type, RegistryItemCreator const &)
        {
            return ModelType::Name();
        }

        template<typename ModelType>
        static QString computeName(std::false_type, RegistryItemCreator const &creator)
        {
            return creator()->name();
        }

        template<typename T>
        struct UnwrapUniquePtr
        {
            // Assert always fires, but the compiler doesn't know this:
            static_assert(!std::is_same<T, T>::value, "The ModelCreator must return a std::unique_ptr<T>, where T "
                                                      "inherits from NodeDelegateModel");
        };

        template<typename T>
        struct UnwrapUniquePtr<std::unique_ptr<T>>
        {
            static_assert(std::is_base_of<NodeDelegateModel, T>::value, "The ModelCreator must return a std::unique_ptr<T>, where T "
                                                                        "inherits from NodeDelegateModel");
            using type = T;
        };

        template<typename CreatorResult>
        using compute_model_type_t = typename UnwrapUniquePtr<CreatorResult>::type;
    };

} // namespace QtNodes
