
#pragma once

#include <map>
#include <filesystem>

class FileList {
    std::map<std::string, std::filesystem::directory_entry> _map;
public:
    std::filesystem::path path;
    bool dir{true};
    bool other{true};
    bool block{true};
    void refresh();
    bool contains(const std::string&) const;
    const std::map<std::string, std::filesystem::directory_entry> &getEntries() const { return _map; }
};
