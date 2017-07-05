#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include "light.h"
#include <vector>

typedef std::vector<Light*> LightContainer;

class LightManager
{
public:
    static Light* getNewLight();
    static Light* getDefaultLight();
    static void initialize();
    static void releaseLightInstances();
    static LightContainer& getLightInstances();
};

#endif // LIGHTMANAGER_H
