#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class CSingleton
{
public:

	CSingleton(const CSingleton<T>&) = delete;
	const CSingleton<T>& operator=(const CSingleton<T>) = delete;

	static T* getInstance()
	{
		if (!m_instance)
		{
			m_instance = new T;
		}
		return m_instance;
	}

protected:
	CSingleton() {}
	virtual ~CSingleton() { delete m_instance; }

	static T* m_instance;
};

template <typename T>
T* CSingleton<T>::m_instance = nullptr;

#endif /*SINGLETON_H*/