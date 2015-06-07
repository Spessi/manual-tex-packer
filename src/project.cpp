#include "project.h"

Project::Project() {
    mSpriteLoader = new SpriteLoader();
}

Project::~Project() {
    for(int i=0; i < mTilesets.size(); i++) {
        delete this->getTileset(i);
    }
    delete mSpriteLoader;
}

void Project::addTileset(Tileset* tileset) {
    if(tileset != nullptr)
        mTilesets.append(tileset);
}

Tileset* Project::getTileset(int idx) {
    if(idx >= mTilesets.size())
        return nullptr;

    return mTilesets.at(idx);
}

SpriteLoader* Project::getSpriteLoader() {
    return mSpriteLoader;
}


int Project::saveToFile(QString path) {
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

int Project::loadFromFile(QString path) {
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
    file.close();

    return 0;
}
