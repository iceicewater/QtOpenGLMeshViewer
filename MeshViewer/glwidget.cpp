#include "glwidget.h"
#include <QKeyEvent>

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_edgeColor(0.4f, 0.4f, 0.9f, 1.0f),
    m_normalColor(0.9f, 0.9f, 0.4f, 1.0f),
    m_backgroundColor(0.2f, 0.2f, 0.2f, 1.0f),
    m_drawEdges(false),
    m_fillPolygons(true),
    m_backFaceCulling(true),
    m_displayNormals(false),
    m_flatShading(false),
    m_postProcessingMode(0),
    m_camera(NULL),
    m_grid(NULL),
    m_skybox(NULL),
    m_frameBuffer(NULL)
{
    connect(this, SIGNAL(frameSwapped()), this, SLOT(processFrame()));
}

GLWidget::~GLWidget()
{
    makeCurrent();
    NodeManager::releaseNodeInstances();
    EntityManager::releaseEntityInstances();
    MaterialManager::releaseMaterialInstances();
    TextureManager::releaseTextureInstances();
    LightManager::releaseLightInstances();
    delete m_skybox;
    delete m_grid;
    delete m_camera;
    delete m_debugManager;
    ShaderManager::destroy();
    doneCurrent();
}

void GLWidget::initializeGL()
{
    //initialize opengl backend
    initializeOpenGLFunctions();
    m_debugManager = new DebugManager(this);
    m_debugManager->printContextInformation();
    m_debugManager->initializeDebugLogger();
    //m_debugManager->startFPSCounter();

    MaterialManager::initialize();
    LightManager::initialize();
    ShaderManager::initialize();
    ShaderManager::getPhongShader()->bind();
    ShaderManager::update_p_flatShading(m_flatShading);
    ShaderManager::getPhongShader()->release();
    ShaderManager::getNormalShader()->bind();
    ShaderManager::update_n_color(m_normalColor);
    ShaderManager::getNormalShader()->release();
    ShaderManager::getEdgeShader()->bind();
    ShaderManager::update_e_color(m_edgeColor);
    ShaderManager::getEdgeShader()->release();
    ShaderManager::getPostProcessingShader()->bind();
    ShaderManager::update_pp_exposure(1.0f);
    ShaderManager::getPostProcessingShader()->release();

    //set global info
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_STENCIL_TEST);
    //glEnable(GL_BLEND);
    glClearDepth(1.0f);
    glClearColor(m_backgroundColor.x(), m_backgroundColor.y(), m_backgroundColor.z(), m_backgroundColor.w());
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //glStencilFunc(GL_ALWAYS, 1, 0xFF);

    //default objects
    m_camera = new Camera3D();
    resetCamera();
    m_grid = new Entity3D();
    m_grid->loadMesh(PrimitiveType::Grid);
    m_grid->setDrawMode(GL_LINES);
    Material* gridMat = MaterialManager::getNewMaterial();
    gridMat->setDiffuse(0.5f, 0.5f, 0.5f, 0.5f);
    m_grid->getMeshInstances()->at(0)->setMaterial(gridMat);
    m_skybox = new Skybox();
    m_frameBuffer = new FrameBuffer(false);

    setupScene();
}

void GLWidget::setupScene()
{
    Light* directionalLight = LightManager::getNewLight();
    directionalLight->setLightType(LightType::Directional);
    directionalLight->translate(0.1f, 1.0f, 0.1f);
    directionalLight->rotate(45.0f, 1.0f, 1.0f, 0.0f);
    directionalLight->setAmbient(0.3f, 0.3f, 0.3f, 1.0f);
    directionalLight->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
    directionalLight->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);

    Entity3D* ground = EntityManager::loadEntity(PrimitiveType::Plane);
    Material* groundMat = MaterialManager::getNewMaterial();
    ground->getMeshInstances()->at(0)->setMaterial(groundMat);
    groundMat->loadDiffuseMap(":/images/brickwall.jpg");
    groundMat->loadNormalMap(":/images/brickwall_normal.jpg");
    groundMat->setSpecular(0.2f, 0.2f, 0.2f, 1.0f);
    groundMat->setShininess(128.0f);
    Node3D* groundNode = NodeManager::getNewNode();
    groundNode->attachChild(ground);
    groundNode->scale(20.0f);

    Entity3D* sphere = EntityManager::loadEntity(PrimitiveType::Sphere);
    Material* sphereMat = MaterialManager::getNewMaterial();
    sphereMat->setDiffuse(0.1f, 0.1f, 0.1f, 1.0f);
    sphereMat->setSpecular(0.8f, 0.8f, 0.8f, 1.0f);
    sphereMat->setShininess(256.0f);
    sphere->getMeshInstances()->at(0)->setMaterial(sphereMat);
    Node3D* sphereNode = NodeManager::getNewNode();
    sphereNode->attachChild(sphere);
    sphereNode->setTranslation(-3.0f, 1.0f, 0.0f);

    Entity3D* box = EntityManager::loadEntity(PrimitiveType::Cube);
    Material* boxMat = MaterialManager::getNewMaterial();
    box->getMeshInstances()->at(0)->setMaterial(boxMat);
    boxMat->loadDiffuseMap(":/images/container2.png");
    boxMat->loadSpecularMap(":/images/container2_specular.png");
    Node3D* boxNode = NodeManager::getNewNode();
    boxNode->attachChild(box);
    boxNode->setTranslation(3.0f, 0.0f, 0.0f);
}

void GLWidget::resizeGL(int width, int height)
{
    if (height == 0) height = 1;
    float aspectratio = float(width) / float(height);
    if (m_camera) m_camera->setAspectRatio(aspectratio);
    if (m_frameBuffer) m_frameBuffer->update(defaultFramebufferObject(), width, height);
}

void GLWidget::paintGL()
{
    if (m_frameBuffer->isEnabled()) m_frameBuffer->bindFrameBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderManager::update_UBO_Matrices(m_camera->getProjection(), m_camera->toMatrix());

    //draw m_grid
    if (m_grid->isEnabled())
    {
        ShaderManager::getSimpleShader()->bind();
        QMatrix4x4 m;
        m.setToIdentity();
        ShaderManager::update_UBO_Matrices(m);
        ShaderManager::update_s_color(m_grid->getMeshInstances()->at(0)->getMaterial()->getDiffuse());
        m_grid->getMeshInstances()->at(0)->drawMesh();
        ShaderManager::getSimpleShader()->release();
    }

    //update lighting
    if (m_fillPolygons)
    {
        ShaderManager::update_UBO_Lights(m_camera->toMatrix());
        ShaderManager::getPhongShader()->bind();
        if (m_skybox->isEnabled())
        {
            ShaderManager::update_p_activateSkybox(true);
            ShaderManager::update_p_skybox(SKYBOX_TEXTURE_UNIT);
            m_skybox->bind();
        }
        else ShaderManager::update_p_activateSkybox(false);
        ShaderManager::getPhongShader()->release();
    }

    NodeContainer::iterator it_node = NodeManager::getNodeInstances().begin();
    while (it_node != NodeManager::getNodeInstances().end())
    {
        Node3D* node = *it_node;
        Entity3D* entity = node->getChild();
        if (!entity->isEnabled()) { it_node++; continue;}
        ShaderManager::update_UBO_Matrices(node->toMatrix(), (m_camera->toMatrix() * node->toMatrix()).normalMatrix());

        //draw entity
        if (m_fillPolygons)
        {
            ShaderManager::getPhongShader()->bind();
            for (int i = 0; i < entity->getMeshInstances()->size(); i++)
            {
                Material* material;
                if (entity->getOverrideMaterial() != NULL) material = entity->getOverrideMaterial();
                else material = entity->getMeshInstances()->at(i)->getMaterial();
                ShaderManager::update_p_lockAmbientToDiffuse(material->lockAmbientToDiffuse());
                ShaderManager::update_p_matShininess(material->getShininess());
                if (material->getAmbientMap()) ShaderManager::update_p_ambientMap(AMBIENT_MAP_UNIT);
                else ShaderManager::update_p_matAmbient(material->getAmbient());
                if (material->getDiffuseMap()) ShaderManager::update_p_diffuseMap(DIFFUSE_MAP_UNIT);
                else ShaderManager::update_p_matDiffuse(material->getDiffuse());
                if (material->getSpecularMap()) ShaderManager::update_p_specularMap(SPECULAR_MAP_UNIT);
                else ShaderManager::update_p_matSpecular(material->getSpecular());
                if (material->getNormalMap()) ShaderManager::update_p_normalMap(NORMAL_MAP_UNIT);
                else ShaderManager::update_p_normalMap(0);
                entity->getMeshInstances()->at(i)->drawMesh();
            }
            ShaderManager::getPhongShader()->release();
        }

        //display normals
        if (m_displayNormals)
        {
            ShaderManager::getNormalShader()->bind();
            for (int i = 0; i < entity->getMeshInstances()->size(); i++)
            {
                entity->getMeshInstances()->at(i)->drawMesh();
            }
            ShaderManager::getNormalShader()->release();
        }

        //draw edges
        if (m_drawEdges)
        {
            glDisable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            ShaderManager::getEdgeShader()->bind();
            for (int i = 0; i < entity->getMeshInstances()->size(); i++)
            {
                entity->getMeshInstances()->at(i)->drawMesh();
            }
            ShaderManager::getEdgeShader()->release();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            if (m_backFaceCulling) glEnable(GL_CULL_FACE);
        }

        it_node++;
    }

    //draw skybox
    if (m_skybox->isEnabled())
    {
        ShaderManager::getSkyboxShader()->bind();
        ShaderManager::update_sk_skybox(SKYBOX_TEXTURE_UNIT);
        m_skybox->draw();
        ShaderManager::getSkyboxShader()->release();
    }

    if (m_frameBuffer->isEnabled())
    {
        m_frameBuffer->releaseFrameBuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        ShaderManager::getPostProcessingShader()->bind();
        ShaderManager::update_pp_texture(FRAME_BUFFER_COLOR_BUFFER_UNIT);
        m_frameBuffer->draw();
        ShaderManager::getPostProcessingShader()->release();
        glEnable(GL_DEPTH_TEST);
    }
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) event->ignore();
    else InputManager::registerKeyPress(event->key());
    InputManager::registerModifiers(event->modifiers());
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) event->ignore();
    else InputManager::registerKeyRelease(event->key());
    InputManager::registerModifiers(event->modifiers());
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    InputManager::registerMousePress(event->button());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    InputManager::registerMouseRelease(event->button());
}

void GLWidget::processFrame()
{
    // Update input
    InputManager::update();
    m_camera->updateInput();

    if (InputManager::keyTriggered(Qt::Key_Z)) resetCamera();

    if (InputManager::keyTriggered(Qt::Key_F1)) enableGrid(!m_grid->isEnabled());
    if (InputManager::keyTriggered(Qt::Key_F2)) setBackFaceCulling(!m_backFaceCulling);
    if (InputManager::keyTriggered(Qt::Key_F3)) setFillPolygons(!m_fillPolygons);
    if (InputManager::keyTriggered(Qt::Key_F4)) setDrawEdges(!m_drawEdges);
    if (InputManager::keyTriggered(Qt::Key_F5)) setFlatShading(!m_flatShading);
    if (InputManager::keyTriggered(Qt::Key_F6)) setDisplayNormals(!m_displayNormals);
    if (InputManager::keyTriggered(Qt::Key_F7)) enableSkybox(!m_skybox->isEnabled());
    if (InputManager::keyTriggered(Qt::Key_F8)) enablePostProcessing(!m_frameBuffer->isEnabled());

    if (InputManager::keyTriggered(Qt::Key_1)) setPostProcessingMode(0);
    if (InputManager::keyTriggered(Qt::Key_2)) setPostProcessingMode(1);
    if (InputManager::keyTriggered(Qt::Key_3)) setPostProcessingMode(2);
    if (InputManager::keyTriggered(Qt::Key_4)) setPostProcessingMode(3);
    if (InputManager::keyTriggered(Qt::Key_5)) setPostProcessingMode(4);
    if (InputManager::keyTriggered(Qt::Key_6)) setPostProcessingMode(5);
    if (InputManager::keyTriggered(Qt::Key_0)) setPostProcessingMode(9);

    if (LightManager::getDefaultLight()->isEnabled())
        LightManager::getDefaultLight()->setTranslation(m_camera->getTranslation());

    update();
}

void GLWidget::resetCamera()
{
    m_camera->setTranslation(10.0f, 10.0f, 10.0f);
    m_camera->setTarget(0.0f, 0.0f, 0.0f);
}

void GLWidget::setDrawEdges(bool f)
{
    if (m_fillPolygons) m_drawEdges = f;
    else m_drawEdges = true;
}

void GLWidget::setFillPolygons(bool f)
{
    m_fillPolygons = f;
    if (!m_fillPolygons) m_drawEdges = true;
}

void GLWidget::enableGrid(bool f)
{
    if (f) m_grid->enable();
    else m_grid->disable();
}

void GLWidget::enableSkybox(bool f)
{
    if (!m_skybox) return;
    if (f) m_skybox->enable();
    else m_skybox->disable();
    if (m_skybox->isEnabled() && !m_skybox->isLoaded())
    {
        QStringList files;
        files << ":/images/skybox/right.jpg"
            << ":/images/skybox/top.jpg"
            << ":/images/skybox/back.jpg"
            << ":/images/skybox/left.jpg"
            << ":/images/skybox/bottom.jpg"
            << ":/images/skybox/front.jpg";
        makeCurrent();
        m_skybox->loadCubeMap(files);
        doneCurrent();
    }
}

void GLWidget::enablePostProcessing(bool f)
{
    if (!m_frameBuffer) return;
    if (f) m_frameBuffer->enable();
    else m_frameBuffer->disable();
}

void GLWidget::setBackFaceCulling(bool f)
{
    m_backFaceCulling = f;
    makeCurrent();
    if (m_backFaceCulling) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);
    doneCurrent();
}

void GLWidget::setDisplayNormals(bool f)
{
    m_displayNormals = f;
}

void GLWidget::setFlatShading(bool f)
{
    m_flatShading = f;
    makeCurrent();
    ShaderManager::getPhongShader()->bind();
    ShaderManager::update_p_flatShading(m_flatShading);
    ShaderManager::getPhongShader()->release();
    doneCurrent();
}

void GLWidget::setPostProcessingMode(int mode)
{
    m_postProcessingMode = mode;
    makeCurrent();
    ShaderManager::getPostProcessingShader()->bind();
    ShaderManager::update_pp_mode(m_postProcessingMode);
    ShaderManager::getPostProcessingShader()->release();
    doneCurrent();
}

void GLWidget::setBackgroundColor(float r, float g, float b, float a)
{
    m_backgroundColor = QVector4D(r, g, b, a);
    makeCurrent();
    glClearColor(m_backgroundColor.x(), m_backgroundColor.y(), m_backgroundColor.z(), m_backgroundColor.w());
    doneCurrent();
}
