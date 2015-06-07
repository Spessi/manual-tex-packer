#include "tilesetview.h"

/* TODO
 * - Sprites werden aus XML importiert und in Tileset Liste gespeichert -> check
 * - Geladene Sprites müssen gezeichnet werden, evtl. TilesetView noch mehr von Logik trennen.
 * */

QGraphicsRectItem* mSelectedSpriteBorder;
bool mSpriteIsFloating;

TilesetView::TilesetView(QWidget *parent) :  QGraphicsView(parent) {
    setMouseTracking(true);
    mTilesetMgr = nullptr;
    mSpriteLoader = nullptr;
    mSelectedSprite = nullptr;
    mSelectedSpriteBorder = nullptr;
    mMouseTilePosX = mMouseTilePosY = mMouseX = mMouseY = 0;
    mSpriteIsFloating = false;

    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    mScene = new QGraphicsScene();
    mScene->setSceneRect(0, 0, TILESET_WIDTH, TILESET_HEIGHT);
    setScene(mScene);
}

void TilesetView::init(TilesetManager* tilesetMgr) {
    mTilesetMgr = tilesetMgr;
    this->setEnabled(true);

    int width = TILESET_WIDTH;
    int height = TILESET_HEIGHT;
    int sceneWidth = mTilesetMgr->getTileset(0)->getWidth();
    int sceneHeight = mTilesetMgr->getTileset(0)->getHeight();

    if(sceneWidth >= width) {
        width += qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    }
    if(sceneHeight >= height) {
        height += qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    }

    mScene->setSceneRect(0, 0, sceneWidth, sceneHeight);
    setScene(mScene);
    setGeometry(x(), y(), width, height);


    // Remove ALL items from Scene _AND_ delete them
    mScene->clear();


    // Add vertical grid
    for(int i = 0; i <= mTilesetMgr->getTileset(0)->getWidth(); i += mTilesetMgr->getTileset(0)->getTileWidth()) {
        mScene->addLine(i, 0, i, mTilesetMgr->getTileset(0)->getHeight(), QPen(QColor(0, 0, 0, 64)));
    }


    // Add horizontal grid
    for(int i = 0; i <= mTilesetMgr->getTileset(0)->getHeight(); i += mTilesetMgr->getTileset(0)->getTileHeight()) {
        mScene->addLine(0, i, mTilesetMgr->getTileset(0)->getWidth(), i, QPen(QColor(0, 0, 0, 64)));
    }

    // Initialize SelectedSpriteBorder (Rectangle)
    mSelectedSpriteBorder = new QGraphicsRectItem();
    mSelectedSpriteBorder->setZValue(99);
    mScene->addItem(mSelectedSpriteBorder);
}

void TilesetView::addSpriteToScene(Sprite* sprite) {
    addSpriteToScene(sprite, mMouseTilePosX, mMouseTilePosY);
}

void TilesetView::addSpriteToScene(Sprite* sprite, int x, int y) {
    if(sprite != nullptr) {
        sprite->setPos(x, y);
        mScene->addItem(sprite->getPixmapItem());


        // Move the rectangle to the border of the Sprite
        mSelectedSpriteBorder->setRect(x, y, sprite->getWidth(), sprite->getHeight());
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

void TilesetView::removeSpriteFromScene(Sprite* sprite) {
    mScene->removeItem(sprite->getPixmapItem());
}


void TilesetView::mouseMoveEvent(QMouseEvent* event) {
    // Do some caluclations
    mMouseX = event->pos().x() + this->horizontalScrollBar()->value();
    mMouseY = event->pos().y() + this->verticalScrollBar()->value();
    mMouseTilePosX = mMouseX / mTilesetMgr->getTileset(0)->getTileWidth() * mTilesetMgr->getTileset(0)->getTileWidth();
    mMouseTilePosY = mMouseY / mTilesetMgr->getTileset(0)->getTileHeight() * mTilesetMgr->getTileset(0)->getTileHeight();

    if(mTilesetMgr == nullptr)
        return;


    Sprite* sprite;
    if(mTilesetMgr->getSpriteLoader()->isFinished() == false) {
        sprite = mTilesetMgr->getSpriteLoader()->getSprite(mTilesetMgr->getSpriteLoader()->getSpriteIndex());
    }
    else if(mSelectedSprite != nullptr) {
        sprite = mSelectedSprite;
    }

    if(mTilesetMgr->getSpriteLoader()->isFinished() == false || mSelectedSprite != nullptr) {
        // If SpriteLoader has not-added Sprites....

        // Position the Sprite
        sprite->setPos(mMouseTilePosX, mMouseTilePosY);
        mSelectedSpriteBorder->setRect(mMouseTilePosX, mMouseTilePosY, sprite->getWidth(), sprite->getHeight());

        // Check collision with exisiting Sprites
        if(mTilesetMgr->getTileset(0)->checkCollision(sprite)) {
            mSelectedSpriteBorder->setPen(QPen(Qt::red));
        }
        else {
            mSelectedSpriteBorder->setPen(QPen(Qt::blue));
        }
    }
    else {
        // 'Hovering mode' -> Nothing selected, but show Border if we hover a selectable item
        int idx;
        if((idx = mTilesetMgr->getTileset(0)->selectSprite(mMouseX, mMouseY)) >= 0) {
            mSelectedSpriteBorder->setRect(mTilesetMgr->getTileset(0)->getSprites().at(idx)->getX(), mTilesetMgr->getTileset(0)->getSprites().at(idx)->getY(), mTilesetMgr->getTileset(0)->getSprites().at(idx)->getWidth(), mTilesetMgr->getTileset(0)->getSprites().at(idx)->getHeight());
            mSelectedSpriteBorder->setVisible(true);
        }
        else
            mSelectedSpriteBorder->setVisible(false);
    }

    // Redraw scene, otherwise there could be some ugly fragments (especially while moving the scrollbars)
    this->scene()->update(this->sceneRect());
}


void TilesetView::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        if(mTilesetMgr->getSpriteLoader()->isFinished() == false) {
            // 'Add new Sprites' mode

            // Check collision with existing Sprites
            if(mTilesetMgr->getTileset(0)->checkCollision(mTilesetMgr->getSpriteLoader()->getSprite(mTilesetMgr->getSpriteLoader()->getSpriteIndex())) == false) {
                // Add Sprite to Tileset for management and storing
                Sprite* floatingSprite = mTilesetMgr->getSpriteLoader()->getSprite(mTilesetMgr->getSpriteLoader()->getSpriteIndex());
                floatingSprite->setIsOnScene(true);
                mTilesetMgr->getTileset(0)->addSprite(floatingSprite);

                // Load new Sprite from SpriteLoader
                if(mTilesetMgr->getSpriteLoader()->next() == 0) {
                    // New sprite is available
                    Sprite* newSprite = mTilesetMgr->getSpriteLoader()->getSprite(mTilesetMgr->getSpriteLoader()->getSpriteIndex());
                    addSpriteToScene(newSprite, mMouseTilePosX, mMouseTilePosY);;
                }
                else {
                    // No new sprite is available in the forward direction
                    if(mTilesetMgr->getSpriteLoader()->prev() == 0) {
                        Sprite* newSprite = mTilesetMgr->getSpriteLoader()->getSprite(mTilesetMgr->getSpriteLoader()->getSpriteIndex());
                        addSpriteToScene(newSprite, mMouseTilePosX, mMouseTilePosY);;
                    }
                    else {
                        // Neither in forward nor in backward direction a sprite is available -> finished!
                        mTilesetMgr->getSpriteLoader()->setIsFinished(true);
                    }
                }
            }
            else {
                // Collision with an existing Sprite!
                QApplication::beep();
            }
        }
        else {
            // Selection mode!

            if(mSelectedSprite == nullptr) {
                int k = mTilesetMgr->getTileset(0)->selectSprite(mMouseX, mMouseY);
                if(k >= 0) {
                    mSelectedSprite = mTilesetMgr->getTileset(0)->getSprites().at(k);
                    mSelectedSpriteBorder->setPen(QPen(Qt::blue));
                }
            }
            else {
                // Check if we can place Sprite here (not overlapping)
                if(mTilesetMgr->getTileset(0)->checkCollision(mSelectedSprite) == false) {
                    mSelectedSpriteBorder->setPen(QPen(Qt::black));
                    mSelectedSprite = nullptr;
                }
                else
                    QApplication::beep();
            }
        }

        emit mouseReleased();
    }
}

