#include "tilesetmanager.h"

TilesetManager::TilesetManager() {
    mSpriteLoader = new SpriteLoader();
}

TilesetManager::~TilesetManager() {
    for(int i=0; i < mTilesets.size(); i++) {
        delete this->getTileset(i);
    }
    delete mSpriteLoader;
}

void TilesetManager::addTileset(Tileset* tileset) {
    if(tileset != nullptr)
        mTilesets.append(tileset);
}

Tileset* TilesetManager::getTileset(int idx) {
    if(idx >= mTilesets.size())
        return nullptr;

    return mTilesets.at(idx);
}

SpriteLoader* TilesetManager::getSpriteLoader() {
    return mSpriteLoader;
}


int TilesetManager::saveToFile(QString path) {
    QFile file(path);
    file.open(QFile::WriteOnly | QFile::Text);

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);

    stream.writeStartDocument();
    stream.writeStartElement("tilesets");

    for(int i=0; i < mTilesets.size(); ++i) {
        stream.writeStartElement("tileset");
        stream.writeAttribute("width", QString::number(mTilesets.at(i)->getWidth()));
        stream.writeAttribute("height", QString::number(mTilesets.at(i)->getHeight()));
        stream.writeAttribute("tile_width", QString::number(mTilesets.at(i)->getTileWidth()));
        stream.writeAttribute("tile_height", QString::number(mTilesets.at(i)->getTileHeight()));


        // Write sprite information
        for(int j=0; j < mTilesets.at(i)->getSprites().size(); ++j) {
            stream.writeStartElement("sprite");
            stream.writeAttribute("x", QString::number(mTilesets.at(i)->getSprites().at(j)->getX()));
            stream.writeAttribute("y", QString::number(mTilesets.at(i)->getSprites().at(j)->getY()));
            stream.writeAttribute("path", mTilesets.at(i)->getSprites().at(j)->getPath());
            stream.writeEndElement();
        }

        stream.writeEndElement();
    }


    stream.writeEndElement();
    stream.writeEndDocument();

    file.close();

    return 0;
}

int TilesetManager::loadFromFile(QString path) {
    /*
    int sprites_counter = 0, sprites_count = 0;
    Sprite* sprite = nullptr;
    int sprite_x, sprite_y, sprite_width, sprite_height;
    QString sprite_path;*/

    QFile file(path);
    file.open(QFile::ReadOnly | QFile::Text);


    Tileset* tileset = nullptr;
    Sprite* sprite = nullptr;

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
                if(stream.name() == "tilesets") {
                    continue;
                }

                if(stream.name() == "tileset") {
                    // neues Tileset anlegen
                    tileset = new Tileset();

                    tileset->setWidth(stream.attributes().at(0).value().toInt());
                    tileset->setHeight(stream.attributes().at(1).value().toInt());
                    tileset->setTileWidth(stream.attributes().at(2).value().toInt());
                    tileset->setTileHeight(stream.attributes().at(3).value().toInt());

                    addTileset(tileset);
                }
                else if(stream.name() == "sprite") {
                    sprite = new Sprite(stream.attributes().at(2).value().toString());
                    sprite->setPos(stream.attributes().at(0).value().toInt(), stream.attributes().at(1).value().toInt());
                    sprite->setIsOnScene(true);
                }
            }
            if(token == QXmlStreamReader::EndElement) {
                if(stream.name() == "tilesets")
                    break;

                if(stream.name() == "sprite") {
                    if(mTilesets.at(mTilesets.size()-1) != nullptr) {
                        mTilesets.at(mTilesets.size()-1)->addSprite(sprite);
                    }
                    else
                        delete sprite;
                }
            }
    }
/*
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
                    this->getTileset(0)->addSprite(sprite);



                    sprites_counter++;
                    if(sprites_counter >= sprites_count)
                        break;
                }
            }
    }
*/
    file.close();

    // TODO check if read was successful -> add it, otherwise delete it
//    this->addTileset(tileset);

    return 0;
}
