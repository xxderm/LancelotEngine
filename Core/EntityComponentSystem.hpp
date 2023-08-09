#pragma once
#include "pch.hpp"
#include "../impexp.hpp"

namespace LL::Core {
    class LL_CALL System {
    public:
        virtual ~System() = 0;
        // TODO: Switch float to Timespan
        virtual void Update(float dt) = 0;
    };

    class LL_CALL Component {
    public:
        virtual ~Component() = 0;
    };

    class LL_CALL Entity final {
    public:
        Entity(uint32_t id) : mId(id) {}
        template<typename T, typename... Args>
        void AddComponent(Args&&... args);
        template<typename T>
        T* GetComponent() const;
    private:
        uint32_t mId{};
        std::vector<std::shared_ptr<Component>> mComponents;
    };

    class LL_CALL EntityManager final {
    public:
        EntityManager() : mNextEntityId(0) {}
        Entity* CreateEntity();
        void RemoveEntity(Entity* entity);
        template<typename T>
        std::vector<Entity*> GetEntitiesWithComponent() const;
    private:
        uint32_t mNextEntityId{};
        std::vector<std::shared_ptr<Entity>> mEntities;
    };

    class LL_CALL ComponentManager final {
    public:
        template<typename T, typename... Args>
        T* CreateComponent(Entity* entity, Args&&... args);
        template<typename T>
        void RemoveComponent(Entity* entity);
        template<typename T>
        T* GetComponent(Entity* entity) const;
        template<typename T>
        bool HasComponent(Entity* entity) const;
    private:
        std::unordered_map<Entity*, std::unordered_map<std::type_index, std::shared_ptr<Component>>> mComponents;
    };

    class LL_CALL SystemManager final {
    public:
        template<typename T, typename... Args>
        T* CreateSystem(Args&&... args);
        template<typename T>
        void RemoveSystem();
        template<typename T>
        T* GetSystem() const;
        template<typename T>
        bool HasSystem() const;
        // TODO: Switch float to Timespan
        void UpdateSystems(float dt);
    private:
        std::unordered_map<const std::type_info*, System*> mSystems;
    };
}