#pragma once
#include "CommandDispatcher.h"
#include "RelativeVelocityCommand.h"
#include <sol/sol.hpp>

#define NAMEOF(variable) #variable

namespace Potator
{
	enum class Axis
	{
		X,
		Y,
		Z
	};

	class MovementApi
	{
	public:
		MovementApi(CommandDispatcher& commandDispatcher, ComponentStorage<VelocityComponent>& movements, ComponentStorage<TransformComponent>& transforms, sol::state& lua) :
			_commandDispatcher { commandDispatcher },
			_movements { movements },
			_transforms { transforms }
		{
			lua.new_enum("Axis",
				"X", Potator::Axis::X,
				"Y", Potator::Axis::Y,
				"Z", Potator::Axis::Z
			);

			lua.set_function(NAMEOF(SetAngularVelocity), [this](Potator::Entity entity, float radiansPerSecond, Potator::Axis axis)
				{
					this->SetAngularVelocity(entity, radiansPerSecond, axis);
				});

			lua.set_function(NAMEOF(SetLinearVelocity), [this](Potator::Entity entity, float unitsPerSecond, Potator::Axis axis)
				{
					this->SetLinearVelocity(entity, unitsPerSecond, axis);
				});
		}

		void SetAngularVelocity(Entity entity, float radiansPerSecond, Axis arround)
		{
			std::shared_ptr<RelativeVelocityCommand> command = std::make_shared<RelativeVelocityCommand>(_movements, _transforms);
			switch (arround)
			{
			case Axis::X:
				command->AngularVelocity.x() = radiansPerSecond;
				break;
			case Axis::Y:
				command->AngularVelocity.y() = radiansPerSecond;
				break;
			case Axis::Z:
				command->AngularVelocity.z() = radiansPerSecond;
				break;
			}
			
			_commandDispatcher.Enqueue(entity, command);
		}

		void SetLinearVelocity(Entity entity, float unitsPerSecond, Axis arround)
		{
			std::shared_ptr<RelativeVelocityCommand> command = std::make_shared<RelativeVelocityCommand>(_movements, _transforms);
			switch (arround)
			{
			case Axis::X:
				command->LinearVelocity.x() = unitsPerSecond;
				break;
			case Axis::Y:
				command->LinearVelocity.y() = unitsPerSecond;
				break;
			case Axis::Z:
				command->LinearVelocity.z() = unitsPerSecond;
				break;
			}

			_commandDispatcher.Enqueue(entity, command);
		}

	private:
		CommandDispatcher& _commandDispatcher;
		ComponentStorage<VelocityComponent>& _movements;
		ComponentStorage<TransformComponent>& _transforms;
	};
}