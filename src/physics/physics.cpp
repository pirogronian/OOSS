
#include <iostream>

#include "physics.h"

using namespace std;

void dump(const btRigidBody *btrb) {
    cout << "            >>>=| Dump btRigidBody |=>>>\n";
    cout << "is in world: " << btrb->isInWorld() << endl;
    cout << "Motion state: " << btrb->getMotionState() << endl;
    cout << "            <<<=| Dump btRigidBody |=<<<\n";
}
