#ifndef ENTITY3D_H
#define ENTITY3D_H

#include <vector>
#include "mesh.h"

enum PrimitiveType
{
    Cone,
    Cube,
    Cylinder,
    Grid,
    Plane,
    Pyramid,
    Sphere,
    Teapot,
    Torus,
    Tube
};

typedef std::vector<Mesh*> MeshContainer;

class Entity3D
{
public:
    Entity3D();
    Entity3D(PrimitiveType type);
    Entity3D(const QString& fileName);
    ~Entity3D();

    void loadMesh(PrimitiveType type);
    void loadMesh(const QString& fileName);
    void setDrawMode(GLenum mode);

    void enable();
    void disable();
    void enableLighting();
    void disableLighting();
    void setEntityName(const QString& entityName);
    void setOverrideMaterial(Material* material);

    bool isEnabled();
    bool isLightingEnabled();
    const QString& getEntityName();
    Material* getOverrideMaterial();
    MeshContainer* getMeshInstances();
    const QVector3D& getMinPoint();
    const QVector3D& getMaxPoint();
    const QVector3D& getCenterPoint();

private:
    void loadObjFile(const QString& fileName);
    void loadMFile(const QString& fileName);
    void generateCone();
    void generateCube();
    void generateCylinder();
    void generateGrid(int size = 20);
    void generatePlane();
    void generatePyramid();
    void generateSphere();
    void generateTeapot();
    void generateTorus();
    void generateTube();

    QString m_entityName;
    MeshContainer m_meshInstances;
    bool m_lighting;
    bool m_enable;
    Material* m_overrideMaterial;

    QVector3D m_min;
    QVector3D m_max;
    QVector3D m_center;
};

inline void Entity3D::enable() { m_enable = true; }
inline void Entity3D::disable() { m_enable = false; }
inline void Entity3D::enableLighting() { m_lighting = true; }
inline void Entity3D::disableLighting() { m_lighting = false; }
inline void Entity3D::setOverrideMaterial(Material *material) { m_overrideMaterial = material; }
inline void Entity3D::setEntityName(const QString &entityName) { m_entityName = entityName; }

inline bool Entity3D::isEnabled() { return m_enable; }
inline bool Entity3D::isLightingEnabled() { return m_lighting; }
inline const QString& Entity3D::getEntityName() { return m_entityName; }
inline Material* Entity3D::getOverrideMaterial() { return m_overrideMaterial; }
inline MeshContainer* Entity3D::getMeshInstances() { return &m_meshInstances; }
inline const QVector3D& Entity3D::getMinPoint() { return m_min; }
inline const QVector3D& Entity3D::getMaxPoint() { return m_max; }
inline const QVector3D& Entity3D::getCenterPoint() { return m_center; }

#endif // ENTITY3D_H
