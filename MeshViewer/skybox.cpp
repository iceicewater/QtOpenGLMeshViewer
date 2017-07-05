#include "skybox.h"
#include "global.h"

Skybox::Skybox()
    : m_cubeMap(QOpenGLTexture::TargetCubeMap), m_enable(false), m_loaded(false)
{
    m_funcs = ShaderManager::getOpenGLFunctions();
    generateCube();
}

Skybox::Skybox(const QStringList& textureFileList)
    : m_cubeMap(QOpenGLTexture::TargetCubeMap), m_enable(false)
{
    m_funcs = ShaderManager::getOpenGLFunctions();
    generateCube();
    loadCubeMap(textureFileList);
}

Skybox::~Skybox()
{
    if (m_VBO != 0) m_funcs->glDeleteBuffers(1, &m_VBO);
    if (m_VAO != 0) m_funcs->glDeleteVertexArrays(1, &m_VAO);
}

void Skybox::loadCubeMap(const QStringList& textureFileList)
{
    QImage posx = QImage(textureFileList.at(0)).convertToFormat(QImage::Format_RGBA8888);
    QImage posy = QImage(textureFileList.at(1)).convertToFormat(QImage::Format_RGBA8888);
    QImage posz = QImage(textureFileList.at(2)).convertToFormat(QImage::Format_RGBA8888);
    QImage negx = QImage(textureFileList.at(3)).convertToFormat(QImage::Format_RGBA8888);
    QImage negy = QImage(textureFileList.at(4)).convertToFormat(QImage::Format_RGBA8888);
    QImage negz = QImage(textureFileList.at(5)).convertToFormat(QImage::Format_RGBA8888);
    m_cubeMap.create();
    m_cubeMap.setSize(posx.width(), posx.height(), posx.depth());
    m_cubeMap.setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_cubeMap.allocateStorage();
    m_cubeMap.setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, posx.constBits(), 0);
    m_cubeMap.setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, posy.constBits(), 0);
    m_cubeMap.setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, posz.constBits(), 0);
    m_cubeMap.setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, negx.constBits(), 0);
    m_cubeMap.setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, negy.constBits(), 0);
    m_cubeMap.setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, negz.constBits(), 0);
    m_cubeMap.setWrapMode(QOpenGLTexture::ClampToEdge);
    m_cubeMap.setMinificationFilter(QOpenGLTexture::Linear);
    m_cubeMap.setMagnificationFilter(QOpenGLTexture::Linear);
    m_loaded = true;
}

void Skybox::generateCube()
{
    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    ShaderManager::getSkyboxShader()->bind();
    m_funcs->glGenVertexArrays(1, &m_VAO);
    m_funcs->glGenBuffers(1, &m_VBO);
    m_funcs->glBindVertexArray(m_VAO);
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    m_funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(GL_FLOAT) * 3, (GLvoid*)0);
    m_funcs->glEnableVertexAttribArray(0);
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_funcs->glBindVertexArray(0);
    ShaderManager::getSkyboxShader()->release();
}

void Skybox::draw()
{
    m_funcs->glBindVertexArray(m_VAO);
    if (m_cubeMap.isCreated()) m_cubeMap.bind(SKYBOX_TEXTURE_UNIT);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    if (m_cubeMap.isCreated()) m_cubeMap.release(SKYBOX_TEXTURE_UNIT);
    m_funcs->glBindVertexArray(0);
}

void Skybox::bind()
{
    m_cubeMap.bind(SKYBOX_TEXTURE_UNIT);
}

void Skybox::release()
{
    m_cubeMap.release(SKYBOX_TEXTURE_UNIT);
}
