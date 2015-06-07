#ifndef PROJECT_H
#define PROJECT_H

#include <QList>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "tileset.h"
#include "spriteloader.h"

class Project
{
public:
    Project();
    ~Project();
    void addTileset(Tileset* tileset);
    Tileset* getTileset(int idx);
    SpriteLoader* getSpriteLoader();
    int saveToFile(QString path);
    int loadFromFile(QString path);

private:
    QList<Tileset*> mTilesets;
    SpriteLoader* mSpriteLoader;
};

#endif // PROJECT_H
