#pragma once

namespace ECS
{
	class Component
	{
		friend class Entity;

	private:
		Component(Entity * parent);

	private:
		Entity * parent_ = nullptr;

	private:
		virtual void Pause(void) {};
		virtual void Update(void) {};
		virtual void Always(void) {};
	};
}