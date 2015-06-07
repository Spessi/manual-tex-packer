#ifndef TILESET_H
#define TILESET_H

#include <QList>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include "Sprite.h"

class Tileset
{
public:
    Tileset();
    Tileset(int width, int height, int tileWidth, int tileHeight);
    ~Tileset();
    int addSprite(Sprite* sprite);
    int removeSprite(Sprite* sprite);
    int removeAllSprites();
    bool checkCollision(Sprite* sprite);
    int selectSprite(int x, int y);
    int saveToFile(QString path);
    int loadFromFile(QString path);

    void setWidth(int width);
    void setHeight(int height);
    void setTileWidth(int width);
    void setTileHeight(int height);
    int getWidth();
    int getHeight();
    int getTileWidth();
    int getTileHeight();
    QList<Sprite*> getSprites();

public slots:

private:
    QList<Sprite*> mSprites;
    int mWidth, mHeight, mTileWidth, mTileHeight;
};

#endif // TILESET_H
