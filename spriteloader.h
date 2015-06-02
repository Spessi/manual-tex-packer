#ifndef SPRITELOADER_H
#define SPRITELOADER_H

#include <QList>
#include <QGraphicsItem>
#include <QDirIterator>
#include <QDebug>

#include "sprite.h"

class SpriteLoader
{
public:
    enum PathType: int {
        File = 0,
        Directory,
        Subdirectory
    };
    SpriteLoader();
    void setLoadPath(QString path, PathType pathType);
    int getNextSprite(Sprite** sprite);
    int getNextSprite(Sprite** sprite, int x, int y);
    Sprite* getCurrentSprite();
    bool isFinished();



private:
    int loadSprite(QDirIterator* it);
    int loadSprite(QString path);

    Sprite* mCurrentSprite;
    QDirIterator* mDirIterator;
    QString mPath;
    PathType mType;
    bool mIsFinished;

};

#endif // SPRITELOADER_H
