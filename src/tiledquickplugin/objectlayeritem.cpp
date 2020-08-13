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

QList<MapObjectItem*> const& ObjectLayerItem::collidedObjects() const
{
    return m_collidedObjects;
}

void ObjectLayerItem::registerCollidedObject(MapObjectItem* object)
{
    m_collidedObjects.append(object);
}

void ObjectLayerItem::unregisterCollidedObject(MapObjectItem* object)
{
    m_collidedObjects.removeOne(object);
}
}
