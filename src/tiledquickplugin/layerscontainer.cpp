#include "layerscontainer.h"

#include "tilelayeritem.h"
#include "objectlayeritem.h"
#include "grouplayeritem.h"
#include "imagelayeritem.h"

#include "tilelayer.h"


namespace TiledQuick
{
LayersContainer::LayersContainer(QList<Tiled::Layer*> const& layers, TiledItem* parent)
{
    for (Tiled::Layer* layer : layers)
    {
        LayerItem* layerItem = nullptr;

        switch (layer->layerType())
        {
        case Tiled::Layer::TileLayerType:
            layerItem = new TileLayerItem(layer->asTileLayer(), parent);
            break;
        case Tiled::Layer::ObjectGroupType:
            layerItem = new ObjectLayerItem(layer->asObjectGroup(), parent);
            break;
        case Tiled::Layer::ImageLayerType:
            layerItem = new ImageLayerItem(layer->asImageLayer(), parent);
            break;
        case Tiled::Layer::GroupLayerType:
            layerItem = new GroupLayerItem(layer->asGroupLayer(), parent);
            break;
        }

        if (!layerItem)
        {
            continue;
        }

        m_layerItems.append(layerItem);
    }
}


LayersContainer::~LayersContainer()
{
    qDeleteAll(m_layerItems);
}

void LayersContainer::start()
{
    for (auto layer : m_layerItems)
    {
        layer->start();
    }
}
}
