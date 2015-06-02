#include "spriteloader.h"

SpriteLoader::SpriteLoader() {
    mIsFinished = true;
    mDirIterator = nullptr;
    mCurrentSprite = nullptr;
}

void SpriteLoader::setLoadPath(QString path, PathType pathType) {
    mPath = path;
    mType = pathType;
    mIsFinished = false;


    if(pathType == SpriteLoader::File) {
        mDirIterator = nullptr;
    }
    else if(pathType == SpriteLoader::Directory) {
        mDirIterator = new QDirIterator(mPath, QDirIterator::NoIteratorFlags);
    }
    else if(pathType == SpriteLoader::Subdirectory) {
        mDirIterator = new QDirIterator(mPath, QDirIterator::Subdirectories);
    }
}



int SpriteLoader::loadSprite(QString path) {
    Sprite* sprite = new Sprite(path);
    if(sprite == nullptr)
        return -1;


    mCurrentSprite = sprite;
    mIsFinished = true;

    return 0;
}

int SpriteLoader::loadSprite(QDirIterator* it) {
    Sprite* sprite = nullptr;

    // If current file reference isn't a really file, load next one
    while(it->fileInfo().isFile() == false) {
        // Check if a next file is available, otherwise return 0
        if(it->hasNext() == false) {
            mIsFinished = true;
            mCurrentSprite = nullptr;
            return 0; // Finished
        }

        // Reference next file
        it->next();
    }

    qDebug() << "Sprite path: " << it->filePath();
    sprite = new Sprite(it->filePath());

    // Reference next file
    it->next();

    if(sprite == nullptr)
        return -1;  // Error

    mCurrentSprite = sprite;
    return 1;   // Sprite OK
}


/**
 * @brief Returns a Sprite
 * @return Returns pointer to sprite if file is valid, 0 if finished
 */
int SpriteLoader::getNextSprite(Sprite** sprite) {
    int res = -1;
    if(mType == SpriteLoader::File) {
        res = loadSprite(mPath);
    }
    else if(mType == SpriteLoader::Directory) {
        res = loadSprite(mDirIterator);
    }

    *sprite = mCurrentSprite;


    return res;
}

/**
 * @brief Returns a Sprite at a predefined position
 * @param x
 * @param y
 * @return Returns pointer to sprite if file is valid, 0 if finished
 */
int SpriteLoader::getNextSprite(Sprite** sprite, int x, int y) {
    int res = -1;
    if(mType == SpriteLoader::File)
        res = loadSprite(mPath);
    else if(mType == SpriteLoader::Directory)
        res = loadSprite(mDirIterator);
    mCurrentSprite->setPos(x, y);

    *sprite = mCurrentSprite;

    return res;
}

Sprite* SpriteLoader::getCurrentSprite() {
    return mCurrentSprite;
}

bool SpriteLoader::isFinished() {
    return mIsFinished;
}
