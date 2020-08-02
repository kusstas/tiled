#include "grouplayeritem.h"

#include "grouplayer.h"


namespace TiledQuick
{
GroupLayerItem::GroupLayerItem(Tiled::GroupLayer* layer, TiledQuick::MapItem* mapItem, QQuickItem* parent)
    : LayerItem(layer, mapItem, parent)
    , m_layersContainer(this, mapItem)
{
    m_layersContainer.create(layer->layers());
}
}
