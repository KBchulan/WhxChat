#include <tuple>
#include <iostream>
#include <functional>

template <typename ReturnType, typename... Args>
class RAII
{
public:
    explicit RAII(std::function<ReturnType(Args...)> func) : _func(func) {}
    ~RAII() { std::apply(_func, _args); }

    void SetArgs(Args... args)
    {
        _args = std::make_tuple(std::forward<Args>(args)...);
    }

private:
    std::tuple<Args...> _args;
    std::function<ReturnType(Args...)> _func;
};

int main()
{
    RAII<void, int, double> guard(
        [](int a, double b)
        {
            std::cout << a + b << std::endl;
        }
    );
    guard.SetArgs(2, 3.3);
    std::cout << "---------------\n";
}