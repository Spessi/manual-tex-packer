#include "tileset.h"

Tileset::Tileset() {
    mWidth = 0;
    mHeight = 0;
    mTileWidth = 0;
    mTileHeight = 0;
}

Tileset::Tileset(int width, int height, int tileWidth, int tileHeight) {
    mWidth = width;
    mHeight = height;
    mTileWidth = tileWidth;
    mTileHeight = tileHeight;
}

Tileset::~Tileset() {
    for(int i=0; i < mSprites.size(); i++) {
        delete mSprites.at(i);
    }
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

    return 0;
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


void Tileset::setWidth(int width) {
    mWidth = width;
}

void Tileset::setHeight(int height) {
    mHeight = height;
}

void Tileset::setTileWidth(int width) {
    mTileWidth = width;
}

void Tileset::setTileHeight(int height) {
    mTileHeight = height;
}

int Tileset::getWidth() {
    return mWidth;
}

int Tileset::getHeight() {
    return mHeight;
}

int Tileset::getTileWidth() {
    return mTileWidth;
}

int Tileset::getTileHeight() {
    return mTileHeight;
}

QList<Sprite*> Tileset::getSprites() {
    return mSprites;
}

int Tileset::saveToFile(QString path) {
    QFile file(path);
    file.open(QFile::WriteOnly | QFile::Text);

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);

    stream.writeStartDocument();
    stream.writeStartElement("project");

    stream.writeStartElement("properties");
        stream.writeTextElement("width", QString::number(getWidth()));
        stream.writeTextElement("height", QString::number(getHeight()));
        stream.writeTextElement("tile_width", QString::number(getTileWidth()));
        stream.writeTextElement("tile_height", QString::number(getTileHeight()));
        stream.writeTextElement("sprites_count", QString::number(mSprites.size()));
    stream.writeEndElement();

    // Write sprite information
    for(int i=0; i < mSprites.size(); ++i) {
        stream.writeStartElement("sprite_" + QString::number(i));
        stream.writeTextElement("x", QString::number(mSprites.at(i)->getX()));
        stream.writeTextElement("y", QString::number(mSprites.at(i)->getY()));
        stream.writeTextElement("width", QString::number(mSprites.at(i)->getWidth()));
        stream.writeTextElement("height", QString::number(mSprites.at(i)->getHeight()));
        stream.writeTextElement("path", mSprites.at(i)->getPath());
        stream.writeEndElement();
    }


    stream.writeEndElement();
    stream.writeEndDocument();

    file.close();

    return 0;
}


int Tileset::loadFromFile(QString path) {
    int sprites_counter = 0, sprites_count = 0;
    Sprite* sprite = nullptr;
    int sprite_x, sprite_y, sprite_width, sprite_height;
    QString sprite_path;

    QFile file(path);
    file.open(QFile::ReadOnly | QFile::Text);


    QXmlStreamReader stream(&file);

    // Get properties
    while(!stream.atEnd() && !stream.hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = stream.readNext();
        //If token is just StartDocument - go to next
            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }

            //If token is StartElement - read it
            if(token == QXmlStreamReader::StartElement) {
                if(stream.name() == "properties") {
                    continue;
                }

                if(stream.name() == "width") {
                    this->mWidth = stream.readElementText().toInt();
                }
                else if(stream.name() == "height") {
                    this->mHeight = stream.readElementText().toInt();
                }
                else if(stream.name() == "tile_width") {
                    this->mTileWidth = stream.readElementText().toInt();
                }
                else if(stream.name() == "tile_height") {
                    this->mTileWidth = stream.readElementText().toInt();
                }
                else if(stream.name() == "sprites_count") {
                    sprites_count = stream.readElementText().toInt();
                }
            }
            if(token == QXmlStreamReader::EndElement) {
                if(stream.name() == "properties")
                    break;
            }
    }

    // Get sprites
    while(!stream.atEnd() && !stream.hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = stream.readNext();
        //If token is just StartDocument - go to next
            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }

            //If token is StartElement - read it
            if(token == QXmlStreamReader::StartElement) {
                if(stream.name() == "sprite_" + QString::number(sprites_counter)) {
                    continue;
                }

                if(stream.name() == "width") {
                    sprite_width = stream.readElementText().toInt();
                }
                else if(stream.name() == "height") {
                    sprite_height = stream.readElementText().toInt();
                }
                else if(stream.name() == "x") {
                    sprite_x = stream.readElementText().toInt();
                }
                else if(stream.name() == "y") {
                    sprite_y = stream.readElementText().toInt();
                }
                else if(stream.name() == "path") {
                    sprite_path = stream.readElementText();
                }
            }
            if(token == QXmlStreamReader::EndElement) {
                if(stream.name() == "sprite_" + QString::number(sprites_counter)) {
                    // Create new Sprite from XML informations
                    sprite = new Sprite(sprite_path);

                    // Check if real width matches with XML informations
                    if(sprite->getWidth() != sprite_width || sprite->getHeight() != sprite_height)
                        return -1;

                    // Position sprite
                    sprite->setPos(sprite_x, sprite_y);

                    // Add sprite to sprite list
                    this->addSprite(sprite);


                    sprites_counter++;
                    if(sprites_counter >= sprites_count)
                        break;
                }
            }
    }

    file.close();


    return 0;
}
