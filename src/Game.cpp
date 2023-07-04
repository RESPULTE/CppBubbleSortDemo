#pragma once

#include "Game.hpp"

Game::Game()
{
    init();
}

void Game::init()
{
    mComponentManager = std::make_unique<ComponentManager>();
    mEntityManager = std::make_unique<EntityManager>();
    mSystemManager = std::make_unique<SystemManager>();
}

Entity Game::CreateEntity()
{
    return mEntityManager->createEntity();
}

void Game::DestroyEntity(Entity entity)
{
    mEntityManager->destroyEntity(entity);
    mComponentManager->handleDestroyedEntity(entity);
    mSystemManager->handleDestroyedEntity(entity);
}

template <typename T>
void Game::RegisterComponent()
{
    mComponentManager->RegisterComponent<T>();
}

template <typename T>
void Game::AttachComponent(Entity entity, T component)
{
    mComponentManager->AttachComponent<T>(entity, component);

    auto signature = mEntityManager->GetSignature(entity);
    signature.set(mComponentManager->GetComponentType<T>(), true);
    mEntityManager->SetSignature(entity, signature);

    mSystemManager->handleEntitySignatureChanged(entity, signature);
}

template <typename T>
void Game::DetachComponent(Entity entity)
{
    mComponentManager->DetachComponent<T>(entity);

    auto signature = mEntityManager->GetSignature(entity);
    signature.set(mComponentManager->GetComponentType<T>(), false);
    mEntityManager->SetSignature(entity, signature);

    mSystemManager->handleEntitySignatureChanged(entity, signature);
}

template <typename T>
T &Game::GetComponent(Entity entity)
{
    return mComponentManager->GetComponent<T>(entity);
}

template <typename T>
ComponentTypeBitPosition Game::GetComponentType()
{
    return mComponentManager->GetComponentType<T>();
}

template <typename T>
std::shared_ptr<T> Game::RegisterSystem()
{
    return mSystemManager->RegisterSystem<T>();
}

template <typename T>
void Game::SetSystemSignature(Signature signature)
{
    mSystemManager->setSignature<T>(signature);
}
