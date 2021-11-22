
#pragma once

#include <vector>
#include <string>
#include <iostream>

class DumpHelper
{
    std::ostream *_out{&std::cout};
    std::vector<std::string> _stack;
public:
    void setStream(std::ostream &os) { _out = &os; }
    void setStream(std::ostream *os) { _out = os; }
    void putIndent() const;
    std::size_t currentLevel() const { return _stack.size(); }
    std::size_t indentSize() const { return currentLevel(); }
    void begin(const std::string &);
    void end();
    template<typename T>
    void putParam(const char *msg, T v) {
        putIndent();
        (*_out) << msg << ": " << v << std::endl;
    }
};

extern DumpHelper Dump;
