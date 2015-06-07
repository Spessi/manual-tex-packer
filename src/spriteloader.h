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
    ~SpriteLoader();
    void setLoadPath(QString path, PathType pathType);
    int next();
    int prev();
    Sprite* getSprite(int idx);
    int getSpritesCount();
    int getSpriteIndex();
    bool isFinished();
    void finished();

private:
    QDirIterator* mDirIterator;
    QList<Sprite*> mTempSprites;
    QString mPath;
    PathType mType;
    int mSpriteIndex;
    bool mIsFinished;

};

#endif // SPRITELOADER_H
