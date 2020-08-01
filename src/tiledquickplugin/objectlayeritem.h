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
    ObjectLayerItem(Tiled::ObjectGroup* layer, Tiled::MapRenderer* renderer, MapItem* parent);

private:
    void onParentWidthChanged();
    void onParentHeightChanged();
};
}

