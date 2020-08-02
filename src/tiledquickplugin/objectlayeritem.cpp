#include "objectlayeritem.h"

#include "objectgroup.h"
#include "mapobject.h"
#include "map.h"

#include "mapobjectitem.h"
#include "mapitem.h"


namespace TiledQuick
{
ObjectLayerItem::ObjectLayerItem(Tiled::ObjectGroup* layer, MapItem* mapItem, QQuickItem* parent)
    : LayerItem(layer, mapItem, parent)
{
    for (auto obj : layer->objects())
    {
        new MapObjectItem(obj, this); // child, controled by qt parent system
    }
}
}
