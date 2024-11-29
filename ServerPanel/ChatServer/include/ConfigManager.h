#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "Singleton.h"

#include <map>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

struct SectionInfo
{
    ~SectionInfo();

    SectionInfo() = default;

    SectionInfo(const SectionInfo &other);

    SectionInfo &operator=(const SectionInfo &other);

    // 方便通过对象直接获取值
    std::string operator[](const std::string &key);

    std::map<std::string, std::string> _section_datas;
};

class ConfigManager final : public Singleton<ConfigManager>
{
    friend class Singleton<ConfigManager>;
public:
    ~ConfigManager();

    static ConfigManager &GetInstance();

    ConfigManager(const ConfigManager &other);

    ConfigManager &operator=(const ConfigManager &other);

    // 方便通过对象直接获取值
    SectionInfo operator[](const std::string &section);

private:
    ConfigManager();

private:
    std::map<std::string, SectionInfo> _config_map;
};

#endif // !CONFIGMANAGER_H