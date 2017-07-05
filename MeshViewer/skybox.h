#ifndef SKYBOX_H
#define SKYBOX_H

#include <QOpenGLTexture>
#include "shadermanager.h"

class Skybox
{
public:
    Skybox();
    Skybox(const QStringList& textureFileList);
    ~Skybox();

    void loadCubeMap(const QStringList& textureFileList);
    void draw();
    void bind();
    void release();

    void enable();
    void disable();
    bool isEnabled();
    bool isLoaded();

private:
    bool m_enable;
    bool m_loaded;
    QOpenGLTexture m_cubeMap;
    GLuint m_VBO = 0;
    GLuint m_VAO = 0;
    QOpenGLFunctions_3_3_Core* m_funcs;

    void generateCube();
};

inline void Skybox::enable() { m_enable = true; }
inline void Skybox::disable() { m_enable = false; }
inline bool Skybox::isEnabled() { return m_enable; }
inline bool Skybox::isLoaded() { return m_loaded; }

#endif // SKYBOX_H
