#pragma once

#include <bitset>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
protected:
  static int nextId;
};

template <typename T>
class Component: public IComponent {
  // Return a unique ID for Component<T>
  static int GetId() {
    static auto id = nextId++;
    return id;
  }
};

class Entity {
private:
  int id;

public:
  Entity(int id) : id(id) {};
  int GetId() const;

  bool operator ==(const Entity& other) const { return id == other.id; }
  bool operator <(const Entity& other) const { return id < other.id; }
};

class System {
private:
  Signature componentSignature;
  std::vector<Entity> entities;

public:
  System() = default;
  ~System() = default;

  void AddEntityToSystem(Entity entity);
  void RemoveEntityFromSystem(Entity entity);
  std::vector<Entity> GetSystemEntities() const;
  const Signature& GetComponentSignature() const;

  template <typename T> void RequireComponent();
};

class IPool {
public:
  virtual ~IPool() {}
};

template <typename T>
class Pool: public IPool {
private:
  std::vector<T> data;

public:
  Pool(int size = 100) {
    data.resize(size);
  }
  virtual ~Pool() = default;

  bool isEmpty() const {
    return data.empty();
  }

  int GetSize() const {
    return data.size();
  }

  void Resize(int size) {
    data.resize(size);
  }

  void Clear() {
    data.clear();
  }

  void Add(T object) {
    data.push_back(object);
  }

  void Set(int index, T object) {
    data[index] = object;
  }

  T& Get(int index) {
    return static_cast<T&>(data[index]);
  }

  T& operator [](unsigned int index) {
    return data[index];
  }
};

class Registry {
private:
  int numEntities = 0;
  std::set<Entity> entitiesToBeAdded;
  std::set<Entity> entitiesToBeDestroyed;
  std::vector<IPool*> componentPools;
  std::vector<Signature> entityComponentSignatures;
  std::unordered_map<std::type_index, System*> systems;

public:
  Registry() = default;
  ~Registry() = default;

  void Update();

  // Entities
  Entity CreateEntity();

  // Components
  template <typename T, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
  template <typename T> void RemoveComponent(Entity entity);
  template <typename T> bool HasComponent(Entity entity) const;

  // Systems
  template <typename T, typename ...TArgs> void AddSystem(TArgs&& ...args);
  template <typename T> void RemoveSystem();
  template <typename T> bool HasSystem() const;
  template <typename T> T& GetSystem() const;
  void AddEntityToSystems(Entity entity);
};

template <typename T>
void System::RequireComponent() {
  const auto componentId = Component<T>::GetId();
  componentSignature.set(componentId);
}

template<typename T, typename ...TArgs>
inline void Registry::AddComponent(Entity entity, TArgs && ...args) {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.GetId();

  // Increase the pool sizes if necessary
  if (componentId > componentPools.size()) {
    componentPools.resize(componentId + 1, nullptr);
  }

  // Create new pool if new
  if (!componentPools[componentId]) {
    Pool<T>* newComponentPool = new Pool<T>();
    componentPools[componentId] = newComponentPool;
  }

  // Fetch the proper pool and resize if necessary
  Pool<T>& componentPool = Pool<T>(componentPools[componentId]);

  if (entityId >= componentPool->GetSize()) {
    componentPool->Resize(numEntities);
  }

  // Create and set new component
  T newComponent(std::forward<TArgs>(args)...);

  componentPool->Set(entityId, newComponent);

  entityComponentSignatures[entityId].set(componentId);
}

template <typename T>
void Registry::RemoveComponent(Entity entity) {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.GetId();

  entityComponentSignatures[entityId].set(componentId, false);
}

template <typename T>
bool Registry::HasComponent(Entity entity) const {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.GetId();

  entityComponentSignatures[entityId].test(componentId);
}

template<typename T, typename ...TArgs>
inline void Registry::AddSystem(TArgs && ...args) {
  T* newSystem(new T(std::forward<TArgs>(args)...));
  systems.insert(std::make_pair(std::type_index(typeid(T)), newSystem));
}

template<typename T>
inline void Registry::RemoveSystem() {
  auto system = systems.find(std::type_index(typeid(T)));
  systems.erase(system);
}

template<typename T>
inline bool Registry::HasSystem() const {
  return systems.find(std::type_index(typeid(T))) != systems.end();
}

template<typename T>
inline T& Registry::GetSystem() const {
  auto system = systems.find(std::type_index(typeid(T)));
  return *(std::static_pointer_cast<T>(system->second));
}
