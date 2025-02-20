#pragma once

#include <vector>
#include <functional>

// Класс Delegate
template<typename... Args>
class Action 
{
public:
	using HandlerType = std::function<void(Args...)>; // Тип обработчика

	void Bind(HandlerType handler)
	{
		handlers.push_back(handler);
	}

	void Invoke(Args... args)
	{
		for (auto& handler : handlers)
		{
			handler(args...);
		}
	}

	// Метод для отписки от события
	void Unbind(HandlerType handler)
	{
		handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
			[&handler](const HandlerType& h) { return h.target_type() == handler.target_type(); }), handlers.end());
	}

private:
	std::vector<HandlerType> handlers{};
};