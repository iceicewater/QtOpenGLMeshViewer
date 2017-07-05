#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QOpenGLTexture>

class TextureManager
{
public:
    static QOpenGLTexture* loadTexture(const QString& textureFile);
    static void releaseTextureInstances();
};

#endif // TEXTUREMANAGER_H
