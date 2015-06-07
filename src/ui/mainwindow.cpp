#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mProject = nullptr;


//    connect(ui->inp_width, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DialogNewTileset::recalcDimension);
    connect(ui->tilesetView, &TilesetView::mouseReleased, this, &MainWindow::refreshSpriteLoaderUI);
}

MainWindow::~MainWindow()
{
    delete mProject;
    delete ui;
}


/*
 * CUSTOM SLOTS
 */

void MainWindow::refreshSpriteLoaderUI() {
    if(mProject->getSpriteLoader()->isFinished()) {
        ui->btn_sprites_playpause->setEnabled(false);
        ui->btn_sprites_stop->setEnabled(false);
        ui->btn_sprites_for->setEnabled(false);
        ui->btn_sprites_rew->setEnabled(false);
        ui->btn_importSpritesFile->setEnabled(true);
        ui->btn_importSpritesDir->setEnabled(true);
        ui->lbl_sprites_queue->setText("Queue: -/-");
    }
    else {
        ui->lbl_sprites_queue->setText("Queue: " + QString::number(mProject->getSpriteLoader()->getSpriteIndex()+1) + "/" + QString::number(mProject->getSpriteLoader()->getSpritesCount()));
    }

}


/*
 * BUTTON SLOTS
 */


void MainWindow::on_btn_importSpritesFile_clicked()
{
    if(mProject == nullptr)
        return;


    // Import single file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Import image"), "", tr("Image Files (*.png)"));
    if(filePath.isNull())
        return;
    mProject->getSpriteLoader()->setLoadPath(filePath, SpriteLoader::File);

    // Get sprite
    Sprite* sprite = mProject->getSpriteLoader()->getSprite(mProject->getSpriteLoader()->getSpriteIndex());
    ui->tilesetView->addSpriteToScene(sprite, 0 ,0);

    // Allow adding Sprites
    ui->tilesetView->run();

    // Disable import buttons
    ui->btn_importSpritesFile->setEnabled(false);
    ui->btn_importSpritesDir->setEnabled(false);

}


void MainWindow::on_btn_importSpritesDir_clicked()
{
    QStringList folderPath;

    if(mProject == nullptr)
        return;

    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Import images"));
    dialog.setNameFilter(tr("Image files (*.png)"));
    dialog.setFileMode(QFileDialog::Directory);
    if(dialog.exec()) {
        folderPath = dialog.selectedFiles();
    }

    mProject->getSpriteLoader()->setLoadPath(folderPath.at(0), SpriteLoader::Subdirectory);
    if(mProject->getSpriteLoader()->getSpritesCount() > 0) {
        ui->btn_sprites_playpause->setEnabled(true);
        ui->btn_sprites_stop->setEnabled(true);
        ui->btn_sprites_for->setEnabled(true);
        ui->btn_sprites_rew->setEnabled(true);
        ui->btn_importSpritesFile->setEnabled(false);
        ui->btn_importSpritesDir->setEnabled(false);

        ui->lbl_sprites_queue->setText("Queue: " + QString::number(1) + "/" + QString::number(mProject->getSpriteLoader()->getSpritesCount()));

        // Get first sprite
        Sprite* sprite = mProject->getSpriteLoader()->getSprite(mProject->getSpriteLoader()->getSpriteIndex());
        ui->tilesetView->addSpriteToScene(sprite, 0 ,0);

        // Allow adding Sprites
        ui->tilesetView->run();
    }
}


void MainWindow::on_btn_sprites_playpause_clicked()
{
    if(ui->btn_sprites_playpause->text() == "►") {
        ui->btn_sprites_playpause->setText("▮▮");

        ui->tilesetView->run();
        ui->btn_sprites_for->setEnabled(true);
        ui->btn_sprites_rew->setEnabled(true);
        ui->tilesetView->addSpriteToScene(mProject->getSpriteLoader()->getSprite(mProject->getSpriteLoader()->getSpriteIndex()));
    }
    else {
        ui->btn_sprites_playpause->setText("►");

        ui->tilesetView->pause();
        ui->btn_sprites_for->setEnabled(false);
        ui->btn_sprites_rew->setEnabled(false);
        ui->tilesetView->removeSpriteFromScene(mProject->getSpriteLoader()->getSprite(mProject->getSpriteLoader()->getSpriteIndex()));
    }
}


void MainWindow::on_btn_sprites_for_clicked()
{
    Sprite* sprite;

    int oldIndex = mProject->getSpriteLoader()->getSpriteIndex();
    if(mProject->getSpriteLoader()->next() == 0) {
        // If next Sprite is available
        sprite = mProject->getSpriteLoader()->getSprite(mProject->getSpriteLoader()->getSpriteIndex());

        ui->tilesetView->removeSpriteFromScene(mProject->getSpriteLoader()->getSprite(oldIndex));
        ui->tilesetView->addSpriteToScene(sprite);

        // Refresh UI
        refreshSpriteLoaderUI();
    }
    else {
        QApplication::beep();
    }

    qDebug() << mProject->getSpriteLoader()->getSpriteIndex();
}


void MainWindow::on_btn_sprites_rew_clicked()
{
        Sprite* sprite;

        int oldIndex = mProject->getSpriteLoader()->getSpriteIndex();
        if(mProject->getSpriteLoader()->prev() == 0) {
            // If next Sprite is available
            sprite = mProject->getSpriteLoader()->getSprite(mProject->getSpriteLoader()->getSpriteIndex());

            ui->tilesetView->removeSpriteFromScene(mProject->getSpriteLoader()->getSprite(oldIndex));
            ui->tilesetView->addSpriteToScene(sprite);

            // Refresh UI
            refreshSpriteLoaderUI();
        }
        else {
            QApplication::beep();
        }
        qDebug() << mProject->getSpriteLoader()->getSpriteIndex();
}

void MainWindow::on_btn_sprites_stop_clicked()
{
    ui->tilesetView->removeSpriteFromScene(mProject->getSpriteLoader()->getSprite(mProject->getSpriteLoader()->getSpriteIndex()));
    mProject->getSpriteLoader()->finished();

    refreshSpriteLoaderUI();

}


/*
 * MENUBAR ACTIONS
 */

void MainWindow::on_actionNew_triggered()
{
    int width=VIEW_WIDTH, height=VIEW_HEIGHT, tile_width=TILE_WIDTH, tile_height=TILE_HEIGHT;

    DialogNewTileset* dialog;
    dialog = new DialogNewTileset();
    if(dialog->exec()) {
        if(mProject != nullptr)
            delete mProject;

        // Get dimensions for TilesetView
        tile_width = dialog->getTileWidth();
        tile_height = dialog->getTileHeight();
        width = dialog->getWidth() * tile_width;
        height = dialog->getHeight() * tile_height;

        // Create TilesetMgr (aka ProjectManager)  and a new Tileset
        mProject = new Project();
        mProject->addTileset(new Tileset(width, height, tile_width, tile_height));

        // Show Tileset in TilesetView
        ui->tilesetView->init(mProject);

        ui->btn_importSpritesFile->setEnabled(true);
        ui->btn_importSpritesDir->setEnabled(true);
    }
    delete dialog;
}

void MainWindow::on_actionLoad_triggered()
{
    // Load project configuration
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("TexProject Files (*.texproj)"));
    if(filePath.isNull())
        return;

    if(mProject != nullptr)
        delete mProject;

    mProject = new Project();

    mProject->loadFromFile(filePath);

    // Draw grid and initialize other things
    ui->tilesetView->init(mProject);

    // Draw Sprites
    ui->tilesetView->addSpritesToScene(mProject->getTileset(0)->getSprites());
    ui->tilesetView->pause();

    ui->btn_importSpritesFile->setEnabled(true);
    ui->btn_importSpritesDir->setEnabled(true);
}

void MainWindow::on_actionSave_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Project"), "", tr("TexProject Files (*.texproj)"));
    if(filePath.isNull())
        return;
    mProject->saveToFile(filePath);
}



