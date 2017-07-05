#include "framebuffer.h"
#include "global.h"
#include "shadermanager.h"

FrameBuffer::~FrameBuffer()
{
    if (m_VBO != 0) m_funcs->glDeleteBuffers(1, &m_VBO);
    if (m_VAO != 0) m_funcs->glDeleteVertexArrays(1, &m_VAO);
    if (m_frameBuffer != 0) m_funcs->glDeleteFramebuffers(1, &m_frameBuffer);
    if (m_renderBuffer != 0) m_funcs->glDeleteRenderbuffers(1, &m_renderBuffer);
}

FrameBuffer::FrameBuffer()
    : m_colorBuffer(QOpenGLTexture::Target2D), m_hdr(false), m_enable(false)
{
    m_funcs = ShaderManager::getOpenGLFunctions();
    generateQuad();
}

FrameBuffer::FrameBuffer(bool useHDR)
    : m_colorBuffer(QOpenGLTexture::Target2D), m_hdr(useHDR), m_enable(false)
{
    m_funcs = ShaderManager::getOpenGLFunctions();
    generateQuad();
}

void FrameBuffer::update(GLuint defaultFramebuffer, int width, int height)
{
    m_defaultFrameBuffer = defaultFramebuffer;
    if (m_frameBuffer != 0) { m_funcs->glDeleteFramebuffers(1, &m_frameBuffer); m_frameBuffer = 0; }
    if (m_renderBuffer != 0) { m_funcs->glDeleteRenderbuffers(1, &m_renderBuffer); m_renderBuffer = 0; }
    if (m_colorBuffer.isCreated()) { m_colorBuffer.destroy(); }
    m_funcs->glGenFramebuffers(1, &m_frameBuffer);
    m_funcs->glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    m_colorBuffer.create();
    m_colorBuffer.setSize(width, height);
    if (m_hdr)
    {
        m_colorBuffer.setFormat(QOpenGLTexture::RGBA16F);
        m_colorBuffer.allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::Float16);
    }
    else
    {
        m_colorBuffer.setFormat(QOpenGLTexture::RGBA8_UNorm);
        m_colorBuffer.allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8);
    }
    m_colorBuffer.setWrapMode(QOpenGLTexture::ClampToEdge);
    m_colorBuffer.setMinificationFilter(QOpenGLTexture::Linear);
    m_colorBuffer.setMagnificationFilter(QOpenGLTexture::Linear);
    m_funcs->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer.textureId(), 0);
    m_funcs->glGenRenderbuffers(1, &m_renderBuffer);
    m_funcs->glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
    m_funcs->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    m_funcs->glBindRenderbuffer(GL_RENDERBUFFER, 0);
    m_funcs->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
    m_funcs->glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFrameBuffer);
}

void FrameBuffer::generateQuad()
{
    GLfloat quadVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    ShaderManager::getPostProcessingShader()->bind();
    ShaderManager::update_pp_hdr(m_hdr);
    m_funcs->glGenVertexArrays(1, &m_VAO);
    m_funcs->glGenBuffers(1, &m_VBO);
    m_funcs->glBindVertexArray(m_VAO);
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    m_funcs->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid*)0);
    m_funcs->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid*)(sizeof(GLfloat) * 2));
    m_funcs->glEnableVertexAttribArray(0);
    m_funcs->glEnableVertexAttribArray(1);
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_funcs->glBindVertexArray(0);
    ShaderManager::getPostProcessingShader()->release();
}

void FrameBuffer::draw()
{
    m_funcs->glBindVertexArray(m_VAO);
    m_colorBuffer.bind(FRAME_BUFFER_COLOR_BUFFER_UNIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_colorBuffer.release(FRAME_BUFFER_COLOR_BUFFER_UNIT);
    m_funcs->glBindVertexArray(0);
}

void FrameBuffer::bindFrameBuffer()
{
    m_funcs->glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
}

void FrameBuffer::bindColorBuffer()
{
    m_colorBuffer.bind(FRAME_BUFFER_COLOR_BUFFER_UNIT);
}

void FrameBuffer::releaseFrameBuffer()
{
    m_funcs->glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFrameBuffer);
}

void FrameBuffer::releaseColorBuffer()
{
    m_colorBuffer.release(FRAME_BUFFER_COLOR_BUFFER_UNIT);
}
