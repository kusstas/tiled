#pragma once

#include "mapobject.h"
#include "tileditem.h"

#include <QScopedPointer>


namespace Tiled
{
class MapObject;
}

namespace TiledQuick
{
class ObjectLayerItem;
class StateMachine;
class TextData;

class MapObjectItem : public TiledItem
{
    Q_OBJECT

    Q_PROPERTY(TiledQuick::StateMachine* stateMachine READ stateMachine CONSTANT)
    Q_PROPERTY(TiledQuick::TextData* textData READ textData CONSTANT)
    Q_PROPERTY(bool pressed READ pressed NOTIFY pressedChanged)

public:
    MapObjectItem(Tiled::MapObject* mapObject, ObjectLayerItem* parent);

    StateMachine* stateMachine() const;
    TextData* textData() const;
    bool pressed() const;

    ObjectLayerItem* objectLayer() const;
    Tiled::MapObject* mapObject() const;
    int id() const override;

    void collideEnter(MapObjectItem* collidedObject);
    void collideExit(MapObjectItem* collidedObject);

signals:
    void pressedChanged(bool pressed);

protected:
    static QPointF convertCoordinates(QRectF const& rect, Tiled::Alignment origin);
    static QQuickItem::TransformOrigin convert(Tiled::Alignment origin);

    void onDestroyItem() override;
    void paint(QPainter* painter) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void transformPolygon();
    void drawDebug(QPainter* painter);
    bool containsPoint(QPointF const& localPos) const;
    bool containsPointInBoundingRect(QPointF const& localPos) const;
    TextData* tryCreateTextData();
    void setPressed(bool pressed);

private:
    Callback m_pressCallback;
    Callback m_releaseCallback;
    Callback m_clickCallback;
    Callback m_collideEnterCallback;
    Callback m_collideExitCallback;
    StateMachine* m_stateMachine;
    TextData* m_textData;
    bool m_drawDebug;
    bool m_pressByShape;
    bool m_clikable;
    bool m_pressed;
    bool m_collidingEnabled;
};
}
