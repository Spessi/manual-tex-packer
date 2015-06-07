#include "sprite.h"

Sprite::Sprite(QString spritePath) {
    QImage* img = new QImage(spritePath);
    mPixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*img));
    delete img;

    mPath = spritePath;

    // Extract Sprite name from path (filename without suffix)
    QStringRef name(&mPath, mPath.lastIndexOf('/')+1, mPath.lastIndexOf('.')-mPath.lastIndexOf('/')-1);
    mName = name.toString();

    mIsOnScene = false;
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

QGraphicsPixmapItem* Sprite::getPixmapItem() {
    return mPixmapItem;
}


QString Sprite::getPath() {
    return mPath;
}

QString Sprite::getName() {
    return mName;
}

void Sprite::setIsOnScene(bool state) {
    mIsOnScene = state;
}

bool Sprite::isOnScene() {
    return mIsOnScene;
}
