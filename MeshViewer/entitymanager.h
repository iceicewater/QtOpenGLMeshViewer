#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "entity3d.h"
#include <vector>

typedef std::vector<Entity3D*> EntityContainer;

class EntityManager
{
public:
    static Entity3D* loadEntity(const QString& fileName);
    static Entity3D* loadEntity(PrimitiveType type);
    static void releaseEntityInstances();
    static EntityContainer& getEntityInstances();
};

#endif // ENTITYMANAGER_H
