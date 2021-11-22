
#include <iostream>

#include <utils/DumpHelper.h>

#include "physics.h"

using namespace std;

void dump(const btRigidBody *btrb) {
    Dump.begin("btRigidBody");
    Dump.putParam("Is in world", btrb->isInWorld());
    Dump.putParam("Motion state", btrb->getMotionState());
    Dump.end();
}
