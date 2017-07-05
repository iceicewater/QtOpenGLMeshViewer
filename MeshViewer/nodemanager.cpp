#include "nodemanager.h"

NodeContainer nodeInstances;

NodeContainer& NodeManager::getNodeInstances()
{
    return nodeInstances;
}

Node3D* NodeManager::getNewNode()
{
    Node3D* node = new Node3D();
    nodeInstances.push_back(node);
    return node;
}

void NodeManager::releaseNodeInstances()
{
    NodeContainer::iterator it = nodeInstances.begin();
    while (it != nodeInstances.end())
    {
        delete *it;
        it++;
    }
    nodeInstances.clear();
}
