#include "entity3d.h"
#include "materialmanager.h"
#include <QFile>
#include <QTextStream>

Entity3D::Entity3D()
    : m_lighting(true),
      m_enable(true),
      m_overrideMaterial(NULL)
{

}

Entity3D::Entity3D(PrimitiveType type)
    : m_lighting(true),
      m_enable(true),
      m_overrideMaterial(NULL)
{
    loadMesh(type);
}

Entity3D::Entity3D(const QString &fileName)
    : m_lighting(true),
      m_enable(true),
      m_overrideMaterial(NULL)
{
    loadMesh(fileName);
}

Entity3D::~Entity3D()
{
    MeshContainer::iterator it = m_meshInstances.begin();
    while (it != m_meshInstances.end())
    {
        delete *it;
        it++;
    }
    m_meshInstances.clear();
}

void Entity3D::loadMesh(PrimitiveType type)
{
    switch (type)
    {
    case PrimitiveType::Cone: generateCone(); break;
    case PrimitiveType::Cube: generateCube(); break;
    case PrimitiveType::Cylinder: generateCylinder(); break;
    case PrimitiveType::Grid: generateGrid(); break;
    case PrimitiveType::Plane: generatePlane(); break;
    case PrimitiveType::Pyramid: generatePyramid(); break;
    case PrimitiveType::Sphere: generateSphere(); break;
    case PrimitiveType::Teapot: generateTeapot(); break;
    case PrimitiveType::Torus: generateTorus(); break;
    case PrimitiveType::Tube: generateTube(); break;
    }
}

void Entity3D::loadMesh(const QString &fileName)
{
    QString fileExtension = fileName.section('.', -1, -1);
    if (fileExtension == "obj") loadObjFile(fileName);
    else if (fileExtension == "m") loadMFile(fileName);
}

void Entity3D::loadObjFile(const QString &fileName)
{
    QString fileDirectory = fileName.section('/', 0, -2) + "/";
    Mesh* mesh = NULL;
    VertexContainer* m_vertices = NULL;

    std::vector<QVector3D> v_list;
    std::vector<QVector3D> vn_list;
    std::vector<QVector2D> vt_list;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().simplified();
        QString type = line.section(' ', 0, 0);
        if (type == "v")
        {
            float x = line.section(' ', 1, 1).toFloat();
            float y = line.section(' ', 2, 2).toFloat();
            float z = line.section(' ', 3, 3).toFloat();

            if (v_list.empty())
            {
                m_max.setX(x);
                m_max.setY(y);
                m_max.setZ(z);
                m_min.setX(x);
                m_min.setY(y);
                m_min.setZ(z);
            }
            else
            {
                if (x > m_max.x()) m_max.setX(x);
                if (y > m_max.y()) m_max.setY(y);
                if (z > m_max.z()) m_max.setZ(z);
                if (x < m_min.x()) m_min.setX(x);
                if (y < m_min.y()) m_min.setY(y);
                if (z < m_min.z()) m_min.setZ(z);
            }
            v_list.push_back(QVector3D(x, y, z));
        }
        else if (type == "vt")
        {
            float x = line.section(' ', 1, 1).toFloat();
            float y = line.section(' ', 2, 2).toFloat();
            vt_list.push_back(QVector2D(x, y));
        }
        else if (type == "vn")
        {
            float x = line.section(' ', 1, 1).toFloat();
            float y = line.section(' ', 2, 2).toFloat();
            float z = line.section(' ', 3, 3).toFloat();
            vn_list.push_back(QVector3D(x, y, z));
        }
        else if (type == "f")
        {
            QString sec1 = line.section(' ', 1, 1);
            int v1 = sec1.section('/', 0, 0).toInt() - 1;
            int vt1 = sec1.section('/', 1, 1).toInt() - 1;
            int vn1 = sec1.section('/', 2, 2).toInt() - 1;

            QString sec2 = line.section(' ', 2, 2);
            int v2 = sec2.section('/', 0, 0).toInt() - 1;
            int vt2 = sec2.section('/', 1, 1).toInt() - 1;
            int vn2 = sec2.section('/', 2, 2).toInt() - 1;

            QString sec3 = line.section(' ', 3, 3);
            int v3 = sec3.section('/', 0, 0).toInt() - 1;
            int vt3 = sec3.section('/', 1, 1).toInt() - 1;
            int vn3 = sec3.section('/', 2, 2).toInt() - 1;

            QVector3D edge1 = v_list[v2] - v_list[v1];
            QVector3D edge2 = v_list[v3] - v_list[v1];
            QVector2D deltaUV1 = vt_list[vt2] - vt_list[vt1];
            QVector2D deltaUV2 = vt_list[vt3] - vt_list[vt1];

            float f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

            QVector3D tangent = f * (deltaUV2.y() * edge1 - deltaUV1.y() * edge2);
            tangent.normalize();
            QVector3D bitangent = f * (deltaUV2.x() * edge1 - deltaUV1.x() * edge2);
            bitangent.normalize();

            m_vertices->push_back(Vertex(v_list[v1], vt_list[vt1], vn_list[vn1], tangent, bitangent));
            m_vertices->push_back(Vertex(v_list[v2], vt_list[vt2], vn_list[vn2], tangent, bitangent));
            m_vertices->push_back(Vertex(v_list[v3], vt_list[vt3], vn_list[vn3], tangent, bitangent));
        }
        else if (type == "o" || type == "g")
        {
            mesh = new Mesh();
            m_meshInstances.push_back(mesh);
            mesh->setMeshName(line.section(' ', 1, 1));
            m_vertices = mesh->getVertexInstances();
        }
        else if (type == "mtllib")
        {
            QString m_matFile = fileDirectory + line.section(' ', 1, 1);
            MaterialManager::loadMaterialFile(m_matFile);
        }
        else if (type == "usemtl")
        {
            QString m_matName = line.section(' ', 1, 1);
            mesh->setMaterial(MaterialManager::getMaterial(m_matName));
        }
    }

    file.close();
    v_list.clear();
    vt_list.clear();
    vn_list.clear();
    m_entityName = fileName;
    m_center.setX((m_max.x() + m_min.x()) * 0.5f);
    m_center.setY((m_max.y() + m_min.y()) * 0.5f);
    m_center.setZ((m_max.z() + m_min.z()) * 0.5f);

    MeshContainer::iterator it = m_meshInstances.begin();
    while (it != m_meshInstances.end())
    {
        if ((*it)->getMaterial() == NULL) (*it)->setMaterial(MaterialManager::getDefaultMaterial());
        it++;
    }
}

void Entity3D::loadMFile(const QString &fileName)
{
    Mesh* mesh = new Mesh();
    m_meshInstances.push_back(mesh);
    mesh->setMeshName(fileName.section('/', -1, -1));
    VertexContainer* m_vertices = mesh->getVertexInstances();

    //QVector<QVector3D> v_list;
    std::vector<std::pair<QVector3D, QVector<QVector3D>>> v_list;
    std::vector<QVector3D> f_list;
    std::vector<QVector3D> n_list;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().simplified();
        QString type = line.section(' ', 0, 0);
        if (type == "Vertex")
        {
            float x = line.section(' ', 2, 2).toFloat();
            float y = line.section(' ', 3, 3).toFloat();
            float z = line.section(' ', 4, 4).toFloat();

            if (v_list.empty())
            {
                m_max.setX(x);
                m_max.setY(y);
                m_max.setZ(z);
                m_min.setX(x);
                m_min.setY(y);
                m_min.setZ(z);
            }
            else
            {
                if (x > m_max.x()) m_max.setX(x);
                if (y > m_max.y()) m_max.setY(y);
                if (z > m_max.z()) m_max.setZ(z);
                if (x < m_min.x()) m_min.setX(x);
                if (y < m_min.y()) m_min.setY(y);
                if (z < m_min.z()) m_min.setZ(z);
            }
            v_list.push_back(std::pair<QVector3D, QVector<QVector3D>>(QVector3D(x, y, z), QVector<QVector3D>()));
        }
        else if (type == "Face")
        {
            int v1 = line.section(' ', 2, 2).toInt() - 1;
            int v2 = line.section(' ', 3, 3).toInt() - 1;
            int v3 = line.section(' ', 4, 4).toInt() - 1;
            f_list.push_back(QVector3D(v1, v2, v3));

            QVector3D edge1 = v_list[v2].first - v_list[v1].first;
            QVector3D edge2 = v_list[v3].first - v_list[v1].first;
            //QVector3D normal = QVector3D::normal(edge1, edge2);
            QVector3D normal = QVector3D::crossProduct(edge1 * 10.0f, edge2 * 10.0f);
            //normal.normalize();
            v_list[v1].second.push_back(normal);
            v_list[v2].second.push_back(normal);
            v_list[v3].second.push_back(normal);
        }
    }
    file.close();
    m_center.setX((m_max.x() + m_min.x()) * 0.5f);
    m_center.setY((m_max.y() + m_min.y()) * 0.5f);
    m_center.setZ((m_max.z() + m_min.z()) * 0.5f);

    for (int i = 0; i < v_list.size(); i++)
    {
        int n = v_list[i].second.size();
        QVector3D sum;
        for (int j = 0; j < n; j++)
        {
            sum += v_list[i].second[j];
        }
        QVector3D normal = sum / n;
        normal.normalize();
        n_list.push_back(normal);
        v_list[i].second.clear();
    }

    for (int i = 0; i < f_list.size(); i++)
    {
        int v1 = f_list[i].x();
        int v2 = f_list[i].y();
        int v3 = f_list[i].z();
        m_vertices->push_back(Vertex(v_list[v1].first, QVector2D(), n_list[v1], QVector3D(), QVector3D()));
        m_vertices->push_back(Vertex(v_list[v2].first, QVector2D(), n_list[v2], QVector3D(), QVector3D()));
        m_vertices->push_back(Vertex(v_list[v3].first, QVector2D(), n_list[v3], QVector3D(), QVector3D()));
    }

    v_list.clear();
    n_list.clear();
    f_list.clear();

    m_entityName = fileName;
    mesh->setMaterial(MaterialManager::getDefaultMaterial());
}

void Entity3D::setDrawMode(GLenum mode)
{
    MeshContainer::iterator it = m_meshInstances.begin();
    while (it != m_meshInstances.end())
    {
        (*it)->setDrawMode(mode);
        it++;
    }
}

void Entity3D::generateCone()
{
    loadMesh(":/models/primitive/cone.obj");
}

void Entity3D::generateCube()
{
    loadMesh(":/models/primitive/cube.obj");
}

void Entity3D::generateCylinder()
{
    loadMesh(":/models/primitive/cylinder.obj");
}

void Entity3D::generatePyramid()
{
    loadMesh(":/models/primitive/pyramid.obj");
}

void Entity3D::generateSphere()
{
    loadMesh(":/models/primitive/sphere.obj");
}

void Entity3D::generateTeapot()
{
    loadMesh(":/models/primitive/teapot.obj");
}

void Entity3D::generateTorus()
{
    loadMesh(":/models/primitive/torus.obj");
}

void Entity3D::generateTube()
{
    loadMesh(":/models/primitive/tube.obj");
}

void Entity3D::generatePlane()
{
    loadMesh(":/models/primitive/plane.obj");
}

void Entity3D::generateGrid(int size)
{
    Mesh* mesh = new Mesh();
    mesh->setMaterial(MaterialManager::getDefaultMaterial());
    mesh->setMeshName("mesh_" + QString::number(m_meshInstances.size()));
    m_meshInstances.push_back(mesh);
    VertexContainer* m_vertices = mesh->getVertexInstances();

    int s = size / 2;
    for (int i = 0; i <= size; i++)
    {
        m_vertices->push_back(Vertex(QVector3D(-s, 0, -s + i)));
        m_vertices->push_back(Vertex(QVector3D( s, 0, -s + i)));
    }
    for (int i = 0; i <= size; i++)
    {
        m_vertices->push_back(Vertex(QVector3D(-s + i, 0, -s)));
        m_vertices->push_back(Vertex(QVector3D(-s + i, 0,  s)));
    }
    m_entityName = "Grid";
}
