#pragma once

#include "mapobject.h"
#include "tileditem.h"


namespace Tiled
{
class MapObject;
}

namespace TiledQuick
{
class ObjectLayerItem;

class MapObjectItem : public TiledItem
{
    Q_OBJECT

public:
    MapObjectItem(Tiled::MapObject* mapObject, ObjectLayerItem* parent);

    ObjectLayerItem* objectLayer() const;
    Tiled::MapObject* mapObject() const;

protected:
    static QPointF convertCoordinates(QRectF const& rect, Tiled::Alignment origin);
    static QQuickItem::TransformOrigin convert(Tiled::Alignment origin);

    void paint(QPainter* painter) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void transformPolygon();
    void drawDebug(QPainter* painter);

    bool containsPoint(QPointF const& localPos) const;
    bool containsPointInBoundingRect(QPointF const& localPos) const;

private:
    Callback m_pressCallback;
    Callback m_releaseCallback;
    Callback m_clickCallback;
    bool m_drawDebug;
    bool m_pressByShape;
    bool m_clikable;
    bool m_pressed;
};
}
