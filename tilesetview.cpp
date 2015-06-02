#include "tilesetview.h"

/* TODO
 * - Sprites werden aus XML importiert und in Tileset Liste gespeichert -> check
 * - Geladene Sprites müssen gezeichnet werden, evtl. TilesetView noch mehr von Logik trennen.
 * */

QGraphicsRectItem* mSelectedSpriteBorder;

TilesetView::TilesetView(QWidget *parent) :  QGraphicsView(parent) {
    setMouseTracking(true);
    mTilesetMgr = nullptr;
    mSpriteLoader = nullptr;
    mSelectedSpriteBorder = nullptr;
    mMouseTilePosX = mMouseTilePosY = 0;


    mScene = new QGraphicsScene();
    mScene->setSceneRect(0, 0, TILESET_WIDTH, TILESET_HEIGHT);
    setScene(mScene);
}

void TilesetView::init(TilesetManager* tilesetMgr) {
    mTilesetMgr = tilesetMgr;

    setGeometry(x(), y(), TILESET_WIDTH + 3, TILESET_HEIGHT);
    setSceneRect(0, 0, TILESET_WIDTH + 3, TILESET_HEIGHT);

    // Remove ALL items from Scene _AND_ delete them
    mScene->clear();


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

void TilesetView::addSpriteToScene(Sprite* sprite, int x, int y) {
    if(sprite != nullptr) {
        sprite->setPos(x, y);
        mScene->addItem(sprite->getPixmapItem());

        // Move the rectangle to the border of the Sprite
        mSelectedSpriteBorder->setRect(mMouseTilePosX, mMouseTilePosY, sprite->getWidth(), sprite->getHeight());
        mSelectedSpriteBorder->setVisible(true);

        // Check collision
        if(mTilesetMgr->getTileset(0)->checkCollision(sprite)) {
            mSelectedSpriteBorder->setPen(QPen(Qt::red));
        }
        else {
            mSelectedSpriteBorder->setPen(QPen(Qt::black));
        }
    }
    else {
        mSelectedSpriteBorder->setVisible(false);
    }
}

void TilesetView::addSpritesToScene(QList<Sprite*> sprites) {
    for(int i=0; i <sprites.size(); i++) {
        mScene->addItem(sprites.at(i)->getPixmapItem());
    }
}


void TilesetView::mouseMoveEvent(QMouseEvent* event) {
    int mouseX, mouseY;

    // Do some caluclations
    mouseX = event->pos().x();
    mouseY = event->pos().y();
    mMouseTilePosX = mouseX / TILE_WIDTH * TILE_WIDTH;
    mMouseTilePosY = mouseY / TILE_HEIGHT * TILE_HEIGHT;

    if(mTilesetMgr == nullptr)
        return;

    if(mTilesetMgr->getSpriteLoader()->isFinished() == false) {
        mTilesetMgr->getSpriteLoader()->getCurrentSprite()->setPos(mMouseTilePosX, mMouseTilePosY);
        mSelectedSpriteBorder->setRect(mMouseTilePosX, mMouseTilePosY, mTilesetMgr->getSpriteLoader()->getCurrentSprite()->getWidth(), mTilesetMgr->getSpriteLoader()->getCurrentSprite()->getHeight());

        // Check collision
        if(mTilesetMgr->getTileset(0)->checkCollision(mTilesetMgr->getSpriteLoader()->getCurrentSprite())) {
            mSelectedSpriteBorder->setPen(QPen(Qt::red));
        }
        else {
            mSelectedSpriteBorder->setPen(QPen(Qt::black));
        }
    }

}

void TilesetView::mouseReleaseEvent(QMouseEvent* event) {

    if(event->button() == Qt::LeftButton) {
        if(mTilesetMgr->getSpriteLoader()->isFinished() == false) {
            // Check collision with existing Sprites
            if(mTilesetMgr->getTileset(0)->checkCollision(mTilesetMgr->getSpriteLoader()->getCurrentSprite()) == false) {
                // Add Sprite to Tileset for management and storing
                mTilesetMgr->getTileset(0)->addSprite(mTilesetMgr->getSpriteLoader()->getCurrentSprite());

                if(mTilesetMgr->getSpriteLoader()->getCurrentSprite() != nullptr) {
                    Sprite* sprite;
                    mTilesetMgr->getSpriteLoader()->getNextSprite(&sprite);
                    addSpriteToScene(sprite, mMouseTilePosX, mMouseTilePosY);
                }
            }
            else {
                // Make system sound to signalize the collision
                QApplication::beep();
            }
        }
    }
}

