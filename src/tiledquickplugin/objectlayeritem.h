#pragma once

#include "layeritem.h"


namespace Tiled
{
class ObjectGroup;
}

namespace TiledQuick
{
class MapObjectItem;

class ObjectLayerItem : public LayerItem
{
    Q_OBJECT

public:
    ObjectLayerItem(Tiled::ObjectGroup* layer, TiledItem* parent);

    QList<MapObjectItem*> const& collidedObjects() const;

    void registerCollidedObject(MapObjectItem* object);
    void unregisterCollidedObject(MapObjectItem* object);

private:
    QList<MapObjectItem*> m_collidedObjects;
};
}

