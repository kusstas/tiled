#include "mapviewport.h"

#include "isometricrenderer.h"
#include "staggeredrenderer.h"
#include "hexagonalrenderer.h"
#include "orthogonalrenderer.h"


namespace TiledQuick
{
constexpr auto ANCHORS_PROPERTY = "anchors";
constexpr auto CENTER_IN_PROPERTY = "centerIn";

MapViewport::MapViewport(QQuickItem* parent)
    : QQuickItem(parent)
{
    connect(this, &QQuickItem::widthChanged, this, &MapViewport::updateScale);
    connect(this, &QQuickItem::heightChanged, this, &MapViewport::updateScale);
    setClip(true);
}

MapRef const& MapViewport::map() const
{
    return m_map;
}

QObject* MapViewport::external() const
{
    return m_external;
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

void MapViewport::setExternal(QObject* external)
{
    if (m_external == external)
    {
        return;
    }

    m_external = external;
    emit externalChanged(m_external);

    if (m_mapItem)
    {
        m_mapItem->resetExternal(m_external);
    }
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

    m_mapItem.reset(new MapItem(m_map, m_renderer, m_external, this));
    setImplicitSize(m_mapItem->width(), m_mapItem->height());

    auto const anchors = m_mapItem->property(ANCHORS_PROPERTY).value<QObject*>();
    anchors->setProperty(CENTER_IN_PROPERTY, QVariant::fromValue(this));
    updateScale();
}

void MapViewport::updateScale()
{
    if (!m_mapItem)
    {
        return;
    }

    if (width() / m_mapItem->width() > height() / m_mapItem->height())
    {
        m_mapItem->setScale(height() / m_mapItem->height());
    }
    else
    {
        m_mapItem->setScale(width() / m_mapItem->width());
    }
}
}
