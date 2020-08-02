#pragma once

#include <QQuickPaintedItem>


namespace Tiled
{
class Layer;
class MapRenderer;
}

namespace TiledQuick
{
class MapItem;

class LayerItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QColor tintColor READ tintColor CONSTANT)

public:
    LayerItem(Tiled::Layer* layer, MapItem* mapItem, QQuickItem* parent);

    QColor const& tintColor() const;
    Tiled::Layer* layer() const;
    Tiled::MapRenderer* renderer() const;
    MapItem* mapItem() const;
    QQmlEngine* qqmlEngine() const;

protected:
    void updatePaint();
    void paint(QPainter* painter) override;

private:
    Tiled::Layer* m_layer;
    MapItem* m_mapItem;
};
}

