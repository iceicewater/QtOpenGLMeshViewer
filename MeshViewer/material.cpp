#include "material.h"
#include "texturemanager.h"

Material::Material()
    : m_ambient(0.5f, 0.5f, 0.5f, 1.0f),
      m_diffuse(0.5f, 0.5f, 0.5f, 1.0f),
      m_specular(0.5f, 0.5f, 0.5f, 1.0f),
      m_shininess(64),
      m_ambientMap(0),
      m_diffuseMap(0),
      m_specularMap(0),
      m_normalMap(0),
      m_lockAmbientToDiffuse(true)
{

}

void Material::loadAmbientMap(const QString& fileName)
{
    m_ambientMap = TextureManager::loadTexture(fileName);
    m_lockAmbientToDiffuse = false;
}

void Material::loadDiffuseMap(const QString& fileName)
{
    m_diffuseMap = TextureManager::loadTexture(fileName);
}

void Material::loadSpecularMap(const QString& fileName)
{
    m_specularMap = TextureManager::loadTexture(fileName);
}

void Material::loadNormalMap(const QString& fileName)
{
    m_normalMap = TextureManager::loadTexture(fileName);
}
