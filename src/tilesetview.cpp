#include "tilesetview.h"

TilesetView::TilesetView(QWidget *parent) :  QGraphicsView(parent) {
    setMouseTracking(true);
    mProject = nullptr;
    mSpriteLoader = nullptr;
    mSelectedSprite = nullptr;
    mSelectedSpriteBorder = nullptr;
    mMouseTilePosX = mMouseTilePosY = mMouseX = mMouseY = 0;
    mCanAddSprite = false;

    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    mScene = new QGraphicsScene();
    mScene->setSceneRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT);
    setScene(mScene);
}

void TilesetView::init(Project* project) {
    mProject = project;
    this->setEnabled(true);
    mSelectedSprite = nullptr;
    mSelectedSpriteBorder = nullptr;
    mMouseTilePosX = mMouseTilePosY = mMouseX = mMouseY = 0;
    mCanAddSprite = false;

    int width = VIEW_WIDTH;
    int height = VIEW_HEIGHT;
    int sceneWidth = mProject->getTileset(0)->getWidth();
    int sceneHeight = mProject->getTileset(0)->getHeight();

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
    for(int i = 0; i <= mProject->getTileset(0)->getWidth(); i += mProject->getTileset(0)->getTileWidth()) {
        mScene->addLine(i, 0, i, mProject->getTileset(0)->getHeight(), QPen(QColor(0, 0, 0, 64)));
    }


    // Add horizontal grid
    for(int i = 0; i <= mProject->getTileset(0)->getHeight(); i += mProject->getTileset(0)->getTileHeight()) {
        mScene->addLine(0, i, mProject->getTileset(0)->getWidth(), i, QPen(QColor(0, 0, 0, 64)));
    }

    // Initialize SelectedSpriteBorder (Rectangle)
    if(mSelectedSpriteBorder != nullptr)
        delete mSelectedSpriteBorder;

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
        if(mProject->getTileset(0)->checkCollision(sprite)) {
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

    if(sprite == mSelectedSprite) {
        mSelectedSprite = nullptr;
        mSelectedSpriteBorder->setVisible(false);
    }
}

void TilesetView::removeAllSpritesFromScene() {
    // Delete all Sprites in SpriteLoader queue
    mProject->getSpriteLoader()->finished();

    // Reinitialize the TilesetView
    init(mProject);

    // Refresh Queue UI
    emit mouseReleased();
}

void TilesetView::run() {
    mCanAddSprite = true;
    mSelectedSpriteBorder->setVisible(true);
}

void TilesetView::pause() {
    mCanAddSprite = false;
    mSelectedSpriteBorder->setVisible(false);
}


void TilesetView::mouseMoveEvent(QMouseEvent* event) {
    // Do some caluclations
    mMouseX = event->pos().x() + this->horizontalScrollBar()->value();
    mMouseY = event->pos().y() + this->verticalScrollBar()->value();
    mMouseTilePosX = mMouseX / mProject->getTileset(0)->getTileWidth() * mProject->getTileset(0)->getTileWidth();
    mMouseTilePosY = mMouseY / mProject->getTileset(0)->getTileHeight() * mProject->getTileset(0)->getTileHeight();

    if(mProject == nullptr || mProject->getSpriteLoader() == nullptr)
        return;


    Sprite* sprite;
    if(mProject->getSpriteLoader()->isFinished() == false && mCanAddSprite) {
        sprite = mProject->getSpriteLoader()->getSprite(mProject->getSpriteLoader()->getSpriteIndex());
    }
    else if(mSelectedSprite != nullptr) {
        sprite = mSelectedSprite;
    }

    if((mProject->getSpriteLoader()->isFinished() == false && mCanAddSprite) || mSelectedSprite != nullptr) {
        // If SpriteLoader has not-added Sprites....

        // Position the Sprite
        sprite->setPos(mMouseTilePosX, mMouseTilePosY);
        mSelectedSpriteBorder->setRect(mMouseTilePosX, mMouseTilePosY, sprite->getWidth(), sprite->getHeight());

        // Check collision with exisiting Sprites
        if(mProject->getTileset(0)->checkCollision(sprite)) {
            mSelectedSpriteBorder->setPen(QPen(Qt::red));
        }
        else {
            mSelectedSpriteBorder->setPen(QPen(Qt::blue));
        }
    }
    else {
        // 'Hovering mode' -> Nothing selected, but show Border if we hover a selectable item
        int idx;
        if((idx = mProject->getTileset(0)->selectSprite(mMouseX, mMouseY)) >= 0) {
            mSelectedSpriteBorder->setRect(mProject->getTileset(0)->getSprites().at(idx)->getX(), mProject->getTileset(0)->getSprites().at(idx)->getY(), mProject->getTileset(0)->getSprites().at(idx)->getWidth(), mProject->getTileset(0)->getSprites().at(idx)->getHeight());
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
        if(mProject->getSpriteLoader()->isFinished() == false && mCanAddSprite) {
            // 'Add new Sprites' mode

            // Check collision with existing Sprites
            if(mProject->getTileset(0)->checkCollision(mProject->getSpriteLoader()->getSprite(mProject->getSpriteLoader()->getSpriteIndex())) == false) {
                // Add Sprite to Tileset for management and storing
                Sprite* floatingSprite = mProject->getSpriteLoader()->getSprite(mProject->getSpriteLoader()->getSpriteIndex());
                floatingSprite->setIsOnScene(true);
                mProject->getTileset(0)->addSprite(floatingSprite);

                // Load new Sprite from SpriteLoader
                if(mProject->getSpriteLoader()->next() == 0) {
                    // New sprite is available
                    Sprite* newSprite = mProject->getSpriteLoader()->getSprite(mProject->getSpriteLoader()->getSpriteIndex());
                    addSpriteToScene(newSprite, mMouseTilePosX, mMouseTilePosY);;
                }
                else {
                    // No new sprite is available in the forward direction
                    if(mProject->getSpriteLoader()->prev() == 0) {
                        Sprite* newSprite = mProject->getSpriteLoader()->getSprite(mProject->getSpriteLoader()->getSpriteIndex());
                        addSpriteToScene(newSprite, mMouseTilePosX, mMouseTilePosY);;
                    }
                    else {
                        // Neither in forward nor in backward direction a sprite is available -> finished!
                        mProject->getSpriteLoader()->finished();
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
                // Check if cursor is above a Sprite
                int k = mProject->getTileset(0)->selectSprite(mMouseX, mMouseY);
                if(k >= 0) {
                    // If yes, select it
                    mSelectedSprite = mProject->getTileset(0)->getSprites().at(k);
                    mSelectedSpriteBorder->setPen(QPen(Qt::blue));
                }
            }
            else {
                // Check if we can place Sprite here (not overlapping)
                if(mProject->getTileset(0)->checkCollision(mSelectedSprite) == false) {
                    mSelectedSpriteBorder->setPen(QPen(Qt::black));
                    mSelectedSprite = nullptr;
                }
                else
                    QApplication::beep();
            }
            emit spriteSelected(&mSelectedSprite);
        }

        emit mouseReleased();
    }
}

