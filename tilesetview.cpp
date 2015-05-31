#include "tilesetview.h"

float mousX, mousY;
int curInitState, curInitGraphic;
QGraphicsPixmapItem* curInitGraphicItem;

TilesetView::TilesetView(QWidget *parent) :  QGraphicsView(parent)
{
    setMouseTracking(true);

    curInitState = curInitGraphic = 0;

}

void TilesetView::init(TileLoader* tileLoader) {
    mTileLoader = tileLoader;

    setGeometry(x(), y(), TILESET_WIDTH + 3, TILESET_HEIGHT);
    setSceneRect(0, 0, TILESET_WIDTH + 3, TILESET_HEIGHT);

    mScene = new QGraphicsScene();
    mScene->setSceneRect(0, 0, TILESET_WIDTH + 3, TILESET_HEIGHT);
    setScene(mScene);


    // Add vertical grid
    for(int i = 0; i <= TILESET_WIDTH; i += TILE_WIDTH) {
        mScene->addLine(i, 0, i, TILESET_HEIGHT, QPen(QColor(0, 0, 0, 64)));
    }


    // Add horizontal grid
    for(int i = 0; i <= TILESET_HEIGHT; i += TILE_HEIGHT) {
        mScene->addLine(0, i, TILESET_WIDTH, i, QPen(QColor(0, 0, 0, 64)));
    }

    // Show first tile, the rest is done in onMouseReleaseEvent
    curInitGraphicItem = mTileLoader->getTiles()->at(curInitGraphic);
    mScene->addItem(curInitGraphicItem);
    curInitGraphicItem->setPos(0,0);
    curInitGraphic++;
}

void TilesetView::mouseMoveEvent(QMouseEvent* event) {
    mousX = event->pos().x();
    mousY = event->pos().y();

    mMouseTilePosX = event->pos().x() / TILE_WIDTH * TILE_WIDTH;

    mMouseTilePosY = event->pos().y() / TILE_HEIGHT * TILE_HEIGHT;

    if(curInitState == 0 && curInitGraphicItem != 0) {
        curInitGraphicItem->setPos(mMouseTilePosX, mMouseTilePosY);
    }

    if(mTileOverlayItem != 0) {
        mTileOverlayRect.setX(mMouseTilePosX);
        mTileOverlayRect.setY(mMouseTilePosY);
        mTileOverlayRect.setWidth(curInitGraphicItem->boundingRect().width());
        mTileOverlayRect.setHeight(curInitGraphicItem->boundingRect().height());
        mTileOverlayItem->setRect(mTileOverlayRect);
    }
    else {
        mTileOverlayRect = QRectF(0, 0, TILE_WIDTH, TILE_HEIGHT);
        mTileOverlayItem = mScene->addRect(mTileOverlayRect, QPen(QColor(255, 0, 0, 196)), QBrush(QColor(0, 0, 0, 0)));
    }
}

void TilesetView::mouseReleaseEvent(QMouseEvent* event) {
    switch(curInitState) {
    case 0:
        if(curInitGraphic >= mTileLoader->getTiles()->size()) {
            curInitState = 1;
            break;
        }

        curInitGraphicItem = mTileLoader->getTiles()->at(curInitGraphic);
        mScene->addItem(curInitGraphicItem);
        curInitGraphicItem->setPos(mMouseTilePosX, mMouseTilePosY);
        mTileOverlayRect.setWidth(curInitGraphicItem->boundingRect().width());
        mTileOverlayRect.setHeight(curInitGraphicItem->boundingRect().height());
        mTileOverlayItem->setRect(mTileOverlayRect);
        mScene->removeItem(mTileOverlayItem);
        mScene->addItem(mTileOverlayItem);
        curInitGraphic++;
        break;

    case 1:

        break;
    }
}
