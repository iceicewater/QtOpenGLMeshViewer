#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector4D>
#include <QOpenGLTexture>

class Material
{
public:
    Material();

    void loadAmbientMap(const QString& fileName);
    void loadDiffuseMap(const QString& fileName);
    void loadSpecularMap(const QString& fileName);
    void loadNormalMap(const QString& fileName);
    void setAmbient(const QVector4D& ambient);
    void setAmbient(float ax, float ay, float az, float aw);
    void setDiffuse(const QVector4D& diffuse);
    void setDiffuse(float dx, float dy, float dz, float dw);
    void setSpecular(const QVector4D& Specular);
    void setSpecular(float sx, float sy, float sz, float sw);
    void setShininess(float shininess);
    void setMaterialName(const QString& matName);

    bool hasAmbientMap();
    bool hasDiffuseMap();
    bool hasSpecularMap();
    bool hasNormalMap();
    bool lockAmbientToDiffuse();
    void lockAmbientToDiffuse(bool lock);
    QOpenGLTexture* getAmbientMap() const;
    QOpenGLTexture* getDiffuseMap() const;
    QOpenGLTexture* getSpecularMap() const;
    QOpenGLTexture* getNormalMap() const;
    const QVector4D& getAmbient() const;
    const QVector4D& getDiffuse() const;
    const QVector4D& getSpecular() const;
    float getShininess() const;
    const QString& getMaterialName() const;

private:
    QString m_materialName;
    QOpenGLTexture* m_ambientMap;
    QOpenGLTexture* m_diffuseMap;
    QOpenGLTexture* m_specularMap;
    QOpenGLTexture* m_normalMap;
    QVector4D m_ambient;
    QVector4D m_diffuse;
    QVector4D m_specular;
    float m_shininess;
    bool m_lockAmbientToDiffuse;
};

inline void Material::setAmbient(const QVector4D &ambient) { m_ambient = ambient; }
inline void Material::setAmbient(float ax, float ay, float az, float aw) { setAmbient(QVector4D(ax, ay, az, aw)); }
inline void Material::setDiffuse(const QVector4D &diffuse) { m_diffuse = diffuse; }
inline void Material::setDiffuse(float dx, float dy, float dz, float dw) { setDiffuse(QVector4D(dx, dy, dz, dw)); }
inline void Material::setSpecular(const QVector4D &Specular) { m_specular = Specular; }
inline void Material::setSpecular(float sx, float sy, float sz, float sw) { setSpecular(QVector4D(sx, sy, sz, sw)); }
inline void Material::setShininess(float shininess) { m_shininess = shininess; }
inline void Material::setMaterialName(const QString &matName) { m_materialName = matName; }

inline bool Material::lockAmbientToDiffuse() { return m_lockAmbientToDiffuse; }
inline void Material::lockAmbientToDiffuse(bool lock) { m_lockAmbientToDiffuse = lock; }
inline bool Material::hasAmbientMap() { return (m_ambientMap != NULL); }
inline bool Material::hasDiffuseMap() { return (m_diffuseMap != NULL); }
inline bool Material::hasSpecularMap() { return (m_specularMap != NULL); }
inline bool Material::hasNormalMap() { return (m_normalMap != NULL); }
inline QOpenGLTexture* Material::getAmbientMap() const { return m_ambientMap; }
inline QOpenGLTexture* Material::getDiffuseMap() const { return m_diffuseMap; }
inline QOpenGLTexture* Material::getSpecularMap() const { return m_specularMap; }
inline QOpenGLTexture* Material::getNormalMap() const { return m_normalMap; }
inline const QVector4D& Material::getAmbient() const { return m_ambient; }
inline const QVector4D& Material::getDiffuse() const { return m_diffuse; }
inline const QVector4D& Material::getSpecular() const { return m_specular; }
inline float Material::getShininess() const { return m_shininess; }
inline const QString& Material::getMaterialName() const { return m_materialName; }

#endif // MATERIAL_H
