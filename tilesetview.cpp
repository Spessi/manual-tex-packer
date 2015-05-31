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

    setGeometry(x(), y(), TILESET_WIDTH + LINE_WIDTH*(TILESET_WIDTH/TILE_WIDTH)+3, TILESET_HEIGHT + LINE_WIDTH*(TILESET_HEIGHT/TILE_HEIGHT)+3);
    setSceneRect(0, 0, TILESET_WIDTH + LINE_WIDTH*(TILESET_WIDTH/TILE_WIDTH)+3, TILESET_HEIGHT + LINE_WIDTH*(TILESET_HEIGHT/TILE_HEIGHT)+3);

    mScene = new QGraphicsScene();
    mScene->setSceneRect(0, 0, TILESET_WIDTH + LINE_WIDTH*(TILESET_WIDTH/TILE_WIDTH)+1, TILESET_HEIGHT + LINE_WIDTH*(TILESET_HEIGHT/TILE_HEIGHT)+2);
    setScene(mScene);


    // Add vertical grid
    for(int i = 0; i <= TILESET_WIDTH + LINE_WIDTH*(TILESET_WIDTH/TILE_WIDTH); i += TILE_WIDTH+LINE_WIDTH) {
        mScene->addLine(i, 0, i, TILESET_HEIGHT + LINE_WIDTH*(TILESET_HEIGHT/TILE_HEIGHT)+1, QPen(QColor(0, 0, 0, 64)));
    }


    // Add horizontal grid
    for(int i = 0; i <= TILESET_HEIGHT + LINE_WIDTH*(TILESET_HEIGHT/TILE_HEIGHT); i += TILE_HEIGHT+LINE_WIDTH) {
        mScene->addLine(0, i, TILESET_WIDTH + LINE_WIDTH*(TILESET_WIDTH/TILE_WIDTH)+1, i, QPen(QColor(0, 0, 0, 64)));
    }

    // Show first tile, the rest is done in onMouseReleaseEvent
    curInitGraphicItem = mTileLoader->getTiles()->at(curInitGraphic);
    mScene->addItem(curInitGraphicItem);
    curInitGraphicItem->setPos(1,1);
    curInitGraphic++;
}

void TilesetView::mouseMoveEvent(QMouseEvent* event) {
    mousX = event->pos().x();
    mousY = event->pos().y();

    mMouseTilePosX = (event->pos().x() - event->pos().x()/TILE_WIDTH) / TILE_WIDTH * TILE_WIDTH;
    mMouseTilePosX += (event->pos().x() - event->pos().x()/TILE_WIDTH) / TILE_WIDTH;

    mMouseTilePosY = (event->pos().y() - event->pos().y()/TILE_HEIGHT) / TILE_HEIGHT * TILE_HEIGHT;
    mMouseTilePosY += (event->pos().y() - event->pos().y()/TILE_HEIGHT) / TILE_HEIGHT;

    if(curInitState == 0 && curInitGraphicItem != 0) {
        curInitGraphicItem->setPos(mMouseTilePosX+1, mMouseTilePosY+1);
    }

    if(mTileOverlayItem != 0) {
        mTileOverlayRect.setX(mMouseTilePosX+1);
        mTileOverlayRect.setY(mMouseTilePosY+1);
        mTileOverlayRect.setWidth(TILE_WIDTH-1);
        mTileOverlayRect.setHeight(TILE_HEIGHT-1);
        mTileOverlayItem->setRect(mTileOverlayRect);
    }
}

void TilesetView::mouseReleaseEvent(QMouseEvent* event) {
    switch(curInitState) {
    case 0:
        if(curInitGraphic >= mTileLoader->getTiles()->size()) {
            curInitState = 1;
            // Add current tile overlay
            mTileOverlayRect = QRectF(0, 0, TILE_WIDTH, TILE_HEIGHT);
            mTileOverlayItem = mScene->addRect(mTileOverlayRect, QPen(QColor(64, 64, 64, 255)), QBrush(QColor(0, 0, 0, 128)));
            break;
        }
        curInitGraphicItem = mTileLoader->getTiles()->at(curInitGraphic);
        mScene->addItem(curInitGraphicItem);
        curInitGraphicItem->setPos(mMouseTilePosX+1, mMouseTilePosY+1);
        curInitGraphic++;
        break;

    case 1:

        break;
    }
}
