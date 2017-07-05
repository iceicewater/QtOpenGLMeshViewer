#include "renderoptions.h"
#include "ui_renderoptions.h"
#include "materialmanager.h"
#include "lightmanager.h"

RenderOptions::RenderOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenderOptions)
{
    ui->setupUi(this);
}

RenderOptions::~RenderOptions()
{
    delete ui;
}

void RenderOptions::update()
{
    ui->checkBox_backface->setChecked(glwidget->getBackFaceCulling());
    ui->checkBox_fillpoly->setChecked(glwidget->getFillPolygons());
    ui->checkBox_drawdeges->setChecked(glwidget->getDrawEdges());
    ui->checkBox_flat->setChecked(glwidget->getFlatShading());
    ui->checkBox_grid->setChecked(glwidget->isGridEnabled());
    ui->checkBox_normals->setChecked(glwidget->getDisplayNormals());
    ui->checkBox_post->setChecked(glwidget->isFrameBufferEnabled());
    ui->checkBox_skybox->setChecked(glwidget->isSkyboxEnabled());
    ui->comboBox_postMode->setCurrentIndex(glwidget->getPostProcessingMode());

    QString qss;
    QVector4D vec;
    vec = MaterialManager::getDefaultMaterial()->getAmbient();
    mat_ambi = QColor::fromRgbF(vec.x(), vec.y(), vec.z(), vec.w());
    qss = QString("background-color: %1").arg(mat_ambi.name());
    ui->pushButton_matAmbi->setStyleSheet(qss);
    vec = MaterialManager::getDefaultMaterial()->getDiffuse();
    mat_diff = QColor::fromRgbF(vec.x(), vec.y(), vec.z(), vec.w());
    qss = QString("background-color: %1").arg(mat_diff.name());
    ui->pushButton_matDiff->setStyleSheet(qss);
    vec = MaterialManager::getDefaultMaterial()->getSpecular();
    mat_spec = QColor::fromRgbF(vec.x(), vec.y(), vec.z(), vec.w());
    qss = QString("background-color: %1").arg(mat_spec.name());
    ui->pushButton_matSpec->setStyleSheet(qss);
    ui->spinBox_matShin->setValue(MaterialManager::getDefaultMaterial()->getShininess());
    vec = LightManager::getDefaultLight()->getAmbient();
    light_ambi = QColor::fromRgbF(vec.x(), vec.y(), vec.z(), vec.w());
    qss = QString("background-color: %1").arg(light_ambi.name());
    ui->pushButton_lightAmbi->setStyleSheet(qss);
    vec = LightManager::getDefaultLight()->getDiffuse();
    light_diff = QColor::fromRgbF(vec.x(), vec.y(), vec.z(), vec.w());
    qss = QString("background-color: %1").arg(light_diff.name());
    ui->pushButton_lightDiff->setStyleSheet(qss);
    vec = LightManager::getDefaultLight()->getSpecular();
    light_spec = QColor::fromRgbF(vec.x(), vec.y(), vec.z(), vec.w());
    qss = QString("background-color: %1").arg(light_spec.name());
    ui->pushButton_lightSpec->setStyleSheet(qss);
    vec = glwidget->getBackgroundColor();
    backgroundColor = QColor::fromRgbF(vec.x(), vec.y(), vec.z(), vec.w());
    qss = QString("background-color: %1").arg(backgroundColor.name());
    ui->pushButton_bgColor->setStyleSheet(qss);
}

void RenderOptions::on_RenderOptions_accepted()
{
    glwidget->setBackFaceCulling(ui->checkBox_backface->isChecked());
    glwidget->setFillPolygons(ui->checkBox_fillpoly->isChecked());
    glwidget->setDrawEdges(ui->checkBox_drawdeges->isChecked());
    glwidget->setFlatShading(ui->checkBox_flat->isChecked());
    glwidget->enableGrid(ui->checkBox_grid->isChecked());
    glwidget->setDisplayNormals(ui->checkBox_normals->isChecked());
    glwidget->enablePostProcessing(ui->checkBox_post->isChecked());
    glwidget->enableSkybox(ui->checkBox_skybox->isChecked());
    glwidget->setPostProcessingMode(ui->comboBox_postMode->currentIndex());

    MaterialManager::getDefaultMaterial()->setAmbient(mat_ambi.redF(), mat_ambi.greenF(), mat_ambi.blueF(), mat_ambi.alphaF());
    MaterialManager::getDefaultMaterial()->setDiffuse(mat_diff.redF(), mat_diff.greenF(), mat_diff.blueF(), mat_diff.alphaF());
    MaterialManager::getDefaultMaterial()->setSpecular(mat_spec.redF(), mat_spec.greenF(), mat_spec.blueF(), mat_spec.alphaF());
    MaterialManager::getDefaultMaterial()->setShininess(ui->spinBox_matShin->value());
    LightManager::getDefaultLight()->setAmbient(light_ambi.redF(), light_ambi.greenF(), light_ambi.blueF(), light_ambi.alphaF());
    LightManager::getDefaultLight()->setDiffuse(light_diff.redF(), light_diff.greenF(), light_diff.blueF(), light_diff.alphaF());
    LightManager::getDefaultLight()->setSpecular(light_spec.redF(), light_spec.greenF(), light_spec.blueF(), light_spec.alphaF());
    glwidget->setBackgroundColor(backgroundColor.redF(), backgroundColor.greenF(), backgroundColor.blueF(), backgroundColor.alphaF());
}

void RenderOptions::on_pushButton_matAmbi_clicked()
{
    mat_ambi = QColorDialog::getColor(Qt::white, this);
    QString qss = QString("background-color: %1").arg(mat_ambi.name());
    ui->pushButton_matAmbi->setStyleSheet(qss);
}

void RenderOptions::on_pushButton_matDiff_clicked()
{
    mat_diff = QColorDialog::getColor(Qt::white, this);
    QString qss = QString("background-color: %1").arg(mat_diff.name());
    ui->pushButton_matDiff->setStyleSheet(qss);
}

void RenderOptions::on_pushButton_matSpec_clicked()
{
    mat_spec = QColorDialog::getColor(Qt::white, this);
    QString qss = QString("background-color: %1").arg(mat_spec.name());
    ui->pushButton_matSpec->setStyleSheet(qss);
}

void RenderOptions::on_pushButton_lightAmbi_clicked()
{
    light_ambi = QColorDialog::getColor(Qt::white, this);
    QString qss = QString("background-color: %1").arg(light_ambi.name());
    ui->pushButton_lightAmbi->setStyleSheet(qss);
}

void RenderOptions::on_pushButton_lightDiff_clicked()
{
    light_diff = QColorDialog::getColor(Qt::white, this);
    QString qss = QString("background-color: %1").arg(light_diff.name());
    ui->pushButton_lightDiff->setStyleSheet(qss);
}

void RenderOptions::on_pushButton_lightSpec_clicked()
{
    light_spec = QColorDialog::getColor(Qt::white, this);
    QString qss = QString("background-color: %1").arg(light_spec.name());
    ui->pushButton_lightSpec->setStyleSheet(qss);
}

void RenderOptions::on_pushButton_bgColor_clicked()
{
    backgroundColor = QColorDialog::getColor(Qt::white, this);
    QString qss = QString("background-color: %1").arg(backgroundColor.name());
    ui->pushButton_bgColor->setStyleSheet(qss);
}
