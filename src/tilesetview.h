#ifndef TILESETVIEW_H
#define TILESETVIEW_H

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QDebug>

#include "spriteloader.h"
#include "tilesetmanager.h"
#include "tileset.h"

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
    void init(TilesetManager* tilesetMgr);
    void addSpriteToScene(Sprite* sprite, int x, int y);
    void addSpritesToScene(QList<Sprite*> sprites);

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

signals:

public slots:

private:
    QGraphicsScene* mScene;
    QRectF mTileOverlayRect;
    TilesetManager* mTilesetMgr;
    SpriteLoader* mSpriteLoader;
    QGraphicsRectItem* mTileOverlayItem;
    int mMouseTilePosX, mMouseTilePosY;
    void showSprite(Sprite* sprite, float x, float y);
};

#endif // TILESETVIEW_H
