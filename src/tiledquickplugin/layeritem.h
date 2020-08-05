#pragma once

#include "tileditem.h"


namespace Tiled
{
class Layer;
}

namespace TiledQuick
{
class LayerItem : public TiledItem
{
    Q_OBJECT

public:
    LayerItem(Tiled::Layer* layer, TiledItem* parent);

    Tiled::Layer* layer() const;
};
}

