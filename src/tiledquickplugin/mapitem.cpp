/*
 * mapitem.cpp
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

#include "mapitem.h"

#include "tilelayeritem.h"
#include "objectlayeritem.h"

#include "hexagonalrenderer.h"
#include "isometricrenderer.h"
#include "map.h"
#include "orthogonalrenderer.h"
#include "staggeredrenderer.h"
#include "tilelayer.h"

#include <QPainter>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlComponent>
#include <cmath>


namespace TiledQuick
{
constexpr auto PARENT_PROP = "parent";
constexpr auto LAYER_PROP = "tiledLayer";

QUrl const MapItem::TINT_COLOR_OVERLAY_URL("qrc:/tiledquick_private_qml/TintColorOverlay.qml");
QSharedPointer<QQmlComponent> MapItem::tintColorOverlayComponent;

MapItem::MapItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_map(nullptr)
{
}

MapItem::~MapItem() = default;

void MapItem::setMap(MapRef map)
{
    if (m_map == map.m_map)
    {
        return;
    }

    m_map = map.m_map;
    refresh();
    emit mapChanged();
}

void MapItem::setVisibleArea(QRectF const& visibleArea)
{
    m_visibleArea = visibleArea;
    emit visibleAreaChanged();
}

QRectF MapItem::boundingRect() const
{
    if (!m_renderer)
    {
        return QQuickPaintedItem::boundingRect();
    }

    return m_renderer->mapBoundingRect();
}

QPointF MapItem::screenToTileCoords(qreal x, qreal y) const
{
    if (!m_renderer)
    {
        return QPointF(x, y);
    }

    return m_renderer->screenToTileCoords(x, y);
}

QPointF MapItem::screenToTileCoords(QPointF const& position) const
{
    if (!m_renderer)
    {
        return position;
    }

    return m_renderer->screenToTileCoords(position);
}

QPointF MapItem::tileToScreenCoords(qreal x, qreal y) const
{
    if (!m_renderer)
    {
        return QPointF(x, y);
    }

    return m_renderer->tileToScreenCoords(x, y);
}

QPointF MapItem::tileToScreenCoords(QPointF const& position) const
{
    if (!m_renderer)
    {
        return position;
    }

    return m_renderer->tileToScreenCoords(position);
}

QPointF MapItem::screenToPixelCoords(qreal x, qreal y) const
{
    if (!m_renderer)
    {
        return QPointF(x, y);
    }

    return m_renderer->screenToPixelCoords(x, y);
}

QPointF MapItem::screenToPixelCoords(QPointF const& position) const
{
    if (!m_renderer)
    {
        return position;
    }

    return m_renderer->screenToPixelCoords(position);
}

QPointF MapItem::pixelToScreenCoords(qreal x, qreal y) const
{
    if (!m_renderer)
    {
        return QPointF(x, y);
    }

    return m_renderer->pixelToScreenCoords(x, y);
}

QPointF MapItem::pixelToScreenCoords(QPointF const& position) const
{
    if (!m_renderer)
    {
        return position;
    }

    return m_renderer->pixelToScreenCoords(position);
}

QPointF MapItem::pixelToTileCoords(qreal x, qreal y) const
{
    if (!m_renderer)
    {
        return QPointF(x, y);
    }

    return m_renderer->pixelToTileCoords(x, y);
}

QPointF MapItem::pixelToTileCoords(QPointF const& position) const
{
    if (!m_renderer)
    {
        return position;
    }

    return m_renderer->pixelToTileCoords(position);
}

void MapItem::componentComplete()
{
    QQuickPaintedItem::componentComplete();

    if (!tintColorOverlayComponent)
    {
        tintColorOverlayComponent.reset(new QQmlComponent(qmlEngine(this), TINT_COLOR_OVERLAY_URL));
    }

    if (m_map)
    {
        refresh();
    }
}

void MapItem::paint(QPainter* /*painter*/)
{
}

void MapItem::refresh()
{
    if (!isComponentComplete())
    {
        return;
    }

    qDeleteAll(m_layerItems);
    m_layerItems.clear();

    m_renderer = nullptr;

    if (!m_map)
    {
        return;
    }

    setFillColor(m_map->backgroundColor());

    switch (m_map->orientation())
    {
    case Tiled::Map::Isometric:
        m_renderer = std::make_unique<Tiled::IsometricRenderer>(m_map);
        break;
    case Tiled::Map::Staggered:
        m_renderer = std::make_unique<Tiled::StaggeredRenderer>(m_map);
        break;
    case Tiled::Map::Hexagonal:
        m_renderer = std::make_unique<Tiled::HexagonalRenderer>(m_map);
        break;
    default:
        m_renderer = std::make_unique<Tiled::OrthogonalRenderer>(m_map);
        break;
    }

    for (Tiled::Layer* layer : m_map->layers())
    {
        LayerItem* layerItem = nullptr;

        switch (layer->layerType())
        {
        case Tiled::Layer::TileLayerType:
            layerItem = new TileLayerItem(layer->asTileLayer(), m_renderer.get(), this);
            break;
        case Tiled::Layer::ObjectGroupType:
            layerItem = new ObjectLayerItem(layer->asObjectGroup(), m_renderer.get(), this);
            break;
        case Tiled::Layer::ImageLayerType:
            break;
        case Tiled::Layer::GroupLayerType:
            break;
        }

        if (!layerItem)
        {
            continue;
        }

        m_layerItems.append(layerItem);

        if (layer->tintColor().isValid())
        {
            attachColorOverlay(layerItem);
        }
    }

    QRect const rect = m_renderer->mapBoundingRect();
    setImplicitSize(rect.width(), rect.height());
}

void MapItem::attachColorOverlay(LayerItem* layer)
{
    QVariantMap const initProps =
    {
        {PARENT_PROP, QVariant::fromValue(this)},
        {LAYER_PROP, QVariant::fromValue(layer)}
    };

    auto const object = tintColorOverlayComponent->createWithInitialProperties(initProps, qmlEngine(this)->contextForObject(this));

    if (!object)
    {
        qCritical() << "Can't create color overlay for:" << layer << "reason:" << tintColorOverlayComponent->errorString();
        return;
    }

    auto const quickItem = qobject_cast<QQuickItem*>(object);

    if (!quickItem)
    {
        qCritical() << "Can't create color overlay for:" << layer << "reason: component should be QQuickItem";
        object->deleteLater();
        return;
    }

    QQmlEngine::setObjectOwnership(quickItem, QQmlEngine::CppOwnership);
    quickItem->setParentItem(this);
    quickItem->setParent(layer);
}
}
