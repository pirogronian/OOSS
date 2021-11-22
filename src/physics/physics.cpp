
#include <iostream>

#include <utils/DumpHelper.h>

#include "physics.h"

using namespace std;

void dump(const btRigidBody *btrb) {
    Dump.begin("btRigidBody");
    Dump.putParam("Is active", btrb->isActive());
    Dump.putParam("Is static", btrb->isStaticObject());
    Dump.putParam("Is in world", btrb->isInWorld());
    Dump.putParam("Position", btrb->getWorldTransform().getOrigin());
    Dump.putParam("Rotation", btrb->getWorldTransform().getRotation());
    Dump.putParam("Motion state", btrb->getMotionState());
    Dump.putParam("Linear factor", btrb->getLinearFactor());
    Dump.putParam("Angular factor", btrb->getAngularFactor());
    Dump.putParam("Total force", btrb->getTotalForce());
    Dump.putParam("Total torque", btrb->getTotalTorque());
//     Dump.putParam("Wants sleep", btrb->wantsSleeping());
    Dump.putParam("Local inertia", btrb->getLocalInertia());
    Dump.putParam("Inverse mass", btrb->getInvMass());
    Dump.putParam("InvInertiaDiagLocal", btrb->getInvInertiaDiagLocal());
    Dump.putParam("Linear velocity", btrb->getLinearVelocity());
    Dump.putParam("Angular velocity", btrb->getAngularVelocity());
    Dump.end();
}
