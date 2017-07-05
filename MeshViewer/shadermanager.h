#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QVector4D>

class ShaderManager
{
public:
    static QOpenGLFunctions_3_3_Core* getOpenGLFunctions();

    static void initialize();
    static void initializeEdgeShader();
    static void initializeSimpleShader();
    static void initializeSkyboxShader();
    static void initializePhongShader();
    static void initializeNormalShader();
    static void initializePostProcessingShader();
    static void initializeMatricesUBO();
    static void initializeLightsUBO();
    static void destroy();

    static QOpenGLShaderProgram* getEdgeShader();
    static QOpenGLShaderProgram* getSimpleShader();
    static QOpenGLShaderProgram* getSkyboxShader();
    static QOpenGLShaderProgram* getPhongShader();
    static QOpenGLShaderProgram* getNormalShader();
    static QOpenGLShaderProgram* getPostProcessingShader();

    static void update_UBO_Matrices(const QMatrix4x4& projection, const QMatrix4x4& view);
    static void update_UBO_Matrices(const QMatrix4x4& model, const QMatrix3x3& normal);
    static void update_UBO_Matrices(const QMatrix4x4& model);
    static void update_UBO_Lights(const QMatrix4x4& view);

    static void update_e_color(const QVector4D& c);

    static void update_s_color(const QVector4D& c);

    static void update_n_color(const QVector4D& c);

    static void update_sk_skybox(int i);

    static void update_pp_texture(int i);
    static void update_pp_mode(int i);
    static void update_pp_hdr(bool hdr);
    static void update_pp_exposure(float e);

    static void update_p_flatShading(bool f);
    static void update_p_activateSkybox(bool a);
    static void update_p_skybox(int i);
    static void update_p_lockAmbientToDiffuse(bool l);
    static void update_p_matAmbient(const QVector4D& v);
    static void update_p_matDiffuse(const QVector4D& v);
    static void update_p_matSpecular(const QVector4D& v);
    static void update_p_matShininess(float s);
    static void update_p_ambientMap(int i);
    static void update_p_diffuseMap(int i);
    static void update_p_specularMap(int i);
    static void update_p_normalMap(int i);
};

#endif // SHADERMANAGER_H
