#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "renderoptions.h"
#include "helpdialog.h"
#include "glwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionOpen_File_triggered();
    void on_actionClear_Scene_triggered();
    void on_actionRender_Options_triggered();

private:
    Ui::MainWindow *ui;
    RenderOptions *renderoptions;
    HelpDialog *helpdialog;
    GLWidget *glwidget;
};

#endif // MAINWINDOW_H
