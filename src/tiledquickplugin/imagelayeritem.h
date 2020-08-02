#pragma once

#include "layeritem.h"


namespace Tiled
{
class ImageLayer;
}

namespace TiledQuick
{
class ImageLayerItem : public LayerItem
{
public:
    ImageLayerItem(Tiled::ImageLayer* layer, MapItem* mapItem, QQuickItem* parent);

protected:
    void paint(QPainter* painter) override;
};
}

