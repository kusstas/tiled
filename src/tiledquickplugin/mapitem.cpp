#include "mapitem.h"


namespace TiledQuick
{
MapItem::MapItem(QSharedPointer<Tiled::Map> const& map,
                 QSharedPointer<Tiled::MapRenderer> const& renderer,
                 QPointer<QObject> const& external,
                 QQuickItem* parent)
    : TiledItem(map.get(), this, parent)
    , m_map(map)
    , m_renderer(renderer)
    , m_scriptEngine(this)
    , m_external(external)
{
    if (m_map->backgroundColor().isValid())
    {
        setFillColor(m_map->backgroundColor());
    }
    setSize(QSize(m_map->tileWidth() * m_map->width(), m_map->tileHeight() * m_map->height()));
    m_layersContainer.reset(new LayersContainer(m_map->layers(), this));
}

QObject* MapItem::external()
{
    return m_external;
}

Tiled::Map* MapItem::map()
{
    return m_map.get();
}

Tiled::MapRenderer* MapItem::renderer()
{
    return m_renderer.get();
}

ScriptEngine* MapItem::scriptEngine()
{
    return &m_scriptEngine;
}

QQmlEngine* MapItem::qqmlEngine()
{
    return qmlEngine(parentItem());
}

void MapItem::start()
{
    TiledItem::start();
    m_layersContainer->start();
}

void MapItem::resetExternal(QPointer<QObject> const& external)
{
    m_external = external;
}
}
