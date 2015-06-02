#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsPixmapItem>
#include <QImage>
#include <QRectF>

class Sprite
{
public:
    Sprite(QString spritePath);
    void setPos(float x, float y);
    float getX();
    float getY();
    int getWidth();
    int getHeight();
    int getTileX();
    int getTileY();
    QRectF getBoundingRect();
    QGraphicsPixmapItem* getPixmapItem();

private:
    QGraphicsPixmapItem* mPixmapItem;
};

#endif // SPRITE_H
