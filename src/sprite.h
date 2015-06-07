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
    QString getPath();
    QGraphicsPixmapItem* getPixmapItem();
    QString getName();
    void setIsOnScene(bool state);
    bool isOnScene();

private:
    QString mName;
    QGraphicsPixmapItem* mPixmapItem;
    QString mPath;
    bool mIsOnScene;
    QRect getBoundingRect();
};

#endif // SPRITE_H
