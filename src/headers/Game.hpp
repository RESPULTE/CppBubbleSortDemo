#pragma once

#include "Entity.hpp"
#include "Component.hpp"
#include "System.hpp"

/** top level class to interface with the entire ECS system */
class Game
{
public:
    Game();

    // initializer for creaing all the managerial level classes in ECS ecosystem
    void init();

    // returns an unsigned integer that repreents an entity
    Entity CreateEntity();

    // a high level method that invokes all the corresponding updating methods when an entity is destroyed
    void DestroyEntity(Entity entity);

    // register a new type of component into the ECS ecosystem
    template <typename T>
    void RegisterComponent();

    template <typename T>
    void AttachComponent(Entity entity, T component);

    template <typename T>
    void DetachComponent(Entity entity);

    template <typename T>
    T &GetComponent(Entity entity);

    template <typename T>
    ComponentTypeBitPosition GetComponentType();

    // register a new type of system into the ECS ecosystem
    template <typename T>
    std::shared_ptr<T> RegisterSystem();

    // set the signature that represents the type of component that will be processed by it
    template <typename T>
    void SetSystemSignature(Signature signature);

private:
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<SystemManager> mSystemManager;
};