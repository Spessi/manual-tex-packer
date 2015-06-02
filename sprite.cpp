#include "sprite.h"

Sprite::Sprite(QString spritePath) {
    QImage* img = new QImage(spritePath);
    mPixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*img));
    free(img);
}

void Sprite::setPos(float x, float y) {
    mPixmapItem->setPos(x, y);
}

float Sprite::getX() {
    return mPixmapItem->pos().x();
}

float Sprite::getY() {
    return mPixmapItem->pos().y();
}

int Sprite::getWidth() {
    return mPixmapItem->pixmap().width();
}

int Sprite::getHeight() {
    return mPixmapItem->pixmap().height();
}

int Sprite::getTileX() {
    // TODO remove hardcoded tile width
    return getX() / 32;
}

int Sprite::getTileY() {
    // TODO remove hardcoded tile height
    return getY() / 32;
}

QGraphicsPixmapItem* Sprite::getPixmapItem() {
    return mPixmapItem;
}

QRectF  Sprite::getBoundingRect() {
    return QRectF(getX(), getY(), getWidth(), getHeight());
}

