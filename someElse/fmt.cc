#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/core.h>
#include <iostream>
#include <iomanip>
#include <vector>

struct Point 
{
    int x, y;
};

template <>
struct fmt::formatter<Point>
{
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Point &p, FormatContext &ctx) const
    {
        return fmt::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

template <typename T>
constexpr T pi = 3.141592653;

template <>
constexpr auto pi<const char *> = "pi";

int main()
{
    std::string result = fmt::format("hello, {}", "world");
    fmt::print("{}\n", result);

    // 多参数
    fmt::print(fmt::format("x={}, y={}\n", 10, 20));

    // 指定参数顺序
    fmt::print(fmt::format("x={1}, y={0}\n", 10, 20));

    // 数值的格式化
    fmt::print("{:05d}\n", pi<int>);
    fmt::print("{:<10}\n", pi<int>);
    fmt::print("{:>10}\n", pi<int>);
    fmt::print("{:.2f}\n", pi<float>);
    fmt::print("{}\n", pi<const char *>);
    std::cout << std::fixed << std::setprecision(2) << pi<double> << '\n';

    fmt::print("{}\n", Point{2, 5});

    std::vector<int> nums = {1, 2, 3, 4, 5};
    fmt::print("{}\n", nums);
}