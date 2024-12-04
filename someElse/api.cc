#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;
using namespace string_literals;

std::function<string(string)> xorString = [](string input)
{
    string result = input;
    int length = input.length();
    
    // 使用固定密钥进行异或加密
    const string key = "WhxChat";
    int keyLength = key.length();
    
    // 对每个字符进行异或操作
    for(int i = 0; i < length; i++) 
        result[i] = input[i] ^ key[i % keyLength];
    
    return result;
};

int main()
{
    std::vector<int> nums = {1, 2, 2, 3, 4, 5, 8, 8, 8};
    nums.erase(std::unique(nums.begin(), nums.end()), nums.end());
    for (const auto &num : nums)
        cout << num << '\n';

    string str = "Whx051021"s;
    str = xorString(str);
    cout << str << endl;

    str = xorString(str);
    cout << str << endl;
}