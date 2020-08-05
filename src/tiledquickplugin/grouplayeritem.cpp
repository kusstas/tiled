#include "grouplayeritem.h"
#include "grouplayer.h"


namespace TiledQuick
{
GroupLayerItem::GroupLayerItem(Tiled::GroupLayer* layer, TiledItem* parent)
    : LayerItem(layer, parent)
    , LayersContainer(layer->layers(), this)
{
}

void GroupLayerItem::start()
{
    TiledItem::start();
    LayersContainer::start();
}
}
