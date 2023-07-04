#pragma once

#include <set>
#include <memory>
#include <cassert>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "Entity.hpp"
#include "Component.hpp"

// alias for clarity
using SystemTypeID = std::type_index;

/** a virtual base class for all system */
class System
{
public:
    // a set of entity that is eligible to be processed with said system
    std::set<Entity> mEntities;

    virtual void update() = 0;
};

/** Managerial level class to handle all things related to system class */
class SystemManager
{
public:
    // registering a new type of system into the ECS system
    // must be invoked to validate a system type
    template <typename T>
    std::shared_ptr<T> registerSystem(Signature signature);

    template <typename T>
    void setSignature(Signature signature);

    // a common interface to propagate changes to each ComponentArray when handling entity destruction event
    void handleDestroyedEntity(Entity entity);

    // add or remove entity from each system's set based on the entity and system's signature
    void handleEntitySignatureChanged(Entity entity, Signature entity_signature);

private:
    // Map system type's ID (std::type_index) to its corresponding signature
    std::unordered_map<SystemTypeID, Signature> mSignatures{};

    // Map system type's ID (std::type_index) to pointers to system instances
    std::unordered_map<SystemTypeID, std::shared_ptr<System>> mSystems{};
};