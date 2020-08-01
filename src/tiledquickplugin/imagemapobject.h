#pragma once

#include <QQuickPaintedItem>


namespace Tiled
{
class MapObject;
}

namespace TiledQuick
{
class ObjectLayerItem;

class ImageMapObject : public QQuickPaintedItem
{
    Q_OBJECT

public:
    ImageMapObject(Tiled::MapObject* mapObject, ObjectLayerItem* parent);

protected:
    void paint(QPainter* painter) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void handleAlignment();

private:
    Tiled::MapObject* m_mapObject;
};
}
