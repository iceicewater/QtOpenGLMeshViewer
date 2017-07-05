#include "materialmanager.h"
#include <vector>
#include <QFile>
#include <QTextStream>

typedef std::vector<Material*> MaterialContainer;
typedef std::vector<QString> FileNameContainer;

MaterialContainer materialInstances;
FileNameContainer fileNameInstances;

void MaterialManager::releaseMaterialInstances()
{
    MaterialContainer::iterator it = materialInstances.begin();
    while (it != materialInstances.end())
    {
        delete *it;
        it++;
    }
    materialInstances.clear();
    fileNameInstances.clear();
}

void MaterialManager::initialize()
{
    Material* mat = new Material();
    mat->setMaterialName("material_default");
    materialInstances.push_back(mat);
}

Material* MaterialManager::getDefaultMaterial()
{
    return materialInstances[0];
}

Material* MaterialManager::getNewMaterial()
{
    Material* mat = new Material();
    mat->setMaterialName("material_" + QString::number(materialInstances.size()));
    materialInstances.push_back(mat);
    return mat;
}

Material* MaterialManager::getMaterial(QString matName)
{
    Material* mat = NULL;
    MaterialContainer::iterator it = materialInstances.begin();
    while (it != materialInstances.end())
    {
        if ((*it)->getMaterialName() == matName)
        {
            mat = (*it);
            break;
        }
        it++;
    }
    if (mat == NULL) mat = getNewMaterial();
    return mat;
}

void MaterialManager::loadMaterialFile(const QString &fileName)
{
    FileNameContainer::iterator it = fileNameInstances.begin();
    while (it != fileNameInstances.end())
    {
        if ((*it) == fileName) return;
        it++;
    }

    Material* mat;
    QString fileDirectory = fileName.section('/', 0, -2) + "/";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().simplified();
        QString type = line.section(' ', 0, 0);
        if (type == "newmtl")
        {
            mat = new Material();
            mat->setMaterialName(line.section(' ', 1, 1));
            materialInstances.push_back(mat);
        }
        else if (type == "Ka")
        {
            float x = line.section(' ', 1, 1).toFloat();
            float y = line.section(' ', 2, 2).toFloat();
            float z = line.section(' ', 3, 3).toFloat();
            mat->setAmbient(x, y, z, 1.0f);
        }
        else if (type == "Kd")
        {
            float x = line.section(' ', 1, 1).toFloat();
            float y = line.section(' ', 2, 2).toFloat();
            float z = line.section(' ', 3, 3).toFloat();
            mat->setDiffuse(x, y, z, 1.0f);
        }
        else if (type == "Ks")
        {
            float x = line.section(' ', 1, 1).toFloat();
            float y = line.section(' ', 2, 2).toFloat();
            float z = line.section(' ', 3, 3).toFloat();
            mat->setSpecular(x, y, z, 1.0f);
        }
        else if (type == "Ns")
        {
            float s = line.section(' ', 1, 1).toFloat();
            mat->setShininess(s);
        }
        else if (type == "map_Ka")
        {
            QString ambiMapFileName = fileDirectory + line.section(' ', 1, 1);
            mat->loadAmbientMap(ambiMapFileName);
        }
        else if (type == "map_Kd")
        {
            QString diffMapFileName = fileDirectory + line.section(' ', 1, 1);
            mat->loadDiffuseMap(diffMapFileName);
        }
        else if (type == "map_Ks")
        {
            QString specMapFileName = fileDirectory + line.section(' ', 1, 1);
            mat->loadSpecularMap(specMapFileName);
        }
        else if (type == "map_bump" || type == "bump")
        {
            QString normMapFileName = fileDirectory + line.section(' ', 1, 1);
            mat->loadNormalMap(normMapFileName);
        }
    }
    file.close();
    fileNameInstances.push_back(fileName);
}
