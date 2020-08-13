#include "grouplayeritem.h"
#include "grouplayer.h"
#include "layerscreator.h"


namespace TiledQuick
{
GroupLayerItem::GroupLayerItem(Tiled::GroupLayer* layer, TiledItem* parent)
    : LayerItem(layer, parent)
{
    LayersCreator::create(layer->layers(), this);
}
}
