#include "spriteloader.h"

SpriteLoader::SpriteLoader() {
    mDirIterator = nullptr;
    mSpriteIndex = 0;
    mIsFinished = true;
}

SpriteLoader::~SpriteLoader() {
    if(mDirIterator != nullptr)
        delete mDirIterator;

    mTempSprites.clear();
}



void SpriteLoader::setLoadPath(QString path, PathType pathType) {
    mPath = path;
    mType = pathType;
    mSpriteIndex = 0;
    mIsFinished = false;


    if(pathType == SpriteLoader::File) {
        mDirIterator = nullptr;
        QFileInfo info(mPath);
        if(info.isFile() &&  info.suffix() == "png")
            mTempSprites.append(new Sprite(mPath));
    }
    else if(pathType == SpriteLoader::Directory) {
        mDirIterator = new QDirIterator(mPath, QDirIterator::NoIteratorFlags);
    }
    else if(pathType == SpriteLoader::Subdirectory) {
        mDirIterator = new QDirIterator(mPath, QDirIterator::Subdirectories);
        // Load all sprites in memory
        while(1) {
            if(mDirIterator->fileInfo().isFile() == false || mDirIterator->fileInfo().suffix() != "png") {
                if(mDirIterator->hasNext() == false)
                    break;

                mDirIterator->next();
                continue;
            }

            mTempSprites.append(new Sprite(mDirIterator->filePath()));
            qDebug() << "Sprite path: " << mDirIterator->filePath();

            if(mDirIterator->hasNext() == false)
                break;
            mDirIterator->next();
        }


    }

    // If no file was loaded
    if(mTempSprites.size() == 0)
        finished();
}

int SpriteLoader::getSpritesCount() {
    return mTempSprites.size();
}



/**
 * @brief SpriteLoader::next
 * @return  0 = OK
 *          1 = No more Sprite available on the "right" side
 */
int SpriteLoader::next() {
    int tmpIndex = mSpriteIndex + 1;

    while(1) {
        if(tmpIndex >= getSpritesCount()) {
            // If the index exceeds the list dimension -> ret 1
            return 1;
        }
        else if(mTempSprites.at(tmpIndex)->isOnScene()) {
            // If the Sprite is already on scene, go to the next Sprite
            tmpIndex++;
        }
        else {
            // If everything is OK -> ret 0
            mSpriteIndex = tmpIndex;
            return 0;
        }
    }


    return 0;
}

int SpriteLoader::prev() {
    int tmpIndex = mSpriteIndex - 1;

    while(1) {
        if(tmpIndex < 0) {
            // If the index is negative

            return 1;
        }
        else if(mTempSprites.at(tmpIndex)->isOnScene()) {
            // If the Sprite is already on scene, go to the previous Sprite
            tmpIndex--;
        }
        else {
            // If everything is OK -> ret 0
            mSpriteIndex = tmpIndex;
            return 0;
        }
    }

    return 0;
}


Sprite* SpriteLoader::getSprite(int idx) {
    return mTempSprites.at(idx);
}

int SpriteLoader::getSpriteIndex() {
    return mSpriteIndex;
}


bool SpriteLoader::isFinished() {
    return mIsFinished;
}

void SpriteLoader::finished() {
    mIsFinished = true;

    mSpriteIndex = 0;
    delete mDirIterator;
    mDirIterator = nullptr;
    mTempSprites.clear();
}


