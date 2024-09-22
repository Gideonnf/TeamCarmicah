#ifndef SINGLETON_TEMPLATE_H
#define SINGLETON_TEMPLATE_H

template <typename T>
class Singleton
{
public:
	static T* GetInstance()
	{
		static T instance;


		return &instance;
	}

protected:
	Singleton() {};
	virtual ~Singleton() {};
};




#endif // SINGLETON_TEMPLATE_H
