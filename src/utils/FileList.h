
#pragma once

#include <map>
#include <filesystem>

class FileList {
    std::map<std::string, std::filesystem::directory_entry> _map;
public:
    enum FileTypeFlags {
        None = 0,
        BlockFile = 1,
        CharacterFile = 2,
        Directory = 4,
        Fifo = 8,
        Other = 16,
        RegularFile = 32,
        Socket = 64,
        Symlink = 128,
        AllTypes = BlockFile | CharacterFile | Directory | Fifo | Other | RegularFile | Socket | Symlink
    };
protected:
    FileTypeFlags _mask{AllTypes};
public:
    std::filesystem::path path;
    void refresh();
    FileTypeFlags fileTypeMask() const { return _mask; }
    void setFileTypeMask(FileTypeFlags m) { _mask = m; }
    void addFileType(FileTypeFlags m) { _mask = static_cast<FileTypeFlags>(_mask | m); }
    void removeFileType(FileTypeFlags m) { _mask = static_cast<FileTypeFlags>(_mask & ~m); }
    bool hasFileType(FileTypeFlags m) const { return _mask & m; }
    bool contains(const std::string&) const;
    const std::map<std::string, std::filesystem::directory_entry> &getEntries() const { return _map; }
};
