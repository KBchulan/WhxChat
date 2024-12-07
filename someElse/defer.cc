#include <chrono>
#include <thread>
#include <iostream>
#include <functional>

class Defer
{
public:
    Defer(std::function<void()> func): _func(func) {}
    ~Defer() { _func(); }
    
private:
    std::function<void()> _func;
};

int main()
{
    auto end = std::chrono::steady_clock::now() + std::chrono::seconds(10);

    while (std::chrono::steady_clock::now() < end) 
    {
        auto *a = new int(random());
        
        Defer guard([&]
        {
            delete a;
        });

        if(*a > 100)
            continue;
        
        std::cout << *a << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "10秒已到,循环结束" << std::endl;
}