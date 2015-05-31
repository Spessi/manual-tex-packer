#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_loadTileset_clicked() {
    TileLoader* tileLoader = new TileLoader(QString("D:/Users/Marcel/Documents/Projekte/Handy/Android/MobileMan/data_new/tiles"));
    ui->tilesetView->init(tileLoader);


    //ui->tilesetView->init("D:/Users/Marcel/Documents/Projekte/Handy/Android/MobileMan/data_new/tileset_unoptimized");
}
