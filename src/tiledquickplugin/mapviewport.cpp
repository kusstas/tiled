#include "mapviewport.h"

#include "isometricrenderer.h"
#include "staggeredrenderer.h"
#include "hexagonalrenderer.h"
#include "orthogonalrenderer.h"


namespace TiledQuick
{
MapViewport::MapViewport(QQuickItem* parent)
    : QQuickItem(parent)
{
}

MapRef const& MapViewport::map() const
{
    return m_map;
}

void MapViewport::setMap(MapRef const& map)
{
    if (m_map == map)
    {
        return;
    }

    m_map = map;
    emit mapChanged(m_map);

    refresh();
}

void MapViewport::componentComplete()
{
    QQuickItem::componentComplete();

    if (m_map)
    {
        refresh();
    }
}

void MapViewport::refresh()
{
    if (!isComponentComplete())
    {
        return;
    }

    m_mapItem.reset();
    m_renderer.reset();

    if (!m_map)
    {
        return;
    }

    switch (m_map->orientation())
    {
    case Tiled::Map::Isometric:
        m_renderer.reset(new Tiled::IsometricRenderer(m_map.get()));
        break;
    case Tiled::Map::Staggered:
        m_renderer.reset(new Tiled::StaggeredRenderer(m_map.get()));
        break;
    case Tiled::Map::Hexagonal:
        m_renderer.reset(new Tiled::HexagonalRenderer(m_map.get()));
        break;
    default:
        m_renderer.reset(new Tiled::OrthogonalRenderer(m_map.get()));
        break;
    }

    m_mapItem.reset(new MapItem(m_map, m_renderer, this));
    m_mapItem->start();
}
}
