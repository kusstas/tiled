#pragma once

#include <QList>


namespace Tiled
{
class Layer;
} // namespace Tiled

namespace TiledQuick
{
class TiledItem;
class LayerItem;

class LayersCreator
{
public:
    static void create(QList<Tiled::Layer*> const& layers, TiledItem* parent);
};
}

