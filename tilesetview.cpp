#include "tilesetview.h"

Sprite* mSelectedSprite;
QGraphicsRectItem* mSelectedSpriteBorder;

TilesetView::TilesetView(QWidget *parent) :  QGraphicsView(parent)
{
    setMouseTracking(true);
    mSpriteLoader = nullptr;

    mTileset = new Tileset(TILESET_WIDTH, TILESET_HEIGHT, TILE_WIDTH, TILE_HEIGHT);

    mScene = new QGraphicsScene();
    mScene->setSceneRect(0, 0, TILESET_WIDTH, TILESET_HEIGHT);
    setScene(mScene);
}

void TilesetView::init() {
    setGeometry(x(), y(), TILESET_WIDTH + 3, TILESET_HEIGHT);
    setSceneRect(0, 0, TILESET_WIDTH + 3, TILESET_HEIGHT);


    // Add vertical grid
    for(int i = 0; i <= TILESET_WIDTH; i += TILE_WIDTH) {
        mScene->addLine(i, 0, i, TILESET_HEIGHT, QPen(QColor(0, 0, 0, 64)));
    }


    // Add horizontal grid
    for(int i = 0; i <= TILESET_HEIGHT; i += TILE_HEIGHT) {
        mScene->addLine(0, i, TILESET_WIDTH, i, QPen(QColor(0, 0, 0, 64)));
    }

    // Initialize SelectedSpriteBorder (Rectangle)
    mSelectedSpriteBorder = new QGraphicsRectItem();
    mSelectedSpriteBorder->setZValue(99);
    mScene->addItem(mSelectedSpriteBorder);
}

void TilesetView::loadSprites(SpriteLoader* spriteLoader) {
    mSpriteLoader = spriteLoader;

    // Get first Sprite and locate it at 0,0
    mSelectedSprite = mSpriteLoader->getSprite();
    showSprite(mSelectedSprite, 0, 0);
}

/**
 * @brief Adds Sprite to scene. Will be movable until User placed it with a left-click.
 * @param sprite Sprite to be shown
 * @param x x display position
 * @param y y display position
 */
void TilesetView::showSprite(Sprite* sprite, float x, float y) {
    if((mSpriteLoader != nullptr) && (sprite != nullptr)) {
        // Set Sprite to position x,y
        sprite->setPos(x, y);

        // Add Sprite Pixmap to Scene to make it visible
        mScene->addItem(sprite->getPixmapItem());

        // Move the rectangle to the border of the Sprite
        mSelectedSpriteBorder->setRect(mMouseTilePosX, mMouseTilePosY, mSelectedSprite->getWidth(), mSelectedSprite->getHeight());

        // Check collision
        if(mTileset->checkCollision(mSelectedSprite)) {
            mSelectedSpriteBorder->setPen(QPen(Qt::red));
        }
        else {
            mSelectedSpriteBorder->setPen(QPen(Qt::black));
        }
    }
}

void TilesetView::mouseMoveEvent(QMouseEvent* event) {
    int mouseX, mouseY;

    // Do some caluclations
    mouseX = event->pos().x();
    mouseY = event->pos().y();
    mMouseTilePosX = mouseX / TILE_WIDTH * TILE_WIDTH;
    mMouseTilePosY = mouseY / TILE_HEIGHT * TILE_HEIGHT;


    // If a Sprite is selected
    if(mSelectedSprite != nullptr) {
        // Move the current selected Sprite to the mouse position
        mSelectedSprite->getPixmapItem()->setPos(mMouseTilePosX, mMouseTilePosY);
        mSelectedSpriteBorder->setRect(mMouseTilePosX, mMouseTilePosY, mSelectedSprite->getWidth(), mSelectedSprite->getHeight());

        if(mTileset->checkCollision(mSelectedSprite)) {
            mSelectedSpriteBorder->setPen(QPen(Qt::red));
        }
        else {
            mSelectedSpriteBorder->setPen(QPen(Qt::black));
        }
    }
}

void TilesetView::mouseReleaseEvent(QMouseEvent* event) {

    if(event->button() == Qt::LeftButton) {
        // Load and show every Sprite until SpriteLoader has finished
        // User has to put the Sprite in place
        if(mSpriteLoader->isFinished() == false) {
            // Check collision with existing Sprites
            if(mTileset->checkCollision(mSelectedSprite) == false) {
                // Add Sprite to Tileset for management and storing
                mTileset->addSprite(mSelectedSprite);

                mSelectedSprite = mSpriteLoader->getSprite();

                if(mSelectedSprite != nullptr) {
                    showSprite(mSelectedSprite, mMouseTilePosX, mMouseTilePosY);
                    mSelectedSpriteBorder->setVisible(true);
                }
                else {
                    mSelectedSpriteBorder->setVisible(false);
                }
            }
            else {
                // Make system sound to signalize the collision
                QApplication::beep();
            }
        }
    }
}
