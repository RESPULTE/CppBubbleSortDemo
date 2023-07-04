#include "Component.hpp"

template <typename T>
void ComponentArray<T>::attachComponent(Entity entity, T component)
{
    assert(mEntityToComponentIndex.count(entity) == 0 && "Component to add already existed on Entity");
    size_t index = mSize;

    mEntityToComponentIndex[entity] = index;
    mComponentIndexToEntity[index] = entity;

    mComponentArray[index] = component;

    ++mSize;
}

template <typename T>
void ComponentArray<T>::detachComponent(Entity entity)
{
    assert(mEntityToComponentIndex.count(entity) > 0 && "Component to remove does not exist on Entity.");

    // getting the index that corrresponds to the component's index of the deleted entity
    size_t deletedEntityComponentIndex = mEntityToComponentIndex[entity];

    size_t lastComponentIndex = mSize - 1;
    // Copy element at end into deleted element's place to maintain density
    mComponentArray[deletedEntityComponentIndex] = mComponentArray[lastComponentIndex];

    // resetting the mapping to the deleted entity's spot
    Entity lastComponentEntity = mComponentIndexToEntity[lastComponentIndex];
    mEntityToComponentIndex[lastComponentEntity] = deletedEntityComponentIndex;
    mComponentIndexToEntity[deletedEntityComponentIndex] = lastComponentEntity;

    mEntityToComponentIndex.erase(entity);
    mComponentIndexToEntity.erase(lastComponentIndex);

    --mSize;
}

template <typename T>
T &ComponentArray<T>::getComponent(Entity entity)
{
    assert(mEntityToComponentIndex.count(entity) > 0 && "Component to get does not exist on entity");
    return mComponentArray[mEntityToComponentIndex[entity]];
}

template <typename T>
void ComponentArray<T>::handleDestroyedEntity(Entity entity)
{
    if (mEntityToComponentIndex.count(entity) > 0)
    {
        detachComponent(entity);
    }
}

// registering a new type of component into the ECS system
// must be invoked to validate a Component type
template <typename T>
void ComponentManager::registerComponentType()
{
    ComponentTypeID name = typeid(T);

    assert(mComponentIDtoBitPosition.count(name) == 0 && "Component type already exist");

    mComponentIDtoBitPosition.insert({name, mNextComponentTypeBitPosition});
    mComponentIDtoArrayMap.insert({name, std::make_shared<ComponentArray<T>>()});

    ++mNextComponentTypeBitPosition;
}

template <typename T>
ComponentTypeBitPosition ComponentManager::GetComponentType()
{
    ComponentTypeID name = typeid(T);

    assert(mComponentIDtoBitPosition.count(name) > 0 && "Component not registered before use.");

    // Return this component's type - used for creating signatures
    return mComponentIDtoBitPosition[name];
}

template <typename T>
void ComponentManager::AttachComponent(Entity entity, T component)
{
    GetComponentArray<T>()->attachComponent(entity, component);
}

template <typename T>
void ComponentManager::DetachComponent(Entity entity)
{
    GetComponentArray<T>()->detachComponent(entity);
}

// a common interface to propagate changes to each ComponentArray when handling entity destruction event
void ComponentManager::handleDestroyedEntity(Entity entity)
{
    // Notify each component array that an entity has been destroyed
    // If it has a component for that entity, it will remove it
    for (auto const &pair : mComponentIDtoArrayMap)
    {
        auto const &component_array = pair.second;

        component_array->handleDestroyedEntity(entity);
    }
}

template <typename T>
Signature ComponentManager::getSignature()
{
    ComponentTypeID name = typeid(T);

    assert(mComponentIDtoBitPosition.count(name) > 0 && "Component not registered before use.");

    // Get this entity's signature from the array
    Signature signature;
    signature.set(mComponentIDtoBitPosition[name]);
    return signature;
}
