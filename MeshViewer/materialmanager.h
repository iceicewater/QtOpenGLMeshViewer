#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include "material.h"
#include <QString>

class MaterialManager
{
public:
    static Material* getDefaultMaterial();
    static Material* getNewMaterial();
    static Material* getMaterial(QString matName);
    static void initialize();
    static void releaseMaterialInstances();
    static void loadMaterialFile(const QString& fileName);
};

#endif // MATERIALMANAGER_H
