#include "camera3d.h"
#include "inputmanager.h"
#include "global.h"
#include <QDebug>

const QVector3D Camera3D::LocalForward(0.0f, 0.0f, -1.0f);
const QVector3D Camera3D::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Camera3D::LocalRight(1.0f, 0.0f, 0.0f);

Camera3D::Camera3D()
    : m_dirty(true),
      m_mode(CameraMode::Target),
      p_mode(ProjectionMode::Perspective),
      fov(60.0f)
{

}

void Camera3D::updateInput()
{
    if (m_mode == CameraMode::Free && InputManager::buttonPressed(Qt::RightButton))
    {
        static const float f_transSpeed = 0.3f;
        static const float f_rotSpeed   = 0.2f;

        // Handle rotations
        rotate(-f_rotSpeed * InputManager::mouseDelta().x(), Camera3D::LocalUp);
        rotate(-f_rotSpeed * InputManager::mouseDelta().y(), getRightVector());

        // Handle translations
        QVector3D translation;
        if (InputManager::keyPressed(Qt::Key_W))
        {
            translation += getForwardVector();
        }
        if (InputManager::keyPressed(Qt::Key_S))
        {
            translation -= getForwardVector();
        }
        if (InputManager::keyPressed(Qt::Key_A))
        {
            translation -= getRightVector();
        }
        if (InputManager::keyPressed(Qt::Key_D))
        {
            translation += getRightVector();
        }
        if (InputManager::keyPressed(Qt::Key_Q))
        {
            translation -= getUpVector();
        }
        if (InputManager::keyPressed(Qt::Key_E))
        {
            translation += getUpVector();
        }
        translate(f_transSpeed * translation);
    }
    else if (m_mode == CameraMode::Target)
    {
        static const float t_transSpeed = 0.01f;
        static const float t_rotSpeed   = 0.2f;
        static const float t_zoomSpeed   = 0.05f;
        if (InputManager::buttonPressed(Qt::MidButton) ||
           (InputManager::buttonPressed(Qt::LeftButton) && InputManager::buttonPressed(Qt::RightButton)))
        {
            translate(-t_transSpeed * InputManager::mouseDelta().x() * getRightVector());
            translate(t_transSpeed * InputManager::mouseDelta().y() * getUpVector());
        }
        else if (InputManager::buttonPressed(Qt::LeftButton))
        {
            rotate(-t_rotSpeed * InputManager::mouseDelta().x(), Camera3D::LocalUp);
            rotate(-t_rotSpeed * InputManager::mouseDelta().y(), getRightVector());
        }
        else if (InputManager::buttonPressed(Qt::RightButton))
        {
            translate(t_zoomSpeed * InputManager::mouseDelta().x() * getForwardVector());
            translate(t_zoomSpeed * InputManager::mouseDelta().y() * getForwardVector());
        }
    }
}

// Transform By
void Camera3D::translate(const QVector3D &dt)
{
    m_dirty = true;
    if (m_mode == CameraMode::Free)
    {
        m_translation += dt;
    }
    else if (m_mode == CameraMode::Target)
    {
        distance = (m_target - m_translation).length();
        m_translation += dt * distance * 0.1f;
        setAspectRatio(aspectRatio);
    }
}

void Camera3D::rotate(const QQuaternion &dr)
{
    m_dirty = true;
    if (m_mode == CameraMode::Free)
    {
        m_rotation = dr * m_rotation;
    }
    else if (m_mode == CameraMode::Target)
    {
        m_rotation = dr * m_rotation;
        QVector3D delta_old = m_target - m_translation;
        QVector3D delta_new = dr.rotatedVector(delta_old);
        m_translation += delta_old - delta_new;
    }
}

// Setters
void Camera3D::setTranslation(const QVector3D &t)
{
    m_dirty = true;
    if (m_mode == CameraMode::Free)
    {
        m_translation = t;
    }
    else if (m_mode == CameraMode::Target)
    {
        m_translation = t;
        distance = (m_target - m_translation).length();
        setAspectRatio(aspectRatio);
    }
}

void Camera3D::setRotation(const QQuaternion &r)
{
    m_dirty = true;
    if (m_mode == CameraMode::Free)
    {
        m_rotation = r;
    }
    else if (m_mode == CameraMode::Target)
    {
        m_rotation = r;
        QVector3D delta_old = (m_target - m_translation);
        QVector3D delta_new = getForwardVector();
        m_translation += delta_old - delta_new;
    }
}

void Camera3D::setTarget(const QVector3D &t)
{
    m_target = t;
    m_dirty = true;
    QVector3D delta = m_translation - m_target;
    m_rotation = QQuaternion::fromDirection(delta, Camera3D::LocalUp);
    distance = delta.length();
    setAspectRatio(aspectRatio);
}

void Camera3D::setCameraMode(CameraMode mode)
{
    m_mode = mode;
    if(mode == CameraMode::Target) setTarget(m_translation + getForwardVector() * 5);
}

void Camera3D::setProjectionMode(ProjectionMode mode)
{
    p_mode = mode;
    m_projection.setToIdentity();
    if (p_mode == ProjectionMode::Perspective)
    {
        m_projection.perspective(fov, aspectRatio, zNear, zFar);
    }
    else if (p_mode == ProjectionMode::Orthographic)
    {
        m_projection.ortho(left, right, bottom, top, zNear, zFar);
    }
}

void Camera3D::setAspectRatio(float r)
{
    aspectRatio = r;
    zNear = 0.1f * distance;
    zFar = distance >= 50.0f ? 2.0f * distance : 100.0f;
    m_projection.setToIdentity();
    if (p_mode == ProjectionMode::Perspective)
    {
        m_projection.perspective(fov, aspectRatio, zNear, zFar);
    }
    else if (p_mode == ProjectionMode::Orthographic)
    {
        left = -aspectRatio * distance * (float)tan(D_TO_R(fov / 2.0f));
        right = aspectRatio * distance * (float)tan(D_TO_R(fov / 2.0f));
        bottom = -distance * (float)tan(D_TO_R(fov / 2.0f));
        top = distance * (float)tan(D_TO_R(fov / 2.0f));
        m_projection.ortho(left, right, bottom, top, zNear, zFar);
    }
}

// Accessors
const QMatrix4x4 &Camera3D::toMatrix()
{
    if (m_dirty)
    {
        m_dirty = false;
        m_world.setToIdentity();
        m_world.rotate(m_rotation.conjugated());
        m_world.translate(-m_translation);
    }
    return m_world;
}

// Queries
QVector3D Camera3D::getForwardVector() const
{
    return m_rotation.rotatedVector(LocalForward);
}

QVector3D Camera3D::getRightVector() const
{
    return m_rotation.rotatedVector(LocalRight);
}

QVector3D Camera3D::getUpVector() const
{
    return m_rotation.rotatedVector(LocalUp);
}

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const Camera3D &transform)
{
    dbg << "Camera3D\n{\n";
    dbg << "Position: <" << transform.getTranslation().x() << ", " << transform.getTranslation().y() << ", " << transform.getTranslation().z() << ">\n";
    dbg << "Rotation: <" << transform.getRotation().x() << ", " << transform.getRotation().y() << ", " << transform.getRotation().z() << " | " << transform.getRotation().scalar() << ">\n}";
    return dbg;
}
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const Camera3D &transform)
{
    out << transform.m_translation;
    out << transform.m_rotation;
    return out;
}

QDataStream &operator>>(QDataStream &in, Camera3D &transform)
{
    in >> transform.m_translation;
    in >> transform.m_rotation;
    transform.m_dirty = true;
    return in;
}
#endif
