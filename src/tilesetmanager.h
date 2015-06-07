#ifndef TILESETMANAGER_H
#define TILESETMANAGER_H

#include <QList>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "tileset.h"
#include "spriteloader.h"

class TilesetManager
{
public:
    TilesetManager();
    ~TilesetManager();
    void addTileset(Tileset* tileset);
    Tileset* getTileset(int idx);
    SpriteLoader* getSpriteLoader();
    int saveToFile(QString path);
    int loadFromFile(QString path);

private:
    QList<Tileset*> mTilesets;
    SpriteLoader* mSpriteLoader;
};

#endif // TILESETMANAGER_H
