#ifndef DIALOGNEWTILESET_H
#define DIALOGNEWTILESET_H

#include <QDialog>
#include <QSpinBox>

namespace Ui {
class DialogNewTileset;
}

class DialogNewTileset : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewTileset(QWidget *parent = 0);
    ~DialogNewTileset();
    int getWidth();
    int getHeight();
    int getTileWidth();
    int getTileHeight();

private slots:
    void recalcDimension();

private:
    Ui::DialogNewTileset *ui;
};

#endif // DIALOGNEWTILESET_H
