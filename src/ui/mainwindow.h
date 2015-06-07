#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "tilesetview.h"
#include "spriteloader.h"
#include "project.h"
#include "ui/dialognewtileset.h"

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
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_actionNew_triggered();
    void on_btn_sprites_playpause_clicked();
    void refreshSpriteLoaderUI();
    void on_btn_sprites_rew_clicked();
    void on_btn_sprites_for_clicked();
    void on_btn_sprites_stop_clicked();
    void on_btn_importSpritesFile_clicked();
    void on_btn_importSpritesDir_clicked();

private:
    Ui::MainWindow *ui;
    Project* mProject;
};

#endif // MAINWINDOW_H
