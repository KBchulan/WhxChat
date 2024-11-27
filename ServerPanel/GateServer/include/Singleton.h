#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>
#include <memory>
#include <iostream>

template <typename T>
class Singleton
{
public:
    ~Singleton()
    {
        std::cout << "Singleton destructor called \n";
    }

    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

    static std::shared_ptr<T> GetInstance()
    {
        static std::once_flag g_flag;
        std::call_once(g_flag, []()
        {
            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }

protected:
    Singleton() = default;

private:
    inline static std::shared_ptr<T> _instance = nullptr;

};

#endif // SINGLETON_H
