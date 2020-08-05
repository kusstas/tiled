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
    Q_OBJECT

public:
    ImageLayerItem(Tiled::ImageLayer* layer, TiledItem* parent);

protected:
    void paint(QPainter* painter) override;
};
}

