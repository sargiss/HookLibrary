#include <functional>

#pragma once
template <class T>
class CLazy
{
	typedef std::function<T*()> type_constructor;
private:
	T* m_data;
	type_constructor m_constructor;
public:
	CLazy(type_constructor constructor)
	{
		m_constructor = constructor;
	}
	~CLazy(void)
	{
		delete m_data;
	}
	T* get()
	{
		if (!m_data)
			m_data = m_constructor();
		return m_data;
	}
};

