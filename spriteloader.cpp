#include "spriteloader.h"

SpriteLoader::SpriteLoader(QString tilePath) {
    mDirIterator = new QDirIterator(tilePath, QDirIterator::Subdirectories);
    mIsFinished = false;
}

/**
 * @brief SpriteLoader::getSprite
 * @return Returns pointer to sprite if file is valid, 0 if finished
 */
Sprite* SpriteLoader::getSprite() {
    Sprite* sprite = nullptr;

    // If current file reference isn't a really file, load next one
    while(mDirIterator->fileInfo().isFile() == false) {
        // Check if a next file is available, otherwise return 0
        if(mDirIterator->hasNext() == false) {
            mIsFinished = true;
            return sprite;
        }

        // Reference next file
        mDirIterator->next();
    }

    qDebug() << "Sprite path: " << mDirIterator->filePath();
    sprite = new Sprite(mDirIterator->filePath());

    // Reference next file
    mDirIterator->next();
    return sprite;
}


bool SpriteLoader::isFinished() {
    return mIsFinished;
}
