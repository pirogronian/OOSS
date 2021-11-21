
#pragma once

#include <iostream>

#include <btBulletDynamicsCommon.h>

class btRigidBody;
class RigidBody;
class DynamicsWorld;

inline std::ostream& operator<<(std::ostream &out, const btVector3& v) {
    out << "[ " << v.x() << ", " << v.y() << ", " << v.z() << " ]";
    return out;
}

inline std::ostream& operator<<(std::ostream &out, const btQuaternion& q) {
    out << "[ " << q.w() << q.x() << ", " << q.y() << ", " << q.z() << " ]";
    return out;
}

inline void dump(const btVector3& v) {
    std::cout << "btVector3" << v << std::endl;
}

inline void dump(const btQuaternion& q) {
    std::cout << "btQuaternion" << q << std::endl;
}

inline void dump(const btTransform& tr) {
    std::cout << "btTransform\n";
    dump(tr.getOrigin());
    dump(tr.getRotation());
}
