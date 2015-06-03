#include "sprite.h"

Sprite::Sprite(QString spritePath) {
    QImage* img = new QImage(spritePath);
    mPixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*img));
    delete img;

    mPath = spritePath;
}

Sprite::~Sprite() {
    delete mPixmapItem;
}

bool Sprite::contains(int x, int y) {
    return getBoundingRect().contains(x, y);
}

void Sprite::setPos(float x, float y) {
    mPixmapItem->setPos(x, y);
    mPixmapItem->update();
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

QRect Sprite::getBoundingRect() {
    return QRect(getX(), getY(), getWidth(), getHeight());
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


QString Sprite::getPath() {
    return mPath;
}
