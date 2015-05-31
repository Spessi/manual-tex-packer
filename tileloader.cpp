#include "tileloader.h"

TileLoader::TileLoader(QString tilePath)
{
    mTiles = new QList<QGraphicsPixmapItem*>();

    QDirIterator it(tilePath, QDirIterator::Subdirectories);

    while(it.hasNext()) {
        if(it.fileInfo().isFile()) {
            QImage* img = new QImage(it.filePath());
            QGraphicsPixmapItem* pixMap = new QGraphicsPixmapItem(QPixmap::fromImage(*img));
            mTiles->append(pixMap);
            free(img);
            qDebug() << it.filePath();
        }
        it.next();
    }

    if(it.fileInfo().isFile()) {
        QImage* img = new QImage(it.filePath());
        QGraphicsPixmapItem* pixMap = new QGraphicsPixmapItem(QPixmap::fromImage(*img));
        mTiles->append(pixMap);
        free(img);
        qDebug() << it.filePath();
    }
    it.next();
}

QList<QGraphicsPixmapItem*>* TileLoader::getTiles() {
    return mTiles;
}


void TileLoader::dispose() {
    QList<QGraphicsPixmapItem*>::iterator i;

    for(i = mTiles->begin(); i != mTiles->end(); ++i) {
        free(*i);
    }

    mTiles->clear();
    free(mTiles);
}
