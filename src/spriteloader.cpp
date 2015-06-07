#include "spriteloader.h"

SpriteLoader::SpriteLoader() {
    mIsFinished = true;
    mDirIterator = nullptr;
    mSpriteIndex = 0;
}

SpriteLoader::~SpriteLoader() {
    if(mDirIterator != nullptr)
        delete mDirIterator;

    mTempSprites.clear();
}



void SpriteLoader::setLoadPath(QString path, PathType pathType) {
    mPath = path;
    mType = pathType;
    mIsFinished = false;
    mSpriteIndex = 0;



    if(pathType == SpriteLoader::File) {
        mDirIterator = nullptr;
    }
    else if(pathType == SpriteLoader::Directory) {
        mDirIterator = new QDirIterator(mPath, QDirIterator::NoIteratorFlags);
    }
    else if(pathType == SpriteLoader::Subdirectory) {
        mDirIterator = new QDirIterator(mPath, QDirIterator::Subdirectories);
        // Load all sprites in memory
        while(1) {
            if(mDirIterator->fileInfo().isFile() == false) {
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
}

int SpriteLoader::getSpritesCount() {
    return mTempSprites.size();
}

bool SpriteLoader::checkIsFinished() {
    for(int i=0; i < getSpritesCount(); i++) {
        if(mTempSprites.at(i)->isOnScene() == false)
            return false;
    }

    return true;
}

/**
 * @brief SpriteLoader::next
 * @return  0 = OK
 *          1 = Grenze nach rechts
 */
int SpriteLoader::next() {
    int tmpIndex = mSpriteIndex + 1;

    while(1) {
        if(tmpIndex >= getSpritesCount()) {
            // If the index exceeds the list dimension -> ret 1
            if(checkIsFinished() == true)
                mIsFinished = true;
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
            if(checkIsFinished() == true)
                mIsFinished = true;
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

void SpriteLoader::setIsFinished(bool state) {
    mIsFinished = state;

    mSpriteIndex = 0;
    delete mDirIterator;
    mDirIterator = nullptr;
    mTempSprites.clear();
}


/**
 * @brief Returns a Sprite
 * @return Returns pointer to sprite if file is valid, 0 if finished
 */
/*
int SpriteLoader::getNextSprite(Sprite** sprite) {
    int index = mSpriteIndex;
    int ret = -1;
    bool leftSpace = false;


    if(mType == SpriteLoader::File) {
        *sprite = new Sprite(mPath);
        if(*sprite == nullptr)
            ret = -1;

        mIsFinished = true;

        ret = 0;
    }
    else if(mType == SpriteLoader::Subdirectory) {
        do {
            index++;

            if(mTempSprites.at(index)->isOnScene() == false)
                leftSpace = true;

            if(index > mTempSprites.size()) {
                ret = 2;
                break;
            }


        } while(index < mTempSprites.size()-1 && mTempSprites.at(index)->isOnScene());


        if(index >= mTempSprites.size() || ((index < mTempSprites.size() && leftSpace == false))) {
            ret = 2; // There's nothing left in this direction, need to check backward
            *sprite = nullptr;
        }
        else {
            mSpriteIndex = index;
            *sprite = mTempSprites.at(mSpriteIndex);
            ret = 1;
        }

    }

    return ret;
}

int SpriteLoader::getPrevSprite(Sprite** sprite) {
    int index = mSpriteIndex;
    int ret = -1;

    do {
        index--;

    } while(index >= 0 && mTempSprites.at(index)->isOnScene());

    if(index < 0) {
        ret = 2; // There's nothing left in this direction
        *sprite = nullptr;
    }
    else {
        mSpriteIndex = index;
        *sprite = mTempSprites.at(mSpriteIndex);
        ret = 1;
    }

    return ret;
}*/


