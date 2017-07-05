#include "texturemanager.h"
#include <vector>
#include <QString>

struct TextureInstance
{
    QOpenGLTexture* m_texture;
    QString m_fileName;

    inline TextureInstance(QOpenGLTexture* texture, QString fileName) : m_texture(texture), m_fileName(fileName) {}
};

typedef std::vector<TextureInstance> TextureContainer;

TextureContainer sg_textureInstances;

void TextureManager::releaseTextureInstances()
{
    TextureContainer::iterator it = sg_textureInstances.begin();
    while (it != sg_textureInstances.end())
    {
        delete it->m_texture;
        it++;
    }
    sg_textureInstances.clear();
}

QOpenGLTexture* TextureManager::loadTexture(const QString &fileName)
{
    QOpenGLTexture* t = NULL;
    TextureContainer::iterator it = sg_textureInstances.begin();
    while (it != sg_textureInstances.end())
    {
        if (it->m_fileName == fileName)
        {
            t = it->m_texture;
            break;
        }
        it++;
    }
    if (t == NULL)
    {
        t = new QOpenGLTexture(QImage(fileName).mirrored().convertToFormat(QImage::Format_RGBA8888));
    }
    return t;
}

