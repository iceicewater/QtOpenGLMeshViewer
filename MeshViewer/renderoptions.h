#ifndef RENDEROPTIONS_H
#define RENDEROPTIONS_H

#include <QDialog>
#include <QColorDialog>
#include "glwidget.h"

namespace Ui {
class RenderOptions;
}

class RenderOptions : public QDialog
{
    Q_OBJECT

public:
    explicit RenderOptions(QWidget *parent = 0);
    ~RenderOptions();

    void update();
    void setGLWidget(GLWidget* glw);

private slots:
    void on_RenderOptions_accepted();
    void on_pushButton_matAmbi_clicked();
    void on_pushButton_matDiff_clicked();
    void on_pushButton_matSpec_clicked();
    void on_pushButton_lightAmbi_clicked();
    void on_pushButton_lightDiff_clicked();
    void on_pushButton_lightSpec_clicked();
    void on_pushButton_bgColor_clicked();

private:
    Ui::RenderOptions *ui;
    GLWidget* glwidget;

    QColor mat_ambi;
    QColor mat_diff;
    QColor mat_spec;
    QColor light_ambi;
    QColor light_diff;
    QColor light_spec;
    QColor backgroundColor;
};

inline void RenderOptions::setGLWidget(GLWidget* glw) { glwidget = glw; }

#endif // RENDEROPTIONS_H
