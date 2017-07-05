#include "entitymanager.h"

EntityContainer entityInstances;

EntityContainer& EntityManager::getEntityInstances()
{
    return entityInstances;
}

void EntityManager::releaseEntityInstances()
{
    EntityContainer::iterator it_obj = entityInstances.begin();
    while (it_obj != entityInstances.end())
    {
        delete *it_obj;
        it_obj++;
    }
    entityInstances.clear();
}

Entity3D* EntityManager::loadEntity(const QString &fileName)
{
    Entity3D* entity = NULL;
    EntityContainer::iterator it = entityInstances.begin();
    while (it != entityInstances.end())
    {
        if ((*it)->getEntityName() == fileName)
        {
            entity = (*it);
            break;
        }
        it++;
    }
    if (entity == NULL)
    {
        entity = new Entity3D(fileName);
        entityInstances.push_back(entity);
    }
    return entity;
}

Entity3D* EntityManager::loadEntity(PrimitiveType type)
{
    Entity3D* entity = new Entity3D(type);
    entityInstances.push_back(entity);
    return entity;
}
