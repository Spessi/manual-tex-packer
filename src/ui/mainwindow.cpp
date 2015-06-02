#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mTilesetMgr = nullptr;
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

void MainWindow::on_actionNew_triggered()
{
    int width=TILESET_WIDTH, height=TILESET_HEIGHT, tile_width=TILE_WIDTH, tile_height=TILE_HEIGHT;

    DialogNewTileset* dialog;
    dialog = new DialogNewTileset();
    if(dialog->exec()) {
        if(mTilesetMgr != nullptr)
            delete mTilesetMgr;

        // Get dimensions for TilesetView
        tile_width = dialog->getTileWidth();
        tile_height = dialog->getTileHeight();
        width = dialog->getWidth() * tile_width;
        height = dialog->getHeight() * tile_height;

        // Create TilesetMgr (aka ProjectManager)  and a new Tileset
        mTilesetMgr = new TilesetManager();
        mTilesetMgr->addTileset(new Tileset(width, height, tile_width, tile_height));

        // Show Tileset in TilesetView
        ui->tilesetView->init(mTilesetMgr);
    }
    delete dialog;
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
