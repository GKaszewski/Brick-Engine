#include "EntityManager.hpp"

EntityManager* EntityManager::instance = 0;

EntityManager* EntityManager::getInstance()
{
    if (instance == 0) {
        instance = new EntityManager();
    }

    return instance;
}

void EntityManager::destroyBodies() {
    auto it = physicsBodiesToRemove.begin();
    auto end = physicsBodiesToRemove.end();
    for (; it != end; ++it) {
        auto entity = *it;
        entity->Body().GetWorld()->DestroyBody(&entity->Body());
        auto it = std::find(physicsEntities.begin(), physicsEntities.end(), entity);
        if (it != physicsEntities.end()) {
            delete entity;
            physicsEntities.erase(it);
        }
    }
    physicsBodiesToRemove.clear();
}

EntityManager::EntityManager() {

}

EntityManager::~EntityManager() {
    for (auto entity : physicsEntities) {
        delete entity;
    }
    physicsEntities.clear();
    physicsBodiesToRemove.clear();
    delete instance;
}
