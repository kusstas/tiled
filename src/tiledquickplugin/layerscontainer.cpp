#include "layerscontainer.h"

#include "tilelayeritem.h"
#include "objectlayeritem.h"
#include "grouplayeritem.h"
#include "imagelayeritem.h"

#include "map.h"
#include "hexagonalrenderer.h"
#include "isometricrenderer.h"
#include "orthogonalrenderer.h"
#include "staggeredrenderer.h"
#include "tilelayer.h"

#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlComponent>


namespace TiledQuick
{
LayersContainer::LayersContainer(QQuickItem* parentItem, MapItem* mapItem)
    : m_parentItem(parentItem)
    , m_mapItem(mapItem)
{
}

LayersContainer::~LayersContainer()
{
    qDeleteAll(m_layerItems);
}

void LayersContainer::create(QList<Tiled::Layer*> const& layers)
{
    qDeleteAll(m_layerItems);
    m_layerItems.clear();

    for (Tiled::Layer* layer : layers)
    {
        LayerItem* layerItem = nullptr;

        switch (layer->layerType())
        {
        case Tiled::Layer::TileLayerType:
            layerItem = new TileLayerItem(layer->asTileLayer(), m_mapItem, m_parentItem);
            break;
        case Tiled::Layer::ObjectGroupType:
            layerItem = new ObjectLayerItem(layer->asObjectGroup(), m_mapItem, m_parentItem);
            break;
        case Tiled::Layer::ImageLayerType:
            layerItem = new ImageLayerItem(layer->asImageLayer(), m_mapItem, m_parentItem);
            break;
        case Tiled::Layer::GroupLayerType:
            layerItem = new GroupLayerItem(layer->asGroupLayer(), m_mapItem, m_parentItem);
            break;
        }

        if (!layerItem)
        {
            continue;
        }

        m_layerItems.append(layerItem);
    }
}
}
