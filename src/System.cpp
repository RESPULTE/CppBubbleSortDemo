#include "System.hpp"

template <typename T>
std::shared_ptr<T> SystemManager::registerSystem(Signature signature)
{
    SystemTypeID name = typeid(T);
    assert(mSystems.count(name) == 0 && "System to register already exists");

    auto system = std::make_shared<T>();
    mSystems.insert({name, system});
    mSignatures.insert({name, signature});

    return system;
}

template <typename T>
void SystemManager::setSignature(Signature signature)
{
    SystemTypeID name = typeid(T);

    assert(mSystems.count(name) > 0 && "System used before registered.");

    mSignatures.insert({name, signature});
}

// a common interface to propagate changes to each ComponentArray when handling entity destruction event
void SystemManager::handleDestroyedEntity(Entity entity)
{
    // Erase a destroyed entity from all system lists
    // mEntities is a set so no check needed since no error will be thrown
    for (auto const &pair : mSystems)
    {
        auto const &system = pair.second;

        system->mEntities.erase(entity);
    }
}

// add or remove entity from each system's set based on the entity and system's signature
void SystemManager::handleEntitySignatureChanged(Entity entity, Signature entity_signature)
{
    // Notify each system that an entity's signature changed
    for (auto const &pair : mSystems)
    {
        auto const &name = pair.first;
        auto const &system = pair.second;
        auto const &system_signature = mSignatures[name];

        // Entity signature matches system signature - insert into set
        if ((entity_signature & system_signature) == system_signature)
        {
            system->mEntities.insert(entity);
            continue;
        }
        system->mEntities.erase(entity);
    }
}
