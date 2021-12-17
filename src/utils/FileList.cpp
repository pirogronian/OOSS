
#include "FileList.h"

using namespace std;
using namespace std::filesystem;

void FileList::refresh() {
    _map.clear();
    for (auto &entry : directory_iterator{path}) {
        if (entry.is_directory() && !dir)  continue;
        _map.insert(pair(entry.path().filename(), entry));
    }
}

bool FileList::contains(const string &name) const {
    return _map.find(name) != _map.end();
}
