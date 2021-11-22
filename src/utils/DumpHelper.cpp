
#include "DumpHelper.h"

using namespace std;

void DumpHelper::putIndent() const {
    std::size_t i = 0, n = currentLevel();
    while(i < n) {
        (*_out) << " ";
        i++;
    }
}

void DumpHelper::begin(const string &msg) {
    putIndent();
    (*_out) << ">>>==| " << msg << "|==>>>" << endl;
    _stack.push_back(msg);
}

void DumpHelper::end() {
    if (!_stack.size())  return;
    string msg = _stack.back();
    _stack.pop_back();
    putIndent();
    (*_out) << "<<<==| " << msg << "|==<<<" << endl;
}

DumpHelper Dump;
