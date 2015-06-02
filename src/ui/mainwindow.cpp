#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mTilesetMgr = nullptr;

    // Create new Tileset with default settings
    on_actionNew_triggered();
}

MainWindow::~MainWindow()
{
    delete mTilesetMgr;
    delete ui;
}


/*
 * BUTTON SLOTS
 */

void MainWindow::on_btn_importTileset_clicked() {
    if(mTilesetMgr == nullptr)
        return;

    mTilesetMgr->getSpriteLoader()->setLoadPath("D:/Users/Marcel/Documents/Projekte/Handy/Android/MobileMan/data_new/tiles", SpriteLoader::Directory);

    Sprite* sprite;
    mTilesetMgr->getSpriteLoader()->getNextSprite(&sprite);
    ui->tilesetView->addSpriteToScene(sprite, 0, 0);
}


/*
 * MENUBAR ACTIONS
 */
DialogNewTileset* d1;
void MainWindow::on_actionNew_triggered()
{
    int width=TILESET_WIDTH, height=TILESET_HEIGHT, tile_width=TILE_WIDTH, tile_height=TILE_HEIGHT;

    if(mTilesetMgr != nullptr) {
        delete mTilesetMgr;
        d1 = new DialogNewTileset();
        if(d1->exec()) {
            tile_width = d1->getTileWidth();
            tile_height = d1->getTileHeight();
            width = d1->getWidth() * tile_width;
            height = d1->getHeight() * tile_height;
        }
    }

    mTilesetMgr = new TilesetManager();
    mTilesetMgr->addTileset(new Tileset(width, height, tile_width, tile_height));
    ui->tilesetView->init(mTilesetMgr);

}

void MainWindow::on_actionLoad_triggered()
{

    if(mTilesetMgr != nullptr)
        delete mTilesetMgr;

    mTilesetMgr = new TilesetManager();
    // Load project configuration
    mTilesetMgr->loadFromFile("D:/Users/Marcel/Documents/Projekte/PC/Qt/ManualTexPack/project.xml");

    // Draw grid and initialize other things
    ui->tilesetView->init(mTilesetMgr);
    ui->tilesetView->addSpritesToScene(mTilesetMgr->getTileset(0)->getSprites());
}

void MainWindow::on_actionSave_triggered()
{
    /*
    qDebug() << "Write!";
    mTileset->saveToFile("D:/Users/Marcel/Documents/Projekte/PC/Qt/ManualTexPack/project.xml");*/
}
