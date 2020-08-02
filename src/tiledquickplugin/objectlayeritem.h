#pragma once

#include "layeritem.h"


namespace Tiled
{
class ObjectGroup;
}

namespace TiledQuick
{
class MapItem;

class ObjectLayerItem : public LayerItem
{
    Q_OBJECT

public:
    ObjectLayerItem(Tiled::ObjectGroup* layer,MapItem* mapItem, QQuickItem* parent);
};
}

