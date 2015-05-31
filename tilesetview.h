#ifndef TILESETVIEW_H
#define TILESETVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QDebug>

#include "TileLoader.h"


#define TILESET_WIDTH   512
#define TILESET_HEIGHT  512
#define TILE_WIDTH      32
#define TILE_HEIGHT     32
#define LINE_WIDTH      1

class TilesetView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TilesetView(QWidget *parent = 0);
    void init(TileLoader* tileLoader);

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

signals:

public slots:

private:
    QGraphicsScene* mScene;
    QRectF mTileOverlayRect;
    QGraphicsRectItem* mTileOverlayItem;
    bool mTileClicked;

};

#endif // TILESETVIEW_H
