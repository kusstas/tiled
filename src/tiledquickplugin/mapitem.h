/*
 * mapitem.h
 * Copyright 2014, Thorbjørn Lindeijer <bjorn@lindeijer.nl>
 *
 * This file is part of Tiled Quick.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "mapref.h"

#include <QQuickPaintedItem>
#include <QSharedPointer>
#include <QUrl>

#include <memory>

namespace Tiled
{
class MapRenderer;
} // namespace Tiled

namespace TiledQuick
{
class LayerItem;
class TileLayerItem;
class ObjectLayerItem;

/**
 * A declarative item that displays a map.
 */
class MapItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(TiledQuick::MapRef map READ map WRITE setMap RESET unsetMap NOTIFY mapChanged)
    Q_PROPERTY(QRectF visibleArea READ visibleArea WRITE setVisibleArea NOTIFY visibleAreaChanged)

public:
    explicit MapItem(QQuickItem* parent = nullptr);
    ~MapItem();

    MapRef map() const;
    void setMap(MapRef map);
    void unsetMap();

    QRectF const& visibleArea() const;
    void setVisibleArea(QRectF const& visibleArea);

    QRectF boundingRect() const override;

    Q_INVOKABLE QPointF screenToTileCoords(qreal x, qreal y) const;
    Q_INVOKABLE QPointF screenToTileCoords(QPointF const& position) const;
    Q_INVOKABLE QPointF tileToScreenCoords(qreal x, qreal y) const;
    Q_INVOKABLE QPointF tileToScreenCoords(QPointF const& position) const;
    Q_INVOKABLE QPointF screenToPixelCoords(qreal x, qreal y) const;
    Q_INVOKABLE QPointF screenToPixelCoords(QPointF const& position) const;
    Q_INVOKABLE QPointF pixelToScreenCoords(qreal x, qreal y) const;
    Q_INVOKABLE QPointF pixelToScreenCoords(QPointF const& position) const;
    Q_INVOKABLE QPointF pixelToTileCoords(qreal x, qreal y) const;
    Q_INVOKABLE QPointF pixelToTileCoords(QPointF const& position) const;

    void componentComplete() override;

signals:
    void mapChanged();
    void visibleAreaChanged();

protected:
    void paint(QPainter* painter) override;

private:
    void refresh();
    void attachColorOverlay(LayerItem* layer);

private:
    static QUrl const TINT_COLOR_OVERLAY_URL;
    static QSharedPointer<QQmlComponent> tintColorOverlayComponent;

    Tiled::Map* m_map;
    QRectF m_visibleArea;

    std::unique_ptr<Tiled::MapRenderer> m_renderer;
    QList<LayerItem*> m_layerItems;
};

inline QRectF const& MapItem::visibleArea() const
{
    return m_visibleArea;
}

inline MapRef MapItem::map() const
{
    return m_map;
}

inline void MapItem::unsetMap()
{
    setMap(nullptr);
}

} // namespace TiledQuick
