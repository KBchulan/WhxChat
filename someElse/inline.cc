#include <iostream>

struct Add
{
    void operator()(int a, int b)
    {
        std::cout << a + b << '\n';
    }
};

template <typename Type>
struct Less
{
    bool operator()(Type &&left, Type &&right)
    {
        return left < right;
    }
};

__attribute__((always_inline)) void add(int a, int b)
{
    std::cout << a + b << '\n';
}

int main()
{
    Add a;
    a(3, 5);

    add(3, 5);

    Less<int> lessInt;
    if(lessInt(2, 5))
    {
        std::cout << "litter\n";
    }
}
