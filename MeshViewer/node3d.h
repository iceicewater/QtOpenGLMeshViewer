#ifndef NODE3D_H
#define NODE3D_H

#include "entity3d.h"
#include "transform3d.h"

class Node3D : public Transform3D
{
public:
    Node3D();

    void attachChild(Entity3D* child);
    void dettachChild();

    Entity3D* getChild();

private:
    Entity3D* m_child;
};

inline Entity3D* Node3D::getChild() { return m_child; }

#endif // NODE3D_H
