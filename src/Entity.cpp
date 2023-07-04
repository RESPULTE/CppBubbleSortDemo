#include "Entity.hpp"

EntityManager::EntityManager()
{
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
    {
        mAvailableEntities.push(entity);
    }
}

Entity EntityManager::createEntity()
{
    assert(mNumLivingEntity < MAX_ENTITIES && "Max Entity count exceeded");

    Entity entity = mAvailableEntities.front();
    mAvailableEntities.pop();
    ++mNumLivingEntity;

    return entity;
}

void EntityManager::destroyEntity(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");

    mSignatures[entity].reset();
    mAvailableEntities.push(entity);
}

void EntityManager::SetSignature(Entity entity, Signature signature)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");

    // Put this entity's signature into the array
    mSignatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");

    // Get this entity's signature from the array
    return mSignatures[entity];
}
