#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include "debugmanager.h"
#include "inputmanager.h"
#include "entitymanager.h"
#include "materialmanager.h"
#include "texturemanager.h"
#include "lightmanager.h"
#include "nodemanager.h"
#include "shadermanager.h"
#include "skybox.h"
#include "camera3d.h"
#include "framebuffer.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

    bool getDrawEdges();
    bool getFillPolygons();
    bool getBackFaceCulling();
    bool getDisplayNormals();
    bool getFlatShading();
    int getPostProcessingMode();
    bool isSkyboxEnabled();
    bool isGridEnabled();
    bool isFrameBufferEnabled();
    const QVector4D& getBackgroundColor();
    Camera3D* getCamera();

    void setDrawEdges(bool f);
    void setFillPolygons(bool f);
    void setBackFaceCulling(bool f);
    void setDisplayNormals(bool f);
    void setFlatShading(bool f);
    void enableGrid(bool f);
    void enableSkybox(bool f);
    void enablePostProcessing(bool f);
    void setPostProcessingMode(int mode);
    void setBackgroundColor(float r, float g, float b, float a);

    void resetCamera();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

protected slots:
    void processFrame();

private:
    DebugManager* m_debugManager;

    Camera3D *m_camera;
    Entity3D *m_grid;
    Skybox *m_skybox;
    FrameBuffer *m_frameBuffer;

    // render controls
    bool m_drawEdges;
    bool m_fillPolygons;
    bool m_backFaceCulling;
    bool m_displayNormals;
    bool m_flatShading;
    int m_postProcessingMode;

    QVector4D m_edgeColor;
    QVector4D m_normalColor;
    QVector4D m_backgroundColor;

    void setupScene();
};

//bool getDrawEdges();
//bool getFillPolygons();
//bool getBackFaceCulling();
//bool getDisplayNormals();
//bool getFlatShading();
//bool isSkyboxEnabled();
//bool isGridEnabled();
//bool isFrameBufferEnabled();

inline bool GLWidget::getDrawEdges() { return m_drawEdges; }
inline bool GLWidget::getFillPolygons() { return m_fillPolygons; }
inline bool GLWidget::getBackFaceCulling() { return m_backFaceCulling; }
inline bool GLWidget::getDisplayNormals() { return m_displayNormals; }
inline bool GLWidget::getFlatShading() { return m_flatShading; }
inline int GLWidget::getPostProcessingMode() { return m_postProcessingMode; }
inline bool GLWidget::isSkyboxEnabled() { return m_skybox->isEnabled(); }
inline bool GLWidget::isGridEnabled() { return m_grid->isEnabled(); }
inline bool GLWidget::isFrameBufferEnabled() { return m_frameBuffer->isEnabled(); }
inline const QVector4D& GLWidget::getBackgroundColor() { return m_backgroundColor; }
inline Camera3D* GLWidget::getCamera() { return m_camera; }

#endif // GLWIDGET_H
