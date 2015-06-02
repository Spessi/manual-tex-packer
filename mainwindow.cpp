#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tilesetView->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_loadTileset_clicked() {
    SpriteLoader* spriteLoader = new SpriteLoader(QString("D:/Users/Marcel/Documents/Projekte/Handy/Android/MobileMan/data_new/tiles"));

    ui->tilesetView->loadSprites(spriteLoader);
}
