#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tilesetview.h"
#include "spriteloader.h"
#include "tilesetmanager.h"
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

    void on_btn_importTileset_clicked();

private:
    Ui::MainWindow *ui;
    TilesetManager* mTilesetMgr;
};

#endif // MAINWINDOW_H
