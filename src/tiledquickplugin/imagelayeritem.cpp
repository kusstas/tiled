#include "imagelayeritem.h"

#include "imagelayer.h"
#include "drawutils.h"


namespace TiledQuick
{
ImageLayerItem::ImageLayerItem(Tiled::ImageLayer* layer, TiledQuick::MapItem* mapItem, QQuickItem* parent)
    : LayerItem(layer, mapItem, parent)
{
    setSize(layer->image().size());
}

void ImageLayerItem::paint(QPainter* painter)
{
    QRect const rect(0, 0, width(), height());
    QPixmap const& image = layer()->asImageLayer()->image();

    DrawUtils::paintPixmap(painter, rect, rect, image, layer()->tintColor());
}
}
