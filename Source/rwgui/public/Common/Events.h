#pragma once

#include "Containers.h"
#include <functional>

template<typename ...Args>
class MulticastEvent
{
public:
	void Bind(std::function<void(Args...)> inFunction)
	{
		Functions.Add(inFunction);
	}

	void Unbind(std::function<void(Args...)> inFunction)
	{
		Functions.Remove(inFunction);
	}

	void UnbindAll()
	{
		Functions = List<std::function<void(Args...)>>();
	}

	void Broadcast(Args... args)
	{
		for(size_t id = 0; id < Functions.Size();id++)
		{
			Functions[id](args...);
		}
	}

	void operator()(Args... args)
	{
		Broadcast(args...);
	}
private:
	List<std::function<void(Args...)>> Functions;
};