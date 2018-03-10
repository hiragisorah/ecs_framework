#pragma once

// �t���[�����[�N
#include <game\system.h>

// �W��
#include <vector>
#include <unordered_map>
#include <string>
#include <typeinfo>

class Game
{
private:
	static std::unordered_map<std::string, ECS::System*> systems_;

private:
	static bool running_;
	static bool pause_;

public:
	//@�V�X�e���̒ǉ�
	template<class _System, class ... Args>
	static _System * const AddSystem(Args ... args)
	{
		auto name = typeid(_System).name();
		auto system = new _System(args ...);
		Game::systems_[name] = system;
		return system;
	}
	//@�V�X�e���̎擾
	template<class _System>
	static _System * const GetSystem(void)
	{
		auto name = typeid(_System).name();
		return static_cast<_System*>(Game::systems_[name]);
	}

public:
	//@�V�X�e���̋N��
	static void Run(void)
	{
		while (Game::running_)
		{
			for (auto itr = Game::systems_.begin(); itr != Game::systems_.end();)
			{
				auto & system = (*itr).second;
				if (system)
				{
					(Game::pause_) ? system->Pause() : system->Update();

					system->Always();

					++itr;
				}
				else
				{
					itr = Game::systems_.erase(itr);
				}
			}
		}

		for (auto & system : Game::systems_)
			delete system.second;
	}
public:
	static void Shutdown(void)
	{
		Game::running_ = false;
	}
	static void Pause(void)
	{
		Game::pause_ = true;
	}
	static void Resume(void)
	{
		Game::pause_ = false;
	}
};