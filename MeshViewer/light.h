#ifndef LIGHT_H
#define LIGHT_H

#include "global.h"
#include "transform3d.h"
#include <QVector4D>

enum LightType
{
    Point,
    Directional,
    Spotlight
};

enum AttenuationType
{
    Constant,
    Linear,
    Quadratic
};

class Light : public Transform3D
{
public:
    Light();

    void setLightType(LightType type);
    void setAttenuationType(AttenuationType type);
    void setAmbient(const QVector4D& ambient);
    void setAmbient(float ax, float ay, float az, float aw);
    void setDiffuse(const QVector4D& diffuse);
    void setDiffuse(float dx, float dy, float dz, float dw);
    void setSpecular(const QVector4D& Specular);
    void setSpecular(float sx, float sy, float sz, float sw);
    void setInnerCutOff(float cutoff);
    void setOuterCutOff(float cutoff);
    void enable();
    void disable();

    LightType getLightType() const;
    AttenuationType getAttenuationType() const;
    const QVector4D& getAmbient() const;
    const QVector4D& getDiffuse() const;
    const QVector4D& getSpecular() const;
    float getInnerCutOff() const;
    float getOuterCutOff() const;
    bool isEnabled() const;

private:
    bool m_enable;
    LightType m_lightType;
    AttenuationType m_attenuationType;
    QVector4D m_ambient;
    QVector4D m_diffuse;
    QVector4D m_specular;
    float m_innerCutOff;
    float m_outerCutOff;
};

inline void Light::setLightType(LightType type) { m_lightType = type; }
inline void Light::setAttenuationType(AttenuationType type) { m_attenuationType = type; }
inline void Light::setAmbient(const QVector4D &ambient) { m_ambient = ambient; }
inline void Light::setAmbient(float ax, float ay, float az, float aw) { setAmbient(QVector4D(ax, ay, az, aw)); }
inline void Light::setDiffuse(const QVector4D &diffuse) { m_diffuse = diffuse; }
inline void Light::setDiffuse(float dx, float dy, float dz, float dw) { setDiffuse(QVector4D(dx, dy, dz, dw)); }
inline void Light::setSpecular(const QVector4D &Specular) { m_specular = Specular; }
inline void Light::setSpecular(float sx, float sy, float sz, float sw) { setSpecular(QVector4D(sx, sy, sz, sw)); }
inline void Light::setInnerCutOff(float cutoff) { m_innerCutOff = cutoff; }
inline void Light::setOuterCutOff(float cutoff) { m_outerCutOff = cutoff; }
inline void Light::enable() { m_enable = true; }
inline void Light::disable() { m_enable = false; }

inline LightType Light::getLightType() const { return m_lightType; }
inline AttenuationType Light::getAttenuationType() const { return m_attenuationType; }
inline const QVector4D& Light::getAmbient() const { return m_ambient; }
inline const QVector4D& Light::getDiffuse() const { return m_diffuse; }
inline const QVector4D& Light::getSpecular() const { return m_specular; }
inline float Light::getInnerCutOff() const { return cos(D_TO_R(m_innerCutOff)); }
inline float Light::getOuterCutOff() const { return cos(D_TO_R(m_outerCutOff)); }
inline bool Light::isEnabled() const { return m_enable; }

#endif // LIGHT_H
