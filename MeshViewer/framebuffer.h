#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "shadermanager.h"
#include <QOpenGLTexture>

class FrameBuffer
{
public:
    FrameBuffer();
    FrameBuffer(bool useHDR);
    ~FrameBuffer();

    void update(GLuint defaultFramebuffer, int width, int height);
    void bindFrameBuffer();
    void bindColorBuffer();
    void releaseFrameBuffer();
    void releaseColorBuffer();
    void draw();

    void enable();
    void disable();
    bool isEnabled();

private:
    bool m_enable;
    QOpenGLFunctions_3_3_Core* m_funcs;
    GLuint m_defaultFrameBuffer = 0;
    GLuint m_VBO = 0;
    GLuint m_VAO = 0;
    GLuint m_frameBuffer = 0;
    GLuint m_renderBuffer = 0;
    QOpenGLTexture m_colorBuffer;
    bool m_hdr;

    void generateQuad();
};

inline void FrameBuffer::enable() { m_enable = true; }
inline void FrameBuffer::disable() { m_enable = false; }
inline bool FrameBuffer::isEnabled() { return m_enable; }

#endif // FRAMEBUFFER_H
