#pragma once

//��N���X
class Game;

namespace ECS
{
	class System
	{
		friend Game;

	private:
		virtual void Pause(void) {};
		virtual void Update(void) {};
		virtual void Always(void) {};
	};
}