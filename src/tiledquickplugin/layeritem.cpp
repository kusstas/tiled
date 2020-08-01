#include "layeritem.h"

#include "mapitem.h"
#include "map"
#include "layer.h"

#include <QPainter>


namespace TiledQuick
{
LayerItem::LayerItem(Tiled::Layer* layer, Tiled::MapRenderer* renderer, MapItem* parent)
    : QQuickPaintedItem(parent)
    , m_layer(layer)
    , m_renderer(renderer)
{
    setObjectName(layer->name());
    setParentItem(parent);
    setX(layer->totalOffset().x());
    setY(layer->totalOffset().y());
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
    return m_renderer;
}

MapItem* LayerItem::mapItem() const
{
    return static_cast<MapItem*>(parent());
}

QRectF LayerItem::visibleArea() const
{
    return mapItem()->visibleArea();
}

void LayerItem::updatePaint()
{
    update();
}

void LayerItem::paint(QPainter* /*painter*/)
{
}
}
