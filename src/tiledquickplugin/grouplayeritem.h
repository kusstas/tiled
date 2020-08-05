#pragma once

#include "layeritem.h"
#include "layerscontainer.h"


namespace Tiled
{
class GroupLayer;
}

namespace TiledQuick
{
class GroupLayerItem : public LayerItem, public LayersContainer
{
    Q_OBJECT

public:
    GroupLayerItem(Tiled::GroupLayer* layer, TiledItem* parent);

    void start() override;
};
}

