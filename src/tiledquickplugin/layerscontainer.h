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

class LayersContainer
{
public:
    LayersContainer(QList<Tiled::Layer*> const& layers, TiledItem* parent);
    ~LayersContainer();

    void start();

private:
    QList<LayerItem*> m_layerItems;
};
}

