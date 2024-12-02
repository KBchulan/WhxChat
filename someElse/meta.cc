#include <tuple>
#include <vector>
#include <variant>
#include <utility>
#include <optional>
#include <iostream>
#include <type_traits>

template <typename T>
concept TupleLike = requires(const T &t)
{
    std::get<int>(t);
};

template <typename T>
concept Varient = requires(const T &t)
{
    std::visit([](auto &arg){}, t);
};

template <TupleLike T>
decltype(auto) PrintInfo(T &&t)
{
    std::cout << std::get<1>(t) << '\n'; 
}

template <typename T>
void CheckType(T &&t)
{
    if constexpr (std::is_const_v<std::remove_reference_t<T>> && 
                  std::is_integral_v<std::remove_reference_t<T>>)
    {
        std::cout << "const整数类型\n";
    }
    else
    {
        std::cout << "no const整数类型\n";
    }
}

template <typename T>
class TypeInfo
{
    static constexpr bool is_pointer = std::is_pointer_v<T>;
    static constexpr bool is_const = std::is_const_v<T>;
    static constexpr bool is_class = std::is_class_v<T>;

public:
    static void print_info() 
    {
        std::cout << "类型信息:\n";
        std::cout << "是指针" << is_pointer << "\n";
        std::cout << "是const" << is_const << "\n";
        std::cout << "是类" << is_class << "\n";
    }
};

int main()
{
    std::tuple<int, std::string, double> myTuple = std::make_tuple<int, std::string, double>(2, "tuple", 2.2);
    PrintInfo(myTuple);

    int num = 10;
    CheckType(num);
    CheckType(std::as_const(num));

    TypeInfo<const int *>::print_info();
    return 0;
}