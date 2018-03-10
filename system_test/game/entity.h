#pragma once

//フレームワーク
#include <game\component.h>

//標準
#include <vector>
#include <unordered_map>
#include <string>
#include <typeinfo>

namespace ECS
{
	class Entity
	{
		friend class Game;

	private:
		std::unordered_map<std::string, Component*> components_;
		std::vector<Entity*> children_;

	private:
		template<class _Component, class ... Args>
		_Component * const Add(Args ... args) const
		{
			auto name = typeid(_Component).name();
			auto & component = new _Component(this, args ...);
			this->components_[name] = component;
			return component;
		}
		template<class _Child, class ... Args>
		_Child * const Create(Args ... args) const
		{
			auto & child = new _Child(this, args ...);
			this->children_.emplace_back(child);
			return child;
		}

	private:
		bool destroy_ = false;

	public:
		void Destroy(void)
		{
			this->destroy_ = true;
		}

	private:
		virtual void Update(void) {}
		virtual void Pause(void) {}
		virtual void Always(void) {}

	private:
		void _Update(void)
		{
			this->Update();
			for (auto itr = this->components_.begin(); itr != this->components_.end();)
			{
				auto & component = (*itr).second;
				if (component)
				{
					component->Update();
					++itr;
				}
				else
				{
					itr = this->components_.erase(itr);
				}
			}
			for (int n = 0; n < this->children_.size();)
			{
				auto & child = this->children_[n];
				if (child->destroy_)
				{
					this->children_.erase(this->children_.begin() + n);
				}
				else
				{
					child->_Update();
					++n;
				}
			}
		}
		void _Pause(void)
		{
			this->Pause();
			for (auto itr = this->components_.begin(); itr != this->components_.end();)
			{
				auto & component = (*itr).second;
				if (component)
				{
					component->Pause();
					++itr;
				}
				else
				{
					itr = this->components_.erase(itr);
				}
			}
			for (int n = 0; n < this->children_.size();)
			{
				auto & child = this->children_[n];
				if (child->destroy_)
				{
					this->children_.erase(this->children_.begin() + n);
				}
				else
				{
					child->_Pause();
					++n;
				}
			}
		}
		void _Always(void)
		{
			this->Always();
			for (auto itr = this->components_.begin(); itr != this->components_.end();)
			{
				auto & component = (*itr).second;
				if (component)
				{
					component->Always();
					++itr;
				}
				else
				{
					itr = this->components_.erase(itr);
				}
			}
			for (int n = 0; n < this->children_.size();)
			{
				auto & child = this->children_[n];
				if (child->destroy_)
				{
					this->children_.erase(this->children_.begin() + n);
				}
				else
				{
					child->_Always();
					++n;
				}
			}
		}
	};
}