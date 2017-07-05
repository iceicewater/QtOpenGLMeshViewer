#ifndef CAMERA3D_H
#define CAMERA3D_H

#include "transform3d.h"

enum CameraMode
{
    Free,
    Target
};

enum ProjectionMode
{
    Perspective,
    Orthographic
};

class Camera3D
{
public:
    // Constructors
    Camera3D();

    void updateInput();

    // Constants
    static const QVector3D LocalForward;
    static const QVector3D LocalUp;
    static const QVector3D LocalRight;

    // Transform By
    void translate(const QVector3D &dt);
    void translate(float dx, float dy, float dz);
    void rotate(const QQuaternion &dr);
    void rotate(float angle, const QVector3D &axis);
    void rotate(float angle, float ax, float ay, float az);

    // Setters
    void setTranslation(const QVector3D &t);
    void setTranslation(float x, float y, float z);
    void setRotation(const QQuaternion &r);
    void setRotation(float angle, const QVector3D &axis);
    void setRotation(float angle, float ax, float ay, float az);
    void setTarget(const QVector3D &t);
    void setTarget(float x, float y, float z);
    void setCameraMode(CameraMode mode);
    void setProjectionMode(ProjectionMode mode);
    void setAspectRatio(float r);

    // Accessors
    const QVector3D& getTranslation() const;
    const QQuaternion& getRotation() const;
    const QVector3D& getTarget() const;
    const CameraMode& getCameraMode() const;
    const QMatrix4x4& getProjection() const;
    const QMatrix4x4& toMatrix();

    // Queries
    QVector3D getForwardVector() const;
    QVector3D getRightVector() const;
    QVector3D getUpVector() const;

private:
    bool m_dirty;

    CameraMode m_mode;
    QVector3D m_target;
    QVector3D m_translation;
    QQuaternion m_rotation;
    QMatrix4x4 m_world;

    //projection
    ProjectionMode p_mode;
    QMatrix4x4 m_projection;
    float left, right, bottom, top, zNear, zFar;
    float fov;		//field of view
    float aspectRatio;	//aspect radio
    float distance;	//distance from camera to target

#ifndef QT_NO_DATASTREAM
    friend QDataStream &operator<<(QDataStream &out, const Camera3D &transform);
    friend QDataStream &operator>>(QDataStream &in, Camera3D &transform);
#endif
};

Q_DECLARE_TYPEINFO(Camera3D, Q_MOVABLE_TYPE);

// Transform By (Add/Scale)
inline void Camera3D::translate(float dx, float dy,float dz) { translate(QVector3D(dx, dy, dz)); }
inline void Camera3D::rotate(float angle, const QVector3D &axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Camera3D::rotate(float angle, float ax, float ay,float az) { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Transform To (Setters)
inline void Camera3D::setTarget(float x, float y, float z) { setTarget(QVector3D(x, y, z)); }
inline void Camera3D::setTranslation(float x, float y, float z) { setTranslation(QVector3D(x, y, z)); }
inline void Camera3D::setRotation(float angle, const QVector3D &axis) { setRotation(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Camera3D::setRotation(float angle, float ax, float ay, float az) { setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Accessors
inline const QVector3D& Camera3D::getTarget() const { return m_target; }
inline const CameraMode& Camera3D::getCameraMode() const { return m_mode; }
inline const QVector3D& Camera3D::getTranslation() const { return m_translation; }
inline const QQuaternion& Camera3D::getRotation() const { return m_rotation; }
inline const QMatrix4x4& Camera3D::getProjection() const { return m_projection; }

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const Camera3D &transform);
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const Camera3D &transform);
QDataStream &operator>>(QDataStream &in, Camera3D &transform);
#endif

#endif // CAMERA3D_H
