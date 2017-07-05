#include "lightmanager.h"
#include "global.h"

LightContainer lightInstances;

LightContainer& LightManager::getLightInstances()
{
    return lightInstances;
}

void LightManager::initialize()
{
    Light* defaultLight = new Light();
    lightInstances.push_back(defaultLight);
    defaultLight->setLightType(LightType::Point);
    defaultLight->setAmbient(0.2f, 0.2f, 0.2f, 1.0f);
    defaultLight->setDiffuse(0.8f, 0.8f, 0.8f, 1.0f);
    defaultLight->setSpecular(0.8f, 0.8f, 0.8f, 1.0f);
}

Light* LightManager::getDefaultLight()
{
    return lightInstances[0];
}

Light* LightManager::getNewLight()
{
    Light* light;
    if (lightInstances.size() < MAX_LIGHTS_NUMBER)
    {
        light = new Light();
        lightInstances.push_back(light);
    }
    else
    {
        light = lightInstances[MAX_LIGHTS_NUMBER - 1];
    }
    return light;
}

void LightManager::releaseLightInstances()
{
    LightContainer::iterator it = lightInstances.begin();
    while (it != lightInstances.end())
    {
        delete *it;
        it++;
    }
    lightInstances.clear();
}
