
#include <cassert>

#include <utils/AutoIndexer.h>

typedef AutoIndexer<int> IntAI;

void TestAddArbitraryIndex() {
    IntAI ai;
    assert(ai.firstFreeSlot() == IntAI::InvalidIndex);
    assert(ai.freeSlots() == 0);
    assert(ai.size() == 0);
    assert(ai.itemsNumber() == 0);
    assert(ai.add(1) == 0);
    assert(ai.firstFreeSlot() == IntAI::InvalidIndex);
    assert(ai.freeSlots() == 0);
    assert(ai.size() == 1);
    assert(ai.itemsNumber() == 1);
    assert(ai.add(5, 2) == 2);
    assert(ai.size() == 3);
    assert(ai.itemsNumber() == 2);
    assert(ai.freeSlots() == 1);
    assert(ai.firstFreeSlot() == 1);
    assert(ai.add(3, 1) == 1);
    assert(ai.size() == 3);
    assert(ai.itemsNumber() == 3);
    assert(ai.freeSlots() == 0);
    assert(ai.firstFreeSlot() == IntAI::InvalidIndex);
}

int main() {
    TestAddArbitraryIndex();

    return 0;
}
