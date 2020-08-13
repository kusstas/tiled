#pragma once

#include "layeritem.h"


namespace Tiled
{
class GroupLayer;
}

namespace TiledQuick
{
class GroupLayerItem : public LayerItem
{
    Q_OBJECT

public:
    GroupLayerItem(Tiled::GroupLayer* layer, TiledItem* parent);
};
}

