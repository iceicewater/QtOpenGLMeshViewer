#include "mesh.h"
#include <QFile>
#include <QTextStream>
#include "global.h"

Mesh::Mesh()
    : m_material(NULL),
      m_drawMode(GL_TRIANGLES),
      m_binded(false)
{
    m_funcs = ShaderManager::getOpenGLFunctions();
}

Mesh::~Mesh()
{
    m_vertexInstances.clear();
    m_faceInstances.clear();
    if (m_VBO != 0) m_funcs->glDeleteBuffers(1, &m_VBO);
    if (m_VAO != 0) m_funcs->glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::drawMesh()
{
    bindBuffer();
    if (m_VAO == 0) return;

    m_funcs->glBindVertexArray(m_VAO);
    switch (m_drawMode)
    {
    case GL_TRIANGLES: drawTriangles(); break;
    case GL_LINES: drawLines(); break;
    case GL_POINTS: drawPoints(); break;
    }
    m_funcs->glBindVertexArray(0);
}

void Mesh::drawTriangles()
{
    if (m_material->hasAmbientMap()) m_material->getAmbientMap()->bind(AMBIENT_MAP_UNIT);
    if (m_material->hasDiffuseMap()) m_material->getDiffuseMap()->bind(DIFFUSE_MAP_UNIT);
    if (m_material->hasSpecularMap()) m_material->getSpecularMap()->bind(SPECULAR_MAP_UNIT);
    if (m_material->hasNormalMap()) m_material->getNormalMap()->bind(NORMAL_MAP_UNIT);
    m_funcs->glDrawArrays(GL_TRIANGLES, 0, m_vertexInstances.size());
    if (m_material->hasAmbientMap()) m_material->getAmbientMap()->release(AMBIENT_MAP_UNIT);
    if (m_material->hasDiffuseMap()) m_material->getDiffuseMap()->release(DIFFUSE_MAP_UNIT);
    if (m_material->hasSpecularMap()) m_material->getSpecularMap()->release(SPECULAR_MAP_UNIT);
    if (m_material->hasNormalMap()) m_material->getNormalMap()->release(NORMAL_MAP_UNIT);
}

void Mesh::drawLines()
{
    m_funcs->glDrawArrays(GL_LINES, 0, m_vertexInstances.size());
}

void Mesh::drawPoints()
{
    m_funcs->glDrawArrays(GL_POINTS, 0, m_vertexInstances.size());
}


void Mesh::bindBuffer()
{
    if (m_binded) return;
    if (!m_vertexInstances.empty())
    {
        m_funcs->glGenVertexArrays(1, &m_VAO);
        m_funcs->glGenBuffers(1, &m_VBO);
        m_funcs->glBindVertexArray(m_VAO);
        m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        m_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertexInstances.size(), m_vertexInstances.constData(), GL_STATIC_DRAW);
        m_funcs->glVertexAttribPointer(0, Vertex::PositionTupleSize, GL_FLOAT, GL_TRUE, Vertex::stride(), (GLvoid*)Vertex::positionOffset());
        m_funcs->glVertexAttribPointer(1, Vertex::TexCoordTupleSize, GL_FLOAT, GL_TRUE, Vertex::stride(), (GLvoid*)Vertex::texCoordOffset());
        m_funcs->glVertexAttribPointer(2, Vertex::NormalTupleSize, GL_FLOAT, GL_TRUE, Vertex::stride(), (GLvoid*)Vertex::normalOffset());
        m_funcs->glVertexAttribPointer(3, Vertex::TangentTupleSize, GL_FLOAT, GL_TRUE, Vertex::stride(), (GLvoid*)Vertex::tangentOffset());
        m_funcs->glVertexAttribPointer(4, Vertex::BitangentTupleSize, GL_FLOAT, GL_TRUE, Vertex::stride(), (GLvoid*)Vertex::bitangentOffset());
        m_funcs->glEnableVertexAttribArray(0);
        m_funcs->glEnableVertexAttribArray(1);
        m_funcs->glEnableVertexAttribArray(2);
        m_funcs->glEnableVertexAttribArray(3);
        m_funcs->glEnableVertexAttribArray(4);
        m_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0);
        m_funcs->glBindVertexArray(0);
    }
    m_binded = true;
}
