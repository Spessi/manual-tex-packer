#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsPixmapItem>
#include <QImage>
#include <QRectF>

class Sprite
{
public:
    Sprite(QString spritePath);
    ~Sprite();
    void setPos(float x, float y);
    float getX();
    float getY();
    int getWidth();
    int getHeight();
    int getTileX();
    int getTileY();
    QString getPath();
    QGraphicsPixmapItem* getPixmapItem();

private:
    QGraphicsPixmapItem* mPixmapItem;
    QString mPath;
};

#endif // SPRITE_H
