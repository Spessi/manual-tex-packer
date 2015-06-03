#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsPixmapItem>
#include <QImage>
#include <QRectF>
#include <QDebug>

class Sprite
{
public:
    Sprite(QString spritePath);
    ~Sprite();
    bool contains(int x, int y);
    void setPos(float x, float y);
    float getX();
    float getY();
    int getWidth();
    int getHeight();
    QRect getBoundingRect();
    int getTileX();
    int getTileY();
    QString getPath();
    QGraphicsPixmapItem* getPixmapItem();

private:
    QGraphicsPixmapItem* mPixmapItem;
    QString mPath;
};

#endif // SPRITE_H
