#include "shadermanager.h"
#include "lightmanager.h"

QOpenGLFunctions_3_3_Core* m_funcs = NULL;
QOpenGLShaderProgram *m_edgeShader = NULL;
QOpenGLShaderProgram *m_simpleShader = NULL;
QOpenGLShaderProgram *m_skyboxShader = NULL;
QOpenGLShaderProgram *m_phongShader = NULL;
QOpenGLShaderProgram *m_normalShader = NULL;
QOpenGLShaderProgram *m_postProcessingShader = NULL;

// UBO
GLuint m_UBO_Matrices;
GLuint m_UBO_Lights;

// Shader Information
int e_color;

int s_color;

int n_color;

int sk_skybox;

int pp_texture;
int pp_mode;
int pp_hdr;
int pp_exposure;

int p_flatShading;
int p_activateSkybox;
int p_skybox;
int p_lockAmbientToDiffuse;
int p_matAmbient;
int p_matDiffuse;
int p_matSpecular;
int p_matShininess;
int p_ambientMap;
int p_diffuseMap;
int p_specularMap;
int p_normalMap;
int p_useAmbientMap;
int p_useDiffuseMap;
int p_useSpecularMap;
int p_useNormalMap;

QOpenGLFunctions_3_3_Core* ShaderManager::getOpenGLFunctions() { return m_funcs; }

void ShaderManager::destroy()
{
    delete m_simpleShader;
    delete m_phongShader;
    delete m_skyboxShader;
    delete m_normalShader;
    delete m_edgeShader;
    delete m_postProcessingShader;
    m_funcs->glDeleteBuffers(1, &m_UBO_Matrices);
    m_funcs->glDeleteBuffers(1, &m_UBO_Lights);
}

void ShaderManager::initialize()
{
    m_funcs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

    initializeEdgeShader();
    initializeSimpleShader();
    initializeSkyboxShader();
    initializePhongShader();
    initializeNormalShader();
    initializePostProcessingShader();
    initializeMatricesUBO();
    initializeLightsUBO();
}

void ShaderManager::initializeEdgeShader()
{
    m_edgeShader = new QOpenGLShaderProgram();
    m_edgeShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/edge.vert");
    m_edgeShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
    m_edgeShader->link();

    e_color = m_edgeShader->uniformLocation("color");
}

void ShaderManager::initializeSimpleShader()
{
    m_simpleShader = new QOpenGLShaderProgram();
    m_simpleShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
    m_simpleShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
    m_simpleShader->link();

    s_color = m_simpleShader->uniformLocation("color");
}

void ShaderManager::initializeNormalShader()
{
    m_normalShader = new QOpenGLShaderProgram();
    m_normalShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/normal.vert");
    m_normalShader->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/normal.geom");
    m_normalShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
    m_normalShader->link();

    n_color = m_normalShader->uniformLocation("color");
}

void ShaderManager::initializeSkyboxShader()
{
    m_skyboxShader = new QOpenGLShaderProgram();
    m_skyboxShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/skybox.vert");
    m_skyboxShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple3d.frag");
    m_skyboxShader->link();

    sk_skybox = m_skyboxShader->uniformLocation("texture3d");
}

void ShaderManager::initializePostProcessingShader()
{
    m_postProcessingShader = new QOpenGLShaderProgram();
    m_postProcessingShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/quad.vert");
    m_postProcessingShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/postprocessing.frag");
    m_postProcessingShader->link();

    pp_texture = m_postProcessingShader->uniformLocation("texture2d");
    pp_mode = m_postProcessingShader->uniformLocation("mode");
    pp_hdr = m_postProcessingShader->uniformLocation("hdr");
    pp_exposure = m_postProcessingShader->uniformLocation("exposure");
}

void ShaderManager::initializePhongShader()
{
    m_phongShader = new QOpenGLShaderProgram();
    m_phongShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/phong.vert");
    m_phongShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/phong.frag");
    m_phongShader->link();

    p_flatShading = m_phongShader->uniformLocation("flatShading");
    p_activateSkybox = m_phongShader->uniformLocation("activateSkybox");
    p_skybox = m_phongShader->uniformLocation("skybox");
    p_matAmbient = m_phongShader->uniformLocation("materialAmbient");
    p_matDiffuse = m_phongShader->uniformLocation("materialDiffuse");
    p_matSpecular = m_phongShader->uniformLocation("materialSpecular");
    p_matShininess = m_phongShader->uniformLocation("materialShininess");
    p_lockAmbientToDiffuse = m_phongShader->uniformLocation("lockAmbientToDiffuse");
    p_ambientMap = m_phongShader->uniformLocation("ambientMap");
    p_diffuseMap = m_phongShader->uniformLocation("diffuseMap");
    p_specularMap = m_phongShader->uniformLocation("specularMap");
    p_normalMap = m_phongShader->uniformLocation("normalMap");
    p_useAmbientMap = m_phongShader->uniformLocation("useAmbientMap");
    p_useDiffuseMap = m_phongShader->uniformLocation("useDiffuseMap");
    p_useSpecularMap = m_phongShader->uniformLocation("useSpecularMap");
    p_useNormalMap = m_phongShader->uniformLocation("useNormalMap");
}

void ShaderManager::initializeLightsUBO()
{
    m_funcs->glGenBuffers(1, &m_UBO_Lights);
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, m_UBO_Lights);
    m_funcs->glBufferData(GL_UNIFORM_BUFFER, 16 + 144 * MAX_LIGHTS_NUMBER, NULL, GL_STREAM_DRAW);
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, 0);
    m_funcs->glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_UBO_Lights);

    GLuint p_UBO_lights = m_funcs->glGetUniformBlockIndex(m_phongShader->programId(), "Lights");
    m_funcs->glUniformBlockBinding(m_phongShader->programId(), p_UBO_lights, 1);
}

void ShaderManager::initializeMatricesUBO()
{
    m_funcs->glGenBuffers(1, &m_UBO_Matrices);
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, m_UBO_Matrices);
    m_funcs->glBufferData(GL_UNIFORM_BUFFER, 240, NULL, GL_STREAM_DRAW);
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, 0);
    m_funcs->glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_UBO_Matrices);

    GLuint s_UBO_matrices = m_funcs->glGetUniformBlockIndex(m_simpleShader->programId(), "Matrices");
    GLuint sk_UBO_matrices = m_funcs->glGetUniformBlockIndex(m_skyboxShader->programId(), "Matrices");
    GLuint p_UBO_matrices = m_funcs->glGetUniformBlockIndex(m_phongShader->programId(), "Matrices");
    GLuint n_UBO_matrices = m_funcs->glGetUniformBlockIndex(m_normalShader->programId(), "Matrices");
    m_funcs->glUniformBlockBinding(m_simpleShader->programId(), s_UBO_matrices, 0);
    m_funcs->glUniformBlockBinding(m_skyboxShader->programId(), sk_UBO_matrices, 0);
    m_funcs->glUniformBlockBinding(m_phongShader->programId(), p_UBO_matrices, 0);
    m_funcs->glUniformBlockBinding(m_normalShader->programId(), n_UBO_matrices, 0);
}

QOpenGLShaderProgram* ShaderManager::getEdgeShader()
{
    return m_edgeShader;
}

QOpenGLShaderProgram* ShaderManager::getSimpleShader()
{
    return m_simpleShader;
}

QOpenGLShaderProgram* ShaderManager::getNormalShader()
{
    return m_normalShader;
}

QOpenGLShaderProgram* ShaderManager::getSkyboxShader()
{
    return m_skyboxShader;
}

QOpenGLShaderProgram* ShaderManager::getPostProcessingShader()
{
    return m_postProcessingShader;
}

QOpenGLShaderProgram* ShaderManager::getPhongShader()
{
    return m_phongShader;
}

//================================================================

void ShaderManager::update_UBO_Matrices(const QMatrix4x4& projection, const QMatrix4x4& view)
{
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, m_UBO_Matrices);
    m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, projection.constData());
    m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, view.constData());
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShaderManager::update_UBO_Matrices(const QMatrix4x4& model, const QMatrix3x3& normal)
{
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, m_UBO_Matrices);
    m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, 128, 64, model.constData());
    m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, 192, 48, QMatrix4x4(normal).constData());
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShaderManager::update_UBO_Matrices(const QMatrix4x4& model)
{
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, m_UBO_Matrices);
    m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, 128, 64, model.constData());
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShaderManager::update_UBO_Lights(const QMatrix4x4& view)
{
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, m_UBO_Lights);
    int num = (int)LightManager::getLightInstances().size();
    m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &num);
    for (int i = 0; i < num; i++)
    {
        Light* light = LightManager::getLightInstances().at(i);
        int off = 144 * i + 16;
        m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, off + 0, 16, &light->getTranslation());
        m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, off + 16, 16, &light->getForwardVector());
        QVector3D pos_v(view * QVector4D(light->getTranslation(), 1.0f));
        m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, off + 32, 16, &pos_v);
        QVector3D dir_v(view * QVector4D(light->getForwardVector(), 0.0f));
        m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, off + 48, 16, &dir_v);
        m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, off + 64, 16, &light->getAmbient());
        m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, off + 80, 16, &light->getDiffuse());
        m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, off + 96, 16, &light->getSpecular());
        int lt = light->getLightType();
        m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, off + 112, 4, &lt);
        int at = light->getAttenuationType();
        m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, off + 116, 4, &at);
        float ico = light->getInnerCutOff();
        m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, off + 120, 4, &ico);
        float oco = light->getOuterCutOff();
        m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, off + 124, 4, &oco);
        float en = light->isEnabled();
        m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, off + 128, 4, &en);
    }
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

//================================================================

void ShaderManager::update_e_color(const QVector4D& c)
{
    m_edgeShader->setUniformValue(e_color, c);
}

//================================================================

void ShaderManager::update_s_color(const QVector4D& c)
{
    m_simpleShader->setUniformValue(s_color, c);
}

//================================================================

void ShaderManager::update_n_color(const QVector4D& c)
{
    m_normalShader->setUniformValue(n_color, c);
}

//================================================================

void ShaderManager::update_sk_skybox(int i)
{
    m_skyboxShader->setUniformValue(sk_skybox, i);
}

//================================================================

void ShaderManager::update_pp_texture(int i)
{
    m_postProcessingShader->setUniformValue(pp_texture, i);
}

void ShaderManager::update_pp_mode(int i)
{
    m_postProcessingShader->setUniformValue(pp_mode, i);
}

void ShaderManager::update_pp_hdr(bool hdr)
{
    m_postProcessingShader->setUniformValue(pp_hdr, hdr);
}

void ShaderManager::update_pp_exposure(float exposure)
{
    m_postProcessingShader->setUniformValue(pp_exposure, exposure);
}

//================================================================
void ShaderManager::update_p_flatShading(bool f)
{
    m_phongShader->setUniformValue(p_flatShading, f);
}

void ShaderManager::update_p_activateSkybox(bool a)
{
    m_phongShader->setUniformValue(p_activateSkybox, a);
}

void ShaderManager::update_p_skybox(int i)
{
    m_phongShader->setUniformValue(p_skybox, i);
}

void ShaderManager::update_p_lockAmbientToDiffuse(bool l)
{
    m_phongShader->setUniformValue(p_lockAmbientToDiffuse, l);
}

void ShaderManager::update_p_matAmbient(const QVector4D& v)
{
    m_phongShader->setUniformValue(p_useAmbientMap, false);
    m_phongShader->setUniformValue(p_matAmbient, v);
}

void ShaderManager::update_p_matDiffuse(const QVector4D& v)
{
    m_phongShader->setUniformValue(p_useDiffuseMap, false);
    m_phongShader->setUniformValue(p_matDiffuse, v);
}

void ShaderManager::update_p_matSpecular(const QVector4D& v)
{
    m_phongShader->setUniformValue(p_useSpecularMap, false);
    m_phongShader->setUniformValue(p_matSpecular, v);
}

void ShaderManager::update_p_matShininess(float s)
{
    m_phongShader->setUniformValue(p_matShininess, s);
}

void ShaderManager::update_p_ambientMap(int i)
{
    m_phongShader->setUniformValue(p_useAmbientMap, (bool)i);
    m_phongShader->setUniformValue(p_ambientMap, i);
}

void ShaderManager::update_p_diffuseMap(int i)
{
    m_phongShader->setUniformValue(p_useDiffuseMap, (bool)i);
    m_phongShader->setUniformValue(p_diffuseMap, i);
}

void ShaderManager::update_p_specularMap(int i)
{
    m_phongShader->setUniformValue(p_useSpecularMap, (bool)i);
    m_phongShader->setUniformValue(p_specularMap, i);
}

void ShaderManager::update_p_normalMap(int i)
{
    m_phongShader->setUniformValue(p_useNormalMap, (bool)i);
    m_phongShader->setUniformValue(p_normalMap, i);
}

