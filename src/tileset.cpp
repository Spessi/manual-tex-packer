#include "tileset.h"

Tileset::Tileset() {
    mWidth = 0;
    mHeight = 0;
    mTileWidth = 0;
    mTileHeight = 0;
}

Tileset::Tileset(int width, int height, int tileWidth, int tileHeight) {
    mWidth = width;
    mHeight = height;
    mTileWidth = tileWidth;
    mTileHeight = tileHeight;
}

Tileset::~Tileset() {
    for(int i=0; i < mSprites.size(); i++) {
        delete mSprites.at(i);
    }
}

/**
 * @brief Tileset::addSprite
 * @param sprite
 * @return  0 if Sprite was added
 *          -1 if Sprite is invalid
 */
int Tileset::addSprite(Sprite* sprite) {
    if(sprite == nullptr)
        return -1;

    mSprites.append(sprite);

    return 0;
}

int Tileset::removeSprite(Sprite* sprite) {
    if(sprite == nullptr)
        return -1;

    // Remove the sprite
    mSprites.removeAll(sprite);

    return 0;
}

int Tileset::removeAllSprites() {
    mSprites.clear();

    return 0;
}

bool Tileset::checkCollision(Sprite* sprite) {
    // Check if Sprite is out of the Tileset
    if(sprite->getX()+sprite->getWidth() > mWidth) {
        return true;
    }
    if(sprite->getY()+sprite->getHeight() > mHeight) {
        return true;
    }

    // Check if Sprite is colliding with an existing Sprite
    for(int i=0; i < mSprites.size(); ++i) {
        // No need to check the Sprite against himself
        if(mSprites.at(i) == sprite)
            continue;

        // Return true if the Sprite collides
        if(mSprites.at(i)->getPixmapItem()->collidesWithItem(sprite->getPixmapItem(), Qt::IntersectsItemBoundingRect)) {
            return true;
        }
    }
    return false;
}

int Tileset::selectSprite(int x, int y) {
    for(int i=0; i < mSprites.size(); ++i) {
        if(mSprites.at(i)->contains(x, y)) {
            return i;
        }
    }
    return -1;
}

void Tileset::setWidth(int width) {
    mWidth = width;
}

void Tileset::setHeight(int height) {
    mHeight = height;
}

void Tileset::setTileWidth(int width) {
    mTileWidth = width;
}

void Tileset::setTileHeight(int height) {
    mTileHeight = height;
}

int Tileset::getWidth() {
    return mWidth;
}

int Tileset::getHeight() {
    return mHeight;
}

int Tileset::getTileWidth() {
    return mTileWidth;
}

int Tileset::getTileHeight() {
    return mTileHeight;
}

QList<Sprite*> Tileset::getSprites() {
    return mSprites;
}
