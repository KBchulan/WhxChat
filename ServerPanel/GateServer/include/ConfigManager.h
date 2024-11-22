#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "const.h"

struct SectionInfo
{
    SectionInfo() = default;
    ~SectionInfo()
    {
        _section_datas.clear();
    }

    SectionInfo(const SectionInfo &other)
    {
        _section_datas = other._section_datas;
    }

    SectionInfo &operator=(const SectionInfo &other)
    {
        if (&other == this)
            return *this;

        this->_section_datas = other._section_datas;
    }

    std::string operator[](const std::string &key)
    {
        if (!_section_datas.contains(key))
            return "";

        return _section_datas[key];
    }

    // 三路比较符号，生成大小比较(可能用不上)
    bool operator<=>(const SectionInfo &) const = default;
    std::map<std::string, std::string> _section_datas;
};

class ConfigManager final : public Singleton<ConfigManager>
{
    friend class Singleton<ConfigManager>;

public:
    ~ConfigManager();

private:
    ConfigManager();
};

#endif // !CONFIGMANAGER_H