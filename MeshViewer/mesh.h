#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QString>
#include "vertex.h"
#include "material.h"
#include "shadermanager.h"

typedef QVector<Vertex> VertexContainer;
typedef QVector<int> FaceContainer;

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void setDrawMode(GLenum mode);
    void setMeshName(const QString& meshName);
    void setMaterial(Material* mat);

    const QString& getMeshName() const;
    Material* getMaterial();

    VertexContainer* getVertexInstances();
    FaceContainer* getFaceInstances();

    void drawMesh();

private:
    void bindBuffer();
    void drawTriangles();
    void drawLines();
    void drawPoints();

    GLuint m_VBO = 0;
    GLuint m_VAO = 0;
    QOpenGLFunctions_3_3_Core* m_funcs;

    QString m_meshName;
    VertexContainer m_vertexInstances;
    FaceContainer m_faceInstances;
    Material* m_material;
    GLenum m_drawMode;
    bool m_binded;
};

inline void Mesh::setDrawMode(GLenum mode) { m_drawMode = mode; }
inline void Mesh::setMeshName(const QString &meshName) { m_meshName = meshName; }
inline void Mesh::setMaterial(Material *mat) { m_material = mat; }
inline const QString& Mesh::getMeshName() const { return m_meshName; }
inline Material* Mesh::getMaterial() { return m_material; }
inline VertexContainer* Mesh::getVertexInstances() { return &m_vertexInstances; }
inline FaceContainer* Mesh::getFaceInstances() { return &m_faceInstances; }

#endif // MESH_H
