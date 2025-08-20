#pragma once
#include "Entity.h"
#include <unordered_map>
#include <boost/signals2.hpp>
#include <cassert>

namespace Potator
{
	constexpr size_t INVALID_IDX = ~0u;

	template<class T>
	class ComponentStorage
	{
	public:
		bool HasComponent(Entity entity) const;
		void Store(Entity entity, T& component);
		void Drop(Entity entity);
		std::vector<T>& GetComponents() { return _components; }
		T& operator[](Entity entity);
		boost::signals2::signal<void(Entity, const T&)> ComponentAdded;
		boost::signals2::signal<void(Entity)> ComponentRemoved;
	private:
		std::vector<size_t> _sparseMap;
		std::vector<Entity> _entities;
		std::vector<T> _components;
	};

	template<class T>
	inline bool ComponentStorage<T>::HasComponent(Entity entity) const
	{
		return entity != NONE_ENTITY
			&& entity < _sparseMap.size()
			&& _sparseMap[entity] != INVALID_IDX;
	}

	template<class T>
	inline void ComponentStorage<T>::Store(Entity entity, T& component)
	{
		assert(entity != NONE_ENTITY && "Tried to store component for NONE_ENTITY");
		if (entity >= _sparseMap.size())
		{
			_sparseMap.resize(entity + 1, INVALID_IDX);
		}

		size_t idx = _components.size();
		_sparseMap[entity] = idx;
		_entities.push_back(entity);
		_components.push_back(std::move(component));

		ComponentAdded(entity, _components[idx]);
	}

	template<class T>
	inline void ComponentStorage<T>::Drop(Entity droppedEntity)
	{
		if (HasComponent(droppedEntity))
		{
			return;
		}

		size_t idx = _sparseMap[droppedEntity];
		size_t lastIdx = _components.size() - 1;
		Entity movedEntity = _entities[lastIdx];

		_components[idx] = std::move(_components[lastIdx]);
		_entities[idx] = movedEntity;
		_sparseMap[movedEntity] = idx;
		_sparseMap[droppedEntity] = INVALID_IDX;

		_entities.pop_back();
		_components.pop_back();

		ComponentRemoved(droppedEntity);
	}

	template<class T>
	inline T& ComponentStorage<T>::operator[](Entity entity)
	{
		return _components[_sparseMap[entity]];
	}
}

