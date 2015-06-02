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


int TilesetManager::saveToFile() {

    return 0;
}

int TilesetManager::loadFromFile(QString path) {
    int sprites_counter = 0, sprites_count = 0;
    Sprite* sprite = nullptr;
    int sprite_x, sprite_y, sprite_width, sprite_height;
    QString sprite_path;

    QFile file(path);
    file.open(QFile::ReadOnly | QFile::Text);

    Tileset* tileset = new Tileset();
    this->addTileset(tileset);


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
                    this->getTileset(0)->setWidth(stream.readElementText().toInt());
                }
                else if(stream.name() == "height") {
                    this->getTileset(0)->setHeight(stream.readElementText().toInt());
                }
                else if(stream.name() == "tile_width") {
                    this->getTileset(0)->setTileWidth(stream.readElementText().toInt());
                }
                else if(stream.name() == "tile_height") {
                    this->getTileset(0)->setTileHeight(stream.readElementText().toInt());
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
                    this->getTileset(0)->addSprite(sprite);



                    sprites_counter++;
                    if(sprites_counter >= sprites_count)
                        break;
                }
            }
    }

    file.close();

    return 0;
}
