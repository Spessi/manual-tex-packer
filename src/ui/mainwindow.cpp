#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mTilesetMgr = nullptr;


//    connect(ui->inp_width, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DialogNewTileset::recalcDimension);
    connect(ui->tilesetView, &TilesetView::mouseReleased, this, &MainWindow::refreshSpriteLoaderUI);
}

MainWindow::~MainWindow()
{
    delete mTilesetMgr;
    delete ui;
}


/*
 * CUSTOM SLOTS
 */

void MainWindow::refreshSpriteLoaderUI() {
    if(mTilesetMgr->getSpriteLoader()->isFinished()) {
        ui->btn_sprites_playpause->setEnabled(false);
        ui->btn_sprites_stop->setEnabled(false);
        ui->btn_sprites_for->setEnabled(false);
        ui->btn_sprites_rew->setEnabled(false);
        ui->btn_importTileset->setEnabled(true);
        ui->lbl_sprites_queue->setText("Queue: -/-");
    }
    else {
        ui->lbl_sprites_queue->setText("Queue: " + QString::number(mTilesetMgr->getSpriteLoader()->getSpriteIndex()+1) + "/" + QString::number(mTilesetMgr->getSpriteLoader()->getSpritesCount()));
    }

}


/*
 * BUTTON SLOTS
 */

void MainWindow::on_btn_importTileset_clicked() {
    if(mTilesetMgr == nullptr)
        return;

    mTilesetMgr->getSpriteLoader()->setLoadPath("D:/Users/Marcel/Documents/Projekte/Handy/Android/MobileMan/data_new/tiles", SpriteLoader::Subdirectory);
    if(mTilesetMgr->getSpriteLoader()->getSpritesCount() > 0) {
        ui->btn_sprites_playpause->setEnabled(true);
        ui->btn_sprites_stop->setEnabled(true);
        ui->btn_sprites_for->setEnabled(true);
        ui->btn_sprites_rew->setEnabled(true);
        ui->btn_importTileset->setEnabled(false);

        ui->lbl_sprites_queue->setText("Queue: " + QString::number(1) + "/" + QString::number(mTilesetMgr->getSpriteLoader()->getSpritesCount()));
    }

    // Get first sprite
    Sprite* sprite = mTilesetMgr->getSpriteLoader()->getSprite(mTilesetMgr->getSpriteLoader()->getSpriteIndex());
    ui->tilesetView->addSpriteToScene(sprite, 0 ,0);

    // Allow adding Sprites
    ui->tilesetView->run();
}


void MainWindow::on_btn_sprites_playpause_clicked()
{
    if(ui->btn_sprites_playpause->text() == "►") {
        ui->btn_sprites_playpause->setText("▮▮");

        ui->tilesetView->run();
        ui->btn_sprites_for->setEnabled(true);
        ui->btn_sprites_rew->setEnabled(true);
        ui->tilesetView->addSpriteToScene(mTilesetMgr->getSpriteLoader()->getSprite(mTilesetMgr->getSpriteLoader()->getSpriteIndex()));
    }
    else {
        ui->btn_sprites_playpause->setText("►");

        ui->tilesetView->pause();
        ui->btn_sprites_for->setEnabled(false);
        ui->btn_sprites_rew->setEnabled(false);
        ui->tilesetView->removeSpriteFromScene(mTilesetMgr->getSpriteLoader()->getSprite(mTilesetMgr->getSpriteLoader()->getSpriteIndex()));
    }
}


void MainWindow::on_btn_sprites_for_clicked()
{
    Sprite* sprite;

    int oldIndex = mTilesetMgr->getSpriteLoader()->getSpriteIndex();
    if(mTilesetMgr->getSpriteLoader()->next() == 0) {
        // If next Sprite is available
        sprite = mTilesetMgr->getSpriteLoader()->getSprite(mTilesetMgr->getSpriteLoader()->getSpriteIndex());

        ui->tilesetView->removeSpriteFromScene(mTilesetMgr->getSpriteLoader()->getSprite(oldIndex));
        ui->tilesetView->addSpriteToScene(sprite);

        // Refresh UI
        refreshSpriteLoaderUI();
    }
    else {
        QApplication::beep();
    }

    qDebug() << mTilesetMgr->getSpriteLoader()->getSpriteIndex();
}


void MainWindow::on_btn_sprites_rew_clicked()
{
        Sprite* sprite;

        int oldIndex = mTilesetMgr->getSpriteLoader()->getSpriteIndex();
        if(mTilesetMgr->getSpriteLoader()->prev() == 0) {
            // If next Sprite is available
            sprite = mTilesetMgr->getSpriteLoader()->getSprite(mTilesetMgr->getSpriteLoader()->getSpriteIndex());

            ui->tilesetView->removeSpriteFromScene(mTilesetMgr->getSpriteLoader()->getSprite(oldIndex));
            ui->tilesetView->addSpriteToScene(sprite);

            // Refresh UI
            refreshSpriteLoaderUI();
        }
        else {
            QApplication::beep();
        }
        qDebug() << mTilesetMgr->getSpriteLoader()->getSpriteIndex();
}

void MainWindow::on_btn_sprites_stop_clicked()
{
    ui->tilesetView->removeSpriteFromScene(mTilesetMgr->getSpriteLoader()->getSprite(mTilesetMgr->getSpriteLoader()->getSpriteIndex()));
    mTilesetMgr->getSpriteLoader()->finished();

    refreshSpriteLoaderUI();

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

        ui->btn_importTileset->setEnabled(true);
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

    // Draw Sprites
    ui->tilesetView->addSpritesToScene(mTilesetMgr->getTileset(0)->getSprites());
    ui->tilesetView->pause();

    ui->btn_importTileset->setEnabled(true);
}

void MainWindow::on_actionSave_triggered()
{
    /*
    qDebug() << "Write!";
    mTileset->saveToFile("D:/Users/Marcel/Documents/Projekte/PC/Qt/ManualTexPack/project.xml");*/
}
