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
    bool isFinished();
    void setIsFinished(bool state);
    int getSpritesCount();
    int getSpriteIndex();

private:
    QDirIterator* mDirIterator;
    QList<Sprite*> mTempSprites;
    QString mPath;
    PathType mType;
    bool mIsFinished;
    int mSpriteIndex;
    bool checkIsFinished();

};

#endif // SPRITELOADER_H
