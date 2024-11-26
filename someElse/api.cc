#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
    std::vector<int> nums = {1, 2, 2, 3, 4, 5, 8, 8, 8};
    nums.erase(std::unique(nums.begin(), nums.end()), nums.end());
    for (const auto &num : nums)
        cout << num << '\n';
}