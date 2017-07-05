#include "fpscounter.h"

FPSCounter::FPSCounter(QObject *parent)
    : QTimer(parent), fps(0)
{
    connect(this, SIGNAL(timeout()), this, SLOT(updateFPS()));
    connect(parent, SIGNAL(frameSwapped()), this, SLOT(updateOneFrame()));
}

void FPSCounter::updateOneFrame()
{
    fps++;
}

void FPSCounter::updateFPS()
{
    qDebug() << "Current FPS: " << fps;
    fps = 0;
}
