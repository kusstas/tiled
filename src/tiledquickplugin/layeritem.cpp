#include "layeritem.h"

#include "mapitem.h"
#include "map"
#include "layer.h"

#include <QPainter>


namespace TiledQuick
{
LayerItem::LayerItem(Tiled::Layer* layer, MapItem* mapItem, QQuickItem* parent)
    : QQuickPaintedItem(parent)
    , m_layer(layer)
    , m_mapItem(mapItem)
{
    setObjectName(layer->name());
    setParentItem(parent);
    setPosition(layer->totalOffset());
    setOpacity(layer->opacity());
    setVisible(layer->isVisible());
}

QColor const& LayerItem::tintColor() const
{
    return layer()->tintColor();
}

Tiled::Layer* LayerItem::layer() const
{
    return m_layer;
}

Tiled::MapRenderer* LayerItem::renderer() const
{
    return mapItem()->renderer();
}

MapItem* LayerItem::mapItem() const
{
    return m_mapItem;
}

QQmlEngine* LayerItem::qqmlEngine() const
{
    return qmlEngine(mapItem());
}

void LayerItem::updatePaint()
{
    update();
}

void LayerItem::paint(QPainter* /*painter*/)
{
}
}
