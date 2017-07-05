#include "debugmanager.h"

DebugManager::DebugManager(QOpenGLWidget *glw)
    : m_debugLogger(0), fpscounter(0), glwidget(glw)
{

}

DebugManager::~DebugManager()
{
    if (fpscounter) delete fpscounter;
    if (m_debugLogger) delete m_debugLogger;
}

void DebugManager::startFPSCounter()
{
    if (!glwidget) return;
    fpscounter = new FPSCounter(glwidget);
    fpscounter->start(1000);
}

void DebugManager::initializeDebugLogger()
{
    if (!glwidget) return;
    m_debugLogger = new QOpenGLDebugLogger(glwidget);
    if (m_debugLogger->initialize())
    {
        qDebug() << "GL_DEBUG Debug Logger" << m_debugLogger;
        connect(m_debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(messageLogged(QOpenGLDebugMessage)));
        m_debugLogger->startLogging();
    }
}

void DebugManager::messageLogged(const QOpenGLDebugMessage &msg)
{
    QString error;

    // Format based on severity
    switch (msg.severity())
    {
        case QOpenGLDebugMessage::NotificationSeverity:
        error += "--";
        break;
        case QOpenGLDebugMessage::HighSeverity:
        error += "!!";
        break;
        case QOpenGLDebugMessage::MediumSeverity:
        error += "!~";
        break;
        case QOpenGLDebugMessage::LowSeverity:
        error += "~~";
        break;
    }

    error += " (";

  // Format based on source
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break
    switch (msg.source())
    {
        CASE(APISource);
        CASE(WindowSystemSource);
        CASE(ShaderCompilerSource);
        CASE(ThirdPartySource);
        CASE(ApplicationSource);
        CASE(OtherSource);
        CASE(InvalidSource);
    }
#undef CASE

    error += " : ";

    // Format based on type
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break
    switch (msg.type())
    {
        CASE(ErrorType);
        CASE(DeprecatedBehaviorType);
        CASE(UndefinedBehaviorType);
        CASE(PortabilityType);
        CASE(PerformanceType);
        CASE(OtherType);
        CASE(MarkerType);
        CASE(GroupPushType);
        CASE(GroupPopType);
    }
#undef CASE

    error += ")";
    qDebug() << qPrintable(error) << "\n" << qPrintable(msg.message()) << "\n";
}

void DebugManager::printContextInformation()
{
    if (!glwidget) return;
    QString glType;
    QString glVersion;
    QString glProfile;

    //get version info
    glType = glwidget->context()->isOpenGLES() ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    //get profile Information
    switch (glwidget->format().profile())
    {
        case QSurfaceFormat::NoProfile: glProfile = "NoProfile"; break;
        case QSurfaceFormat::CoreProfile: glProfile = "CoreProfile"; break;
        case QSurfaceFormat::CompatibilityProfile: glProfile = "CompatibilityProfile"; break;
    }
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
