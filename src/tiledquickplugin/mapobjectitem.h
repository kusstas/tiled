#pragma once

#include "mapobject.h"

#include <QQuickPaintedItem>
#include <QJSValue>


class QQuickItem;

namespace Tiled
{
class MapObject;
}

namespace TiledQuick
{
class ObjectLayerItem;

class MapObjectItem : public QQuickPaintedItem
{
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

    bool clickable() const;
    bool containsPoint(QPointF const& localPos) const;
    bool containsPointInBoundingRect(QPointF const& localPos) const;

    QJSValue compileCallback(QString const& name);
    void invokeCallback(QJSValue& callback, QString const& name);

private:
    Tiled::MapObject* m_mapObject;
    QJSValue m_onPressCallback;
    QJSValue m_onReleaseCallback;
    QJSValue m_onClickCallback;
    bool m_drawDebug;
    bool m_pressByShape;
    bool m_pressGuard;
    bool m_pressed;
};
}
