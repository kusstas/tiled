/*
 * tilelayeritem.cpp
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

#include "tilelayeritem.h"

#include "map.h"
#include "maprenderer.h"
#include "tilelayer.h"

#include "mapitem.h"

#include <QtMath>
#include <QQuickWindow>


namespace TiledQuick
{
TileLayerItem::TileLayerItem(Tiled::TileLayer* layer, MapItem* mapItem, QQuickItem* parent)
    : LayerItem(layer, mapItem, parent)
{
    auto const rect = renderer()->boundingRect(layer->rect());

    setSize(rect.size());

    update();
}

void TileLayerItem::paint(QPainter* painter)
{
    renderer()->drawTileLayer(painter, layer()->asTileLayer(), QRect());
}
}
