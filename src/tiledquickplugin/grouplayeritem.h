#pragma once

#include "layeritem.h"
#include "layerscontainer.h"


namespace Tiled
{
class GroupLayer;
}

namespace TiledQuick
{
class GroupLayerItem : public LayerItem
{
public:
    GroupLayerItem(Tiled::GroupLayer* layer, MapItem* mapItem, QQuickItem* parent);

private:
    LayersContainer m_layersContainer;
};
}

