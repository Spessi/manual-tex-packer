#ifndef TILESET_H
#define TILESET_H

#include <QList>
#include <QDebug>
#include "Sprite.h"

class Tileset
{
public:
    Tileset(int width, int height, int tileWidth, int tileHeight);
    int addSprite(Sprite* sprite);
    bool checkCollision(Sprite* sprite);

public slots:

private:
    QList<Sprite*> mSprites;
    int mWidth, mHeight, mTileWidth, mTileHeight;
};

#endif // TILESET_H
