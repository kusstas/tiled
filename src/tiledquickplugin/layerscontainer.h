#pragma once

#include <QList>
#include <QQuickItem>


namespace Tiled
{
class Layer;
class MapRenderer;
} // namespace Tiled

namespace TiledQuick
{
class MapItem;
class LayerItem;

class LayersContainer
{
public:
    LayersContainer(QQuickItem* parentItem, MapItem* mapItem);
    ~LayersContainer();

    void create(QList<Tiled::Layer*> const& layers);

private:
    QQuickItem* m_parentItem;
    MapItem* m_mapItem;
    QList<LayerItem*> m_layerItems;
};
}

