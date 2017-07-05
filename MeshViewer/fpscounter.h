#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <QTimer>
#include <QDebug>

class FPSCounter : public QTimer
{
    Q_OBJECT

private slots:
    void updateOneFrame();
    void updateFPS();

public:
    explicit FPSCounter(QObject* parent = 0);

private:
    int fps;
};

#endif // FPSCOUNTER_H
