#ifndef VERTEX_H
#define VERTEX_H

#include <QVector2D>
#include <QVector3D>

class Vertex
{
public:
    // Constructors
    Q_DECL_CONSTEXPR Vertex();
    Q_DECL_CONSTEXPR explicit Vertex(const QVector3D &position);
    Q_DECL_CONSTEXPR Vertex(const QVector3D &position, const QVector2D &texCoord);
    Q_DECL_CONSTEXPR Vertex(const QVector3D &position, const QVector2D &texCoord, const QVector3D &normal);
    Q_DECL_CONSTEXPR Vertex(const QVector3D &position, const QVector2D &texCoord, const QVector3D &normal,
                            const QVector3D &tangent, const QVector3D &bitangent);

    // Accessors / Mutators
    Q_DECL_CONSTEXPR const QVector3D& position() const;
    Q_DECL_CONSTEXPR const QVector2D& texCoord() const;
    Q_DECL_CONSTEXPR const QVector3D& normal() const;
    Q_DECL_CONSTEXPR const QVector3D& tangent() const;
    Q_DECL_CONSTEXPR const QVector3D& bitangent() const;
    void setPosition(const QVector3D& position);
    void setTexCoord(const QVector2D& texCoord);
    void setNormal(const QVector3D& normal);
    void setTangent(const QVector3D& tangent);
    void setBitangent(const QVector3D& bitangent);

    // OpenGL Helpers
    static const int PositionTupleSize = 3;
    static const int TexCoordTupleSize = 2;
    static const int NormalTupleSize = 3;
    static const int TangentTupleSize = 3;
    static const int BitangentTupleSize = 3;
    static Q_DECL_CONSTEXPR int positionOffset();
    static Q_DECL_CONSTEXPR int texCoordOffset();
    static Q_DECL_CONSTEXPR int normalOffset();
    static Q_DECL_CONSTEXPR int tangentOffset();
    static Q_DECL_CONSTEXPR int bitangentOffset();
    static Q_DECL_CONSTEXPR int stride();

private:
    QVector3D m_position;
    QVector2D m_texCoord;
    QVector3D m_normal;
    QVector3D m_tangent;
    QVector3D m_bitangent;
};

// Note: Q_MOVABLE_TYPE means it can be memcpy'd.
Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline Vertex::Vertex() {}
Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position)
    : m_position(position) {}
Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position, const QVector2D &texCoord)
    : m_position(position), m_texCoord(texCoord) {}
Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position, const QVector2D &texCoord, const QVector3D &normal)
    : m_position(position), m_texCoord(texCoord), m_normal(normal) {}
Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position, const QVector2D &texCoord, const QVector3D &normal,
                                       const QVector3D &tangent, const QVector3D &bitangent)
    : m_position(position), m_texCoord(texCoord), m_normal(normal), m_tangent(tangent), m_bitangent(bitangent) {}

// Accessors / Mutators
Q_DECL_CONSTEXPR inline const QVector3D& Vertex::position() const { return m_position; }
Q_DECL_CONSTEXPR inline const QVector2D& Vertex::texCoord() const { return m_texCoord; }
Q_DECL_CONSTEXPR inline const QVector3D& Vertex::normal() const { return m_normal; }
Q_DECL_CONSTEXPR inline const QVector3D& Vertex::tangent() const { return m_tangent; }
Q_DECL_CONSTEXPR inline const QVector3D& Vertex::bitangent() const { return m_bitangent; }
void inline Vertex::setPosition(const QVector3D& position) { m_position = position; }
void inline Vertex::setTexCoord(const QVector2D& texCoord) { m_texCoord = texCoord; }
void inline Vertex::setNormal(const QVector3D& normal) { m_normal = normal; }
void inline Vertex::setTangent(const QVector3D& tangent) { m_tangent = tangent; }
void inline Vertex::setBitangent(const QVector3D& bitangent) { m_bitangent = bitangent; }

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int Vertex::positionOffset() { return offsetof(Vertex, m_position); }
Q_DECL_CONSTEXPR inline int Vertex::texCoordOffset() { return offsetof(Vertex, m_texCoord); }
Q_DECL_CONSTEXPR inline int Vertex::normalOffset() { return offsetof(Vertex, m_normal); }
Q_DECL_CONSTEXPR inline int Vertex::tangentOffset() { return offsetof(Vertex, m_tangent); }
Q_DECL_CONSTEXPR inline int Vertex::bitangentOffset() { return offsetof(Vertex, m_bitangent); }
Q_DECL_CONSTEXPR inline int Vertex::stride() { return sizeof(Vertex); }

#endif // VERTEX_H
