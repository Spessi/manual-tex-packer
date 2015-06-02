#include "tileset.h"

Tileset::Tileset(int width, int height, int tileWidth, int tileHeight) {
    mWidth = width;
    mHeight = height;
    mTileWidth = tileWidth;
    mTileHeight = tileHeight;
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
        if(mSprites.at(i)->getPixmapItem()->collidesWithItem(sprite->getPixmapItem(), Qt::IntersectsItemBoundingRect)) {
            return true;
        }
    }
    return false;
}
