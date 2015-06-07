#ifndef TILESETVIEW_H
#define TILESETVIEW_H

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QScrollBar>
#include <QStyle>
#include <QDebug>

#include "spriteloader.h"
#include "project.h"
#include "tileset.h"

#define VIEW_WIDTH   512
#define VIEW_HEIGHT  512
#define TILE_WIDTH      32
#define TILE_HEIGHT     32
#define LINE_WIDTH      1

class TilesetView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TilesetView(QWidget *parent = 0);
    void init(Project* project);
    void addSpriteToScene(Sprite* sprite, int x, int y);
    void addSpriteToScene(Sprite* sprite);
    void addSpritesToScene(QList<Sprite*> sprites);
    void removeSpriteFromScene(Sprite* sprite);
    void run();
    void pause();

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

signals:
    void mouseReleased();

public slots:

private:
    QGraphicsScene* mScene;
    QRectF mTileOverlayRect;
    Project* mProject;
    SpriteLoader* mSpriteLoader;
    QGraphicsRectItem* mTileOverlayItem;
    Sprite* mSelectedSprite;
    QGraphicsRectItem* mSelectedSpriteBorder;
    int mMouseX, mMouseY;
    int mMouseTilePosX, mMouseTilePosY;
    bool mCanAddSprite;
    void showSprite(Sprite* sprite, float x, float y);
};

#endif // TILESETVIEW_H
