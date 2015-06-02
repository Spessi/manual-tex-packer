#include "dialognewtileset.h"
#include "ui_dialognewtileset.h"

DialogNewTileset::DialogNewTileset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewTileset)
{
    ui->setupUi(this);

    connect(ui->inp_width, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DialogNewTileset::recalcDimension);
    connect(ui->inp_height, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DialogNewTileset::recalcDimension);
    connect(ui->inp_tilewidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DialogNewTileset::recalcDimension);
    connect(ui->inp_tileheight, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DialogNewTileset::recalcDimension);
}

DialogNewTileset::~DialogNewTileset()
{
    delete ui;
}


int DialogNewTileset::getWidth() {
    return ui->inp_width->value();
}

int DialogNewTileset::getHeight() {
    return ui->inp_height->value();
}

int DialogNewTileset::getTileWidth() {
    return ui->inp_tilewidth->value();
}

int DialogNewTileset::getTileHeight() {
    return ui->inp_tileheight->value();
}

void DialogNewTileset::recalcDimension() {
    int width = ui->inp_width->value() * ui->inp_tilewidth->value();
    int height = ui->inp_height->value() * ui->inp_tileheight->value();

    ui->lbl_val_tilesetdim->setText(QString::number(width) + "x" + QString::number(height) + " Pixels");
}
