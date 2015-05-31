#include "tilesetview.h"



TilesetView::TilesetView(QWidget *parent) :  QGraphicsView(parent)
{
    setMouseTracking(true);



}

void TilesetView::init(TileLoader* tileLoader) {
    setGeometry(x(), y(), TILESET_WIDTH + LINE_WIDTH*(TILESET_WIDTH/TILE_WIDTH)+3, TILESET_HEIGHT + LINE_WIDTH*(TILESET_HEIGHT/TILE_HEIGHT)+3);
    setSceneRect(0, 0, TILESET_WIDTH + LINE_WIDTH*(TILESET_WIDTH/TILE_WIDTH)+3, TILESET_HEIGHT + LINE_WIDTH*(TILESET_HEIGHT/TILE_HEIGHT)+3);

    mScene = new QGraphicsScene();
    mScene->setSceneRect(0, 0, TILESET_WIDTH + LINE_WIDTH*(TILESET_WIDTH/TILE_WIDTH)+1, TILESET_HEIGHT + LINE_WIDTH*(TILESET_HEIGHT/TILE_HEIGHT)+2);
    setScene(mScene);

    mTileClicked = false;


    // Add vertical grid
    for(int i = 0; i <= TILESET_WIDTH + LINE_WIDTH*(TILESET_WIDTH/TILE_WIDTH); i += TILE_WIDTH+LINE_WIDTH) {
        mScene->addLine(i, 0, i, TILESET_HEIGHT + LINE_WIDTH*(TILESET_HEIGHT/TILE_HEIGHT)+1, QPen(QColor(0, 0, 0, 64)));
    }


    // Add horizontal grid
    for(int i = 0; i <= TILESET_HEIGHT + LINE_WIDTH*(TILESET_HEIGHT/TILE_HEIGHT); i += TILE_HEIGHT+LINE_WIDTH) {
        mScene->addLine(0, i, TILESET_WIDTH + LINE_WIDTH*(TILESET_WIDTH/TILE_WIDTH)+1, i, QPen(QColor(0, 0, 0, 64)));
    }

    // Add tiles

    QList<QGraphicsPixmapItem*>::iterator i;
    for(i = tileLoader->getTiles()->begin(); i != tileLoader->getTiles()->end(); ++i) {
        QGraphicsPixmapItem* pix = *i;
        mScene->addPixmap(pix->pixmap());
        while(mTileClicked == false) {
            QCoreApplication::processEvents();
        }
        mTileClicked = false;
    }

    // Add current tile overlay
    mTileOverlayRect = QRectF(0, 0, TILE_WIDTH, TILE_HEIGHT);
    mTileOverlayItem = mScene->addRect(mTileOverlayRect, QPen(QColor(64, 64, 64, 255)), QBrush(QColor(0, 0, 0, 128)));
}

void TilesetView::mouseMoveEvent(QMouseEvent* event) {
    if(mTileOverlayItem != 0) {
        int posX = (event->pos().x() - event->pos().x()/TILE_WIDTH) / TILE_WIDTH * TILE_WIDTH;
        posX += (event->pos().x() - event->pos().x()/TILE_WIDTH) / TILE_WIDTH;

        int posY = (event->pos().y() - event->pos().y()/TILE_HEIGHT) / TILE_HEIGHT * TILE_HEIGHT;
        posY += (event->pos().y() - event->pos().y()/TILE_HEIGHT) / TILE_HEIGHT;

        qDebug() << "Tile:" << posX;

        mTileOverlayRect.setX(posX+1);
        mTileOverlayRect.setY(posY+1);
        mTileOverlayRect.setWidth(TILE_WIDTH-1);
        mTileOverlayRect.setHeight(TILE_HEIGHT-1);
        mTileOverlayItem->setRect(mTileOverlayRect);
    }
}

void TilesetView::mouseReleaseEvent(QMouseEvent* event) {
    if(mTileOverlayItem == 0) {
        qDebug() << "Released!";
        if(event->button() == Qt::LeftButton)
            mTileClicked = true;
    }
}
