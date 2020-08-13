#include "layerscreator.h"

#include "tilelayeritem.h"
#include "objectlayeritem.h"
#include "grouplayeritem.h"
#include "imagelayeritem.h"

#include "tilelayer.h"


namespace TiledQuick
{
void LayersCreator::create(QList<Tiled::Layer*> const& layers, TiledItem* parent)
{
    for (Tiled::Layer* layer : layers)
    {
        switch (layer->layerType())
        {
        case Tiled::Layer::TileLayerType:
            new TileLayerItem(layer->asTileLayer(), parent); // child, controled by qt parent system
            break;
        case Tiled::Layer::ObjectGroupType:
            new ObjectLayerItem(layer->asObjectGroup(), parent); // child, controled by qt parent system
            break;
        case Tiled::Layer::ImageLayerType:
            new ImageLayerItem(layer->asImageLayer(), parent); // child, controled by qt parent system
            break;
        case Tiled::Layer::GroupLayerType:
            new GroupLayerItem(layer->asGroupLayer(), parent); // child, controled by qt parent system
            break;
        }
    }
}
}
