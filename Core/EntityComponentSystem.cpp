#include "EntityComponentSystem.hpp"

namespace LL::Core {
    template<typename T, typename... Args>
    void Entity::AddComponent(Args &&... args) {
        mComponents.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template<typename T>
    T *Entity::GetComponent() const {
        for (const auto& component : mComponents) {
            if (auto ptr = dynamic_cast<T*>(component.get())) {
                return ptr;
            }
        }
        return nullptr;
    }

    Entity *EntityManager::CreateEntity() {
        auto entityId = mNextEntityId++;
        mEntities.emplace_back(std::make_unique<Entity>(entityId));
        return mEntities.back().get();
    }

    void EntityManager::RemoveEntity(Entity *entity) {
        auto it = std::find_if(mEntities.begin(), mEntities.end(),
                [entity](const auto& e) { return e.get() == entity; }
                );
        if (it != mEntities.end())
            mEntities.erase(it);
    }

    template<typename T, typename... Args>
    T *ComponentManager::CreateComponent(Entity *entity, Args &&... args) {
        if (HasComponent<T>(entity)) {
            return GetComponent<T>(entity);
        }
        T* component = new T(std::forward<Args>(args)...);
        mComponents.at(entity).emplace(typeid(T), std::unique_ptr<Component>(component));
        return component;
    }

    template<typename T>
    void ComponentManager::RemoveComponent(Entity *entity) {
        auto it = mComponents.find(entity);
        if (it != mComponents.end()) {
            it->second.erase(typeid(T));
        }
    }

    template<typename T>
    T *ComponentManager::GetComponent(Entity *entity) const {
        auto it = mComponents.find(entity);
        if (it != mComponents.end()) {
            auto componentIt = it->second.find(typeid(T));
            if (componentIt != it->second.end()) {
                return dynamic_cast<T*>(componentIt->second.get());
            }
        }
        return nullptr;
    }

    template<typename T>
    bool ComponentManager::HasComponent(Entity *entity) const {
        auto it = mComponents.find(entity);
        if (it != mComponents.end()) {
            auto componentIt = it->second.find(typeid(T));
            return (componentIt != it->second.end());
        }
        return false;
    }

    template<typename T>
    std::vector<Entity *> EntityManager::GetEntitiesWithComponent() const {
        std::vector<Entity*> entitiesWithComponent;
        for (const auto& entity : mEntities) {
            if (entity->GetComponent<T>() != nullptr) {
                entitiesWithComponent.push_back(entity.get());
            }
        }
        return entitiesWithComponent;
    }

    template<typename T, typename... Args>
    T *SystemManager::CreateSystem(Args &&... args) {
        if (HasSystem<T>()) {
            return GetSystem<T>();
        }
        T* system = new T(std::forward<Args>(args)...);
        mSystems[&typeid(T)] = std::unique_ptr<System>(system);
        return system;
    }

    template<typename T>
    void SystemManager::RemoveSystem() {
        mSystems.erase(&typeid(T));
    }

    template<typename T>
    T *SystemManager::GetSystem() const {
        auto it = mSystems.find(&typeid(T));
        if (it != mSystems.end()) {
            return dynamic_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    bool SystemManager::HasSystem() const {
        return (mSystems.find(&typeid(T)) != mSystems.end());
    }

    void SystemManager::UpdateSystems(float dt) {
        for (const auto& [type, system] : mSystems) {
            system->Update(dt);
        }
    }
}