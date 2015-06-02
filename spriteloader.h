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
    SpriteLoader(QString tilePath);
    Sprite* getSprite();
    bool isFinished();

private:
    QDirIterator* mDirIterator;
    bool mIsFinished;
};

#endif // SPRITELOADER_H
