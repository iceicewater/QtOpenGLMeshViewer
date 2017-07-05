#include "node3d.h"

Node3D::Node3D() : m_child(NULL)
{

}

void Node3D::attachChild(Entity3D *child)
{
    m_child = child;
}

void Node3D::dettachChild()
{
    m_child = NULL;
}
