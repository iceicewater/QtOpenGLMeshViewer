#ifndef NODEMANAGER_H
#define NODEMANAGER_H

#include "node3d.h"
#include <vector>

typedef std::vector<Node3D*> NodeContainer;

class NodeManager
{
public:
    static Node3D* getNewNode();
    static void releaseNodeInstances();
    static NodeContainer& getNodeInstances();
};

#endif // NODEMANAGER_H
