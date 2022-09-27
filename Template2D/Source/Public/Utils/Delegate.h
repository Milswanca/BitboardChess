#pragma once
#include <vector>
#include <typeinfo>

template <typename Ret, typename... Params>
class Callback
{
public:
	virtual Ret Invoke(Params... params) = 0;
};

template <typename Ret, typename... Params>
class StaticCallback : public Callback<Ret, Params...>
{
public:
	StaticCallback(Ret(*func)(Params...))
		: StaticMethod(func)
	{}

	virtual Ret Invoke(Params... params)
	{
		return (*StaticMethod)(params...);
	}

	Ret(*StaticMethod)(Params...);
};

template <typename T, typename Method, typename Ret, typename... Params>
class MethodCallback : public Callback<Ret, Params...>
{
public:
	void* Object;
	Method InMethod;
public:

	MethodCallback(void *object, Method method) : Object(object), InMethod(method)
	{}

	virtual Ret Invoke(Params... params)
	{
		T* obj = static_cast<T*>(Object);
		return (obj->*InMethod)(params...);
	}
};

template <typename Ret, typename... Params>
class Delegate
{
public:

	Delegate()
	{

	}

	~Delegate()
	{
		RemoveAll();
	}

	template<typename T, typename Method>
	void Add(T* object, Method method)
	{
		MethodCallback<T, Method, Ret, Params...>* newMethod = new MethodCallback<T, Method, Ret, Params...>(object, method);
		callbacks.push_back(newMethod);
	}

	void Add(Ret(*func)(Params...))
	{
		StaticCallback<Ret, Params...>* newMethod = new StaticCallback<Ret, Params...>(func);
		callbacks.push_back(newMethod);
	}

	template<typename T, typename Method>
	void Remove(T* object, Method method)
	{
		int foundIndex = 0;
		MethodCallback<T, Method, Ret, Params...>* callback = FindBoundMethod(object, method, foundIndex);
		if (callback)
		{
			callbacks.erase(callbacks.begin() + foundIndex);
			delete callback;
		}
	}

	void Remove(void(*func)(Params...))
	{
		int foundIndex = 0;
		StaticCallback<Ret, Params...>* callback = FindBoundStaticMethod(func, foundIndex);
		if (callback)
		{
			callbacks.erase(callbacks.begin() + foundIndex);
			delete callback;
		}
	}

	void RemoveAll()
	{
		for (unsigned int i = 0; i < callbacks.size();)
		{
			Callback<Ret, Params...>* callback = (*callbacks.begin());
			callbacks.erase(callbacks.begin());
			delete callback;
		}
	}

	//We need a void type specialization function to handle the invoke if a function is void return
	//This is because the compiler will throw an error because void is not returnable
	template<typename = typename enable_if <!is_void<Ret>::value>::type>
	Ret Invoke(Params... param)
	{
		Ret retVal = NULL;

		for (Callback<Ret, Params...>* i : callbacks)
		{
			retVal = i->Invoke(param...);
		}

		return retVal;
	}

	void Invoke(Params... param)
	{
		for (Callback<Ret, Params...>* i : callbacks)
		{
			i->Invoke(param...);
		}
	}

	template<typename T, typename Method>
	bool IsAlreadyBound(T* object, Method method)
	{
		int foundIndex = 0;
		return FindBoundMethod(object, method, foundIndex) != nullptr;
	}

	bool IsAlreadyBound(Ret(*func)(Params...))
	{
		int foundIndex = 0;
		return FindBoundStaticMethod(func, foundIndex) != nullptr;
	}

	template<typename T, typename Method>
	MethodCallback<T, Method, Ret, Params...>* FindBoundMethod(T* object, Method method, int& foundIndex)
	{
		foundIndex = -1;
		for (unsigned int i = 0; i < callbacks.size(); i++)
		{
			MethodCallback<T, Method, Ret, Params...>* callback = static_cast<MethodCallback<T, Method, Ret, Params...>*>(callbacks[i]);
			if (callback->Object == object && callback->InMethod == method)
			{
				foundIndex = i;
				return callback;
			}
		}
		return false;
	}

	StaticCallback<Ret, Params...>* FindBoundStaticMethod(Ret(*func)(Params...), int& foundIndex)
	{
		foundIndex = -1;
		for (unsigned int i = 0; i < callbacks.size(); i++)
		{
			StaticCallback<Ret, Params...>* callback = static_cast<StaticCallback<Ret, Params...>*>(callbacks[i]);
			if (callback->StaticMethod == func)
			{
				foundIndex = i;
				return callback;
			}
		}
		return false;
	}

protected:
	std::vector<Callback<Ret, Params...>*> callbacks;
};