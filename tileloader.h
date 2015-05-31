#ifndef TILELOADER_H
#define TILELOADER_H

#include <QList>
#include <QGraphicsItem>
#include <QDirIterator>
#include <QDebug>

class TileLoader
{
public:
    TileLoader(QString tilePath);
    QList<QGraphicsPixmapItem*>* getTiles();
    void dispose();

private:
    QList<QGraphicsPixmapItem*>* mTiles;
};

#endif // TILELOADER_H
