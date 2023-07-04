#pragma once

#include <bitset>
#include <cassert>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <memory>

#include "Entity.hpp"

// This represents the bit position in "Signature" that a given component type has been assigned to
using ComponentTypeBitPosition = std::uint8_t;
const ComponentTypeBitPosition MAX_COMPONENTS = 32;

// basically a wasy to store the components by its name
using ComponentTypeID = std::type_index;

/**A virtual base class for the sole purpose of using polymorphism
 * when storing component arrays and allow update
 * to be done accordingly when an entity is deleted */
class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void handleDestroyedEntity(Entity entity) = 0;
};

/** A packed array that maps all component of a given type to the entity that owns it
 * and allows for fast attachment and detachment of components
 * when entity is deleted or created*/
template <typename T>
class ComponentArray : public IComponentArray
{

public:
    void attachComponent(Entity entity, T component);

    void detachComponent(Entity entity);

    // returns the Component for the given entity if it exists, error will be thrown if not
    T &getComponent(Entity entity);

    // a common interface that can be invoked by managerial level classes
    void handleDestroyedEntity(Entity entity) override;

private:
    // the actual 'thing' that stores the Components
    std::array<T, MAX_ENTITIES> mComponentArray;

    // a to-and-fro reference between the entity and the component's index in the array
    std::unordered_map<Entity, size_t> mEntityToComponentIndex;

    // a to-and-fro reference between the component's index and the entity in the array
    std::unordered_map<size_t, Entity> mComponentIndexToEntity;

    size_t mSize;
};

/** Managerial level class that links Component and ComponentArray
 *  by handling all the action related to the component */
class ComponentManager
{
public:
    // registering a new type of component into the ECS system
    // must be invoked to validate a Component type
    template <typename T>
    void registerComponentType();

    template <typename T>
    ComponentTypeBitPosition GetComponentType();

    template <typename T>
    void AttachComponent(Entity entity, T component);

    template <typename T>
    void DetachComponent(Entity entity);

    // a common interface to propagate changes to each ComponentArray when handling entity destruction event
    void handleDestroyedEntity(Entity entity);

    template <typename T>
    Signature getSignature();

private:
    // maps component type's ID (std::type_index) to the Bit Position that it occupies in the Signature
    std::unordered_map<ComponentTypeID, ComponentTypeBitPosition> mComponentIDtoBitPosition;

    // maps component type's ID (std::type_index) to the corresponding ComponentArray of that type
    // uses a virtual base class to allow for polymorphism since ComponentArray can be of manu different type
    std::unordered_map<ComponentTypeID, std::shared_ptr<IComponentArray>> mComponentIDtoArrayMap{};

    // a counter variable to indicate the next available bit position for new component type
    ComponentTypeBitPosition mNextComponentTypeBitPosition{};

    // Convenience function to get the statically casted pointer to the ComponentArray of type T.
    template <typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray();
};