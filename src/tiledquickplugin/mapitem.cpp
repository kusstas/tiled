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

#include "map.h"
#include "hexagonalrenderer.h"
#include "isometricrenderer.h"
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
MapItem::MapItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_map(nullptr)
    , m_layersContainer(this, this)
{
}

MapRef TiledQuick::MapItem::map() const
{
    return m_map;
}

Tiled::MapRenderer* MapItem::renderer() const
{
    return m_renderer.get();
}

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

void TiledQuick::MapItem::unsetMap()
{
    setMap(nullptr);
}

void MapItem::componentComplete()
{
    QQuickPaintedItem::componentComplete();

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

    m_renderer.reset();

    if (!m_map)
    {
        return;
    }

    if (m_map->backgroundColor().isValid())
    {
        setFillColor(m_map->backgroundColor());
    }
    setSize(QSize(m_map->tileWidth() * m_map->width(), m_map->tileHeight() * m_map->height()));

    switch (m_map->orientation())
    {
    case Tiled::Map::Isometric:
        m_renderer.reset(new Tiled::IsometricRenderer(m_map));
        break;
    case Tiled::Map::Staggered:
        m_renderer.reset(new Tiled::StaggeredRenderer(m_map));
        break;
    case Tiled::Map::Hexagonal:
        m_renderer.reset(new Tiled::HexagonalRenderer(m_map));
        break;
    default:
        m_renderer.reset(new Tiled::OrthogonalRenderer(m_map));
        break;
    }

    m_layersContainer.create(m_map->layers());
}
}
