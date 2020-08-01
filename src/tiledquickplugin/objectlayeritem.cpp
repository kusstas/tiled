#include "objectlayeritem.h"

#include "objectgroup.h"
#include "mapobject.h"
#include "map.h"

#include "imagemapobject.h"
#include "mapitem.h"


namespace TiledQuick
{
ObjectLayerItem::ObjectLayerItem(Tiled::ObjectGroup* layer, Tiled::MapRenderer* renderer, TiledQuick::MapItem* parent)
    : LayerItem(layer, renderer, parent)
{
    for (auto obj : layer->objects())
    {
        if (!obj->cell().isEmpty())
        {
            new ImageMapObject(obj, this); // child, controled by qt parent system
        }
    }

    onParentWidthChanged();
    onParentHeightChanged();

    connect(parentItem(), &QQuickItem::widthChanged, this, &ObjectLayerItem::onParentWidthChanged);
    connect(parentItem(), &QQuickItem::heightChanged, this, &ObjectLayerItem::onParentHeightChanged);
}


void ObjectLayerItem::onParentWidthChanged()
{
    setWidth(parentItem()->width());
}

void ObjectLayerItem::onParentHeightChanged()
{
    setHeight(parentItem()->height());
}
}
