#include "layeritem.h"

#include "map"
#include "layer.h"

#include <QPainter>


namespace TiledQuick
{
LayerItem::LayerItem(Tiled::Layer* layer, TiledItem* parent)
    : TiledItem(layer, parent)
{
    setObjectName(layer->name());
    setPosition(layer->totalOffset());
    setOpacity(layer->opacity());
    setVisible(layer->isVisible());
    validateObjectName();
}

Tiled::Layer* LayerItem::layer() const
{
    return object<Tiled::Layer>();
}

int LayerItem::id() const
{
    return layer()->id();
}
}
