#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "entitymanager.h"
#include "nodemanager.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    renderoptions(new RenderOptions(this)),
    helpdialog(new HelpDialog(this)),
    glwidget(new GLWidget(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(glwidget);
    glwidget->setFocusPolicy(Qt::StrongFocus);
    renderoptions->setGLWidget(glwidget);
}

MainWindow::~MainWindow()
{
    delete glwidget;
    delete renderoptions;
    delete helpdialog;
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    ui->statusBar->showMessage(tr("Quit Mesh Viewer"), 1000);
    QApplication::instance()->quit();
}

void MainWindow::on_actionAbout_triggered()
{
    ui->statusBar->showMessage(tr("About Mesh Viewer"), 1000);
    helpdialog->show();
}

void MainWindow::on_actionOpen_File_triggered()
{
    ui->statusBar->showMessage(tr("Open file"), 1000);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Model File"), "/home", tr("Model Files (*.obj *.m)"));
    if (fileName != "")
    {
        ui->statusBar->showMessage("File selected: " + fileName, 1000);
        Entity3D* newEntity = EntityManager::loadEntity(fileName);
        Node3D* newNode = NodeManager::getNewNode();
        newNode->attachChild(newEntity);
        Camera3D* camera = glwidget->getCamera();
        camera->setTranslation((newNode->getTranslation() + newEntity->getMaxPoint()) * 1.5f);
        camera->setTarget(newNode->getTranslation() + newEntity->getCenterPoint());
    }
    else ui->statusBar->showMessage(tr("No file selected"), 1000);
}

void MainWindow::on_actionClear_Scene_triggered()
{
    ui->statusBar->showMessage(tr("Clear scene"), 1000);
    NodeManager::releaseNodeInstances();
    glwidget->resetCamera();
}

void MainWindow::on_actionRender_Options_triggered()
{
    ui->statusBar->showMessage(tr("Render Options"), 1000);
    renderoptions->show();
    renderoptions->update();
}
