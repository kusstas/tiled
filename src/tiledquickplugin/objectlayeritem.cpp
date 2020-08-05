#include "objectlayeritem.h"
#include "objectgroup.h"
#include "mapobject.h"
#include "mapobjectitem.h"


namespace TiledQuick
{
ObjectLayerItem::ObjectLayerItem(Tiled::ObjectGroup* layer, TiledItem* parent)
    : LayerItem(layer, parent)
{
    for (auto obj : layer->objects())
    {
        new MapObjectItem(obj, this); // child, controled by qt parent system
    }
}

void ObjectLayerItem::start()
{
    LayerItem::start();

    auto const& objects = findChildren<MapObjectItem*>();

    for (auto const& object : objects)
    {
        object->start();
    }
}
}
