
#include "FileList.h"

using namespace std;
using namespace std::filesystem;

void FileList::refresh() {
    _map.clear();
    for (auto &entry : directory_iterator{path}) {
        if ((entry.is_block_file() && _mask & BlockFile) ||
            (entry.is_character_file() && _mask & CharacterFile) ||
            (entry.is_directory() && _mask & Directory) ||
            (entry.is_fifo() && _mask & Fifo) ||
            (entry.is_other() && _mask & Other) ||
            (entry.is_regular_file() && _mask & RegularFile) ||
            (entry.is_socket() && _mask & Socket) ||
            (entry.is_symlink() && _mask & Symlink))
            _map.insert(pair(entry.path().filename(), entry));
    }
}

bool FileList::contains(const string &name) const {
    return _map.find(name) != _map.end();
}
