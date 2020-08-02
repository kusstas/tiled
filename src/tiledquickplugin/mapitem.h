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
#include "maprenderer.h"

#include "layerscontainer.h"

#include <QQuickPaintedItem>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QUrl>



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

public:
    explicit MapItem(QQuickItem* parent = nullptr);

    MapRef map() const;
    Tiled::MapRenderer* renderer() const;

    void setMap(MapRef map);
    void unsetMap();

signals:
    void mapChanged();

protected:
    void componentComplete() override;
    void paint(QPainter* painter) override;

private:
    void refresh();

private:
    Tiled::Map* m_map;
    QScopedPointer<Tiled::MapRenderer> m_renderer;
    LayersContainer m_layersContainer;
};
} // namespace TiledQuick
