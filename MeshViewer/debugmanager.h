#ifndef DEBUGMANAGER_H
#define DEBUGMANAGER_H

#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>
#include <QDebug>
#include <QOpenGLWidget>
#include "fpscounter.h"

class DebugManager : public QObject
{
    Q_OBJECT

public:
    explicit DebugManager(QOpenGLWidget *glw = 0);
    ~DebugManager();

    void initializeDebugLogger();
    void printContextInformation();
    void startFPSCounter();

protected slots:
    void messageLogged(const QOpenGLDebugMessage &msg);

private:
    QOpenGLWidget *glwidget;
    FPSCounter *fpscounter;
    QOpenGLDebugLogger *m_debugLogger;
};

#endif // DEBUGMANAGER_H
