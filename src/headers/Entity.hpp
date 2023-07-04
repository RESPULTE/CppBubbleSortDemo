#pragma once

#include <queue>
#include <cstdint>
#include <cassert>
#include <bitset>

#include "Component.hpp"

// using an alias since entity in ECS is essentially an ID, plus it makes it more expressive
using Entity = std::uint32_t;

// The max allowed entity to exist in the ecosystem at a time
const Entity MAX_ENTITIES = 5000;

// This represents the type of comppnent that is "attached" to an entity
using Signature = std::bitset<MAX_COMPONENTS>;

/**This is meant to be an interface for all action related to the entity class.
 * - It only stores the available entities
 * - 'living' entity will be "given" out
 * - 'dead' entity will be "taken" back in and stored as available entities
 */
class EntityManager
{
public:
    // generates all the available entities at instantiation
    EntityManager();

    // retrieve and returns the first available entity from the front of the queue
    Entity createEntity();

    // add entity back into the list of available entities
    void destroyEntity(Entity entity);

    // attach a signature to the entity
    void SetSignature(Entity entity, Signature signature);

    Signature GetSignature(Entity entity);

private:
    std::queue<Entity> mAvailableEntities{};

    std::array<Signature, MAX_ENTITIES> mSignatures{};

    std::uint32_t mNumLivingEntity{};
};
